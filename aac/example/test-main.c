#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "aacdecoder_lib.h"
#include "aacenc_lib.h"



HANDLE_AACENCODER aac_encode_init(int he_aac);
void aac_encode_close(HANDLE_AACENCODER* handle);
int encode_aac(HANDLE_AACENCODER handle, uint8_t* indata, int inlen, uint8_t*outdata, int* outlen);

HANDLE_AACDECODER aac_decode_init();
void aac_decode_close(HANDLE_AACDECODER handle);
int decode_aac(HANDLE_AACDECODER handle, uint8_t* indata, int inlen, uint8_t*outdata, int* outlen);

void fill_random_value(uint8_t* indata, int len);
void write_file(uint8_t* data, int len, const char* name);
void parse_adts(uint8_t* data, int len);

HANDLE_AACENCODER aac_encode_init(int he_aac)
{

    HANDLE_AACENCODER encoder = NULL;

    AACENC_ERROR error = aacEncOpen(&encoder, 0, 2);
    if (error != AACENC_OK)
    {
        printf("aacEncOpen error ! \n");
        return NULL;
    }


    int aot = 2;
    if (he_aac)
    {
        aot = 5; // HE-AAC
        //aot = 29; // HE-AAC v2
    }

    error = aacEncoder_SetParam(encoder, AACENC_AOT, aot);
    if (error != AACENC_OK)
    {
        printf("aacEncoder_SetParam profile error ! \n");
        return NULL;
    }

    int sample_rate = 44100;
    int sce = 1;
    int cpe = 0;
    int kbps = ((96*sce + 128*cpe) * sample_rate / 44);
    if (he_aac)
    {
        kbps = kbps / 2;
    }

    error = aacEncoder_SetParam(encoder, AACENC_SAMPLERATE, sample_rate);
    if (error != AACENC_OK)
    {
        printf("aacEncoder_SetParam rate error ! \n");
        return NULL;
    }

    CHANNEL_MODE mode = MODE_1;
    if (he_aac)
    {
        mode = MODE_2;
    }
    error = aacEncoder_SetParam(encoder, AACENC_CHANNELMODE, mode);
    if (error != AACENC_OK)
    {
        printf("aacEncoder_SetParam channel error ! \n");
        return NULL;
    }

    error = aacEncoder_SetParam(encoder, AACENC_CHANNELORDER, 1);
    if (error != AACENC_OK)
    {
        printf("aacEncoder_SetParam AACENC_CHANNELORDER error ! \n");
        return NULL;
    }

    error = aacEncoder_SetParam(encoder, AACENC_BITRATE, kbps);
    if (error != AACENC_OK)
    {
        printf("aacEncoder_SetParam bitrate error ! \n");
        return NULL;
    }

    error = aacEncoder_SetParam(encoder, AACENC_TRANSMUX, 2);
    if (error != AACENC_OK)
    {
        printf("aacEncoder_SetParam transmux error ! \n");
        return NULL;
    }

    error = aacEncoder_SetParam(encoder, AACENC_SIGNALING_MODE, 0);
    if (error != AACENC_OK)
    {
        printf("aacEncoder_SetParam AACENC_SIGNALING_MODE error ! \n");
        return NULL;
    }

    error = aacEncoder_SetParam(encoder, AACENC_AFTERBURNER, 0);
    if (error != AACENC_OK)
    {
        printf("aacEncoder_SetParam AACENC_AFTERBURNER error ! \n");
        return NULL;
    }

    error = aacEncEncode(encoder, NULL, NULL, NULL, NULL);
    if (error != AACENC_OK)
    {
        printf("aacEncEncode error ! 0x%04x \n", error);
        return NULL;
    }

    AACENC_InfoStruct info;
    memset(&info, 0, sizeof(info));
    error = aacEncInfo(encoder, &info);
    if (error != AACENC_OK)
    {
        printf("aacEncInfo error ! \n");
        return NULL;
    }


    printf("framelen %d %d %d \n", info.frameLength, info.encoderDelay, info.confSize);
    for (int i = 0; i < info.confSize; i++)
    {
        uint8_t* p = (uint8_t*)info.confBuf;
        printf("%02x ", p[i] & 0xff);
    }
    return encoder;
}

void aac_encode_close(HANDLE_AACENCODER* handle)
{
    if (handle)
    {
        aacEncClose(handle);
    }

}


