Welcome to the simple-rtmp-server wiki!

## [How To Ask Questions](v1_EN_HowToAskQuestion)

To ask questions or file bug, please see [HowToAskQuestion](v1_EN_HowToAskQuestion)

## [SRS Product](v1_EN_Product)

What's the product plan of SRS? What's the key features? Please see: [SRS Product] (v1_EN_Product)

## [Product Comparision](v1_EN_Compare)

Comparision of SRS and other streaming media server, please see the [Product Comparision](v1_EN_Compare)

## [SRS Features](v1_EN_Features)

The key features of SRS, the implemented, in plan and not plan. Please see: [SRS Features](v1_EN_Features)

## [SRS Demo Server](v1_EN_LiveShow)

Is there any public server for SRS demo? I provides server with public ip and name(ossrs.net) for all users to publish and play, it's free and only for test. Please see: [SRS Demo Server](v1_EN_LiveShow)

## [Applications](v1_EN_Sample)

Who are using SRS? What you can use SRS for your project and customers? Please see: [Applications](v1_EN_Sample)

## [Build SRS](v1_EN_Build)

How to build SRS? How to enable or disable the feature of SRS? How to build and start the demo pages of SRS? Please see: [Build SRS](v1_EN_Build)

## [SRS Init.d Script](v1_EN_LinuxService)

How to use init.d script to start SRS as system service? How to run SRS in daemon? How to auto start SRS when system restart? How to start SRS demo pages(including the players, encoders and video meeting demo)? Please see: [SRS Init.d Script](v1_EN_LinuxService)

## [SRS Log](v1_EN_SrsLog)

How to get the log of SRS? How to trace log to console? How to get less log for log file too large? How to get more detail level log? How to get warn level log only? Please see: [SRS Log](v1_EN_SrsLog)

## [SRS Architecture](v1_EN_Architecture)

The architecture for all SRS developers。Please see: [SRS Architecture](v1_EN_Architecture)

## [RTMP URL & Vhost](v1_EN_RtmpUrlVhost)

What is the vhost or app or stream of RTMP? How to pass the authentication params to server in RTMP url? Please see: [RTMP URL & Vhost](v1_EN_RtmpUrlVhost)

## [RTMP Handshake](v1_EN_RTMPHandshake)

Why build openssl for RTMP server? What is the simple and complex handshake of RTMP? Which scenario to use simple handshake? Please see: [RTMP Handshake](v1_EN_RTMPHandshake)

## [FFMPEG Live Transcoding](v1_EN_FFMPEG)

How to use SRS to transcode live stream? How to transcode audio only? How to passthrough audio or video? How to add logo to stream? What is the transcode params and the sequence for FFPMEG? Please see: [FFMPEG Live Transcoding](v1_EN_FFMPEG)

## [Delivery RTMP](v1_EN_DeliveryRTMP)

How SRS to support RTMP streaming? How to config SRS to support RTMP? What is the benifit of RTMP? Please see: [Delivery RTMP](v1_EN_DeliveryRTMP)

## [Delivery HLS](v1_EN_DeliveryHLS)

How to convert RTMP to delivery as HLS? What is HLS tech? How to config HLS in SRS? Please see: [Delivery HLS](v1_EN_DeliveryHLS)

## [RTMP PK HLS](v1_EN_RTMP.PK.HTTP)

How to choose delivering protocol? What about RTMP? What about HTTP? What about HLS? The history and use scenarios of RTMP, HTTP and HLS. Please see: [RTMP PK HLS](v1_EN_RTMP.PK.HTTP)

## [Embeded HTTP Server](v1_EN_HTTPServer)

Can SRS support HTTP? How to delivery files over HTTP by SRS? Why SRS support embeded HTTP server? Can you use external HTTP Server? Please see: [Embeded HTTP Server](v1_EN_HTTPServer)

## [DVR record live stream] (v1_EN_DVR)

How to record live stream to flv file? How to record to pieces of files which start from keyframe with similar duration? What is the dvr plan, for example, segment and session plan? Please see: [DVR record live stream] (v1_EN_DVR)

## [DRM] (v1_EN_DRM)

How to support hotlink denial? Does SRS support DRM? How to config refer DRM? How to implement token DRM? Does SRS support FMS token traverse? Please see: [DRM] (v1_EN_DRM)

