#ifndef _XX_MSG_H_
#define _XX_MSG_H_
#include <vector>

#ifdef _WIN32
#include <stdint.h>
#endif


#define MAX_MSG_LEN		1000

struct Message
{
	int type;
	int len;
	char *data;
};

struct AVMsg
{
	char* data;
	int len;
};

class utility{
public:
	
	// be care of char buff
	// when encode char buff should use unsigned char buff, else will lost some data
	static void write_int64(char* buff, int64_t val)
	{
		int32_t low = (val << 32) & 0xffffffff;
		int32_t high = (val >> 32) & 0xffffffff;

		write_int32(buff, high);
		write_int32(buff + 4, low);
	}

	static int64_t read_int64(char* buff)
	{
		int64_t p = 0;
		int64_t ret = 0;

		p = read_int32(buff);
		ret += (p << 32) & 0xffffffff;
		p = read_int32(buff + 4);
		ret += p & 0xffffffff;
		return ret;
	}

	static void write_int32(char* buff, int32_t val)
	{
		unsigned char* ubuff = (unsigned char*)buff;
		ubuff[0] = (val >> 24) & 0xff;
		ubuff[1] = (val >> 16) & 0xff;
		ubuff[2] = (val >> 8) & 0xff;
		ubuff[3] = (val) & 0xff;
	}

	static int32_t read_int32(char*buff)
	{
		int32_t p = 0;
		int32_t ret = 0;
		unsigned char* ubuff = (unsigned char*)buff;
		p = ubuff[0];
		ret += p << 24;
		p = ubuff[1];
		ret += p << 16;
		p = ubuff[2];
		ret += p << 8;
		p = ubuff[3];
		ret += p;
		return ret;

	}

	static void write_int24(char* buff, int32_t val)
	{
		unsigned char* ubuff = (unsigned char*)buff;
		ubuff[0] = (val >> 16) & 0xff;
		ubuff[1] = (val >> 8) & 0xff;
		ubuff[2] = (val) & 0xff;
	}

	static int32_t read_int24(char* buff)
	{
		int ret = 0;
		int p = 0;
		unsigned char* ubuff = (unsigned char*)buff;
		p = ubuff[0];
		ret += p << 16;
		p = ubuff[1];
		ret += p << 8;
		p = ubuff[2];
		ret += p;
		return ret;
		
	}

	static void write_int16(char* buff, int16_t val)
	{
		unsigned char* ubuff = (unsigned char*)buff;
		ubuff[0] = (val >> 8) & 0xff;
		ubuff[1] = (val ) & 0xff;
	}

	static int16_t read_int16(char* buff)
	{
		int16_t p = 0;
		int16_t ret = 0;
		unsigned char* ubuff = (unsigned char*)buff;

		p = ubuff[0];
		ret += p << 8;
		p = ubuff[1];
		ret += p ;
		return ret;

	}

	static void write_int8(char* buff, int8_t val)
	{
		unsigned char* ubuff = (unsigned char*)buff;
		ubuff[0] = val & 0xff;
	}

	static int8_t read_int8(char* buff)
	{
		unsigned char* ubuff = (unsigned char*)buff;
		int8_t ret = ubuff[0];
		return ret;
	}

	static void write_vamsg(char* buff, AVMsg* msg)
	{
		write_int32(buff + 0, msg->len);
		memcpy(buff + 4, msg->data, msg->len);
	}

	static int read_vamsg(char* buff)
	{
		int len = read_int32(buff);
		return len;
	}

	static bool read_vamsg(char* buff, AVMsg& msg)
	{
		memset(&msg, 0, sizeof(msg));
		int len = read_int32(buff);
		char* data = buff + 4;
		
		msg.data = data;
		msg.len = len;
		return true;
	}
};

class EncodeMsg
{
public:
	static bool parse_message(int type, int len, const char* data, char* buff);

};


class ICodecMsg
{

public:
	virtual ~ICodecMsg() {}

	virtual void on_message(Message* msg) = 0;
};


class DecodeMsg
{
public:
	DecodeMsg();
	~DecodeMsg();


public:

	bool parse_data(int len, char* data);
	
	void set_call_back(ICodecMsg* notify) { _on_notify = notify; }
private:
	void on_message(Message* msg);

private:
	// here can use vector<char> instead of using std::string
	typedef std::vector<char> DecodeBuff;
	DecodeBuff _data;
	// std::string _data;
	ICodecMsg* _on_notify;
};

#endif
