#include "rtmp_conn.h"


enum
{
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
    if (_rtmp)
    {
        RTMP_Free(_rtmp);
        _rtmp = NULL;
    }


}

void rtmp_conn::close()
{
    if (_rtmp && RTMP_IsConnected(_rtmp))
    {
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


void rtmp_conn::send_h264(const unsigned char* buff, int len, int timestamp)
{

    //  dont send any h264 packet until recv key frame to send avc conf


    int i = 0;
    while(i < len - 4)
    {

        if (buff[i + 0] == 0x00 &&
                buff[i + 1] == 0x00 &&
                buff[i + 2] == 0x00 &&
                buff[i + 3] == 0x01)
        {
            i += 4;
        }
        else if (
            buff[i + 0] == 0x00 &&
            buff[i + 1] == 0x00 &&
            buff[i + 2] == 0x01)
        {
            i += 3;
        }
        else
        {
            // error startcode!!
            return -1;
        }

        unsigned char type = buff[i] & 0x1f;
        int startcode = 0;

        // SPS
        if (type == NAL_SPS)
        {
            // sps
            int sps_len = find_start_code(buff + i, len - i, startcode);

            if (_sps.size() == 0)
            {
                _sps.assign(buff + i, sps_len);
            }
            i += sps_len + startcode;
        }

        // PPS
        if (type == NAL_PPS)
        {
            // pps

            int pps_len = find_start_code(buff + i, len - i, startcode);

            if (_pps.size() == 0)
            {
                _pps.assign(buff + i, pps_len);
            }
            i += pps_len + startcode;
        }

        // SEI
        if (type == NAL_SEI)
        {
            // sei
            int sei_len = find_start_code(buff + i, len - i, startcode);
            i += sei_len + startcode;
        }

        if (_avc_conf_send == false &&
                _sps.size() &&
                _pps.size())
        {

            send_avc_config();
        }

        int bkey = 0;
        if (type == NAL_SLICE_IDR)
        {
            bkey = 1;
        }

        if (true && _avc_conf_send == true)
        {
            send_video_packet(buff + i, len - i, timestamp, bkey);
            break;
        }
    }

}


int rtmp_conn::find_start_code(unsigned char* buff,  int len, int& startcode)
{
    int size = len;
    startcode = 0;

    if (buff == NULL || len < 4)
    {
        return -1;
    }


    int i = 0;
    while(i < len)
    {
        if (buff[i + 0] == 0x00 &&
                buff[i + 1] == 0x00 &&
                buff[i + 2] == 0x00 &&
                buff[i + 3] == 0x01)
        {
            size = i;
            startcode = 4;
            break;
        }
        else if (	buff[i + 0] == 0x00 &&
                    buff[i + 1] == 0x00 &&
                    buff[i + 2] == 0x01)
        {
            size = i;
            startcode = 3;
            break;
        }
        else
        {
            i++;
        }
    }

    return size;
}

void rtmp_conn::send_aac(const unsigned char* buff, int len, int timesamp)
{

    // dont send any aac packet until recv aac header
    if (!_aac_conf_send)
    {
        send_aac_config();
    }

    if (!_aac_conf_send)
    {
        return;
    }

}


void rtmp_conn::send_avc_config()
{

    unsigned char body[2048] = {0};
    int offset = 0;

    body[offset++] = 0x17;  // key frame, AVC
    body[offset++] = 0x00;  // avc sequence header
    body[offset++] = 0x00;  // composit time, should be 0 for sps/pps
    body[offset++] = 0x00;  // composit time
    body[offset++] = 0x00;	// composit time

    //flv VideoTagBody --AVCDecoderCOnfigurationRecord
    body[offset++] = 0x01; //configurationversion
    body[offset++] = spsdata[1]; //avcprofileindication
    body[offset++] = spsdata[2]; //profilecompatibilty
    body[offset++] = spsdata[3]; //avclevelindication
    body[offset++] = 0xff; //reserved + lengthsizeminusone
    body[offset++] = 0xe1; //numofsequenceset
    body[offset++] = (uint8_t)(_sps.size() >> 8); //sequence parameter set length high 8 bits
    body[offset++] = (uint8_t)(_sps.size()); //sequence parameter set  length low 8 bits
    memcpy(body + offset, _sps.c_str(), _sps.size()); //H264 sequence parameter set
    offset += _sps.size();

    output[offset++] = 0x01; //numofpictureset
    output[offset++] = (uint8_t)(_pps.size() >> 8); //picture parameter set length high 8 bits
    output[offset++] = (uint8_t)(_pps.size()); //picture parameter set length low 8 bits
    memcpy(output + offset, _pps.c_str(), _pps.size()); //H264 picture parameter set
    offset += _pps.size();



    RTMPPacket packet;
    RTMPPacket_Reset(&packet);
    RTMPPacket_Alloc(&packet, len);

    packet.m_packetType = 0x09; // video
    packet.m_nChannel = 0x04;
    packet.m_headerType = RTMP_PACKET_SIZE_LARGE;
    packet.m_nTimeStamp = timestamp;
    packet.m_nInfoField2 = _rtmp->m_stream_id;
    packet.m_nBodySize = len;
    memcpy(packet.m_body, buff, len);

    int ret = RTMP_SendPacket(_rtmp, &packet, 0);

    if (ret == 0)
    {
        _avc_conf_send = true;
    }


}


void rtmp_conn::send_aac_config()
{

}

void rtmp_conn::send_video_packet(unsigned char* indata, int inlen, int timestamp, int bkey)
{



}

void rtmp_conn::send_packet(unsigned char type, unsigned char* buff, int len, int timestamp)
{
    if (_rtmp == NULL)
    {
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


