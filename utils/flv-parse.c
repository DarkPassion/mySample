#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

int pkt_frame_len = 0;
int pkt_pload_len = 0;
int pkt_type = 0;
int pkt_iframe_ts = 0;
int pkt_curr_ts = 0;
int pkt_frame = 0;

int read_flv_header(FILE* f);
int read_flv_tag_header(FILE* f);
int read_flv_tag_data(FILE* f);


int read_flv_header(FILE* f)
{

    pkt_pload_len = 9;
    char buf[64] = {0};
    int nread = fread(buf, pkt_pload_len, 1, f);
    if (nread == 0) {
        printf("read flv hreader EOF \n");
        return -1;
    }

    if (buf[0] != 'F' || buf[1] != 'L' || buf[2] != 'V') {
        printf("FLV error 100 \n");
        return -1;
    }
    return 0;
}

int read_flv_tag_header(FILE* f)
{

    const int hds = 15;
    char buf[hds] = {0};
    int nread = fread(buf, hds, 1, f);
    if (nread == 0) {
        printf("fread EOF \n");
        return -1;
    }

    pkt_type = buf[4];

    pkt_pload_len = (uint32_t)((uint8_t)(buf[5]) << 16 | (uint8_t)(buf[6]) << 8 | (uint8_t)(buf[7]));
    int ts = ((uint8_t)(buf[8]) << 16 | (uint8_t)(buf[9]) << 8 | (uint8_t)buf[10] | (uint8_t)(buf[11]) << 24);

    if (pkt_curr_ts > ts) {
        printf("error pkt_ts > ts \n");
    }

    pkt_curr_ts = ts;
    //printf(" flv tag header need len %d ts %d  type %d \n", pkt_pload_len, pkt_curr_ts, pkt_type);


    return 0;
}


int read_flv_tag_data(FILE* f)
{
    if (pkt_pload_len == 0) {
        printf("error need_len == 0 \n");
        return -1;
    }

    char* buf = (char*) malloc(pkt_pload_len);
    memset(buf, 0, pkt_pload_len);

    int nread = fread(buf, pkt_pload_len, 1, f);
    if (nread == 0) {
        printf("flv tag data EOF! \n");
        free(buf);
        return -1;
    }

    if (pkt_type == 0x09) {

        pkt_frame_len += pkt_pload_len;
        if (buf[0] == 0x17) {
            float fps = pkt_curr_ts - pkt_iframe_ts > 0 ? (pkt_frame) * 1000.0/ (pkt_curr_ts - pkt_iframe_ts)  : 0.0f;
            float kbps = (pkt_frame_len * 8 * 1000.0f ) / (1024 * (pkt_curr_ts - pkt_iframe_ts));
            printf("key frame [pkt_pload_len ts gop fps kbps] %d %d %d %.2f %.2f\n", pkt_pload_len, pkt_curr_ts, pkt_frame, fps, kbps);
            pkt_frame = 0;
            pkt_iframe_ts = pkt_curr_ts;
            pkt_frame_len = 0;
        } else if (buf[0] == 0x27) {
            pkt_frame++;
        } else {
            printf("flv vdieo frame type %x \n", buf[0]);
        }
    } else if (pkt_type == 0x08) {

    }

    free(buf);
    return 0;
}

void process_flv_file(const char* name)
{
    char buff[1024] = {0};

    FILE* file = fopen(name, "rb");

    if (!file) {
        printf("process flv file fopen error ! %s \n", name);
        return ;
    }

    do {

        if (read_flv_header(file) != 0) {
            printf("read flv hreader error ! \n");
            break;
        }

        while (1) {

            if (read_flv_tag_header(file) != 0) {
                printf(" read flv tag header error !\n");
                break;
            }

            if (read_flv_tag_data(file) != 0) {
                printf(" read flv tag data error ! \n");
                break;
            }

        }
    } while(0);


    fclose(file);
}

int main(int argc, char** argv)
{

    if (argc < 2) {
        printf("error %s *.flv \n", argv[0]);
        return 0;
    }

    process_flv_file(argv[1]);

    return 0;
}
