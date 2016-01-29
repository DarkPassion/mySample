[**HOME**](Home) > [**CN(2.0)**](v2_CN_Home)

## SRS Overview

SRS定位是运营级的互联网直播服务器集群，追求更好的概念完整性和最简单实现的代码。SRS提供了丰富的接入方案将RTMP流接入SRS，包括[[推送RTMP到SRS | v1_CN_SampleRTMP ]]、[[推送RTSP/UDP/FLV到SRS | v2_CN_Streamer]]、[[拉取流到SRS | v1_CN_Ingest]]。SRS还支持将接入的RTMP流进行各种变换，譬如[[将RTMP流转码 | v2_CN_SampleFFMPEG]]、[[转发给其他服务器 | v1_CN_SampleForward]]、[[转封装成HTTP-FLV流 | v2_CN_SampleHttpFlv]]、[[转封装成HLS | v2_CN_SampleHLS]]、[[转封装成HDS | v2_CN_DeliveryHDS]]、[[录制成FLV | v2_CN_DVR]]。SRS包含支大规模集群如CDN业务的关键特性，譬如[[RTMP多级集群 | v1_CN_SampleRTMPCluster]]、[[VHOST虚拟服务器  | v1_CN_RtmpUrlVhost]]、[[无中断服务Reload | v1_CN_Reload]]、[[HTTP-FLV集群 | v2_CN_SampleHttpFlvCluster]]。此外，SRS还提供丰富的应用接口，包括[[HTTP回调 | v2_CN_HTTPCallback]]、[[安全策略Security | v2_CN_Security]]、[[HTTP API接口 | v2_CN_HTTPApi]]、[[RTMP测速 | v1_CN_BandwidthTestTool]]。

## Downloading

SRS发布版本提供安装包下载，请访问[**ossrs.net**][website]。若您需要自己编译SRS，请参考[[编译SRS | v2_CN_Build]]。

SRS主要运行在Linux系统上，譬如Centos和Ubuntu，包括x86、x86-64、ARM和MIPS。MacOS支持代码编辑和编译。其他Unix-like系统不支持，SRS也不支持Windows系统。SRS的系统偏好主要是由于[state-threads][st]导致的，它极大的简化了服务器在复杂协议的处理。

SRS可以在一台服务器上运行集群，或者在多台服务器上也可以运行集群。SRS是单进程模型，不支持多进程；您可以使用[go-sharp][sharp]支持HTTP FLV的多进程，以及[srs-dolphin][dolphin]支持RTMP的多进程。

## Where to Go from Here

***User Guides:***

* [Quick Start][qstart]: 使用SRS的主要功能的快速手册。您应该从这个文章入手。
* [[Why SRS|v1_CN_Product]]: 为何选择SRS？SRS的路线图？
* [GIT Mirrors][mirrors]: SRS在各个主要GIT站点的镜像，代码都是保持同步的。
* [Main Features][features]: SRS的功能列表。请注意有些功能只有特定的版本才有。请注意有些功能是实验性的。
* [Releases][releases]: SRS目前已经发布的版本。
* [[Docs|v2_CN_Docs]]: SRS的详细文档。

***Deployment Guides:***

* [[Delivery RTMP|v1_CN_SampleRTMP]]: 如何部署SRS提供RTMP服务。
* [[Delivery HLS|v2_CN_SampleHLS]]: 如何部署SRS提供RTMP和HLS服务。
* [[Delivery HTTP FLV|v2_CN_SampleHttpFlv]]: 如何部署SRS分发FLV流。
* [[Delivery HDS|v2_CN_DeliveryHDS]]: 如何部署SRS分发HDS流。
* [[Transcode|v2_CN_SampleFFMPEG]]: 如何部署SRS对直播流转码。
* [[Forward|v1_CN_SampleForward]]: 如何部署SRS转发RTMP流到其他服务器。
* [[Low latency|v2_CN_SampleRealtime]]: 如何部署SRS为低延迟模式。
* [[Ingest|v1_CN_SampleIngest]]: 如何将其他流拉到SRS作为RTMP流。
* [[HTTP Server|v2_CN_SampleHTTP]]: 如何部署SRS为HTTP服务器。
* [[SRS DEMO|v1_CN_SampleDemo]]: 如何启动SRS的DEMO。
* [[Projects|v1_CN_Sample]]: 都有谁在使用SRS。
* [[Setup|v2_CN_Setup]]: SRS安装和部署摘要。

***Cluster Guides:***

* [[RTMP Cluster|v1_CN_SampleRTMPCluster]]: 如何部署RTMP分发集群，譬如CDN支持RTMP分发。
* [[FLV Cluster|v2_CN_SampleHttpFlvCluster]]: 如何部署HTTP-FLV分发集群，譬如CDN支持HTTP-FLV分发。
* [[VHOST|v1_CN_RtmpUrlVhost]]: 如何一个集群支持多个用户，即Vhost。
* [[Reload|v1_CN_Reload]]: 如何不中断服务的前提下应用新的配置，即Reload。
* [[Tracable Log|v1_CN_SrsLog]]: 如何在集群中追溯错误和日志，基于连接的日志，排错日志。

***Integration Guides:***

* [[HTTP Callback | v2_CN_HTTPCallback]]: 使用HTTP回调侦听SRS的事件。
* [[HTTP API | v2_CN_HTTPApi]]: 使用SRS的HTTP API获取数据。
* [[RTMP Bandwidth | v1_CN_BandwidthTestTool]]: 使用SRS测速接口获取带宽数据。
* [[Special Control | v2_CN_SpecialControl]]: 一些特殊的控制配置。

***Benchmarks:***

* [Compare][compare]: SRS和其他服务器的对比。
* [Performance][performance]: SRS的性能测试报告。

***Join Us:***

* [Donation][donation]: 给SRS捐献。
* [File Issue][issue]: 提交需求、Bug和反馈。
* [[Contact|v1_CN_Contact]]: 用QQ、邮箱、微信联系我们。

## Questions or need help?

其他联系方式，参考[联系我们](v1_CN_Contact)

Winlin 2015.7

[st]: https://github.com/winlinvip/state-threads
[website]: http://ossrs.net

[sharp]: https://github.com/simple-rtmp-server/go-sharp
[dolphin]: https://github.com/simple-rtmp-server/srs-dolphin

[qstart]: https://github.com/simple-rtmp-server/srs/tree/2.0release#usage
[mirrors]: https://github.com/simple-rtmp-server/srs/tree/2.0release#mirrors
[features]: https://github.com/simple-rtmp-server/srs/tree/2.0release#features
[releases]: https://github.com/simple-rtmp-server/srs/tree/2.0release#releases

[donation]: http://www.ossrs.net/srs.release/donation/index.html
[issue]: https://github.com/simple-rtmp-server/srs/issues/new

[compare]: https://github.com/simple-rtmp-server/srs/tree/2.0release#compare
[performance]: https://github.com/simple-rtmp-server/srs/tree/2.0release#performance
