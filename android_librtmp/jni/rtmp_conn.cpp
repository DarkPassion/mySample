#include "rtmp_conn.h"


enum {
	NAL_SLICE_IDR = 5,
	NAL_SEI = 6,
	NAL_SPS = 7,
	NAL_PPS = 8,
 };
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

void rtmp_conn::close()
{
	if (_rtmp && RTMP_IsConnected(_rtmp)) {
		RTMP_Close(_rtmp);
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


	int i = 0;
	while(i < len - 4) {

		if (buff[i + 0] == 0x00 && buff[i + 1] == 0x00 && buff[i + 2] == 0x00 && buff[i + 3] == 0x1) {
			i += 4;
		} else if (buff[i + 0] == 0x00 && buff[i + 1] == 0x00 && buff[i + 2] == 0x01) {
			i += 3;
		} else {
			break;
		}

		unsigned char type = buff[i] & 0x1f;

		if (type == NAL_SPS) {
			// sps
			int sps_len = find_start_code(buff + i, len - i);

			if (_sps.size() == 0) {
				_sps.assign(buff + i, sps_len);
			}
			i += sps_len;

		} else if (type == NAL_PPS) {
			// pps

			int pps_len = find_start_code(buff + i, len - i);

			if (_pps.size() == 0) {
				_pps.assign(buff + i, pps_len);
			}
			i += pps_len;

			if (!_avc_conf_send) {
				// send avc conf

			}
		} else if (type == NAL_SEI){
			// sei
			// ingore sei frame
			int sei_len = find_start_code(buff + i, len - i);
			i += sei_len;

		} else {
			if (_avc_conf_send) {
				// send h264 packet
				
			}
		}

	}

}


int rtmp_conn::find_start_code(unsigned char* buff,  int len)
{
	int r = 0;

	int i = 0;
	while(i < len) {
		if (buff[i + 0] == 0x00 && buff[i + 1] == 0x00 && buff[i + 2] == 0x00 && buff[i + 3] == 0x01) {
			r = i;
			break;
		} else if (buff[i + 0] == 0x00 && buff[i + 1] == 0x00 && buff[i + 2] == 0x01) {
			r = i;
			break;
		} else {
			i++;
		}
	}

	return r;
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


