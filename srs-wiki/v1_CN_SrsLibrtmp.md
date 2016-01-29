# SRS提供的librtmp

librtmp是一个客户端库，好像是rtmpdump提供的一个客户端RTMP库。

## 应用场景

librtmp的主要应用场景包括：
* 播放RTMP流：譬如rtmpdump，将服务器的流读取后保存为flv文件。
* 推流：提供推流到RTMP服务器。
* 基于同步阻塞socket，客户端用可以了。
* arm：编译出来给arm-linux用，譬如某些设备上，采集后推送到RTMP服务器。

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

一句话，srs为何提供客户端开发库？因为rtmp客户端开发不方便，不直观，不简洁。

## 编译srs-librtmp

编译SRS时，会自动编译srs-librtmp，譬如：

```bash
./configure --with-librtmp --without-ssl
```

编译会生成srs-librtmp和对应的[实例](v1_CN_SrsLibrtmp#srs-librtmp-examples)。

<strong>备注：支持librtmp只需要打开--with-librtmp，但推荐打开--without-ssl，不依赖于ssl，对于一般客户端（不需要模拟flash）足够了。这样srs-librtmp不依赖于任何其他库，在x86/x64/arm等平台都可以编译和运行</strong>

<strong>备注：就算打开了--with-ssl，srslibrtmp也只提供simple_handshake函数，不提供complex_handshake函数。所以推荐关闭ssl，不依赖于ssl，没有实际的用处。</strong>

SRS编译成功后，用户就可以使用这些库开发

## Windows下编译srs-librtmp

srs-librtmp可以只依赖于c++和socket，可以在windows下编译。不过srs没有提供直接编译的方法，可行的方法是：
* 先在linux下编译通过：`./configure --disable-all --with-librtmp && make`
* 头文件就是`src/libs/srs_librtmp.hpp`，将以下文件拷贝到windows下编译：

```bash
objs/srs_auto_headers.hpp
src/core/*
src/kernel/*
src/rtmp/*
src/libs/*
```

注意：srs-librtmp客户端推流和抓流，不需要ssl库。代码都是c++/stl，网络部分用的是同步socket。
备注：SRS2.0提供将srs-librtmp导出为一个project或者文件，参考[导出srs-librtmp](v2_CN_SrsLibrtmp#export-srs-librtmp)。SRS1.0不支持导出，可以自己合并2.0的修改到1.0。

## 数据格式

srs-librtmp提供了一系列接口函数，就数据按照一定格式发送到服务器，或者从服务器读取音视频数据。

数据接口包括：
* 读取数据包：int srs_read_packet(int* type, u_int32_t* timestamp, char** data, int* size)
* 发送数据包：int srs_write_packet(int type, u_int32_t timestamp, char* data, int size)

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

备注：SRS2.0支持直接发送h264裸码流，参考[publish h.264 raw data](v2_CN_SrsLibrtmp#publish-h264-raw-data)

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

## 运行实例

启动SRS：

```bash
make && ./objs/srs -c srs.conf 
```

推流实例：

```bash
make && ./objs/research/librtmp/objs/srs_publish rtmp://127.0.0.1:1935/live/livestream
```

备注：推流实例发送的视频数据不是真正的视频数据，实际使用时，譬如从摄像头取出h.264裸码流，需要封装成接口要求的数据，然后调用接口发送出去。

播放实例：

```bash
make && ./objs/research/librtmp/objs/srs_play rtmp://ossrs.net/live/livestreamsuck rtmp stream like rtmpdump
```

Winlin 2014.11
