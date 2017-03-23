
#ifndef MYSDK_NET_BUFFER_H
#define MYSDK_NET_BUFFER_H


#include <algorithm>
#include <string>
#include <vector>

#include <assert.h>
#include <unistd.h>  // ssize_t
#include <stdint.h>
#include <stdarg.h>

/// A buffer class modeled after org.jboss.netty.buffer.ChannelBuffer
///
/// @code
/// +-------------------+------------------+------------------+
/// | prependable bytes |  readable bytes  |  writable bytes  |
/// |                   |     (CONTENT)    |                  |
/// +-------------------+------------------+------------------+
/// |                   |                  |                  |
/// 0      <=      readerIndex   <=   writerIndex    <=     size
/// @endcode

	class Buffer
	{
	public:
		static const size_t kCheapPrepend = 12;
		static const size_t kInitialSize = 1012;

		Buffer():
			buffer_(kCheapPrepend + kInitialSize),
			readerIndex_(kCheapPrepend),
			writerIndex_(kCheapPrepend)
		{
			assert(readableBytes() == 0);
			assert(writeableBytes() == kInitialSize);
			assert(prependableBytes() == kCheapPrepend);
		}
		// default copy-ctor, dtor and assignment are fine

		void swap(Buffer& rhs)
		{
			buffer_.swap(rhs.buffer_);
			std::swap(readerIndex_, rhs.readerIndex_);
			std::swap(writerIndex_, rhs.writerIndex_);
		}

		size_t readableBytes()
		{ return writerIndex_ - readerIndex_; }

		size_t writeableBytes()
		{ return buffer_.size() - writerIndex_; }

		size_t prependableBytes()
		{ return readerIndex_; }

		const char* peek() const
		{ return begin() + readerIndex_; }

		const char* findCRLF() const
		{
			const char* crlf = std::search(peek(), beginWrite(), kCRLF, kCRLF + 2);
			return crlf = beginWrite() ? NULL : crlf;
		}

		const char* findCRLF(const char* start) const
		{
			assert(peek() <= start);
			assert(start <= beginWrite());
			const char* crlf = std::search(start, beginWrite(), kCRLF, kCRLF + 2);
			return crlf = beginWrite() ? NULL : crlf;
		}
		// retrieve returns void, to prevent
		// string str(retrieve(readableBytes()), readableBytes());
		// the evaluation of two functions are unspecified
		void retrieve(size_t len)
		{
			assert(len <= readableBytes());
			readerIndex_ += len;
		}

		void retrieveInt16()
		{
			retrieve(sizeof(int16_t));
		}

		void retrieveInt32()
		{
		    retrieve(sizeof(int32_t));
	    }

		void retrieveUntil(const char* end)
		{
			assert(peek() <= end);
			assert(end <= beginWrite());
			retrieve(end - peek());
		}

		void retrieveAll()
		{
			readerIndex_ = kCheapPrepend;
			writerIndex_ = kCheapPrepend;
		}

		std::string retrieveAsString()
		{
			std::string str(peek(), readableBytes());
			retrieveAll();
			return str;
		}

        void append(const char* fmt, ...)
        {
            va_list args;
            va_start(args, fmt);

            char buf[1024] = { '\0' };
            int ret = vsnprintf(buf, sizeof(buf),  fmt, args);
            if (ret <= sizeof(buf) && ret > 0) {
                append(buf, ret);
            }
            va_end(args);
        }

		void append(const std::string& str)
		{
			append(str.data(), str.length());
		}

		void append(const char* data, size_t len)
		{
			if (writeableBytes() < len)
			{
				makeSpace(len);
			}
			assert(len <= writeableBytes());
			std::copy(data, data + len, beginWrite());
			writerIndex_ += len;
		}

		  void append(const void* /*restrict*/ data, size_t len)
		  {
		    append(static_cast<const char*>(data), len);
		  }

		  void ensureWritableBytes(size_t len)
		  {
		    if (writeableBytes() < len)
		    {
		      makeSpace(len);
		    }
		    assert(writeableBytes() >= len);
		  }
		  ///
		  /// Append int32_t using network endian
		  ///
		  void appendInt32(int32_t x);
		  void appendInt16(int16_t x);
		  ///
		  /// Read int32_t from network endian
		  ///
		  /// Require: buf->readableBytes() >= sizeof(int32_t)
		  int32_t readInt32()
		  {
		    int32_t result = peekInt32();
		    retrieveInt32();
		    return result;
		  }

		  int16_t readInt16()
		  {
			  int16_t result = peekInt16();
			  retrieveInt16();
			  return result;
		  }
		  ///
		  /// Peek int32_t from network endian
		  ///
		  /// Require: buf->readableBytes() >= sizeof(int32_t)
		  int32_t peekInt32() const;
		  int16_t peekInt16() const;

		  void get(char* data, int len)
		  {
			  //assert( readableBytes() >= len);
			  std::copy(peek(), peek() + len, data);
		  }

		void prepend(const void* data, size_t len)
		{
			assert(len <= prependableBytes());
			readerIndex_ -= len;
			const char* d = static_cast<const char*>(data);
			std::copy(d, d + len, begin() + readerIndex_);
		}

		void shrink(size_t reserve)
		{
			std::vector<char> buf(kCheapPrepend + readableBytes() + reserve);
			std::copy(peek(), peek() + readableBytes(), buf.begin() + kCheapPrepend);
			buf.swap(buffer_);
		}

        char* beginWrite()
		{ return begin() + writerIndex_; }

		const char* beginWrite() const
		{ return begin() + writerIndex_; }

		  void hasWritten(size_t len)
		  { writerIndex_ += len; }
private:
		char* begin()
		{ return &*buffer_.begin(); }

		const char* begin() const
		{ return &*buffer_.begin(); }

		void makeSpace(size_t more)
		{
			if (writeableBytes() + prependableBytes() < more + kCheapPrepend)
			{
				buffer_.resize(writerIndex_ + more);
			}
			else
			{
			    // move readable data to the front, make space inside buffer
				size_t used = readableBytes();
				std::copy(begin() + readerIndex_, begin() + writerIndex_, begin() + kCheapPrepend);
				readerIndex_ = kCheapPrepend;
				writerIndex_ = readerIndex_ + used;
				assert(used == readableBytes());
			}
		}

	private:
		std::vector<char> buffer_;
		size_t readerIndex_;
		size_t writerIndex_;
		static const char kCRLF[];
	};

#endif
