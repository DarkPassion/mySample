# Build SRS

You can directly use the release binaries, or build SRS step by step. See: [Github: release](http://winlinvip.github.io/srs.release/releases/) or [Mirror of China: release](http://www.ossrs.net/srs.release/releases/)

## OS

* <strong>Centos6.x/Ubuntu12</strong> is proved for Usage of README.
* Recomment to use <strong>Centos6.x/Ubuntu12</strong> for demo of SRS, because it's complex to compile FFMPEG.
* Turn some features off when you need to compile SRS on other OS.

## Iptables and Selinux

Sometimes the stream play failed, but without any error message, or server cann't connect to. Please check the iptables and selinux.

Turn off iptables:

```bash
# disable the firewall
sudo /etc/init.d/iptables stop
sudo /sbin/chkconfig iptables off
```

Disable the selinux, to run `getenforce` to ensure the result is `Disabled`:

1. Edit the config of selinux: `sudo vi /etc/sysconfig/selinux`
1. Change the SELINUX to disabled: `SELINUX=disabled`
1. Rebot: `sudo init 6`

## Build and Run SRS

It's very easy to build SRS:

```
./configure && make
```

Also easy to start SRS:

```bash
./objs/srs -c conf/srs.conf
```

Publish RTMP, please see: [Usage: RTMP](v1_EN_SampleRTMP)

More usages, please see: [Usage](https://github.com/simple-rtmp-server/srs/tree/1.0release#usage)

## Build Options and Presets

Each big feature is controlled by a build option, while a preset provides a set of options.

SRS will apply preset first, then apply user specified options, for example, `./configure --rtmp-hls --with-http-api` will:
* apply preset: --rtmp-hls, enable rtmp ssl and hls, disable others.
* apply user specified options: --with-http-api, enable http api.

So, the built SRS will support RTMP+HLS+HttpApi.

All preset and options supported is specified by command `./configure -h`.

## jobs: Speedup Build

It will take long time to compile SRS when ffmpeg/nginx enabled. You can use multiple cpu to speedup, similar to the `make --jobs=N`.
* configure --jobs=N: to speedup when compile ffmpeg/nginx.
* make --jobs=N: to speedup when compile SRS.

The following components can be speedup, SRS will auto apply the --jobs automatically:
* SRS: support.
* st-1.9: not support, for it's small.
* http-parser: not support, for it's small.
* openssl: not support.
* nginx: support.
* ffmpeg: support.
* lame: support.
* libaacplus: not support.
* x264: support.

The usage to speedup configure:

```bash
./configure --jobs=16
```

Note: configure donot support `-jN`, only support `--jobs=N`.

The usage to speedup make SRS:

```bash
// or make --jobs=16
make -j16
```

## Package

SRS provides package script, to package the install zip on release website.

The package script will build SRS, then zip the files. See help of package script:

```bash
[winlin@dev6 srs]$ ./scripts/package.sh --help

  --help                   print this message

  --x86-x64                configure with x86-x64 and make srs. 
  --arm                    configure with arm and make srs.
```

## SRS depends

SRS depends on g++/gcc/make, st-1.9, http-parser2.1, ffmpeg, cherrypy, nginx, openssl-devel and python2。

The g++/gcc/make and st-1.9 is required, others are optional.

## Custom Compile Tool Chain

User can use other compile tool chain, for instance, arm usig arm-linux-g++. Please read [ARM：compile manually](v1_EN_SrsLinuxArm)

## Build Outputs

The outputs of configure and make are put in objs.

* ./objs/srs The SRS application.
* ./objs/research/librtmp/srs_bandwidth_check linux bandwidth test tool.
* ./objs/research/librtmp/srs_ingest_flv ingest FLV file to RTMP stream to SRS.
* ./objs/research/librtmp/srs_ingest_rtmp ingest RTMP to RTMP to SRS.
* ./objs/nginx/sbin/nginx delivery HLS.
* research/api-server/server.py the api-server for demo of SRS.
* ./objs/ffmpeg/bin/ffmpeg the FFMEPG tool to transcode for SRS.
* ./objs/lib/srs_librtmp.a the client librtmp of SRS. Please read <a href="https://github.com/simple-rtmp-server/srs/wiki/v2_EN_SrsLibrtmp">srs-librtmp</a>
* ./objs/nginx/html/players the players of demo of SRS.

## Configure Options

The options of configure:
* --help print help and exit.
* --with-ssl support RTMP complex handshake. Please read [RTMP Handshake](v1_EN_RTMPHandshake)。
* --with-hls support output HLS for mobile(IOS/Android), and PC can use HLS now. See: [HLS](v2_EN_DeliveryHLS)
* --with-hds support output HDS for Adobe OSMF. See: [HDS](v2_EN_DeliveryHDS)
* --with-dvr dvr RTMP to FLV file. Please read [DVR](v3_EN_DVR)
* --with-nginx build nginx to delivery HLS files and demo pages of SRS.
* --with-http-callback support http callback, for authentication, statistic and event handler. Please read [HTTP callback](v3_EN_HTTPCallback)
* --with-http-api support http api. Please read [HTTP API](v3_EN_HTTPApi)
* --with-http-server enable embeded http server to delivery HTTP files and stream. Please read [HTTP Server](v2_EN_HTTPServer)
* --with-stream-caster enable the stream caster, read [Stream Caster](v2_EN_Streamer)
* --with-kafka report log to Kafka cluster for big-data. Please read [Kafka](v3_EN_Kafka)
* --with-ffmpeg build FFMPEG for transcode, remux and ingest. Please read [FFMPEG](v3_EN_FFMPEG)
* --with-transcode live stream transcode. Please read [FFMPEG](v3_EN_FFMPEG)
* --with-ingest ingest file, stream or device to RTMP to SRS. Please read [Ingest](v1_EN_Ingest)
* --with-stat stat the cpu, memory, network and disk io, user can access stat by http api(not supported by darwin/OSX).
* --with-research buidl the research tools, for instance, ts info, ingest flv/rtmp, flv parser and bandwidth test.
* --with-utest build the utest of SRS, it's ok to disable it.
* --with-gperf use google tcmalloc, seems no effect.
* --with-gmc use gerf memory check, which will hurt performance, only for memory check. Please read [gperf](v1_EN_GPERF)
* --with-gmp use gperf memory performance analysis, which will hurt performance, only for memory performance analysis. Please read [gperf](v1_EN_GPERF)
* --with-gcp use gperf cpu profile, which will hurt performance, only for cpu profile. Please read [gperf](v1_EN_GPERF)
* --with-gprof use gprof, which will hurt performance. Please read [gprof](v1_EN_GPROF)
* --with-librtmp client librtmp. Please read [srs-librtmp](v2_EN_SrsLibrtmp)
* --with-arm-ubuntu12 cross build SRS for arm on ubuntu12. Please read [srs-arm](v1_EN_SrsLinuxArm)
* --jobs[=N] the parallelly process to build, sames to the -j/--jobs of make. Please read [Build: jobs](v3_EN_Build#jobs-speedup-build)
* --static use static link for arm. Please read [ARM](v1_EN_SrsLinuxArm)

The preset of configure:
* --x86-x64, default, for x86(32bits) or x64(64bits) server.
* --osx，for the MAC OSX(Darwin) of apple, depends on xcode and brew.
* --pi，for raspberry-pi, an arm machine.
* --cubie，for cubieboard, similar to ubuntu.
* --arm，for arm, cross build on ubuntu12. Alias is --with-arm-ubuntu12.
* --mips，for mips, cross build on ubuntu12, for hiwifi(experiment for memory leak).
* --dev，for dev to enable the most features.
* --fast，for fast build, disable almost all features, only enable basic RTMP without complex handshake(ssl).
* --pure-rtmp，only enalbe RTMP, with complex handshake(ssl) to support delivery h.264+aac.
* --rtmp-hls，support RTMP and HLS, typical features.
* --disable-all, disable all, only support RTMP without complex handshake, delivery vp6, depneds on st only.
* --demo，enable the demo of SRS.
* --full，enable all features of SRS.

The expert options, maybe compile failed:
* --use-sys-ssl use the ssl of linux system, which installed by yum or apt-get. This is ok when --with-ssl specified and donot compile the ssl of 3rdparty.
* --export-librtmp-project=&lt;path&gt; export srs-librtmp to specified project in path.

Winlin 2014.11
