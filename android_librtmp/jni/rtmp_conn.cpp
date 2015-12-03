#include "rtmp_conn.h"

rtmp_conn::rtmp_conn()  : _rtmp(NULL), _aac_conf_send(false), _avc_conf_send(false)
{

	_rtmp = RTMP_Alloc();
	RTMP_Init(_rtmp);
}


rtmp_conn::~rtmp_conn()
{
	if (_rtmp) {
		RTMP_Free(_rtmp);
		_rtmp = NULL;
	}


}


void rtmp_conn::connect(const char* url)
{
	_url = url;

	int err = RTMP_SetupURL(_rtmp, (char*)url);

	RTMP_EnableWrite(_rtmp);

	err = RTMP_ConnectStream(_rtmp, 0);

}


void rtmp_conn::send_h264(const unsigned char* buff, int len, int bkey, int timestamp)
{

	//  dont send any h264 packet until recv key frame to send avc conf
	if (!_avc_conf_send) {
		send_avc_config();
	}

	if (!_avc_conf_send) {
		return;
	}



}


void rtmp_conn::send_aac(const unsigned char* buff, int len, int timesamp)
{

	// dont send any aac packet until recv aac header
	if (!_aac_conf_send) {
		send_avc_config();
	}

	if (!_aac_conf_send) {
		return;
	}

}


void rtmp_conn::send_avc_config()
{

}


void rtmp_conn::send_aac_config()
{

}


void rtmp_conn::send_packet(unsigned char type, unsigned char* buff, int len, int timestamp)
{
	if (_rtmp == NULL) {
		return ;
	}

	RTMPPacket packet;
	RTMPPacket_Reset(&packet);
	RTMPPacket_Alloc(&packet, len);

	packet.m_packetType = type;
	packet.m_nChannel = 0x04;
	packet.m_headerType = RTMP_PACKET_SIZE_LARGE;
	packet.m_nTimeStamp = timestamp;
	packet.m_nInfoField2 = _rtmp->m_stream_id;
	packet.m_nBodySize = len;
	memcpy(packet.m_body, buff, len);

	int ret = RTMP_SendPacket(_rtmp, &packet, 0);
	RTMPPacket_Free(&packet);
}


