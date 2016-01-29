Welcome to the simple-rtmp-server wiki!

## [How To Ask Questions](v1_EN_HowToAskQuestion)

To ask questions or file bug reports. Please read [HowToAskQuestion](v1_EN_HowToAskQuestion)

## [SRS Product](v1_EN_Product)

What's the product plan of SRS? What are the key features? Please read [SRS Product] (v1_EN_Product)

## [Product Comparison](v2_EN_Compare)

For a comparison of SRS and other streaming media servers. Please read [Product Comparison](v2_EN_Compare)

## [SRS Features](v2_EN_Features)

The key features of SRS, the implementation, both planned and not planned. Please read [SRS Features](v2_EN_Features)

## [SRS Demo Server](v1_EN_LiveShow)

Is there a public server for an SRS demo? I provide a server with public ip and name (ossrs.net) for all users to publish and play, it's free but it's only for testing. Please read [SRS Demo Server](v1_EN_LiveShow)

## [Applications](v1_EN_Sample)

Who is using SRS? What can you use SRS for, for your project and customers? Please read [Applications](v1_EN_Sample)

## [Build SRS](v3_EN_Build)

How to build SRS? How to enable or disable the feature of SRS? How to build and start the demo pages of SRS? Please read [Build SRS](v3_EN_Build)

## [SRS Init.d Script](v1_EN_LinuxService)

How to use init.d script to start SRS as a system service? How to run SRS as a daemon? How to auto start SRS when the system restarts? How to start SRS demo pages (including the players, encoders and video meeting demo)? Please read [SRS Init.d Script](v1_EN_LinuxService)

## [SRS Log](v1_EN_SrsLog)

How to get the SRS logs? How to trace logs to console? How to get less verbose logs because the log files are too large? How to get more detailed log levels? How to get warning level log only? Please read [SRS Log](v1_EN_SrsLog)

## [SRS Architecture](v3_EN_Architecture)

The architecture for all SRS developers。Please read [SRS Architecture](v3_EN_Architecture)

## [RTMP URL & Vhost](v1_EN_RtmpUrlVhost)

What is the vhost or app or stream of RTMP? How to pass the authentication params to the server in the RTMP URL? Please read [RTMP URL & Vhost](v1_EN_RtmpUrlVhost)

## [RTMP Handshake](v1_EN_RTMPHandshake)

Why build openssl for RTMP server? What are the simple and complex handshakes of RTMP? Which scenarios should use simple handshake? Please read [RTMP Handshake](v1_EN_RTMPHandshake)

## [FFMPEG Live Transcoding](v3_EN_FFMPEG)

How to use SRS to transcode live streams? How to transcode audio only? How to passthrough audio or video? How to add a logo to a stream? What are the transcode params and the sequence for FFPMEG? Please read [FFMPEG Live Transcoding](v3_EN_FFMPEG)

## [RTMP Delivery](v1_EN_DeliveryRTMP)

How to support RTMP streaming with SRS? How to configure SRS to support RTMP? What is the benefit of RTMP? Please read [RTMP Delivery](v1_EN_DeliveryRTMP)

## [HLS Delivery](v2_EN_DeliveryHLS)

How to convert RTMP to be delivered as HLS? What is HLS? How to configure HLS in SRS? Please read [HLS Delivery](v2_EN_DeliveryHLS)

## [HDS Delivery](v2_EN_DeliveryHDS)

How to convert RTMP to be delivered as HDS? What is HDS? How to configure HDS in SRS? Please read [HDS Delivery](v2_EN_DeliveryHDS)

## [HTTP Stream Delivery](v2_EN_DeliveryHttpStream)

How to deliver FLV VOD stream, or to support `xxx.flv?start=offset`? How to deliver FLV live stream, or to remux RTMP to FLV? How to deliver HTTP AAC stream? How to deliver HTTP TS stream? How to deliver HTTP MP3 stream? Please read [HTTP Stream Delivery](v2_EN_DeliveryHttpStream)