## [Reload](v1_EN_Reload)

How to reload SRS? How to add new vhost and donot restart server? How to change config and donot start server? Please see: [Reload](v1_EN_Reload)

## [Low Latency](v1_EN_LowLatency)

How to use SRS in pseudo realtime? What is the latency? What is the mininum latency of SRS in commercial scenario, for example, CDN network? Please see: [Low Latency](v1_EN_LowLatency)

## [HTTP Callback](v1_EN_HTTPCallback)

How to authenticate the client? How to callback external http api when client publish stream? How to notice your system when client connect or disconnect? Please see: [HTTP Callback](v1_EN_HTTPCallback)

## [HTTP Management API](v1_EN_HTTPApi)

How to manage server by http management api? How to manage server, for example, get server status in HTML pages? How your php/python management system to manage the SRS? Please see: [HTTP Management API](v1_EN_HTTPApi)

## [Ingest](v1_EN_Ingest)

How does SRS to pull specified RTMP/RTSP/HTTP stream to itself and delivery it? How to delivery your RTSP stream from your ancient RTSP camera to RTMP, then deverliry to CDN as RTMP or HLS? How to encode the singals from device to output to SRS then delivery to internet? Please see: [Ingest](v1_EN_Ingest)

## [Edge: Huge Live Cluster](v1_EN_Edge)

How to use SRS in CDN? Does SRS suport the key freatures for CDN: vhost, edge, reload, http api and MIT source code? Why forward and ingest is not ok for huge live cluster? Does SRS support FMS edge mode? Please see: [Edge: Huge Live Cluster](v1_EN_Edge)

## [Forward: Tiny Live Cluster](v1_EN_Forward)

How to use SRS for few stream and servers cluster? How to build origin fault backup when only one stream? Please see: [Forward: Tiny Live Cluster](v1_EN_Forward)

## [Performance Benchmark](v1_EN_Performance)

How many concurrency connections can SRS to serve? What about the performance of SRS? We compare the high performance server nginx-rtmp and SRS, and provides some benchmarks for server performance check. Please see: [Performance Benchmark](v1_EN_Performance)

## [RaspberryPi Performance Benchmark](v1_EN_RaspberryPi)

What about the performance benchmark on arm(respberrypi)? How many concurrency connections support on arm for SRS? Please see: [RaspberryPi Performance Benchmark](v1_EN_RaspberryPi)

## [RaspberryPi Transcoding](v1_EN_ARMTranscode)

How to transcode live stream on raspberrypi? How to delivery HLS on raspberrypi? Please see: [RaspberryPi Transcoding](v1_EN_ARMTranscode)

## [ServerSide Script](v1_EN_ServerSideScript)

Why SRS does not support ServerSide Script? How to use http callback instead? Please see: [ServerSide Script](v1_EN_ServerSideScript)

## [SRS-librtmp](v1_EN_SrsLibrtmp)

Why SRS provides client rtmp library? How to use srs-librtmp? How to modify the amf0 metadata use srs-librtmp? How to get server debug info from srs-librtmp? Please see: [SRS-librtmp](v1_EN_SrsLibrtmp)

## [SRS for linux-arm](v1_EN_SrsLinuxArm)

How to use SRS on linux-arm device to delivery RTMP? What is the performance of SRS on linux-arm? Please see: [SRS for linux-arm](v1_EN_SrsLinuxArm)

## [SRS windows Branch](v1_EN_WindowsSRS)

What is the status of SRS windows branch? Why SRS windows branch is experiment and not recomment to use? What about the srs-librtmp windows branch? Why srs-librtmp windows branch is ok for windows? Please see: [SRS windows Branch](v1_EN_WindowsSRS)

## [SRS Bandwidth Test](v1_EN_BandwidthTestTool)

What is the bandwidth between the player and the SRS server? SRS provides c(in srs-librtmp), js and flash sdk to test the bandwidth. Please see: [SRS Bandwidth Test](v1_EN_BandwidthTestTool)(BandwidthTestTool, BWTC)

## [RTMP ATC](v1_EN_RTMP-ATC)

How to delivery stream with ATC timestamp? How to support HDS/HLS origin server fault-backup? Please see: [RTMP ATC](v1_EN_RTMP-ATC)

## [TimeJitter Timestamp Correct](v1_EN_TimeJitter)

