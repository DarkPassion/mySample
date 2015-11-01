#ifndef _XX_MSG_H_
#define _XX_MSG_H_
#include <string>
#include <stdint.h>

#define MAX_MSG_LEN		1000

struct Message
{
	int type;
	int len;
	char *data;
};


class Untily{
public:
	
	static void write_int32(char* buff, int32_t val)
	{
		buff[0] = (val >> 24) & 0xff;
		buff[1] = (val >> 16) & 0xff;
		buff[2] = (val >> 8) & 0xff;
		buff[3] = (val) & 0xff;
	}

	static int32_t read_int32(char*buff)
	{
		int32_t p = 0;
		int32_t ret = 0;
		p = buff[0];
		ret += p << 24;
		p = buff[1];
		ret += p << 16;
		p = buff[2];
		ret += p << 8;
		p = buff[3];
		ret += p;
		return ret;

	}

	static void write_int24(char* buff, int32_t val)
	{
		buff[0] = (val >> 16) & 0xff;
		buff[1] = (val >> 8) & 0xff;
		buff[2] = (val) & 0xff;
	}

	static int32_t read_int24(char* buff)
	{
		int ret = 0;
		int p = 0;
		p = buff[0];
		ret += p << 16;
		p = buff[1];
		ret += p << 8;
		p = buff[2];
		ret += p;
		return ret;
		
	}

	static void write_int16(char* buff, int16_t val)
	{
		buff[0] = (val << 8) & 0xff;
		buff[1] = (val ) & 0xff;
	}

	static int16_t read_int16(char* buff)
	{
		int16_t p = 0;
		int16_t ret = 0;

		p = buff[0];
		ret += p << 8;
		p = buff[1];
		ret += p ;
		return ret;

	}

	static void write_int8(char* buff, int8_t val)
	{
		buff[0] = val & 0xff;
	}

	static int8_t read_int8(char* buff)
	{
		int8_t ret = buff[0];
		return ret;
	}
};

class EncodeMsg
{
public:
	static bool parse_message(int type, int len, char* data, char* buff);

};

class DecodeMsg
{
public:
	DecodeMsg();
	~DecodeMsg();

public:


public:
	bool parse_data(int len, char* data);
	
	void on_message(Message* msg);

private:
	std::string _data;
	int			_len;
};

#endif