## [RTMP PK HLS](v1_EN_RTMP.PK.HTTP)

How to choose a delivery protocol? What about RTMP? What about HTTP? What about HLS? The history and use scenarios of RTMP, HTTP and HLS. Please read [RTMP PK HLS](v1_EN_RTMP.PK.HTTP)

## [Embedded HTTP Server](v2_EN_HTTPServer)

Can SRS support HTTP? How to deliver files over HTTP with SRS? Why does SRS support an embedded HTTP server? Can you use an external HTTP Server? Please read [Embedded HTTP Server](v2_EN_HTTPServer)

## [DVR record live stream] (v3_EN_DVR)

How to record a live stream to an FLV file? How to record to pieces of files which start from keyframes with similar duration? What is the DVR plan, for example, segment and session plan? How to specify the path and filename? Please read [DVR record live stream] (v3_EN_DVR)

## [DRM] (v3_EN_DRM)

How to support hotlink denial? Does SRS support DRM? How to configure referrer DRM? How to implement token DRM? Does SRS support FMS token traversal? Please read [DRM] (v3_EN_DRM)

## [Reload](v1_EN_Reload)

How to reload SRS? How to add new vhosts without restarting the server? How to change the configuration without restarting the server? Please read [Reload](v1_EN_Reload)

## [Low Latency](v3_EN_LowLatency)

How to use SRS in pseudo realtime? What is the latency? What is the minimum latency of SRS in a commercial scenario, for example, CDN networks? Please read [Low Latency](v3_EN_LowLatency)

## [HTTP Callback](v3_EN_HTTPCallback)

About the HttpCallback or HttpHooks. How to authenticate the client? How to make callbacks to external HTTP APIs when clients publish a stream? How to notify your system when clients connect or disconnect? How to hook into the DVR? Please read [HTTP Callback](v3_EN_HTTPCallback)

## [HTTP Management API](v3_EN_HTTPApi)

How to manage the server via the HTTP management API? How to manage the server, for example, to get server status in HTML pages? How can PHP/python management systems manage SRS? Please read [HTTP Management API](v3_EN_HTTPApi)

## [Ingest](v1_EN_Ingest)

How does SRS pull specified RTMP/RTSP/HTTP streams and deliver them? How to deliver your RTSP stream from your ancient RTSP camera to RTMP, then deliver it to CDNs as RTMP or HLS? How to encode the signals from a device to output to SRS and then deliver them to the Internet? Please read [Ingest](v1_EN_Ingest)

## [Stream Cast Service](v2_EN_Streamer)

How to push RTSP to SRS? How does SRS accept RTSP streams? How to push MPEG-TS over UDP to SRS? How does SRS accept streams over other protocols? Read [Streamer](v2_EN_Streamer)

## [Edge: Huge Live Cluster](v3_EN_Edge)

How to use SRS with CDNs? Does SRS support the key features for CDN: vhost, edge, reload, HTTP API and MIT source code? Why forwarding and ingesting is not okay for huge live clusters? Does SRS support FMS edge mode? Please read [Edge: Huge Live Cluster](v3_EN_Edge)

## [Forward: Tiny Live Cluster](v3_EN_Forward)

How to use SRS for a small number of streams and server cluster? How to build origin fault backup with only one stream? Please read [Forward: Tiny Live Cluster](v3_EN_Forward)

## [Kafka/Spark Cluster](v3_EN_Kafka)

SRS how to report to Kafka/Spark big-data system? How to analysis and manage the big SRS clusters? Please read [Kafka/Spark Cluster](v3_EN_Kafka)

## [Security: Allow or Deny Clients](v2_EN_Security)

How to apply a security strategy? How to deny all publishing and allow all playback? How to deny clients by specified IP addresses? Please read [Security: Allow or Deny Clients](v2_EN_Security)

