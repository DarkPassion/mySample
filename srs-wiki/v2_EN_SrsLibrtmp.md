# SRS librtmp

librtmp is a client side library, seems from rtmpdump.

## Use Scenarios

The use scenarios of librtmp:
* Play or suck RTMP stream: For example rtmpdump, dvr RTMP stream to flv file.
* Publish RTMP stream: Publish RTMP stream to server.
* Use sync block socket: It's ok for client.
* ARM: Can used for linux arm, for some embed device, to publish stream to server.
* Publish h.264 raw stream: SRS2.0 supports this feature, read [publish-h264-raw-data](v2_EN_SrsLibrtmp#publish-h264-raw-data)

Note: About the openssl, complex and simple handshake, read [RTMP protocol](v1_EN_RTMPHandshake)

Note: To cross build srs-librtmp for ARM cpu, read [srs-arm](v1_EN_SrsLinuxArm)

## librtmp For Server

librtmp or srs-librtmp only for client side application, impossible for server side.

## Why SRS provides srs-librtmp

SRS provides different librtmp:
* The code of librtmp is hard to maintain.
* The interface of librtmp is hard to use.
* No example, while srs-librtmp provides lots of examples at trunk/research/librtmp.
* Min depends, SRS extract core/kernel/rtmp modules for srs-librtmp.
* Min library requires, srs-librtmp only depends on stdc++.
* NO ST, srs-librtmp does not depends on st.
* Provides bandwidth api, to get the bandwidth data to server, read [Bandwidth Test](v1_EN_BandwidthTestTool)
* Provides tracable log, to get the information on server of client, read [Tracable log](v1_EN_SrsLog)
* Supports directly publish h.264 raw stream, read [publish-h264-raw-data](v2_EN_SrsLibrtmp#publish-h264-raw-data)
* Exports SRS to srs-librtmp as single project which can be make to .h and .a, or exports SRS to a single .h and .cpp file, read [export srs librtmp](v2_EN_SrsLibrtmp#export-srs-librtmp)

In a word, SRS provides more efficient and simple client library srs-librtmp.

## Export Srs Librtmp

SRS2.0 provides options for configure to export srs-librtmp to single project to make .h and .a, or to single .h and .cpp file.

Usage for export project:

```
dir=/home/winlin/srs-librtmp &&
rm -rf $dir &&
./configure --export-librtmp-project=$dir &&
cd $dir && make &&
./objs/research/librtmp/srs_play rtmp://ossrs.net/live/livestream
```

SRS export srs-librtmp project can be make to .h and .a, and compile all release/librtmp examples.

SRS can also export srs-librtmp to a single .h and .cpp file, and generate a simple example:

```
dir=/home/winlin/srs-librtmp &&
rm -rf $dir &&
./configure --export-librtmp-single=$dir &&
cd $dir && gcc example.c srs_librtmp.cpp -g -O0 -lstdc++ -o example && 
strip example && ./example
```

Note: The export librtmp support both relative and absolute dir path.

## Build srs-librtmp

When make SRS, the srs-librtmp will auto generated when configure with librtmp:

```bash
./configure --with-librtmp --without-ssl && make
```

All examples are built, read [Examples](v2_EN_SrsLibrtmp#srs-librtmp-examples).

<strong>Note: Recomment to disable ssl, for librtmp does not depends on ssl.</strong>

<strong>Note: srs-librtmp provides only simple handshake, without complex handshake, eventhough configure with ssl.</strong>

When build ok, user can use .h and .a library to build client application.

## Build srs-librtmp on Windows

srs-librtmp only depends on libc++, so can be build on windows.

SRS 2.0 can export srs-librtmp to single project, or a .h and a .cpp file, read [export srs librtmp](v2_EN_SrsLibrtmp#export-srs-librtmp).

Need to port some linux header files.

Note: Donot need ssl and st.

## RTMP packet specification

This section descrips the RTMP packet specification, for the srs-librtmp api to read or write data.

The api about data:
* Read RTMP packet from server: int srs_read_packet(int* type, u_int32_t* timestamp, char** data, int* size)
* Write RTMP packet to server: int srs_write_packet(int type, u_int32_t timestamp, char* data, int size)
* Write h.264 raw data to server, read [publish-h264-raw-data](v2_EN_SrsLibrtmp#publish-h264-raw-data)

The RTMP packet(char* data) for api, is format in flv Video/Audio, read the trunk/doc [video_file_format_spec_v10_1.pdf](https://raw.github.com/winlinvip/simple-rtmp-server/master/trunk/doc/video_file_format_spec_v10_1.pdf)
* Audio data, read `E.4.2.1 AUDIODATA`，p76, for example, the aac codec audio data.
* Video data, read  `E.4.3.1 VIDEODATA`，p78, for example, the h.264 video data.
* Script data, read `E.4.4.1 SCRIPTDATA`，p80, for example, onMetadata call.

The RTMP packet type(int type) defines (in `E.4.1 FLV Tag`，page 75)：
* Audio: 8, the macro SRS_RTMP_TYPE_AUDIO
* Video: 9, the macro SRS_RTMP_TYPE_VIDEO
* Script: 18, the macro SRS_RTMP_TYPE_SCRIPT

Other parameters, for instance, the timestamp, pass by args.

About the flv specification:
* flv header format: `E.2 The FLV header`，p74。
* flv body format: `E.3 The FLV File Body`，p74。
* tag tag header: `E.4.1 FLV Tag`，p75。

Why use flv format as srs-librtmp api data format:
* Flv is simple enough.
* FFMPEG also use flv for rtmp.
* Only need to add flv tag header, then we can write to flv file.
* When publish flv file to server, only need to parse the tag header, the tag body is the data.

## Publish H.264 Raw Data

srs-librtmp provides api to publish h.264 raw stream to RTMP server.

Please read http://blog.csdn.net/win_lin/article/details/41170653

When convert h.264 raw stream to RTMP packet:

1. The h.264 raw stream does not specifies the dts and pts, which is calculated by encoder.
1. The RTMP sequence header, always sent in the first video packet.
1. The RTMP-packet = 5bytes(RTMP-header) + h.264-header + h.264-NALU-data. Refer to SrsAvcAacCodec::video_avc_demux
1. The srs-librtmp provides api to directly send h.264 raw stream, while the raw stream should starts with annexb header N[00] 00 00 01, where N>=0.

The api of srs-librtmp to send h.264 raw stream:

```
/**
* write h.264 raw frame over RTMP to rtmp server.
* @param frames the input h264 raw data, encoded h.264 I/P/B frames data.
*       frames can be one or more than one frame,
*       each frame prefixed h.264 annexb header, by N[00] 00 00 01, where N>=0, 
*       for instance, frame = header(00 00 00 01) + payload(67 42 80 29 95 A0 14 01 6E 40)
*       about annexb, @see H.264-AVC-ISO_IEC_14496-10.pdf, page 211.
* @paam frames_size the size of h264 raw data. 
*       assert frames_size > 0, at least has 1 bytes header.
* @param dts the dts of h.264 raw data.
* @param pts the pts of h.264 raw data.
* 
* @remark, user should free the frames.
* @remark, the tbn of dts/pts is 1/1000 for RTMP, that is, in ms.
* @remark, cts = pts - dts
* 
* @return 0, success; otherswise, failed.
*/
extern int srs_h264_write_raw_frames(srs_rtmp_t rtmp, 
    char* frames, int frames_size, u_int32_t dts, u_int32_t pts
);
```

For the sample h.264 file: http://winlinvip.github.io/srs.release/3rdparty/720p.h264.raw

The data is:

```
// SPS
000000016742802995A014016E40
// PPS
0000000168CE3880
// IFrame
0000000165B8041014C038008B0D0D3A071.....
// PFrame
0000000141E02041F8CDDC562BBDEFAD2F.....
```

The sps and pps can be sent togother, or not:

```
// SPS+PPS
srs_h264_write_raw_frame('000000016742802995A014016E400000000168CE3880', size, dts, pts)
// IFrame
srs_h264_write_raw_frame('0000000165B8041014C038008B0D0D3A071......', size, dts, pts)
// PFrame
srs_h264_write_raw_frame('0000000141E02041F8CDDC562BBDEFAD2F......', size, dts, pts)
```

The NALU can be sent together, or not:

```
// SPS
srs_h264_write_raw_frame('000000016742802995A014016E4', size, dts, pts)
// PPS
srs_h264_write_raw_frame('00000000168CE3880', size, dts, pts)
// IFrame
srs_h264_write_raw_frame('0000000165B8041014C038008B0D0D3A071......', size, dts, pts)
// PFrame
srs_h264_write_raw_frame('0000000141E02041F8CDDC562BBDEFAD2F......', size, dts, pts) 
```

About the api, read https://github.com/simple-rtmp-server/srs/issues/66#issuecomment-62240521

About to use the api, read https://github.com/simple-rtmp-server/srs/issues/66#issuecomment-62245512

## Publish Audio Raw Stream

srs-librtmp provides api to publish raw audio frame to SRS, and supports aac adts format stream.

The api:

```
/**
* write an audio raw frame to srs.
* not similar to h.264 video, the audio never aggregated, always
* encoded one frame by one, so this api is used to write a frame.
*
* @param sound_format Format of SoundData. The following values are defined:
*               0 = Linear PCM, platform endian
*               1 = ADPCM
*               2 = MP3
*               3 = Linear PCM, little endian
*               4 = Nellymoser 16 kHz mono
*               5 = Nellymoser 8 kHz mono
*               6 = Nellymoser
*               7 = G.711 A-law logarithmic PCM
*               8 = G.711 mu-law logarithmic PCM
*               9 = reserved
*               10 = AAC
*               11 = Speex
*               14 = MP3 8 kHz
*               15 = Device-specific sound
*               Formats 7, 8, 14, and 15 are reserved.
*               AAC is supported in Flash Player 9,0,115,0 and higher.
*               Speex is supported in Flash Player 10 and higher.
* @param sound_rate Sampling rate. The following values are defined:
*               0 = 5.5 kHz
*               1 = 11 kHz
*               2 = 22 kHz
*               3 = 44 kHz
* @param sound_size Size of each audio sample. This parameter only pertains to
*               uncompressed formats. Compressed formats always decode
*               to 16 bits internally.
*               0 = 8-bit samples
*               1 = 16-bit samples
* @param sound_type Mono or stereo sound
*               0 = Mono sound
*               1 = Stereo sound
* @param timestamp The timestamp of audio.
*
* @example /trunk/research/librtmp/srs_aac_raw_publish.c
* @example /trunk/research/librtmp/srs_audio_raw_publish.c
*
* @remark for aac, the frame must be in ADTS format. 
*       @see aac-mp4a-format-ISO_IEC_14496-3+2001.pdf, page 75, 1.A.2.2 ADTS
* @remark for aac, only support profile 1-4, AAC main/LC/SSR/LTP,
*       @see aac-mp4a-format-ISO_IEC_14496-3+2001.pdf, page 23, 1.5.1.1 Audio object type
*
* @see https://github.com/simple-rtmp-server/srs/issues/212
* @see E.4.2.1 AUDIODATA of video_file_format_spec_v10_1.pdf
* 
* @return 0, success; otherswise, failed.
*/
extern int srs_audio_write_raw_frame(srs_rtmp_t rtmp, 
    char sound_format, char sound_rate, char sound_size, char sound_type,
    char* frame, int frame_size, u_int32_t timestamp
);

/**
* whether aac raw data is in adts format,
* which bytes sequence matches '1111 1111 1111'B, that is 0xFFF.
* @param aac_raw_data the input aac raw data, a encoded aac frame data.
* @param ac_raw_size the size of aac raw data.
*
* @reamrk used to check whether current frame is in adts format.
*       @see aac-mp4a-format-ISO_IEC_14496-3+2001.pdf, page 75, 1.A.2.2 ADTS
* @example /trunk/research/librtmp/srs_aac_raw_publish.c
*
* @return 0 false; otherwise, true.
*/
extern srs_bool srs_aac_is_adts(char* aac_raw_data, int ac_raw_size);

/**
* parse the adts header to get the frame size,
* which bytes sequence matches '1111 1111 1111'B, that is 0xFFF.
* @param aac_raw_data the input aac raw data, a encoded aac frame data.
* @param ac_raw_size the size of aac raw data.
*
* @return failed when <=0 failed; otherwise, ok.
*/
extern int srs_aac_adts_frame_size(char* aac_raw_data, int ac_raw_size);
```

The example for bug [#212](https://github.com/simple-rtmp-server/srs/issues/212#issuecomment-63648892) is srs_audio_raw_publish.c and srs_aac_raw_publish.c, read [examples](v2_EN_SrsLibrtmp#srs-librtmp-examples).

About the api, read https://github.com/simple-rtmp-server/srs/issues/212#issuecomment-63755405

About to use the example, read https://github.com/simple-rtmp-server/srs/issues/212#issuecomment-64164018

## srs-librtmp Examples

The examples for srs-librtmp, automatically build when build SRS:
* research/librtmp/srs_play.c: Use srs-librtmp to play RTMP stream.
* research/librtmp/srs_publish.c: Use srs-librtmp to publish RTMP stream.
* research/librtmp/srs_ingest_flv.c: Use srs-librtmp to read local flv to publish as RTMP stream.
* research/librtmp/srs_ingest_rtmp.c: Use srs-librtmp to read RTMP then publish as RTMP stream.
* research/librtmp/srs_bandwidth_check.c: Use srs-librtmp to check bandwidth to server.
* research/librtmp/srs_flv_injecter.c: Use srs-librtmp to inject flv keyframes offset for flv vod stream.
* research/librtmp/srs_flv_parser.c: Use srs-librtmp to show the flv file.
* research/librtmp/srs_detect_rtmp.c: Use srs-librtmp to detect the RTMP stream status.
* research/librtmp/srs_h264_raw_publish.c: Use srs-librtmp to publish h.264 raw stream to RTMP server.
* research/librtmp/srs_audio_raw_publish.c: Use srs-librtmp to publish audio raw stream to RTMP server.
* research/librtmp/srs_aac_raw_publish.c: Use srs-librtmp to publish audio aac adts raw stream to RTMP server.
* research/librtmp/srs_rtmp_dump.c: Use srs-librtmp to dump rtmp stream to flv file.
* ./objs/srs_ingest_hls: Ingest the hls live stream to RTMP to SRS.

## Run Examples

Start SRS:

```bash
make && ./objs/srs -c srs.conf 
```

The publish example:

```bash
make && ./objs/research/librtmp/objs/srs_publish rtmp://127.0.0.1:1935/live/livestream
```

Note: the publish stream send random data, cannot play by player.

The play example:

```bash
make && ./objs/research/librtmp/objs/srs_play rtmp://ossrs.net/live/livestreamsuck rtmp stream like rtmpdump
```

Winlin 2014.11