int encode_aac(HANDLE_AACENCODER handle, uint8_t* indata, int inlen, uint8_t*outdata, int* outlen)
{
    AACENC_ERROR error = AACENC_OK;

    AACENC_BufDesc in_buf = {0}, out_buf = {0};
    AACENC_InArgs in_args = {0};
    AACENC_OutArgs out_args = {0};
    int in_buffer_identifier = IN_AUDIO_DATA;
    int out_buffer_identifier = OUT_BITSTREAM_DATA;
    int in_buffer_size = inlen;
    int in_buffer_element_size = 2;
    int out_buffer_element_size = 1;

    in_args.numInSamples = inlen/2;
    in_buf.numBufs = 1;
    in_buf.bufs =(void**) &indata;
    in_buf.bufferIdentifiers = &in_buffer_identifier;
    in_buf.bufSizes = &in_buffer_size;
    in_buf.bufElSizes = &in_buffer_element_size;

    out_buf.numBufs = 1;
    out_buf.bufs = (void**)&outdata;
    out_buf.bufferIdentifiers = &out_buffer_identifier;
    out_buf.bufSizes = outlen;
    out_buf.bufElSizes = &out_buffer_element_size;

    error = aacEncEncode(handle, &in_buf, &out_buf, &in_args, &out_args);

    if (error != AACENC_OK)
    {
        printf("aacEncEncode error ! \n");
        return -1;
    }

    printf("\n aacEncode aac len %d \n", out_args.numOutBytes);
#if 1
    for (int i = 0; i < 7; i++)
    {
        printf(" %02x ", outdata[i] & 0xff);
    }
#endif

    return 0;
}

HANDLE_AACDECODER aac_decode_init()
{

    HANDLE_AACDECODER handle = NULL;
    handle = aacDecoder_Open(TT_MP4_ADTS, 1);

    return handle;
}


void aac_decode_close(HANDLE_AACDECODER handle)
{
    aacDecoder_Close(handle);
}


int decode_aac(HANDLE_AACDECODER handle, uint8_t* indata, int inlen, uint8_t*outdata, int* outlen)
{

    AAC_DECODER_ERROR error;
    UINT valid = inlen;
    error = aacDecoder_Fill(handle, &indata, (unsigned int*)&inlen, &valid);
    if (error != AAC_DEC_OK)
    {
        printf("aacDecoder_Fill error ! \n");
        return -1;
    }

    error = aacDecoder_DecodeFrame(handle, (short*)outdata, *outlen, 0);
    if (error == AAC_DEC_NOT_ENOUGH_BITS)
    {
        printf("aacDecoder_DecodeFrame == error == 0! \n");
        return -1;
    }

    if (error != AAC_DEC_OK)
    {
        printf("aacDecoder_DecodeFrame == error == 1! \n");
        return -1;
    }

    CStreamInfo *info = aacDecoder_GetStreamInfo(handle);
    printf("decode aac %d \n", info->frameSize*2);
    return 0;
}



void fill_random_value(uint8_t* indata, int len)
{
    int i = 0;
    while (i < len)
    {
        int val = rand();

        indata[i] = val&0xff;
        i++;
    }
}


void write_file(uint8_t* data, int len, const char* name)
{
    FILE* f = fopen(name, "wb");
    if (f)
    {
        int nwrite = fwrite(data, len, 1, f);
        if (nwrite == 0)
        {
            printf("write to file fwrite error ! \n");
        }
        fclose(f);
    }
}

void parse_adts(uint8_t* data, int len)
{

    if (data == NULL && len < 7)
    {
        printf("parse adts error ! \n");
        return ;
    }

    if (data[0] != 0xff || (data[1] &0xf0) != 0xf0)
    {
        printf("parse adts error syncword oxFF ! \n");
        return ;
    }

    printf(" \n");
    for (int i = 0; i < len; i++)
    {
        printf(" %02x ", data[i]);
    }
    printf(" \n");

    uint8_t profile = (((data[2] &0xc0) >> 6) -1);
    uint8_t fequency = ((data[2]&0x3c)>>2);
    uint8_t channel = ((data[2]&0x01)<<2)|((data[3]&0xc0)>>6);

    printf("parse adts profile %d fequency %d channel %d \n", profile, fequency, channel);
}

int main()
{
    srand(131);

    //HANDLE_AACENCODER enc = aac_encode_init(0);
    HANDLE_AACENCODER enc = aac_encode_init(1);
    HANDLE_AACDECODER dec = aac_decode_init();

    for (int i = 0; i < 1; i++)
    {
        uint8_t indata[8192];
        int inlen = 8192;
        uint8_t outdata[2048] = {0};
        int outlen = 2048;

        fill_random_value(indata, inlen);

        int ret = encode_aac(enc, indata, inlen, outdata, &outlen);

        parse_adts(outdata, 7);

        write_file(outdata, outlen, "123.aac");
        if (ret == 0 && 0)
        {
            uint8_t tmp_data[2048];
            int tmp_len = 2048;
            ret = decode_aac(dec, outdata, outlen, tmp_data, &tmp_len);
            if (ret == 0 && tmp_len > 0)
            {
                printf("decode aac succ %d \n", tmp_len);
            }
        }

    }

    aac_encode_close(&enc);
    aac_decode_close(dec);
    return 0;
}
