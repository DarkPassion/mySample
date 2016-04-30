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

    void send_h264(const unsigned char* buff, int len, int timestamp);

    void send_aac(const unsigned char* buff, int len, int timesamp);

    void close();


private:

    void send_avc_config();

    void send_aac_config();

    void send_video_packet(unsigned char* indata, int inlen, int timestamp, int bkey);

    void send_packet(unsigned char type, unsigned char* buff, int len, int timestamp);

    int find_start_code(unsigned char* buff,  int len, int & startcode);

private:
    std::string _url;
    RTMP*	_rtmp;
    bool	_aac_conf_send;
    bool	_avc_conf_send;
    std::string _sps;
    std::string _pps;
    std::string _aac_conf;


};


#endif

