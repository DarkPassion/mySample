#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "aacdecoder_lib.h"
#include "aacenc_lib.h"



HANDLE_AACENCODER aac_encode_init();
void aac_encode_close(HANDLE_AACENCODER* handle);
int encode_aac(HANDLE_AACENCODER handle, uint8_t* indata, int inlen, uint8_t*outdata, int* outlen);


HANDLE_AACENCODER aac_encode_init()
{

    HANDLE_AACENCODER encoder = NULL;

    AACENC_ERROR error = aacEncOpen(&encoder, 0, 1);
    if (error != AACENC_OK) {
        printf("aacEncOpen error ! \n");
        return NULL;
    }


    int aot = 2;
    error = aacEncoder_SetParam(encoder, AACENC_AOT, aot);
    if (error != AACENC_OK) {
        printf("aacEncoder_SetParam profile error ! \n");
        return NULL;
    }

    int sample_rate = 44100;
    error = aacEncoder_SetParam(encoder, AACENC_SAMPLERATE, sample_rate);
    if (error != AACENC_OK) {
        printf("aacEncoder_SetParam rate error ! \n");
        return NULL;
    }

    error = aacEncoder_SetParam(encoder, AACENC_CHANNELMODE, MODE_1);
    if (error != AACENC_OK) {
        printf("aacEncoder_SetParam channel error ! \n");
        return NULL;
    }

    int kbps = 64*1024;
    error = aacEncoder_SetParam(encoder, AACENC_BITRATE, kbps);
    if (error != AACENC_OK) {
        printf("aacEncoder_SetParam bitrate error ! \n");
        return NULL;
    }

    error = aacEncoder_SetParam(encoder, AACENC_TRANSMUX, 2);
    if (error != AACENC_OK) {
        printf("aacEncoder_SetParam transmux error ! \n");
        return NULL;
    }

    error = aacEncoder_SetParam(encoder, AACENC_AFTERBURNER, 0);
    if (error != AACENC_OK) {
        printf("aacEncoder_SetParam AACENC_AFTERBURNER error ! \n");
        return NULL;
    }

    error = aacEncEncode(encoder, NULL, NULL, NULL, NULL);
    if (error != AACENC_OK) {
        printf("aacEncEncode error ! \n");
        return NULL;
    }

    AACENC_InfoStruct info;
    memset(&info, 0, sizeof(info));
    error = aacEncInfo(encoder, &info);
    if (error != AACENC_OK) {
        printf("aacEncInfo error ! \n");
        return NULL;
    }


    printf("framelen %d %d %d \n", info.frameLength, info.encoderDelay, info.confSize);
    for (int i = 0; i < info.confSize; i++) {
        uint8_t* p = (uint8_t*)info.confBuf;
        printf("%02x ", p[i] & 0xff);
    }
    return encoder;
}

void aac_encode_close(HANDLE_AACENCODER* handle)
{
    if (handle) {
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

    if (error != AACENC_OK) {
        printf("aacEncEncode error ! \n");
        return -1;
    }

    printf("\n aacEncode aac len %d \n", out_args.numOutBytes);
#if 1
    for (int i = 0; i < 7; i++) {
        printf(" %02x ", outdata[i] & 0xff);
    }

#endif

    return 0;
}

int main()
{

    HANDLE_AACENCODER handle = aac_encode_init();

    uint8_t indata[2048];
    int inlen = 2048;
    uint8_t outdata[2048] = {0};
    int outlen = 2048;
    encode_aac(handle, indata, inlen, outdata, &outlen);

    aac_encode_close(&handle);

    return 0;
}
