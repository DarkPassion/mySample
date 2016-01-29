# SRS提供的librtmp

librtmp是一个客户端库，好像是rtmpdump提供的一个客户端RTMP库。

## 应用场景

librtmp的主要应用场景包括：
* 播放RTMP流：譬如rtmpdump，将服务器的流读取后保存为flv文件。
* 推流：提供推流到RTMP服务器。
* 基于同步阻塞socket，客户端用可以了。
* arm：编译出来给arm-linux用，譬如某些设备上，采集后推送到RTMP服务器。
* 不支持直接发布h.264裸码流，而srs-librtmp支持，参考：[publish-h264-raw-data](v2_CN_SrsLibrtmp#publish-h264-raw-data)

备注：关于链接ssl，握手协议，简单握手和复杂握手，参考[RTMP握手协议](v1_CN_RTMPHandshake)

备注：ARM上使用srs-librtmp需要交叉编译，参考[srs-arm](v1_CN_SrsLinuxArm)，即使用交叉编译环境编译srs-librtmp（可以不依赖于其他库，ssl/st都不需要）

## librtmp做Server

群里有很多人问，librtmp如何做server，实在不胜其骚扰，所以单列一章。

server的特点是会有多个客户端连接，至少有两个：一个推流连接，一个播放连接。所以server有两种策略：
* 每个连接一个线程或进程：像apache。这样可以用同步socket来收发数据（同步简单）。坏处就是没法支持很高并发，1000个已经到顶了，得开1000个线程/进程啊。
* 使用单进程，但是用异步socket：像nginx这样。好处就是能支持很高并发。坏处就是异步socket麻烦。

rtmpdump提供的librtmp，当然是基于同步socket的。所以使用librtmp做server，只能采取第一种方法，即用多线程处理多个连接。多线程多麻烦啊！要锁，同步，而且还支持不了多少个。

librtmp的定位就是客户端程序，偏偏要超越它的定位去使用，这种大约只有中国人才能这样“无所畏惧”。

嵌入式设备上做rtmp server，当然可以用srs/crtmpd/nginx-rtmp，轮也轮不到librtmp。

## SRS为何提供librtmp

srs提供的客户端srs-librtmp的定位和librtmp不一样，主要是：
* librtmp的代码确实很烂，毋庸置疑，典型的代码堆积。
* librtmp接口定义不良好，这个对比srs就可以看出，使用起来得看实现代码。
* 没有实例：接口的使用最好提供实例，srs提供了publish/play/rtmpdump实例。
* 最小依赖关系：srs调整了模块化，只取出了core/kernel/rtmp三个模块，其他代码没有编译到srs-librtmp中，避免了冗余。
* 最少依赖库：srs-librtmp只依赖c/c++标准库（若需要复杂握手需要依赖openssl，srs也编译出来了，只需要加入链接即可）。
* 不依赖st：srs-librtmp使用同步阻塞socket，没有使用st（st主要是服务器处理并发需要）。
* SRS提供了测速函数，直接调用srs-librtmp就可以完成到服务器的测速。参考：[Bandwidth Test](v1_CN_BandwidthTestTool)
* SRS提供了日志接口，可以获取服务器端的信息，譬如版本，对应的session id。参考：[Tracable log](v1_CN_SrsLog)
* 支持直接发布h.264裸码流，参考：[publish-h264-raw-data](v2_CN_SrsLibrtmp#publish-h264-raw-data)
* SRS可以直接导出一个srs-librtmp的project，编译成.h和.a使用。或者导出为.h和.cpp，一个大文件。参考：[export srs librtmp](v2_CN_SrsLibrtmp#export-srs-librtmp)

一句话，srs为何提供客户端开发库？因为rtmp客户端开发不方便，不直观，不简洁。

## Export Srs Librtmp

SRS在2.0提供了导出srs-librtmp的编译选项，可以将srs-librtmp单独导出为project，单独编译生成.h和.a，方便在linux和windows平台编译。

使用方法，导出为project，可以make成.h和.a：

```
dir=/home/winlin/srs-librtmp &&
rm -rf $dir &&
./configure --export-librtmp-project=$dir &&
cd $dir && make &&
./objs/research/librtmp/srs_play rtmp://ossrs.net/live/livestream
```

SRS将srs-librtmp导出为独立可以make的项目，生成.a静态库和.h头文件，以及生成了srs-librtmp的所有实例。

还可以直接导出为一个文件，提供了简单的使用实例，[其他实例](v2_CN_SrsLibrtmp#srs-librtmp-examples)参考research的其他例子：

```
dir=/home/winlin/srs-librtmp &&
rm -rf $dir &&
./configure --export-librtmp-single=$dir &&
cd $dir && gcc example.c srs_librtmp.cpp -g -O0 -lstdc++ -o example && 
strip example && ./example
```

备注：导出目录支持相对目录和绝对目录。

## 编译srs-librtmp

编译SRS时，会自动编译srs-librtmp，譬如：

```bash
./configure --with-librtmp --without-ssl
```

编译会生成srs-librtmp和对应的[实例](v2_CN_SrsLibrtmp#srs-librtmp-examples)。

<strong>备注：支持librtmp只需要打开--with-librtmp，但推荐打开--without-ssl，不依赖于ssl，对于一般客户端（不需要模拟flash）足够了。这样srs-librtmp不依赖于任何其他库，在x86/x64/arm等平台都可以编译和运行</strong>

<strong>备注：就算打开了--with-ssl，srslibrtmp也只提供simple_handshake函数，不提供complex_handshake函数。所以推荐关闭ssl，不依赖于ssl，没有实际的用处。</strong>

SRS编译成功后，用户就可以使用这些库开发

## Windows下编译srs-librtmp

srs-librtmp可以只依赖于c++和socket，可以在windows下编译。

先使用SRS导出srs-librtmp，然后在vs中编译，参考：[export srs librtmp](v2_CN_SrsLibrtmp#export-srs-librtmp)

使用了一些linux的头文件，需要做一些portal。

注意：srs-librtmp客户端推流和抓流，不需要ssl库。代码都是c++/stl，网络部分用的是同步socket。

## 数据格式

srs-librtmp提供了一系列接口函数，就数据按照一定格式发送到服务器，或者从服务器读取音视频数据。

数据接口包括：
* 读取数据包：int srs_read_packet(int* type, u_int32_t* timestamp, char** data, int* size)
* 发送数据包：int srs_write_packet(int type, u_int32_t timestamp, char* data, int size)
* 发送h.264裸码流：参考[publish-h264-raw-data](v2_CN_SrsLibrtmp#publish-h264-raw-data)

接口接受的的数据(char* data)，音视频数据，格式为flv的Video/Audio数据。参考srs的doc目录的规范文件[video_file_format_spec_v10_1.pdf](https://raw.github.com/winlinvip/simple-rtmp-server/master/trunk/doc/video_file_format_spec_v10_1.pdf)
* 音频数据格式参考：`E.4.2.1 AUDIODATA`，p76，譬如，aac编码的音频数据。
* 视频数据格式参考：`E.4.3.1 VIDEODATA`，p78，譬如，h.264编码的视频数据。
* 脚本数据格式参考：`E.4.4.1 SCRIPTDATA`，p80，譬如，onMetadata，流的信息（宽高，码率，分辨率等）

数据类型(int type)定义如下（`E.4.1 FLV Tag`，page 75）：
* 音频：8 = audio，宏定义：SRS_RTMP_TYPE_AUDIO
* 视频：9 = video，宏定义：SRS_RTMP_TYPE_VIDEO
* 脚本数据：18 = script data，宏定义：SRS_RTMP_TYPE_SCRIPT

其他的数据，譬如时间戳，都是通过参数接受和发送。

另外，文档其他重要信息：
* flv文件头格式：`E.2 The FLV header`，p74。
* flv文件主体格式：`E.3 The FLV File Body`，p74。
* tag头格式：`E.4.1 FLV Tag`，p75。

使用flv格式的原因：
* flv的格式足够简单。
* ffmpeg也是用的这种格式
* 收到流后加上flv tag header，就可以直接保存为flv文件
* 从flv文件解封装数据后，只要将tag的内容给接口就可以，flv的tag头很简单。

## Publish H.264 Raw Data

SRS-librtmp支持发布h.264裸码流，直接调用api即可将数据发送给SRS。

参考博客：http://blog.csdn.net/win_lin/article/details/41170653

总结起来就是说，H264的裸码流（帧）转换RTMP时：

1. dts和pts是不在h264流中的，外部给出。
1. SPS和PPS在RTMP一个包里面发出去。
1. RTMP包=5字节RTMP包头+H264头+H264数据，具体参考：SrsAvcAacCodec::video_avc_demux
1. 直接提供接口，发送h264数据，其中包含annexb的头：N[00] 00 00 01, where N>=0.

加了一个直接发送h264裸码流的接口：

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

对于例子中的h264流文件：http://winlinvip.github.io/srs.release/3rdparty/720p.h264.raw

里面的数据是：

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

调用时，可以SPS和PPS一起发，帧一次发一个：

```
// SPS+PPS
srs_h264_write_raw_frame('000000016742802995A014016E400000000168CE3880', size, dts, pts)
// IFrame
srs_h264_write_raw_frame('0000000165B8041014C038008B0D0D3A071......', size, dts, pts)
// PFrame
srs_h264_write_raw_frame('0000000141E02041F8CDDC562BBDEFAD2F......', size, dts, pts)
```

调用时，可以一次发一次frame也行：

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

参考：https://github.com/simple-rtmp-server/srs/issues/66#issuecomment-62240521

使用：https://github.com/simple-rtmp-server/srs/issues/66#issuecomment-62245512

## Publish Audio Raw Stream

srs-librtmp提供了api可以将音频裸码流发布到SRS，支持AAC ADTS格式。

API定义如下：

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

调用实例参考[#212](https://github.com/simple-rtmp-server/srs/issues/212#issuecomment-63648892)，以及srs_audio_raw_publish.c和srs_aac_raw_publish.c，参考[examples](v2_CN_SrsLibrtmp#srs-librtmp-examples).

参考：https://github.com/simple-rtmp-server/srs/issues/212#issuecomment-63755405

使用实例：https://github.com/simple-rtmp-server/srs/issues/212#issuecomment-64164018

## srs-librtmp Examples

SRS提供了实例sample，也会在编译srs-librtmp时自动编译：
* research/librtmp/srs_play.c：播放RTMP流实例。
* research/librtmp/srs_publish.c：推送RTMP流实例。
* research/librtmp/srs_ingest_flv.c：读取本地文件并推送RTMP流实例。
* research/librtmp/srs_ingest_rtmp.c：读取RTMP流并推送RTMP流实例。
* research/librtmp/srs_bandwidth_check.c：带宽测试工具。
* research/librtmp/srs_flv_injecter.c：点播FLV关键帧注入文件。
* research/librtmp/srs_flv_parser.c：FLV文件查看工具。
* research/librtmp/srs_detect_rtmp.c：RTMP流检测工具。
* research/librtmp/srs_h264_raw_publish.c：H.264裸码流发布到SRS实例。
* research/librtmp/srs_audio_raw_publish.c: Audio裸码流发布到SRS实例。
* research/librtmp/srs_aac_raw_publish.c: Audio AAC ADTS裸码流发布到SRS实例。
* research/librtmp/srs_rtmp_dump.c: 将RTMP流录制成flv文件实例。
* ./objs/srs_ingest_hls: 将HLS流采集成RTMP推送给SRS。

## 运行实例

启动SRS：

```bash
make && ./objs/srs -c srs.conf 
```

推流实例：

```bash
make && ./objs/research/librtmp/objs/srs_publish rtmp://127.0.0.1:1935/live/livestream
```

备注：推流实例发送的视频数据不是真正的视频数据，实际使用时，譬如从摄像头取出h.264裸码流，需要封装成接口要求的数据，然后调用接口发送出去。或者[直接发送h264裸码流](v2_CN_SrsLibrtmp#publish-h264-raw-data)。

播放实例：

```bash
make && ./objs/research/librtmp/objs/srs_play rtmp://ossrs.net/live/livestreamsuck rtmp stream like rtmpdump
```

Winlin 2014.11