How does SRS correct the timestamp of stream? How to correct the time jitter? How to ensure the timestamp of RTMP to monotonically increase? Please see: [TimeJitter Timestamp Correct](v1_EN_TimeJitter)

## [Heartbeat](v1_EN_Heartbeat)

SRS can report the status to external HTTP server by POST to a http url. For example, the SRS on arm can report its data to management server. Please see: [Heartbeat](v1_EN_Heartbeat)

## [FLV VOD stream](v1_EN_FlvVodStream)

Does SRS support VOD? What about the FLV VOD stream? How to support time-shift for live stream? How to delivery the DVR files as VOD stream? Please see: [FLV VOD stream](v1_EN_FlvVodStream)

## [GPERF Memory and Performance Analysis](v1_EN_GPERF)

How to finger out the memory leak? Why use gmc? Why valgrind not supported by SRS? How to finger out the bottle neck of performance. Please see: [GPERF Memory and Performance Analysis](v1_EN_GPERF)

## [GPROF Performance Analysis](v1_EN_GPROF)

How to finger out the performance issue of SRS? How to use gprof? How to draw the performance graph using dot? Please see: [GPROF Performance Analysis](v1_EN_GPROF)

## [Perf Performance Analysis](v1_EN_Perf)

How to use kernel performance tools perf to finger out the performance issue of SRS? Please see: [Perf Performance Analysis](v1_EN_Perf)

## [C++ IDE](v1_EN_IDE)

How to choose C++ IDE? We recomment the UltimateC++ or Clion. Please see: [C++ IDE](v1_EN_IDE)

## [GIT Usage](v1_EN_Git)

How to get the stable release of SRS? How to merge master with your branch? How to use CSDN mirror in China? Please see: [GIT Usage](v1_EN_Git)

## [Usage: RTMP](v1_EN_SampleRTMP)

Usage: How to use SRS to delivery RTMP? Please see: [Usage: RTMP](v1_EN_SampleRTMP)

## [Usage: RTMP Cluster](v1_EN_SampleRTMPCluster)

Usage: How to use SRS to delivery RTMP Cluster? Please see: [Usage: RTMP Cluster](v1_EN_SampleRTMPCluster)

## [Usage: HLS](v1_EN_SampleHLS)

Usage: How to use SRS to delivery HLS? Please see: [Usage: HLS](v1_EN_SampleHLS)

## [Usage: Transcode2HLS](v1_EN_SampleTranscode2HLS)

Usage: How to use SRS to transcode stream(not h.264+aac) to delivery HLS? Please see: [Usage: Transcode2HLS](v1_EN_SampleTranscode2HLS)

## [Usage: FFMPEG](v1_EN_SampleFFMPEG)

Usage: How to use SRS to transcode stream? How to transcode the codec for HLS? Please see: [Usage: FFMPEG](v1_EN_SampleFFMPEG)

## [Usage: Ingest](v1_EN_SampleIngest)

Usage: How to ingest file to live stream? How to ingest exists stream to SRS? How to use SRS for virtual live streaming? How to ingest RTSP to SRS? Please see: [Usage: Ingest](v1_EN_SampleIngest)

## [Usage: Forward](v1_EN_SampleForward)

Usage: How to use SRS to build tiny cluster? How to build origin backup cluster when only one stream? How to forward stream to other servers from SRS? How to forward stream to backup server from SRS? Please see: [Usage: Forward](v1_EN_SampleForward)

## [Usage: Realtime](v1_EN_SampleRealtime)

Usage: How to descrease the lantecy to <1s? How to config SRS in mininum latency mode? Please see: [Usage: Realtime](v1_EN_SampleRealtime)

## [Usage: ARM](v1_EN_SampleARM)

Usage: How to run SRS on ARM linux? Please see: [Usage: ARM](v1_EN_SampleARM)

## [Usage: HTTP](v1_EN_SampleHTTP)

Usage: How to delivery HLS without external HTTP server? How to use the embeded HTTP server of SRS? Please see: [Usage: HTTP](v1_EN_SampleHTTP)

## [Usage: DEMO](v1_EN_SampleDemo)

Usage: How to deploy the demo of SRS? The SRS demo provides players, flash encoders, live meeting demo, flash HLS player, transcoding. Please see: [Usage: DEMO](v1_EN_SampleDemo)

Winlin 2014.10