## [Exec: Fork external program](v3_EN_NgExec)

How to support exec or exec_publish like nginx-rtmp? Please read [Exec: Fork external program](v3_EN_NgExec)

## [Performance Benchmark](v1_EN_Performance)

How many concurrent connections can SRS serve? What about the performance of SRS? We compare the high performance server nginx-rtmp and SRS, and provide some benchmarks for server performance checks. Please read [Performance Benchmark](v1_EN_Performance)

## [RaspberryPi Performance Benchmark](v1_EN_RaspberryPi)

What about the performance benchmark on arm (raspberrypi)? How many concurrent connections are supported on arm for SRS? Please read [RaspberryPi Performance Benchmark](v1_EN_RaspberryPi)

## [RaspberryPi Transcoding](v1_EN_ARMTranscode)

How do you transcode live streams on a raspberrypi? How do you deliver HLS on raspberrypi? Please read [RaspberryPi Transcoding](v1_EN_ARMTranscode)

## [ServerSide Script](v1_EN_ServerSideScript)

Why doesn't SRS support ServerSide script? How to use HTTP callbacks instead? Please read [ServerSide Script](v1_EN_ServerSideScript)

## [Flash/Android/IOS P2P](v2_EN_WebP2P)

How does SRS support Flash/Android/IOS P2P? Please read [Flash/Android/IOS P2P](v2_EN_WebP2P)

## [SRS-librtmp](v2_EN_SrsLibrtmp)

Why does SRS provide a client RTMP library? How to use srs-librtmp? How to modify the amf0 metadata to use srs-librtmp? How to get server debug info from srs-librtmp? Please read [SRS-librtmp](v2_EN_SrsLibrtmp)

## [SRS for linux-arm](v1_EN_SrsLinuxArm)

How to use SRS on linux-arm devices to deliver RTMP? What is the performance of SRS on linux-arm? Please read [SRS for linux-arm](v1_EN_SrsLinuxArm)

## [SRS Windows Branch](v1_EN_WindowsSRS)

What is the status of the SRS Windows branch? Why is the SRS Windows branch experimental and not recommended for use? What about the srs-librtmp Windows branch? Why is srs-librtmp Windows branch okay for Windows? Please read [SRS windows Branch](v1_EN_WindowsSRS)

## [SRS Bandwidth Test](v1_EN_BandwidthTestTool)

What is the bandwidth between the player and the SRS server? SRS provides c (in srs-librtmp), javascript and flash SDKs to test the bandwidth. Please read [SRS Bandwidth Test](v1_EN_BandwidthTestTool)(BandwidthTestTool, BWTC)

## [RTMP ATC](v3_EN_RTMP-ATC)

How to deliver streams with ATC timestamp? How to support HDS/HLS origin server fault-backup? Please read [RTMP ATC](v3_EN_RTMP-ATC)

## [TimeJitter Timestamp Correct](v3_EN_TimeJitter)

How does SRS correct the timestamps of streams? How to correct the time jitter? How to ensure the timestamp of RTMP to increase monotonically? Please read [TimeJitter Timestamp Correct](v3_EN_TimeJitter)

## [Heartbeat](v1_EN_Heartbeat)

SRS can report status to external HTTP servers by POSTing to an HTTP URL. For example, SRS on arm can report its data to a management server. Please read [Heartbeat](v1_EN_Heartbeat)

## [FLV VOD stream](v2_EN_FlvVodStream)

Does SRS support VOD? What about the FLV VOD stream? How to support time-shift for live streams? How to deliver DVR files as VOD streams? Please read [FLV VOD stream](v2_EN_FlvVodStream)

## [GPERF Memory and Performance Analysis](v1_EN_GPERF)

How to figure out memory leaks? Why use gmc? Why isn't valgrind supported by SRS? How to figure out performance bottlenecks? Please read [GPERF Memory and Performance Analysis](v1_EN_GPERF)

