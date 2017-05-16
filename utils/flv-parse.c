#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>


// https://github.com/rainfly123/flvmuxer/blob/master/flv.c
// gcc flv-parse.c -o flv-parse.out


#define MAX_FRAMES 2<<20
#define MAX_BUFF_LEN 256

typedef struct pkt_frame {
    int pkt_type;
    int pkt_pts;
    int pkt_len;
    int pkt_is_iframe;
    char pkt_sps_buf[MAX_BUFF_LEN];
    int pkt_sps_len;
    char pkt_pps_buf[MAX_BUFF_LEN];
    int pkt_pps_len;
} pkt_frame_t;


typedef struct flv_context {
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

void hexDump (char *desc, void *addr, int len);
void process_flv_file(const char* name);
int read_flv_header(flv_context_t* c);
int read_flv_tag_header(flv_context_t* c);
int read_flv_tag_data(flv_context_t* c);
void dump_flv_context(flv_context_t* c);
uint8_t set_aac_configure(int rate, int channels);
void get_aac_configure(uint8_t c);

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
    ctx->reserved_len = 15;
    char buf[64] = {0};
    int nread = fread(buf, ctx->reserved_len, 1, ctx->file);
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
            if (buf[1] == 0x00) {
                // sequence header
                hexDump("H264 sequence header", buf, frame->pkt_len);
                uint8_t* sps = (uint8_t*)(buf + 11);
                frame->pkt_sps_len = sps[0] << 8 | sps[1];
                if (frame->pkt_sps_len > MAX_BUFF_LEN -1) {
                    printf("sps buff too long! maybe error frame->pkt_sps_len! \n");
                }
                memcpy(frame->pkt_sps_buf, sps + 2, frame->pkt_sps_len);

                uint8_t* pps = (uint8_t*)(sps + 2 + frame->pkt_sps_len + 1);
                frame->pkt_pps_len =(uint8_t) pps[0] << 8 | (uint8_t)pps[1];
                if (frame->pkt_pps_len > MAX_BUFF_LEN -1) {
                    printf("pps buff too long! maybe error frame->pkt_pps_len! \n");
                }
                memcpy(frame->pkt_pps_buf, pps + 2, frame->pkt_pps_len);
                printf("got sequence header == %d %d \n", frame->pkt_sps_len, frame->pkt_pps_len);
            } else if (buf[1] == 0x01) {
                uint8_t* data = (uint8_t*) buf + 5;
                int len = frame->pkt_len - 5;
                int i = 0;
                while (i < len) {
                    uint32_t l = data[i + 0] << 24 | data[i + 1] << 16 | data[i + 2] << 8 | data[i + 3];
                    printf("== keyframes %d %d %d %02x \n", i, l, len, data[i+4]);
                    i += (l + 4);
                }
            }
        } else if (buf[0] == 0x27) {
            frame->pkt_is_iframe = 0;
            if (buf[1] == 0x01) {
                uint8_t* data = (uint8_t*) buf + 5;
                int len = frame->pkt_len - 5;
                int i = 0;
                while (i < len) {
                    uint32_t l = data[i + 0] << 24 | data[i + 1] << 16 | data[i + 2] << 8 | data[i + 3];
                    // printf("== pframe %d %d %d %02x \n", i, l, len, data[i+4]);
                    i += (l + 4);
                }
            }
        } else {
            printf("flv vdieo frame type %x \n", buf[0]);
        }
    } else if (frame->pkt_type == 0x08) {
        if (buf[1] == 0x00) {
            // AAC sequence header

            hexDump("AAC sequence header", buf, frame->pkt_len);
            printf("AAC sequence header %02x  %02x ! \n", buf[0] & 0xff,set_aac_configure(44100, 2));
            uint8_t* p = (uint8_t*)buf;
            get_aac_configure(p[0]);

        } else if (buf[1] == 0x01) {

        }

    }

    ctx->reserved_len = 0;
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
    for (int i = 0; i < ctx->pkt_index; i++) {
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

uint8_t set_aac_configure(int rate, int channels)
{
    static int chan[4] = {5500, 11000, 22000, 44100};
    int chan_index = 0;
    for (int i = 0; i < 4; i++) {
        if (chan[i] == rate) {
            chan_index = i;
            break;
        }
    }
    uint8_t conf = 0xa0 | (chan_index << 2) | 0x02 | (channels -1);
    return conf;
}

void get_aac_configure(uint8_t c)
{
    static int chan[4] = {5500, 11000, 22000, 44100};
    static int sound[2] = {8, 16};
    int chan_index = (c & 0x0c)>>2;
    int sound_index = (c & 0x02) >> 1;
    int sound_type = c & 0x01;
    printf(" get_aac_configure %02x %d %d %d \n", c, chan[chan_index], sound[sound_index], sound_type + 1);
}


void hexDump (char *desc, void *addr, int len)
{
    int i;
    unsigned char buff[17];
    unsigned char *pc = (unsigned char*)addr;

    // Output description if given.
    if (desc != NULL)
        printf ("%s:\n", desc);

    if (len == 0) {
        printf("  ZERO LENGTH\n");
        return;
    }
    if (len < 0) {
        printf("  NEGATIVE LENGTH: %i\n",len);
        return;
    }

    // Process every byte in the data.
    for (i = 0; i < len; i++) {
        // Multiple of 16 means new line (with line offset).

        if ((i % 16) == 0) {
            // Just don't print ASCII for the zeroth line.
            if (i != 0)
                printf ("  %s\n", buff);

            // Output the offset.
            printf ("  %04x ", i);
        }

        // Now the hex code for the specific character.
        printf (" %02x", pc[i]);

        // And store a printable ASCII character for later.
        if ((pc[i] < 0x20) || (pc[i] > 0x7e))
            buff[i % 16] = '.';
        else
            buff[i % 16] = pc[i];
        buff[(i % 16) + 1] = '\0';
    }

    // Pad out last line if not exactly 16 characters.
    while ((i % 16) != 0) {
        printf ("   ");
        i++;
    }

    // And print the final ASCII bit.
    printf ("  %s\n", buff);
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
