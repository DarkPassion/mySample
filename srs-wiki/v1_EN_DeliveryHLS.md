# Delivery HLS

SRS supports RTMP and HLS, the most popular delivery protocol for internet.

RTMP is Adobe RTMP(Realtime Message Protocol), for low latency live streaming, and the standard protocol for internet encoder, and the best protocol for flash on PC.

HLS is Apple HLS(Http Live Streaming), for both live and vod streaming over HTTP, and the standard protocol on Apple platform.

Server deliverying HLS and RTMP can support all screen. For RTMP, see: [Delivery RTMP](v1_EN_DeliveryRTMP)。

For information about compare RTMP and HLS, read [RTMP PK HLS](v1_EN_RTMP.PK.HTTP).

For information about how to deploy SRS to support HLS, read [Usage: HLS](v1_EN_SampleHLS).

## Use Scenario

The main use scenario of HLS:
* Cross Platform: Live streaming on PC is RTMP, and some as library can play HLS on flash right now. Android 3.0 can play HLS, and Apple always support HLS.
* Industrial Strength on Apple: The most stable live streaming protocol for OSX/IOS is HLS, similar the RTMP for flash.
* Friendly for CDN: The HLS over HTTP is friendly for CDN to delivery HLS.
* Simple: The HLS is open protocol and there are lots of tools for ts.

In a word, HLS is the best delivery protocol when user donot care about the latency, for both PC and mobile(Android and IOS).

## Delivering Streams

The table bellow describes the different protocols for PC and mobile platform.

| Delivery | Platform | Protocol | Inventor | Description |
|   ----- | -------- | --------- | -------- | ----------- |
| RTMP | Windows Flash | RTMP | Adobe | The RTMP is used to delivery low latency stream over internet, especially for the web explorer application for flash on PC. The RTMP allows you to publish stream to server very stable and for a long time.|
| HLS | Apple/<br/>Android | HTTP | Apple/<br/>Google | HLS lantency >= 10s. Android3+ supports HLS. All Apple platforms support HLS.|
| HDS | - | HTTP | Adobe | HDS is a protocol similar to HLS, develped by Adobe. HDS is complex and no benifits, so SRS never support it.|
| <a href='http://en.wikipedia.org/wiki/Dynamic_Adaptive_Streaming_over_HTTP'>DASH</a> | - | HTTP | - | Dash(Dynamic Adaptive Streaming over HTTP), developed by some company similar to HLS. It's a new protocol, and SRS maybe support it when it already used by many users. |

## HLS Introduction

HLS is a http m3u8 url, which can be play in Apple Safari directly. For example:

```bash
http://demo.srs.com/live/livestream.m3u8
```

The m3u8 url must embed in HTML5 for Android. For example:

```html
<!-- livestream.html -->
<video width="640" height="360"
        autoplay controls autobuffer 
        src="http://demo.srs.com/live/livestream.m3u8"
        type="application/vnd.apple.mpegurl">
</video>
```

