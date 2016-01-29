The real users or companies who are using SRS.

## 北京云博视

2014.10.10 by 谁变  63110982<br/>
[http://www.y-bos.com/](http://www.y-bos.com/)

## verycdn

2014.9.13 by 1163202026 11:19:35<br/>
[verycdn](http://www.verycdn.cn/) 

## SRS产品使用者

2014.7.23 by 阿才(1426953942) 11:04:01 <br/>
我接触srs才几个月，不敢发表什么意见，只是通过这段时间的学习，觉得这个项目做得相当棒，作者及项目团队工作相当出色，精神非常值得赞赏，目前还在学习中。

2014.7.23 by 随想曲(156530446) 11:04:48 <br/>
我作为使用者来说，就是这玩意完全当成正规高大上的产品用啦！

2014.7.23 by 湖中鱼(283946467) 11:06:23 <br/>
me没怎么去具体分析srs只是觉得作者文档写得比较流畅不乏幽默感。但是目前我用到的功能只有rtmp推送直播，及hls这些nginx-rtmp都有，所以还是选择了用老外的东西

2014.7.23 by 我是蝈蝈(383854294)  11:11:59 <br/>
为什么用SRS？轻便，省资源，有中文说明。SRS那些一站式的脚本与演示demo就能看出来作者是很用心的

## web秀场

2014.7 by 刘重驰

我们目前正在调研 准备用到web秀场 和 移动端流媒体服务上

## 视频直播

2014.7 by 大腰怪

## 远程视频直播

2014.7 by 韧

我们的分发服务器用的就是srs，简单易用，稳定性好

我们以前也用过几个分发软件，都没有srs好用，真心的

## chnvideo

2014.7 [chnvideo](http://chnvideo.com/)编码器内置SRS提供RTMP和HLS拉服务。

## verycloud

2014.6 [verycloud](http://verycloud.cn/)

## 某工厂监控系统

2014.4 by 斗破苍穷(154554381)

某工厂的监控系统主要组成：
* 采集端：采集端采用IPC摄像头安装在工厂重要监控位置，通过网线或者wifi连接到监控中心交换机。
* 监控中心：中心控制服务器，负责管理采集端和流媒体服务器，提供PC/Android/IOS观看平台。
* 流媒体服务器：负责接收采集端的流，提供观看端RTMP/HLS的流。
* 观看端：PC/Android/IOS。要求PC端的延迟在3秒内。Android/IOS延迟在20秒之内。

主要流程包括：
* 采集端启动：IPC摄像头像监控中心注册，获得发布地址，并告知监控中心采集端的信息，譬如摄像头设备名，ip地址，位置信息之类。
* 采集端开始推流：IPC摄像头使用librtmp发布到地址，即将音频视频数据推送到RTMP流媒体服务器。
* 流媒体服务器接收流：流媒体服务器使用SRS，接收采集端的RTMP流。FMS-3/3.5/4.5都有问题，估计是和librtmp对接问题。
* 观看端观看：用户使用PC/Android/IOS登录监控中心后，监控中心返回所有的摄像头信息和流地址。PC端使用flash，延迟在3秒之内；Android/IOS使用HLS，延迟在20秒之内。
* 时移：监控中心会开启录制计划，将RTMP流录制为FLV文件。用户可以在监控中心观看录制的历史视频。

## 网络摄像机

2014.4 by camer(2504296471)

网络摄像机使用hi3518芯片，如何用网页无插件直接观看网络摄像机的流呢？

目前有应用方式如下：
* hi3518上跑采集和推流程序（用srslibrtmp）
* 同时hi3518上还跑了srs/nginx-rtmp作为服务器
* 推流程序推到hi3518本机的nginx服务器
* PC上网页直接观看hi3518上的流

## IOS可以看的监控

2014.3 by 独孤不孤独(378668966)

一般监控摄像头只支持输出RTMP/RTSP，或者支持RTSP方式读取流。如果想在IOS譬如IPad上看监控的流，怎么办？先部署一套rtmp服务器譬如nginx-rtmp/crtmpd/wowza/red5之类，然后用ffmpeg把rtsp流转成rtmp（或者摄像头直接推流到rtmp服务器），然后让服务器切片成hls输出，在IOS上观看。想想都觉得比较麻烦额，如果摄像头比较多怎么办？一个服务器还扛不住，部署集群？

最简单的方式是什么？摄像头自己支持输出HLS流不就好了？也就是摄像头有个内网ip作为服务器，摄像头给出一个hls的播放地址，IOS客户端譬如IPad可以播放这个HLS地址。

SRS最适合做这个事情，依赖很少，提供[arm编译脚本](v1_EN_SampleARM)，只需要[改下configure的交叉编译工具](v1_EN_SrsLinuxArm#%E4%BD%BF%E7%94%A8%E5%85%B6%E4%BB%96%E4%BA%A4%E5%8F%89%E7%BC%96%E8%AF%91%E5%B7%A5%E5%85%B7)就可以编译了。

主要流程：
* 编译arm下的srs，部署到树莓派，在摄像头中启动srs。
* 使用ffmpeg将摄像头的rtsp以RTMP方式推到srs。或者用自己程序采集设备数据推送RTMP流到srs。
* srs分发RTMP流和HLS流。其实PC上也可以看了。
* IOS譬如IPad上播放HLS地址。

## 清华活动直播

2014.2 by youngcow(5706022)

清华大学每周都会有活动，譬如名家演讲等，使用SRS支持，少量的机器即可满足高并发。

主要流程：
* 在教室使用播控系统（摄像机+采集卡或者摄像机+导播台）推送RTMP流到主SRS
* 主SRS自动Forward给从SRS（参考[Forward](v1_EN_Forward)）
* PC客户端（Flash）使用FlowerPlayer，支持多个服务器的负载均衡
* FlowerPlayer支持在两个主从SRS，自动选择一个服务器，实现负载均衡

主要的活动包括：
* 2014-02-23，丘成桐清华演讲

## 某农场监控

2014.1 by 孙悟空

农场中摄像头支持RTSP访问，FFMPEG将RTSP转换成RTMP推送到SRS，flash客户端播放RTMP流。同时flash客户端可以和控制服务器通信，控制农场的浇水和施肥。

![农场植物开花了](http://winlinvip.github.io/srs.release/wiki/images/application/farm.jpg)

截图：农场的植物开花了，据说种的是萝卜。。。

Winlin 2014.10

[nginx]: http://192.168.1.170:8080/nginx.html
[srs-player]: http://winlinvip.github.io/srs.release/trunk/research/players/srs_player.html?vhost=__defaultVhost__&autostart=true&server=192.168.1.170&app=live&stream=livestream&port=1935
[srs-player-19350]: http://winlinvip.github.io/srs.release/trunk/research/players/srs_player.html?vhost=__defaultVhost__&autostart=true&server=192.168.1.170&app=live&stream=livestream&port=19350
[srs-player-ff]: http://winlinvip.github.io/srs.release/trunk/research/players/srs_player.html?vhost=__defaultVhost__&autostart=true&server=192.168.1.170&app=live&stream=livestream_ff
[jwplayer]: http://winlinvip.github.io/srs.release/trunk/research/players/jwplayer6.html?vhost=__defaultVhost__&hls_autostart=true&server=192.168.1.170&app=live&stream=livestream&hls_port=8080
[jwplayer-ff]: http://winlinvip.github.io/srs.release/trunk/research/players/jwplayer6.html?vhost=__defaultVhost__&hls_autostart=true&server=192.168.1.170&app=live&stream=livestream_ff&hls_port=8080

