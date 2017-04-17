#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "libAACdec/include/aacdecoder_lib.h"
#include "libAACenc/include/aacenc_lib.h"

// gcc test-main.c -o test-main.out


int encode_aac(uint8_t* indata, int inlen, uint8_t*outdata, int* outlen);





int encode_aac(uint8_t* indata, int inlen, uint8_t*outdata, int* outlen)
{

    HANDLE_AACENCODER encoder = NULL;

    AACENC_ERROR error = aacEncOpen(&encoder, 0, 1);
    if (error != AACENC_OK) {
        printf("aacEncOpen error ! \n");
        return -1;
    }


    int aot = 2;
    error = aacEncoder_SetParam(encoder, AACENC_AOT, aot);
    if (error != AACENC_OK) {
        printf("aacEncoder_SetParam profile error ! \n");
        return -1;
    }

    int sample_rate = 44100;
    error = aacEncoder_SetParam(encoder, AACENC_SAMPLERATE, sample_rate);
    if (error != AACENC_OK) {
        printf("aacEncoder_SetParam rate error ! \n");
        return -1;
    }

    error = aacEncoder_SetParam(encoder, AACENC_CHANNELMODE, MODE_1);
    if (error != AACENC_OK) {
        printf("aacEncoder_SetParam channel error ! \n");
        return -1;
    }

    int kbps = 64*1024;
    error = aacEncoder_SetParam(encoder, AACENC_BITRATE, bkps);
    if (error != AACENC_OK) {
        printf("aacEncoder_SetParam bitrate error ! \n");
        return -1;
    }



    return 0;
}


int main()
{


    return 0;
}
