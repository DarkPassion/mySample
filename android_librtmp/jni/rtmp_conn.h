#ifndef __RTMP_CONN_H__
#define __RTMP_CONN_H__

#include <string.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include "librtmp/rtmp.h"


class rtmp_conn
{

public:
	rtmp_conn();

	~rtmp_conn();


	void connect(const char* url);

	void send_h264(const unsigned char* buff, int len, int bkey, int timestamp);

	void send_aac(const unsigned char* buff, int len, int timesamp);

	void close();


private:

	void send_avc_config();

	void send_aac_config();

	void send_packet(unsigned char type, unsigned char* buff, int len, int timestamp);

private:
	std::string _url;
	RTMP*	_rtmp;
	bool	_aac_conf_send;
	bool	_avc_conf_send;
	std::string _avc_conf;
	std::string _aac_conf;


};


#endif

