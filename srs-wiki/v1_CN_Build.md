# 编译SRS

本文说明了如何编译和打包SRS，另外，可以直接下载release的binary，提供了几个常见系统的安装包，安装程序会安装系统服务，直接以系统服务启动即可。参考：[Github: release](http://winlinvip.github.io/srs.release/releases/)或者[国内镜像: release](http://www.ossrs.net/srs.release/releases/)

## 操作系统

* README中的Usage，在<strong>Centos6.x/Ubuntu12</strong>下面测试成功。按照Step操作后，浏览器中打开服务器地址就能观看所有的DEMO。
* DEMO演示了所有SRS的功能，特别是ffmpeg依赖的库巨多，因此为了简化，推荐使用<strong>Centos6.x/Ubuntu12</strong>.
* 若的确需要在其他系统下编译SRS，下面说明SRS依赖的各种库，可以关掉某些功能减少编译的依赖。

## 关闭防火墙和selinux

有时候启动没有问题，但是就是看不了，原因是防火墙和selinux开着。

可以用下面的方法关掉防火墙：

```bash
# disable the firewall
sudo /etc/init.d/iptables stop
sudo /sbin/chkconfig iptables off
```

selinux也需要disable，运行命令`getenforce`，若不是Disabled，执行下面的步骤：

1. 编辑配置文件：`sudo vi /etc/sysconfig/selinux`
1. 把SELINUX的值改为disabled：`SELINUX=disabled`
1. 重启系统：`sudo init 6`

## 编译和启动

确定用什么编译选项后（参考下面的说明），编译SRS其实很简单。只需要RTMP和HLS：

```
./configure && make
```

指定配置文件，即可启动SRS：

```bash
./objs/srs -c conf/srs.conf
```

推RTMP流和观看，参考[Usage: RTMP](v1_CN_SampleRTMP)

更多使用方法，参考[Usage](https://github.com/simple-rtmp-server/srs/tree/1.0release#usage)

## 编译选项和预设集

SRS提供了详细的编译选项来控制功能的开关，以及提供了一些有用的预设，针对不同的应用场景默认选项。

SRS会先应用预设集，然后应用用户的选项，譬如：`./configure --rtmp-hls --with-http-api`，应用顺序为：
* 首先应用预设集：--rtmp-hls，开启ssl/hls，其他功能都处于关闭状态。
* 应用用户选项：--with-http-api，开启http api接口。

所以这个编译命令最后支持的功能是：RTMP+HLS+HTTP接口

支持的预设集参考末尾的参数列表，或者执行：`./configure -h`查看。

## jobs:加速编译

由于SRS在configure时需要编译ffmpeg/nginx，这个过程会很漫长，如果你有多核机器，那么可以使用jobs来并行编译。
* configure: 在编译srs依赖的工具时可以并行编译。
* make: 在编译srs时可以使用并行编译。

srs并行编译和串行编译的项目包括（srs会自动判断，不需要用户指定）：
* srs: 支持并行编译。
* st-1.9: 串行编译，库比较小，编译时间很短。
* http-parser: 串行编译，库比较小，编译时间很短。
* openssl: 串行编译，并行编译有问题。
* nginx: 支持并行编译。
* ffmpeg: 支持并行编译。
* lame: 支持并行编译。ffmpeg用到的mp3库。
* libaacplus: 串行编译，并行编译有问题。ffmpeg用到的aac库。
* x264: 支持并行编译。ffmpeg用到的x264库。

configure使用并行编译的方法如下：

```bash
./configure --jobs=16
```

注意：configure不支持make那样的"-jN"，只支持"--jobs[=N]"。

make使用并行编译的方法如下：

```bash
// or make --jobs=16
make -j16
```

## Package

SRS提供了打包脚本，可以将srs打包（不包含nginx/ffmpeg等外部程序）。安装包也提供下载，参考本文开头部分。

打包脚本会编译srs，然后将srs的文件打包为zip（zip比tar通用）。详细参考package的帮助：

```bash
[winlin@dev6 srs]$ ./scripts/package.sh --help

  --help                   print this message

  --x86-x64                configure with x86-x64 and make srs. 
  --arm                    configure with arm and make srs.
```

## SRS依赖关系

SRS依赖于g++/gcc/make，st-1.9，http-parser2.1，ffmpeg，cherrypy，nginx，openssl-devel，python2。

除了g++/gcc/make基础工具，st-1.9是必选，其他都可以通过编译选项禁用。

## 自定义编译参数

SRS可以自定义编译器，譬如arm编译时使用arm-linux-g++而非g++。参考[ARM：手动编译](v1_CN_SrsLinuxArm#%E6%89%8B%E5%8A%A8%E7%BC%96%E8%AF%91srs)

注意：SRS和ST都可以通过编译前设置变量编译，但是ssl需要手动修改Makefile。还好ssl不用每次都编译。

## 编译的生成项目

configure和make将会生成一些项目，都在objs目录，其他会自动软链到objs目录。

* ./objs/srs 启动SRS服务器
* ./objs/research/librtmp/srs_bandwidth_check linux测速工具
* ./objs/research/librtmp/srs_ingest_flv 将FLV文件采集为RTMP流工具
* ./objs/research/librtmp/srs_ingest_rtmp 将RTMP流采集为RTMP流工具
* ./objs/nginx/sbin/nginx 分发HLS用到的nginx服务器
* research/api-server/server.py 启动HTTP hooks和DEMO视频会议用到的api-server
* ./objs/ffmpeg/bin/ffmpeg SRS转码用的FFMPEG，DEMO推流也是用它
* ./objs/lib/srs_librtmp.a SRS提供的客户端库，参考<a href="https://github.com/simple-rtmp-server/srs/wiki/v1_CN_SrsLibrtmp">srs-librtmp</a>
* ./objs/nginx/html/players SRS的DEMO的静态页面，当没有开启HttpCallback时

## 配置参数说明

SRS的配置(configure)参数说明如下：
* --help 配置的帮助信息
* --with-ssl 添加ssl支持，ssl用来支持复杂握手。参考：[RTMP Handshake](v1_CN_RTMPHandshake)。
* --with-hls 支持HLS输出，将RTMP流切片成ts，可用于支持移动端HLS（IOS/Android），不过PC端jwplayer也支持HLS。参考：[HLS](v1_CN_DeliveryHLS)
* --with-dvr 支持将RTMP流录制成FLV。参考：[DVR](v1_CN_DVR)
* --with-nginx 编译nginx，使用nginx作为web服务器分发HLS文件，以及demo的静态页等。
* --with-http-callback 支持http回调接口，用于认证，统计，事件处理等。参考：[HTTP callback](v1_CN_HTTPCallback)
* --with-http-api 打开HTTP管理接口。参考：[HTTP API](v1_CN_HTTPApi)
* --with-http-server 打开内置HTTP服务器，支持分发HTTP流。参考：[HTTP Server](v1_CN_HTTPServer)
* --with-ffmpeg 编译转码/转封装/采集用的工具FFMPEG。参考：[FFMPEG](v1_CN_FFMPEG)
* --with-transcode 直播流转码功能。需要在配置中指定转码工具。参考：[FFMPEG](v1_CN_FFMPEG)
* --with-ingest 采集文件/流/设备数据，封装为RTMP流后，推送到SRS。参考：[Ingest](v1_CN_Ingest)
* --with-stat 是否开启数据统计功能，SRS可以采集cpu/内存/网络/磁盘IO等数据，共监控系统通过http-api获取。（目前osx不支持）。
* --with-research 是否编译research目录的文件，research目录是一些调研，譬如ts info是做HLS时调研的ts标准。和SRS的功能没有关系，仅供参考。
* --with-utest 是否编译SRS的单元测试，默认开启，也可以关闭。
* --with-gperf 是否使用google的tcmalloc库，默认关闭。
* --with-gmc 是否使用gperf的内存检测，编译后启动srs会检测内存错误。这个选项会导致低性能，只应该在找内存泄漏时才开启。默认关闭。参考：[gperf](v1_CN_GPERF)
* --with-gmp 是否使用gperf的内存性能分析，编译后srs退出时会生成内存分析报告。这个选项会导致地性能，只应该在调优时开启。默认关闭。参考：[gperf](v1_CN_GPERF)
* --with-gcp 是否启用gperf的CPU性能分析，编译后srs退出时会生成CPU分析报告。这个选项会导致地性能，只应该在调优时开启。默认关闭。参考：[gperf](v1_CN_GPERF)
* --with-gprof 是否启用gprof性能分析，编译后srs会生成CPU分析报告。这个选项会导致地性能，只应该在调优时开启。默认关闭。参考：[gprof](v1_CN_GPROF)
* --with-librtmp 客户端推流/播放库，参考[srs-librtmp](v1_CN_SrsLibrtmp)
* --with-arm-ubuntu12 交叉编译ARM上运行的SRS，要求系统是Ubuntu12。参考[srs-arm](v1_CN_SrsLinuxArm)
* --jobs[=N] 开启的编译进程数，和make的-j（--jobs）一样，在configure时可能会编译nginx/ffmpeg等工具，可以开启多个jobs编译，可以显著加速。参考：[Build: jobs](v1_CN_Build#wiki-jobs%E5%8A%A0%E9%80%9F%E7%BC%96%E8%AF%91)
* --static 使用静态链接。指定arm编译时，会自动打开这个选项。手动编译需要用户自身打开。参考：[ARM](v1_CN_SrsLinuxArm)

预设集：
* --x86-x64，默认预设集，一般的x86或x64服务器使用。release使用这个配置编译。
* --osx，苹果MAC OSX（Darwin）系统下编译，安装好xcode和brew后，可以使用这个选项。
* --pi，树莓派预设集，arm的子集。树莓派的release用这个配置编译。
* --cubie，在cubieboard下直接编译的选项，使用ubuntu差不多的配置集。
* --arm，ubuntu下交叉编译，等价于--with-arm-ubuntu12。release使用这个配置。
* --mips，ubuntu下交叉编译，为hiwifi的mips路由器编译。（目前srs在mips上有内存泄漏，2天左右会把路由器跑死）。
* --dev，开发选项，尽可能开启功能。
* --fast，关闭所有功能，只支持基本RTMP（不支持h264/aac），最快的编译速度。
* --pure-rtmp，支持RTMP（支持h264+aac），需要编译ssl。
* --rtmp-hls，支持RTMP和HLS，典型的应用方式。还可以加上内置的http服务器（--with-http-server）。
* --disable-all, 禁用所有功能，只支持rtmp（vp6）。
* --demo，SRS的演示编译选项。
* --full，开启SRS所有的功能。

专家选项：有可能编译失败，不是专家就不要用这个。
* --use-sys-ssl 使用系统的ssl，不单独编译ssl（在--with-ssl时有效）。

Winlin 2014.2
