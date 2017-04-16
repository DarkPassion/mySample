#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

typedef struct flv_context
{
    FILE* file;
    int pkt_frame_len;
    int pkt_pload_len;
    int pkt_type;
    int pkt_iframe_ts;
    int pkt_curr_ts;
    int pkt_frame;
} flv_context_t;

flv_context_t* alloc_flv_context()
{
    flv_context_t * ctx = (flv_context_t*)malloc(sizeof(flv_context_t));
    memset(ctx, 0, sizeof(flv_context_t));
    return ctx;
}

void free_flv_context(flv_context_t* flv)
{
    free(flv);
}



flv_context_t* ctx = NULL;

int read_flv_header(flv_context_t* c);
int read_flv_tag_header(flv_context_t* c);
int read_flv_tag_data(flv_context_t* c);


int read_flv_header(flv_context_t* ctx)
{

    ctx->pkt_pload_len = 9;
    char buf[64] = {0};
    int nread = fread(buf, ctx->pkt_pload_len, 1, ctx->file);
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

int read_flv_tag_header(flv_context_t* ctx)
{

    const int hds = 15;
    char buf[hds] = {0};
    int nread = fread(buf, hds, 1, ctx->file);
    if (nread == 0) {
        printf("fread EOF \n");
        return -1;
    }

    ctx->pkt_type = buf[4];

    ctx->pkt_pload_len = (uint32_t)((uint8_t)(buf[5]) << 16 | (uint8_t)(buf[6]) << 8 | (uint8_t)(buf[7]));
    int ts = ((uint8_t)(buf[8]) << 16 | (uint8_t)(buf[9]) << 8 | (uint8_t)buf[10] | (uint8_t)(buf[11]) << 24);

    if (ctx->pkt_curr_ts > ts) {
        printf("error pkt_ts > ts \n");
    }

    ctx->pkt_curr_ts = ts;
    //printf(" flv tag header need len %d ts %d  type %d \n", pkt_pload_len, pkt_curr_ts, pkt_type);


    return 0;
}


int read_flv_tag_data(flv_context_t* ctx)
{
    if (ctx->pkt_pload_len == 0) {
        printf("error need_len == 0 \n");
        return -1;
    }

    char* buf = (char*) malloc(ctx->pkt_pload_len);
    memset(buf, 0, ctx->pkt_pload_len);

    int nread = fread(buf, ctx->pkt_pload_len, 1, ctx->file);
    if (nread == 0) {
        printf("flv tag data EOF! \n");
        free(buf);
        return -1;
    }

    if (ctx->pkt_type == 0x09) {

        ctx->pkt_frame_len += ctx->pkt_pload_len;
        if (buf[0] == 0x17) {
            float fps = ctx->pkt_curr_ts - ctx->pkt_iframe_ts > 0 ? (ctx->pkt_frame) * 1000.0/ (ctx->pkt_curr_ts - ctx->pkt_iframe_ts)  : 0.0f;
            float kbps = (ctx->pkt_frame_len * 8 * 1000.0f ) / (1024 * (ctx->pkt_curr_ts - ctx->pkt_iframe_ts));
            printf("key frame [pkt_pload_len ts gop fps kbps] %d %d %d %.2f %.2f\n", ctx->pkt_pload_len, ctx->pkt_curr_ts, ctx->pkt_frame, fps, kbps);
            ctx->pkt_frame = 0;
            ctx->pkt_iframe_ts = ctx->pkt_curr_ts;
            ctx->pkt_frame_len = 0;
        } else if (buf[0] == 0x27) {
            ctx->pkt_frame++;
        } else {
            printf("flv vdieo frame type %x \n", buf[0]);
        }
    } else if (ctx->pkt_type == 0x08) {

    }

    free(buf);
    return 0;
}

void process_flv_file(const char* name)
{
    ctx = alloc_flv_context();
    char buff[1024] = {0};

    ctx->file = fopen(name, "rb");

    if (!ctx->file) {
        printf("process flv file fopen error ! %s \n", name);
        return ;
    }

    do {

        if (read_flv_header(ctx) != 0) {
            printf("read flv hreader error ! \n");
            break;
        }

        while (1) {

            if (read_flv_tag_header(ctx) != 0) {
                printf(" read flv tag header error !\n");
                break;
            }

            if (read_flv_tag_data(ctx) != 0) {
                printf(" read flv tag data error ! \n");
                break;
            }

        }
    } while(0);


    fclose(ctx->file);
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
