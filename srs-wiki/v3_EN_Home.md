[**HOME**](Home) > [**CN(2.0)**](v3_EN_Home)

## SRS Overview

SRS is industrial-strength live streaming cluster, for the best conceptual integrity and the simplest implementation. SRS provides variety of inputs, for example, [[Push RTMP to SRS | v1_EN_SampleRTMP ]], [[Push RTSP/UDP/FLV to SRS | v2_EN_Streamer]], [[Pull Stream to SRS | v1_EN_Ingest]]. SRS can transform the RTMP to other protocols or deliveries, for example, [[RTMP Transcode | v1_EN_SampleFFMPEG]], [[Snapshot| v3_EN_Snapshot]], [[Forward to Other Servers | v3_EN_SampleForward]], [[Remux to HTTP-FLV | v2_EN_SampleHttpFlv]], [[Remux to HLS | v1_EN_SampleHLS]], [[Remux to HDS | v2_EN_DeliveryHDS]], [[DVR to FLV | v3_EN_DVR]]. SRS canbe used in CDN for large stream clusters, for example, [[RTMP Cluster | v3_EN_SampleRTMPCluster]], [[VHOST | v1_EN_RtmpUrlVhost]], [[Reload | v1_EN_Reload]], [[HTTP-FLV Cluster | v3_EN_SampleHttpFlvCluster]]、[[Kafka | v3_EN_Kafka]]. Futhermore, SRS provides apis, for example, [[HTTP Callback | v3_EN_HTTPCallback]], [[Security | v2_EN_Security]], [[HTTP API | v3_EN_HTTPApi]], [[RTMP Bandwidth Test | v1_EN_BandwidthTestTool]].

## Downloading

Get SRS from the [downloads page][website] of the project website. If you'd like to build SRS from scratch, visit [[Build SRS | v3_EN_Build]].

SRS runs on Linuxs, for instace, Centos and Ubuntu, and x86, x86-64, ARM and MIPS is ok. MacOS only supports code edit and build. SRS does not support other Unix-like system, neither windows. SRS build on the coroutine library [state-threads][st], which simplify the complex protocol imlementations.

SRS is easy to run on a machine, or multiple machines, to run the cluster. SRS is single process, not multiple processes model; you can use [go-sharp][sharp] for HTTP-FLV multiple processes, and [srs-dolphin][dolphin] for RTMP.

## Where to Go from Here

***User Guides:***

* [Quick Start][qstart]: A quick introduction of SRS, please start here.
* [[Why SRS|v1_EN_Product]]: Why you should choose SRS? What's the roadmap?
* [GIT Mirrors][mirrors]: The GIT mirrors of SRS to get SRS faster.
* [Main Features][features]: The features list of SRS. Some features is introduced from specified version; while some features are experiment.
* [Releases][releases]: The released versions of SRS.
* [[Docs|v3_EN_Docs]]: The detail tech docs of SRS.

**Deployment Guides:***

* [[Delivery RTMP|v1_EN_SampleRTMP]]: How to delivery RTMP using SRS.
* [[Delivery HLS|v1_EN_SampleHLS]]: How to delivery RTMP and HLS using SRS.
* [[Delivery HTTP FLV|v2_EN_SampleHttpFlv]]: How to remux RTMP to HTTP FLV stream.
* [[Delivery HDS|v2_EN_DeliveryHDS]]: How to delivery HDS using SRS.
* [[Transcode|v1_EN_SampleFFMPEG]]: How to transcode the RTMP stream.
* [[Snapshot| v3_EN_Snapshot]]: How to snapshot thumbnail of RTMP stream.
* [[Forward|v3_EN_SampleForward]]: How to forward RTMP to other servers.
* [[Low latency|v3_EN_SampleRealtime]]: How to deploy the low latency SRS.
* [[Ingest|v1_EN_SampleIngest]]: How to ingest other streams to SRS.
* [[HTTP Server|v1_EN_SampleHTTP]]: How to deploy SRS as HTTP server.
* [[SRS DEMO|v1_EN_SampleDemo]]: How to deploy the demo of SRS.
* [[Projects|v1_EN_Sample]]: Who are using SRS.
* [[Setup|v1_EN_Setup]]: How to setup SRS.

***Cluster Guides:***

* [[RTMP Cluster|v3_EN_SampleRTMPCluster]]: Deploy the RTMP cluster for CDN.
* [[FLV Cluster|v3_EN_SampleHttpFlvCluster]]: Deploy the HTTP-FLV cluser for CDN.
* [[VHOST|v1_EN_RtmpUrlVhost]]: Support multiple customers in the same cluster by vhost.
* [[Reload|v1_EN_Reload]]: Apply changed config by reload.
* [[Tracable Log|v1_EN_SrsLog]]: Debug and trace user in cluster log.
* [[Kafka|v3_EN_Kafka]]: SRS report to Kafka/Spark big-data system.

***Integration Guides:***

* [[HTTP Callback | v3_EN_HTTPCallback]]: Use HTTP callback to hook the events of SRS.
* [[HTTP API | v3_EN_HTTPApi]]: Get system data from HTTP API of SRS.
* [[RTMP Bandwidth | v1_EN_BandwidthTestTool]]: Use bandwidth api to test the available bandwidth for SRS.
* [[Special Control | v3_EN_SpecialControl]]: Some special control config for expert.

***Migrate From NGINX-RTMP:***

* [[NG EXEC | v3_EN_NgExec]]: Fork/Exec external program when got some event.

**Benchmarks:***

* [Compare][compare]: The comparation of SRS to other servers.
* [Performance][performance]: The performance benchmark.

**Join Us:***

* [Donation][donation]: Donate SRS.
* [File Issue][issue]: File an issue.
* [[Contact|v1_EN_Contact]]: Contact us.

## Questions or need help?

For any help, [contact us](v1_EN_Contact).

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
