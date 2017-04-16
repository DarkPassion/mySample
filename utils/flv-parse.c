#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define MAX_FRAMES 2<<20


typedef struct pkt_frame
{
    int pkt_type;
    int pkt_pts;
    int pkt_len;
    int pkt_is_iframe;
    char pkt_sps_buf[256];
    int pkt_sps_len;
    char pkt_pps_buf[256];
    int pkt_pps_len;
} pkt_frame_t;


typedef struct flv_context
{
    FILE* file;
    pkt_frame_t* pkts[MAX_FRAMES];
    int reserved_len;
    int pkt_index;
} flv_context_t;



pkt_frame_t* alloc_pkt_frame()
{
    pkt_frame_t* frame = (pkt_frame_t*)malloc(sizeof(pkt_frame_t));
    memset(frame, 0, sizeof(pkt_frame_t));
    return frame;
}


flv_context_t* alloc_flv_context()
{
    flv_context_t * ctx = (flv_context_t*)malloc(sizeof(flv_context_t));
    memset(ctx, 0, sizeof(flv_context_t));
    return ctx;
}

void free_flv_context(flv_context_t* flv)
{
    for (int i = 0; i < flv->pkt_index; i++) {
        if (flv->pkts[i]) {
            free(flv->pkts[i]);
        }
    }
    free(flv);
}


pkt_frame_t* flv_context_get_frame(flv_context_t* ctx, int is_alloc)
{
    if (is_alloc) {
        pkt_frame_t* frame = alloc_pkt_frame();
        if (ctx->pkt_index >= 0 && ctx->pkt_index < MAX_FRAMES) {
            ctx->pkts[ctx->pkt_index] = frame;
            ctx->pkt_index++;
            return frame;
        }
    } else {
        if (ctx->pkt_index - 1 >= 0 && ctx->pkt_index -1 < MAX_FRAMES && ctx->pkts[ctx->pkt_index -1] != NULL) {
            pkt_frame_t* frame = ctx->pkts[ctx->pkt_index -1];
            return frame;
        }
    }
    return NULL;
}

void process_flv_file(const char* name);
int read_flv_header(flv_context_t* c);
int read_flv_tag_header(flv_context_t* c);
int read_flv_tag_data(flv_context_t* c);
void dump_flv_context(flv_context_t* c);

int read_flv_header(flv_context_t* ctx)
{

    ctx->reserved_len = 9;
    char buf[64] = {0};
    int nread = fread(buf, ctx->reserved_len, 1, ctx->file);
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

    pkt_frame_t* frame = flv_context_get_frame(ctx, 1);
    frame->pkt_type = buf[4];

    frame->pkt_pts = ((uint8_t)(buf[8]) << 16 | (uint8_t)(buf[9]) << 8 | (uint8_t)buf[10] | (uint8_t)(buf[11]) << 24);
    ctx->reserved_len = (uint32_t)((uint8_t)(buf[5]) << 16 | (uint8_t)(buf[6]) << 8 | (uint8_t)(buf[7]));

    // printf(" flv tag header need len %d ts %d  type %d \n", ctx->reserved_len, frame->pkt_pts, frame->pkt_type);


    return 0;
}


int read_flv_tag_data(flv_context_t* ctx)
{
    if (ctx->reserved_len == 0) {
        printf("error need_len == 0 \n");
        return -1;
    }

    char* buf = (char*) malloc(ctx->reserved_len);
    memset(buf, 0, ctx->reserved_len);

    int nread = fread(buf, ctx->reserved_len, 1, ctx->file);
    if (nread == 0) {
        printf("flv tag data EOF! \n");
        free(buf);
        return -1;
    }

    pkt_frame_t* frame = flv_context_get_frame(ctx, 0);
    if (!frame) {
        printf("error flv_context_get_frame ctx 0  \n");
        return -1;
    }

    frame->pkt_len = ctx->reserved_len;
    if (frame->pkt_type == 0x09) {

        if (buf[0] == 0x17) {
            frame->pkt_is_iframe = 1;
       } else if (buf[0] == 0x27) {
           frame->pkt_is_iframe = 0;
        } else {
            printf("flv vdieo frame type %x \n", buf[0]);
        }
    } else if (frame->pkt_type == 0x08) {

    }

    free(buf);
    return 0;
}

void process_flv_file(const char* name)
{
    flv_context_t* ctx = alloc_flv_context();
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


    dump_flv_context(ctx);
    fclose(ctx->file);
    free_flv_context(ctx);
}

void dump_flv_context(flv_context_t* ctx)
{
    printf("dump flv context ! \n");

    if (!ctx) {
        printf("dump flv context error ! \n");
        return ;
    }

    int pkt_pframe_pts = 0;
    int pkt_len = 0;
    int pkt_pframes = 0;
    for (int i = 0; i < ctx->pkt_index; i++)
    {
        pkt_frame_t* frame = ctx->pkts[i];

        //printf("== dump flv context frame %p %d %d \n", frame, i, ctx->pkt_index);
        if (frame->pkt_type == 0x08) {

        } else if (frame->pkt_type == 0x09) {

            pkt_len += frame->pkt_len;
            //printf("dump flv context frame %d %d %d %d \n", frame->pkt_pts, frame->pkt_is_iframe, frame->pkt_type, frame->pkt_len);
            if (frame->pkt_is_iframe) {
                float kbps = frame->pkt_pts - pkt_pframe_pts > 0 ? (pkt_len)*1000.0*8.0/((1024)*(frame->pkt_pts-pkt_pframe_pts)) : 0.0f;
                printf("flv H264 gop %d  kbps %.2f\n", pkt_pframes, kbps);
                pkt_pframes = 0;
                pkt_pframe_pts = frame->pkt_pts;
                pkt_len = 0;
            } else {
                pkt_pframes++;
            }

        }
    }
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