The [m3u8](https://github.com/simple-rtmp-server/srs/blob/master/trunk/doc/hls-m3u8-draft-pantos-http-live-streaming-12.txt) of HLS is a play list actually. For example:

```bash
#EXTM3U
#EXT-X-VERSION:3
#EXT-X-MEDIA-SEQUENCE:64
#EXT-X-TARGETDURATION:12
#EXTINF:11.550
livestream-64.ts
#EXTINF:5.250
livestream-65.ts
#EXTINF:7.700
livestream-66.ts
#EXTINF:6.850
livestream-67.ts
```

The important data item can be configed in SRS config file:

* EXT-X-TARGETDURATION: It's calc by SRS automatically. The EXT-X-TARGETDURATION tag specifies the maximum media segment duration.  The EXTINF duration of each media segment in the Playlist file, when rounded to the nearest integer, MUST be less than or equal to the target duration.  This tag MUST appear once in a Media Playlist file.  It applies to the entire Playlist file.
* EXTINF: It's calc by SRS automatically, and the max value is configed in SRS `hls_fragment`. The EXTINF tag specifies the duration of a media segment.  It applies only to the media segment that follows it, and MUST be followed by a media segment URI.  Each media segment MUST be preceded by an EXTINF tag.
* Number of ts in m3u8: The `hls_window` in seconds specifies how many ts files in m3u8, and SRS will delete the old ts files.
* Name of ts filename: For example, `livestream-67.ts`, SRS will increase the number, the next ts for instance is `livestream-68.ts`.

For example, the `hls_fragment` is 10s, and the `hls_window` is 60s, then there about 60/10=6 ts files i m3u8, and the old ts files are automatically deleted by SRS.

## HLS Workflow

The workflow of HLS: 

1. Encoder, for example, FFMPEG or FMLE, publish RTMP stream to SRS, and the codec of stream must be H.264+AAC(Use transcode for other codecs).
1. SRS demux RTMP then mux mpegts and write to ts file, update the m3u8.
1. Client, for example, the IPhone or VLC, access m3u8 provides by any web server, for instance, SRS embeded HTTP server, or nginx.

Note: SRS only need to config the HLS on vhost, and SRS will create the dir by app name.

## HLS Config

the vhost `with-hls.vhost.com` in conf/full.conf of SRS is a example to config HLS, which user can copy the hls section to vhost:

```bash
vhost __defaultVhost__ {
    hls {
        # whether the hls is enabled.
        # if off, donot write hls(ts and m3u8) when publish.
        # default: off
        enabled         on;
        # the hls output path.
        # the app dir is auto created under the hls_path.
        # for example, for rtmp stream:
        #   rtmp://127.0.0.1/live/livestream
        #   http://127.0.0.1/live/livestream.m3u8
        # where hls_path is /hls, srs will create the following files:
        #   /hls/live       the app dir for all streams.
        #   /hls/live/livestream.m3u8   the HLS m3u8 file.
        #   /hls/live/livestream-1.ts   the HLS media/ts file.
        # in a word, the hls_path is for vhost.
        # default: ./objs/nginx/html
        hls_path        /data/nginx/html;
        # the hls fragment in seconds, the duration of a piece of ts.
        # default: 10
        hls_fragment    10;
        # the hls window in seconds, the number of ts in m3u8.
        # default: 60
        hls_window      60;
    }
}
```

The section `hls` is for HLS config:
* enabled: Whether enable HLS, on to enable, off to disable. Default: off.
* hls_path: The path to save m3u8 and ts file, where SRS will use app as dir and stream as ts file prefix. For example:
```bash
For RTMP stream: rtmp://localhost/live/livestream
HLS path: hls_path        /data/nginx/html;
SRS will generate below files:
/data/nginx/html/live/livestream.m3u8
/data/nginx/html/live/livestream-0.ts
/data/nginx/html/live/livestream-1.ts
/data/nginx/html/live/livestream-2.ts
And the HLS url to play: http://localhost/live/livestream.m3u8
```
* hls_fragment: The HLS duration in seconds. The actual duration of ts file is by:
```bash
TS duration(s) = max(hls_fragment, gop_size)
hls_fragment: The config seconds for ts file, for example, 5s.
gop_size: The stream gop size, for example, the fps is 20, gop is 200frames, then gop_size=gop/fps=10s.
So, the actual ts duration is max(5, 10)=10s, that is why the ts duration is larger than hls_fragment.
```
* hls_window: The total HLS windows size in seconds, the sum of all ts duration in m3u8. SRS will drop the old ts file in m3u8 and delete the file on fs. SRS will keep:
```bash
hls_window >= sum(each ts duration in m3u8)
```

How to deploy SRS to delivery HLS, read [Usage: HLS](v1_EN_SampleHLS)

## HLSAudioOnly

SRS supports to deliver pure audio stream by HLS. The audio codec requires AAC, user must transcode other codecs to aac, read [Usage: Transcode2HLS](v1_EN_SampleTranscode2HLS)

For information about drop video, read [Transcode: Disable Stream](v1_EN_FFMPEG#drop-video-or-audio)

There is no special config for pure audio for HLS. Please read  [Usage: HLS](v1_EN_SampleHLS)

## HLS and Forward

All stream publish by forward will output HLS when HLS enalbed.

## HLS and Transcode

User can use transcode to ensure the video codec is h.264 and audio codec is aac, because h.264+aac is required by HLS.

The below transcode config set the [gop](http://ffmpeg.org/ffmpeg-codecs.html#Options-7) to keep ts duration small:
```bash
vhost hls.transcode.vhost.com {
    transcode {
        enabled     on;
        ffmpeg      ./objs/ffmpeg/bin/ffmpeg;
        engine hls {
            enabled         on;
            vfilter {
            }
            vcodec          libx264;
            vbitrate        500;
            vfps            20;
            vwidth          768;
            vheight         320;
            vthreads        2;
            vprofile        baseline;
            vpreset         superfast;
            vparams {
                g           100;
            }
            acodec          libaacplus;
            abitrate        45;
            asample_rate    44100;
            achannels       2;
            aparams {
            }
            output          rtmp://127.0.0.1:[port]/[app]?vhost=[vhost]/[stream]_[engine];
        }
    }
}
```
The gop is 100/20=5s, where fps specified by vfps is 20, and gop frames specified by g is 100.

## HLS Multiple Bitrate

SRS does not support HLS multiple bitrate.

## HLS M3u8 Examples

### live.m3u8

```
#EXTM3U
#EXT-X-VERSION:3
#EXT-X-ALLOW-CACHE:NO
#EXT-X-TARGETDURATION:13
#EXT-X-MEDIA-SEQUENCE:430
#EXTINF:11.800
news-430.ts
#EXTINF:10.120
news-431.ts
#EXTINF:11.160
news-432.ts
```

### event.m3u8

```
#EXTM3U
#EXT-X-VERSION:3
#EXT-X-ALLOW-CACHE:NO
#EXT-X-TARGETDURATION:13
#EXT-X-MEDIA-SEQUENCE:430
#EXT-X-PLAYLIST-TYPE:EVENT
#EXTINF:11.800
news-430.ts
#EXTINF:10.120
news-431.ts
#EXTINF:11.160
news-432.ts
```

### vod.m3u8

```
#EXTM3U
#EXT-X-VERSION:3
#EXT-X-ALLOW-CACHE:NO
#EXT-X-TARGETDURATION:13
#EXT-X-MEDIA-SEQUENCE:430
#EXT-X-PLAYLIST-TYPE:VOD
#EXTINF:11.800
news-430.ts
#EXTINF:10.120
news-431.ts
#EXTINF:11.160
news-432.ts
#EXT-X-ENDLIST
```

### loop.m3u8

```
#EXTM3U
#EXT-X-VERSION:3
#EXT-X-ALLOW-CACHE:NO
#EXT-X-TARGETDURATION:13
#EXT-X-MEDIA-SEQUENCE:430
#EXT-X-PLAYLIST-TYPE:VOD
#EXTINF:11.800
news-430.ts
#EXTINF:10.120
news-431.ts
#EXT-X-DISCONTINUITY
#EXTINF:11.952
news-430.ts
#EXTINF:12.640
news-431.ts
#EXTINF:11.160
news-432.ts
#EXT-X-DISCONTINUITY
#EXTINF:11.751
news-430.ts
#EXTINF:2.040
news-431.ts
#EXT-X-ENDLIST
```

## SRS How to Support HLS

The ts write code of SRS is refer to nginx-rtmp, and add some comments according to ts specification.

For example:
```c++
// @see: ngx_rtmp_mpegts.c
// TODO: support full mpegts feature in future.
class SrsMpegtsWriter
{
public:
    static int write_frame(int fd, SrsMpegtsFrame* frame, SrsCodecBuffer* buffer)
    {
        int ret = ERROR_SUCCESS;
        
        if (!buffer->bytes || buffer->size <= 0) {
            return ret;
        }
        
        char* last = buffer->bytes + buffer->size;
        char* pos = buffer->bytes;
        
        bool first = true;
        while (pos < last) {
            static char packet[188];
            char* p = packet;
            
            frame->cc++;
            
            // sync_byte; //8bits
            *p++ = 0x47;
            // pid; //13bits
            *p++ = (frame->pid >> 8) & 0x1f;
            // payload_unit_start_indicator; //1bit
            if (first) {
                p[-1] |= 0x40;
            }
            *p++ = frame->pid;
            
            // transport_scrambling_control; //2bits
            // adaption_field_control; //2bits, 0x01: PayloadOnly
            // continuity_counter; //4bits
            *p++ = 0x10 | (frame->cc & 0x0f);
            
            if (first) {
                first = false;
                if (frame->key) {
                    p[-1] |= 0x20; // Both Adaption and Payload
                    *p++ = 7;    // size
                    *p++ = 0x50; // random access + PCR
                    p = write_pcr(p, frame->dts - SRS_HLS_DELAY);
                }
                
                // PES header
                // packet_start_code_prefix; //24bits, '00 00 01'
                *p++ = 0x00;
                *p++ = 0x00;
                *p++ = 0x01;
                //8bits
                *p++ = frame->sid;
                
                // pts(33bits) need 5bytes.
                u_int8_t header_size = 5;
                u_int8_t flags = 0x80; // pts
                
                // dts(33bits) need 5bytes also
                if (frame->dts != frame->pts) {
                    header_size += 5;
                    flags |= 0x40; // dts
                }
                
                // 3bytes: flag fields from PES_packet_length to PES_header_data_length
                int pes_size = (last - pos) + header_size + 3;
                if (pes_size > 0xffff) {
                    /**
                    * when actual packet length > 0xffff(65535),
                    * which exceed the max u_int16_t packet length,
                    * use 0 packet length, the next unit start indicates the end of packet.
                    */
                    pes_size = 0;
                }
                
                // PES_packet_length; //16bits
                *p++ = (pes_size >> 8);
                *p++ = pes_size;
                
                // PES_scrambling_control; //2bits, '10'
                // PES_priority; //1bit
                // data_alignment_indicator; //1bit
                // copyright; //1bit
                // original_or_copy; //1bit	
                *p++ = 0x80; /* H222 */
                
                // PTS_DTS_flags; //2bits
                // ESCR_flag; //1bit
                // ES_rate_flag; //1bit
                // DSM_trick_mode_flag; //1bit
                // additional_copy_info_flag; //1bit
                // PES_CRC_flag; //1bit
                // PES_extension_flag; //1bit
                *p++ = flags;
                
                // PES_header_data_length; //8bits
                *p++ = header_size;

                // pts; // 33bits
                p = write_pts(p, flags >> 6, frame->pts + SRS_HLS_DELAY);
                
                // dts; // 33bits
                if (frame->dts != frame->pts) {
                    p = write_pts(p, 1, frame->dts + SRS_HLS_DELAY);
                }
            }
            
            int body_size = sizeof(packet) - (p - packet);
            int in_size = last - pos;
            
            if (body_size <= in_size) {
                memcpy(p, pos, body_size);
                pos += body_size;
            } else {
                p = fill_stuff(p, packet, body_size, in_size);
                memcpy(p, pos, in_size);
                pos = last;
            }
            
            // write ts packet
            if (::write(fd, packet, sizeof(packet)) != sizeof(packet)) {
                ret = ERROR_HLS_WRITE_FAILED;
                srs_error("write ts file failed. ret=%d", ret);
                return ret;
            }
        }
        
        return ret;
    }
};
```

Winlin 2014.11