## [GPROF Performance Analysis](v1_EN_GPROF)

How to figure out the performance issues of SRS? How to use gprof? How to draw performance graphs using dot? Please read [GPROF Performance Analysis](v1_EN_GPROF)

## [Perf Performance Analysis](v1_EN_Perf)

How to use the kernel performance tool perf to figure out the performance issues of SRS? Please read [Perf Performance Analysis](v1_EN_Perf)

## [C++ IDE](v2_EN_IDE)

How to choose a C++ IDE? We recommend the UltimateC++ or Clion. Please read [C++ IDE](v2_EN_IDE)

## [GIT Usage](v1_EN_Git)

How to get the stable release of SRS? How to merge master with your branch? How to use CSDN mirror in China? Please read [GIT Usage](v1_EN_Git)

## [Usage: RTMP](v1_EN_SampleRTMP)

Usage: How to use SRS to deliver RTMP? Please read [Usage: RTMP](v1_EN_SampleRTMP)

## [Usage: RTMP Cluster](v3_EN_SampleRTMPCluster)

Usage: How to use SRS to deliver RTMP cluster? Please read [Usage: RTMP Cluster](v3_EN_SampleRTMPCluster)

## [Usage: HTTP FLV](v2_EN_SampleHttpFlv)

Usage: How to use SRS to deliver HTTP FLV live streaming? Please read [Usage: HTTP FLV](v2_EN_SampleHttpFlv)

## [Usage: HTTP FLV Cluster](v3_EN_SampleHttpFlvCluster)

Usage: How to use SRS to deliver HTTP FLV live streaming cluster? Please read [Usage: HTTP FLV Cluster](v3_EN_SampleHttpFlvCluster)

## [Usage: HLS](v1_EN_SampleHLS)

Usage: How to use SRS to deliver HLS? Please read [Usage: HLS](v1_EN_SampleHLS)

## [Usage: Transcode2HLS](v1_EN_SampleTranscode2HLS)

Usage: How to use SRS to transcode stream (non H.264+AAC) to deliver HLS? Please read [Usage: Transcode2HLS](v1_EN_SampleTranscode2HLS)

## [Usage: FFMPEG](v1_EN_SampleFFMPEG)

Usage: How to use SRS to transcode streams? How to transcode codecs for HLS? Please read [Usage: FFMPEG](v1_EN_SampleFFMPEG)

## [Usage: Ingest](v1_EN_SampleIngest)

Usage: How to ingest files to live stream? How to ingest existing streams to SRS? How to use SRS for virtual live streaming? How to ingest RTSP to SRS? Please read [Usage: Ingest](v1_EN_SampleIngest)

## [Usage: Forward](v3_EN_SampleForward)

Usage: How to use SRS to build tiny clusters? How to build an origin backup cluster when you only have one stream? How to forward streams to other servers from SRS? How to forward streams to a backup server from SRS? Please read [Usage: Forward](v3_EN_SampleForward)

## [Usage: Realtime](v3_EN_SampleRealtime)

Usage: How to decrease the latency to <1s? How to configure SRS in minimum latency mode? Please read [Usage: Realtime](v3_EN_SampleRealtime)

## [Usage: ARM](v1_EN_SampleARM)

Usage: How to run SRS on ARM linux? Please read [Usage: ARM](v1_EN_SampleARM)

## [Usage: HTTP](v2_EN_SampleHTTP)

Usage: How to deliver HLS without an external HTTP server? How to use the embedded HTTP server of SRS? Please read [Usage: HTTP](v2_EN_SampleHTTP)

## [Usage: DEMO](v1_EN_SampleDemo)

Usage: How to deploy the demo of SRS? The SRS demo provides players, flash encoders, live meeting demo, flash HLS player, and transcoding. Please read [Usage: DEMO](v1_EN_SampleDemo)

Winlin 2015.5
