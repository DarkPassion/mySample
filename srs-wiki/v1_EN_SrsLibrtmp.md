# SRS librtmp

librtmp is a client side library, seems from rtmpdump.

## Use Scenarios

The use scenarios of librtmp:
* Play or suck RTMP stream: For example rtmpdump, dvr RTMP stream to flv file.
* Publish RTMP stream: Publish RTMP stream to server.
* Use sync block socket: It's ok for client.
* ARM: Can used for linux arm, for some embed device, to publish stream to server.

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

In a word, SRS provides more efficient and simple client library srs-librtmp.

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

srs-librtmp only depends on libc++, so can be build on windows:
* Build on linux: `./configure --disable-all --with-librtmp && make`
* Copy the files to build on windows:

```bash
objs/srs_auto_headers.hpp
src/core/*
src/kernel/*
src/rtmp/*
src/libs/*
```

Note: Donot need ssl and st.
Note: SRS2.0 can directly export srs-librtmp to a single project, or .h+.cpp file, read [export srs-librtmp](v2_EN_SrsLibrtmp#export-srs-librtmp).

## RTMP packet specification

This section descrips the RTMP packet specification, for the srs-librtmp api to read or write data.

The api about data:
* Read RTMP packet from server: int srs_read_packet(int* type, u_int32_t* timestamp, char** data, int* size)
* Write RTMP packet to server: int srs_write_packet(int type, u_int32_t timestamp, char* data, int size)

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

Note: SRS2.0 supports directly write h.264 raw stream, read [publish h.264 raw data](v2_EN_SrsLibrtmp#publish-h264-raw-data)

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
