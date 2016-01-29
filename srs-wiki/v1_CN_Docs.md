Welcome to the simple-rtmp-server wiki!

## [QQ群和微信](v1_CN_Contact)

关于SRS的QQ群和微信，参考：[https://github.com/simple-rtmp-server/srs/wiki/v1_CN_Contact](v1_CN_Contact)

## [如何提问](v1_CN_HowToAskQuestion)

提问是一门艺术，不是别人不待见你，是你提的问题实在不招人待见。提问前务必参考：[如何提问](v1_CN_HowToAskQuestion)

## [SRS产品定位](v1_CN_Product)

SRS产品定位是什么？SRS亮点是什么？和其他产品对比呢？参考：[SRS产品定位] (v1_CN_Product)

## [产品对比](v1_CN_Compare)

SRS和其他流媒体产品的对比，差别在哪里，定位、区别、优势、劣势在哪里？参考：[产品对比](v1_CN_Compare)

## [SRS功能列表](v1_CN_Features)

SRS支持的功能，包括已经支持的，计划中的，和没有计划的。参考[SRS功能列表](v1_CN_Features)

## [公用服务器](v1_CN_LiveShow)

SRS是否提供公用服务器？供大家测试，演示，版本发布，自动编译等。是否有现成的SRS服务器可以推流和播放？参考[公用服务器](v1_CN_LiveShow)

## [应用实例](v1_CN_Sample)

SRS的实际应用，可以用SRS实际做点什么？参考：[应用实例](v1_CN_Sample)

## [编译SRS](v1_CN_Build)

编译SRS的常见选项，SRS的功能，以及应用场景，编译和启动，DEMO的查看方法，如何Build SRS。链接：[编译SRS](v1_CN_Build)

## [SRS启动](v1_CN_LinuxService)

如何以系统服务启动SRS？如何后台启动SRS？如何linux启动时自动启动SRS？如何启动SRS和DEMO的api，播放器/编码器/视频会议，但不用观看那些演示流？参考：[SRS启动](v1_CN_LinuxService)

## [SRS日志](v1_CN_SrsLog)

如何查看SRS的日志？如何不以nohup但是后台启动？如何将日志打印到控制台？如何打印出更多或者更少的日志？如何打印出详细级别的日志？如何只打印出警告以上的日志？参考：[SRS日志](v1_CN_SrsLog)

## [SRS的架构](v1_CN_Architecture)

SRS开发者必看的基础wiki，SRS的基础架构。链接：[SRS的架构](v1_CN_Architecture)

## [RTMP URL & Vhost](v1_CN_RtmpUrlVhost)

搞不清楚RTMP的那些个vhost/app/stream？特别还有参数的时候？参考链接：[RTMP URL & Vhost](v1_CN_RtmpUrlVhost)

## [RTMP握手协议](v1_CN_RTMPHandshake)

RTMP为何要依赖ssl？什么是RTMP简单握手和复杂握手？什么时候用Simple就足够了？参考：[RTMP握手协议](v1_CN_RTMPHandshake)

## [FFMPEG直播流转码](v1_CN_FFMPEG)

如何使用SRS对直播流转码？如何只对视频或音频转码？如何配置台标？转码参数的意义和顺序？参考：[FFMPEG直播流转码](v1_CN_FFMPEG)

## [RTMP分发](v1_CN_DeliveryRTMP)

SRS如何支持RTMP流？如何配置SRS分发RTMP流？参考：[RTMP分发](v1_CN_DeliveryRTMP)

## [HLS分发](v1_CN_DeliveryHLS)

如何将RTMP流切片成HLS分发？HLS相关知识，SRS配置HLS？参考链接：[Delivery HLS](v1_CN_DeliveryHLS)

## [RTMP PK HLS](v1_CN_RTMP.PK.HTTP)

如何选择分发协议？RTMP的优势在哪里？什么时候用HTTP？HLS优势和缺点是什么？参考：[RTMP PK HLS](v1_CN_RTMP.PK.HTTP)

## [内置HTTP服务器](v1_CN_HTTPServer)

如何不依赖外部web服务器直接可分发HTTP流譬如HLS？SRS为何支持内置的HTTP服务器？参考：[内置HTTP服务器](v1_CN_HTTPServer)

## [DVR录制] (v1_CN_DVR)

如何将直播RTMP流录制为flv文件？参考：[DVR] (v1_CN_DVR)

## [DRM防盗链] (v1_CN_DRM)

SRS如何支持DRM？如何支持防盗链？Refer防盗链如何配置？Token防盗链如何使用？参考：[DRM] (v1_CN_DRM)

## [Reload](v1_CN_Reload)

如何在不影响正在服务的用户的前提下：将一个转码流的码率调低？如何禁用某些频道的HLS？如何添加和删除频道？参考：[Reload](v1_CN_Reload)

## [低延时应用](v1_CN_LowLatency)

如何配置低延时？延时到底受哪些因素的影响？SRS如何配置？SRS延迟多大？参考：[低延时应用](v1_CN_LowLatency)

## [HTTP回调](v1_CN_HTTPCallback)

如何认证客户端连接？如何在发布流时通知外部程序？如何在客户端连接和关闭时加入额外处理逻辑？SRS在各种事件时可以回调HTTP接口(HTTP Callback、HTTP Hooks)。参考：[HTTP回调](v1_CN_HTTPCallback)

## [HTTP管理接口](v1_CN_HTTPApi)

SRS是否提供HTTP API？如何管理服务器？页面js如何重启服务器，获取信息？php管理系统如何管理SRS服务器？参考：[HTTP管理接口](v1_CN_HTTPApi)

## [Ingest采集](v1_CN_Ingest)

SRS如何拉流？如何将已经存在的RTSP流分发成RTMP/HLS？如何将设备中的媒体数据编码为流输出？参考：[Ingest采集](v1_CN_Ingest)

## [Edge搭建大型集群](v1_CN_Edge)

SRS如何支持大规模CDN集群？在流众多时forward/ingest太浪费怎么办？如何支持FMS Edge方式，在访问时才回源？参考：[Edge搭建大型集群](v1_CN_Edge)

## [Forward搭建小型集群](v1_CN_Forward)

使用forward搭建小型集群的配置方法。链接：[Forward搭建小型集群](v1_CN_Forward)

## [性能测试和对比](v1_CN_Performance)

对比了SRS和高性能服务器nginx-rtmp，提供详细的测试步骤，供其他性能对比进行参考。链接：[性能测试和对比](v1_CN_Performance)

## [RaspberryPi性能测试](v1_CN_RaspberryPi)

SRS在arm（RaspberryPi）下的性能如何？最多支持多少个并发？参考：[Performance: RaspberryPi](v1_CN_RaspberryPi)

## [RaspberryPi下转码](v1_CN_ARMTranscode)

如何在RaspberryPi下转码音频？如何在pi下面开启HTTP服务器分发HLS？参考：[RaspberryPi下转码](v1_CN_ARMTranscode)

## [ServerSideScript服务器端脚本](v1_CN_ServerSideScript)

SRS为何不支持服务器端脚本？链接：[服务器端脚本](v1_CN_ServerSideScript)

## [SRS-librtmp](v1_CN_SrsLibrtmp)

如何使用SRS提供的客户端rtmp库？为何要提供？结构是什么？主要流程是什么？实例如何使用？参考：[SRS-librtmp](v1_CN_SrsLibrtmp)

## [SRS应用于linux-arm](v1_CN_SrsLinuxArm)

linux-arm设备如何使用SRS分发RTMP流？linux-arm上SRS的性能如何？参考：[SRS应用于linux-arm](v1_CN_SrsLinuxArm)

## [SRS的windows分支](v1_CN_WindowsSRS)

SRS在windows上支持到什么程度？客户端库支持得如何？服务器能在windows跑吗？参考：[SRS的windows分支](v1_CN_WindowsSRS)

## [SRS带宽测试](v1_CN_BandwidthTestTool)

视频很卡，播放不了，缓冲区突然很大，推流上不来，都有可能是带宽过低，SRS支持测试客户端到服务器的带宽，支持RTMP测速。参考：[SRS带宽测试](v1_CN_BandwidthTestTool)(BandwidthTestTool, BWTC)

## [RTMP ATC](v1_CN_RTMP-ATC)

如何让SRS分发ATC时间（即不从零开始）？如何支持HDS/HLS源站热备？参考：[RTMP ATC](v1_CN_RTMP-ATC)

## [TimeJitter时间戳矫正](v1_CN_TimeJitter)

SRS如何矫正时间戳？时间戳抖动如何处理？RTMP时间戳单增如何处理？参考：[TimeJitter时间戳矫正](v1_CN_TimeJitter)

## [Heartbeat](v1_CN_Heartbeat)

SRS如何以心跳形式汇报自己的信息？譬如arm嵌入式设备上运行服务器，每隔一定时间将自己的信息上报到api服务器。参考：[Heartbeat](v1_CN_Heartbeat)

## [点播FLV流](v1_CN_FlvVodStream)

SRS如何支持点播？FLV流如何支持？如何支持时移？录制的文件如何作为点播分发？参考：[点播FLV流](v1_CN_FlvVodStream)

## [GPERF内存和性能分析](v1_CN_GPERF)

如何查找内存泄漏？valgrind不支持st怎么办？如何知道哪个函数占用内存多？哪个函数性能有问题？参考：[GPERF内存和性能分析](v1_CN_GPERF)

## [GPROF性能分析](v1_CN_GPROF)

如何对SRS做性能优化？如何用gprof分析SRS性能？如何看性能的函数调用图？参考：[GPROF性能分析](v1_CN_GPROF)

## [Perf性能分析](v1_CN_Perf)

如何使用内核性能分析工具分析srs性能？参考：[Perf性能分析](v1_CN_Perf)

## [C++的开发环境](v1_CN_IDE)

C++如何选择开发环境？建议UltimateC++，当然等[jetbrains](http://www.jetbrains.com/idea/)出Windows下C++的IDE了会更好。参考：[C++的开发环境](v1_CN_IDE)

## [GIT获取代码](v1_CN_Git)

SRS的稳定版本是什么？如何获取SRS最新代码？GIT下载很慢怎么办？参考：[GIT获取代码](v1_CN_Git)

## [Usage: RTMP](v1_CN_SampleRTMP)

使用实例：如何使用SRS分发RTMP流？参考：[Usage: RTMP](v1_CN_SampleRTMP)

## [Usage: RTMP Cluster](v1_CN_SampleRTMPCluster)

使用实例：如何使用SRS分发RTMP直播流集群？参考：[Usage: RTMP Cluster](v1_CN_SampleRTMPCluster)

## [Usage: HLS](v1_CN_SampleHLS)

使用实例：如何使用SRS分发HLS(m3u8/ts)流？参考：[Usage: HLS](v1_CN_SampleHLS)

## [Usage: Transcode2HLS](v1_CN_SampleTranscode2HLS)

使用实例：如何对不符合要求的流(h.264+aac)进行HLS流分发？参考：[Usage: Transcode2HLS](v1_CN_SampleTranscode2HLS)

## [Usage: FFMPEG](v1_CN_SampleFFMPEG)

使用实例：如何使用SRS实时转码RTMP流？编码器不能输出h264+aac如何转码后分发HLS？参考：[Usage: FFMPEG](v1_CN_SampleFFMPEG)

## [Usage: Ingest](v1_CN_SampleIngest)

使用实例：如何将文件变成直播流？如何将已有流采集到SRS？如何做虚拟直播？如何抓取RTSP流？参考：[Usage: Ingest](v1_CN_SampleIngest)

## [Usage: Forward](v1_CN_SampleForward)

使用实例：如何使用SRS搭建小型集群？一路流如何做主备？如何将送到SRS的自动转发到备用服务器？参考：[Usage: Forward](v1_CN_SampleForward)

## [Usage: Realtime](v1_CN_SampleRealtime)

使用实例：如何尽量降低延迟到1秒之内？SRS最低延迟的配置？参考：[Usage: Realtime](v1_CN_SampleRealtime)

## [Usage: ARM](v1_CN_SampleARM)

使用实例：如何搭建ARM虚拟开发环境？如何在ARM上部署SRS服务器端？参考：[Usage: ARM](v1_CN_SampleARM)

## [Usage: HTTP](v1_CN_SampleHTTP)

使用实例：如何不依赖外部服务器分发HLS流？如何使用SRS内置的HTTP Server？参考：[Usage: HTTP](v1_CN_SampleHTTP)

## [Usage: DEMO](v1_CN_SampleDemo)

使用实例：如何部署SRS的DEMO（提供网页演示播放/推送RTMP到SRS，播放HLS，转码，视频会议等）？参考：[Usage: DEMO](v1_CN_SampleDemo)

Winlin 2014.2
