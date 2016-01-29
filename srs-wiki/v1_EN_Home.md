[**HOME**](Home) > [**CN(1.0)**](v1_EN_Home)

## SRS Overview

SRS is industrial-strength live streaming cluster, for the best conceptual integrity and the simplest implementation. SRS provides variety of inputs, for example, [[Push RTMP to SRS | v1_EN_SampleRTMP ]], [[Pull Stream to SRS | v1_EN_Ingest]]. SRS can transform the RTMP to other protocols or deliveries, for example, [[RTMP Transcode | v1_EN_SampleFFMPEG]], [[Forward to Other Servers | v1_EN_SampleForward]], [[Remux to HLS | v1_EN_SampleHLS]], [[DVR to FLV | v1_EN_DVR]]. SRS canbe used in CDN for large stream clusters, for example, [[RTMP Cluster | v1_EN_SampleRTMPCluster]], [[VHOST | v1_EN_RtmpUrlVhost]], [[Reload | v1_EN_Reload]]. Futhermore, SRS provides apis, for example, [[HTTP Callback | v1_EN_HTTPCallback]], [[HTTP API | v1_EN_HTTPApi]], [[RTMP Bandwidth Test | v1_EN_BandwidthTestTool]].

## Downloading

Get SRS from the [downloads page][website] of the project website. If you'd like to build SRS from scratch, visit [[Build SRS | v1_EN_Build]].

SRS runs on Linuxs, for instace, Centos and Ubuntu, and x86, x86-64, ARM and MIPS is ok. MacOS only supports code edit and build. SRS does not support other Unix-like system, neither windows. SRS build on the coroutine library [state-threads][st], which simplify the complex protocol imlementations.

SRS is easy to run on a machine, or multiple machines, to run the cluster. SRS is single process, not multiple processes model.

## Where to Go from Here

***User Guides:***

* [Quick Start][qstart]: A quick introduction of SRS, please start here.
* [[Why SRS|v1_EN_Product]]: Why you should choose SRS? What's the roadmap?
* [GIT Mirrors][mirrors]: The GIT mirrors of SRS to get SRS faster.
* [Main Features][features]: The features list of SRS. Some features is introduced from specified version; while some features are experiment.
* [Releases][releases]: The released versions of SRS.
* [[Docs|v1_EN_Docs]]: The detail tech docs of SRS.

**Deployment Guides:***

* [[Delivery RTMP|v1_EN_SampleRTMP]]: How to delivery RTMP using SRS.
* [[Delivery HLS|v1_EN_SampleHLS]]: How to delivery RTMP and HLS using SRS.
* [[Transcode|v1_EN_SampleFFMPEG]]: How to transcode the RTMP stream.
* [[Forward|v1_EN_SampleForward]]: How to forward RTMP to other servers.
* [[Low latency|v1_EN_SampleRealtime]]: How to deploy the low latency SRS.
* [[Ingest|v1_EN_SampleIngest]]: How to ingest other streams to SRS.
* [[HTTP Server|v1_EN_SampleHTTP]]: How to deploy SRS as HTTP server.
* [[SRS DEMO|v1_EN_SampleDemo]]: How to deploy the demo of SRS.
* [[Projects|v1_EN_Sample]]: Who are using SRS.
* [[Setup|v1_EN_Setup]]: How to setup SRS.

***Cluster Guides:***

* [[RTMP Cluster|v1_EN_SampleRTMPCluster]]: Deploy the RTMP cluster for CDN.
* [[VHOST|v1_EN_RtmpUrlVhost]]: Support multiple customers in the same cluster by vhost.
* [[Reload|v1_EN_Reload]]: Apply changed config by reload.
* [[Tracable Log|v1_EN_SrsLog]]: Debug and trace user in cluster log.

***Integration Guides:***

* [[HTTP Callback | v1_EN_HTTPCallback]]: Use HTTP callback to hook the events of SRS.
* [[HTTP API | v1_EN_HTTPApi]]: Get system data from HTTP API of SRS.
* [[RTMP Bandwidth | v1_EN_BandwidthTestTool]]: Use bandwidth api to test the available bandwidth for SRS.

**Benchmarks:***

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

[qstart]: https://github.com/simple-rtmp-server/srs/tree/1.0release#usage
[mirrors]: https://github.com/simple-rtmp-server/srs/tree/1.0release#mirrors
[features]: https://github.com/simple-rtmp-server/srs/tree/1.0release#summary
[releases]: https://github.com/simple-rtmp-server/srs/tree/1.0release#releases

[donation]: http://www.ossrs.net/srs.release/donation/index.html
[issue]: https://github.com/simple-rtmp-server/srs/issues/new

[performance]: https://github.com/simple-rtmp-server/srs/tree/1.0release#performance
