# Product Compare

We compare some famous media streaming server with SRS.

SRS focus on live streaming server, to provide the best feature or nothing, see [Product](v1_EN_Product).

SRS use the best tech for RTMP protocol, the coroutine, which can simplify the state machine of async epoll states, provides by st(state-threads), see [Architecture](v1_EN_Architecture)。

For detail feature and roadmap, see [Features](v1_EN_Features)

## Introduction

The keyword we using:
* RTMPS/E/T：RTMPS/E is encrypt protocol, RTMPT is RTMP over HTTP to traverse firewalls.
* DASH：the HLS similar protocol design by some company.
* Change Vhost：for upnode speedup, to publish stream to RTMP cluster, CDN need to change vhost to use different device group for publish and play service.
* Readable Config: FMS/Helix/Wowza use xml config, it's very ugly. nginx style config is json-similar, modern server config. SRS use nginx style config.
* Service Script: linux system service script, see /etc/init.d/srs.
* Origin Suite: other origin software suite, for example, vms, transcode system, encoder, recorder, p2p.
* Server Side Script: the as of FMS, lua for crtmpd/nginx. SRS never support server-side script, for it's hard to debug and maintain and more bug introduced.
* Single Thread: modern linux use epoll to support 10k+ concurrency by single thread and none-blocking async socket.
* Coroutine: the coroutine is a domain of C, similar to eventlet of python, golang of google. A new domain of server side development tech.
* Maintainable: to get more maintable code, for the same features, we must write less code, add more comments, simplify the logic and use the best arch for the problem domain.
* Tracable log: client can get its id on server, edge can get its id on upnode, to get the log base on session by grep the id.

## History

The history of media stream industry.

![Media Stream Servers History](http://winlinvip.github.io/srs.release/wiki/images/compare/100.release.png)

## Protocol

Specified use scenarios must use specified protocols, because specified protocol is design for specified use scenarios. For example, udp design for 0-1000ms latency use scenario, RTMP for 1s-10s, while HLS for 10s+, vice versa.

There is no simple or good tech, and a tech is simple or good only when huge users and company spend time on it. For instance, RTMP for flash is ok for Adobe Player support it, user only need 4lines to play a RTMP stream, nothing more than NetConnection.connect(tcUrl) then NetStream.play(streamName). For example, HLS is ok for IOS/MAC for Apple Safari support it, user only need to use html5 or redirect to the http://xxx/xxx.m3u8. But, neither RTMP nor HLS is simple or good, both are complex and complex.

For internet, a "open" protocol is very important, so the RTSP never use in internet for most of company are using RTSP as private protocol. HTTP is "open" enough, and HTTP/HLS can be used in internet streaming protocol. RTMP is private but supported by flash which running almost on all PC, and RTMP can be "open" protocol for internet(PC only), and mobile platform can use RTMP by FFMPEG or AVFormat library. So, RTMP on PC(flash) is very simple for client, but complex for mobile to support RTMP.

![Media Stream Servers Protocol](http://winlinvip.github.io/srs.release/wiki/images/compare/200.protocol.png)

## Feature

It's not good enough for a streaming system to provides delivery feature, especially for public or private CDN. For example, nginx-rtmp can transcode by exec external FFMPEG, and SRS support FFMPEG by specifies the path and video/audio params and SRS will auto restart FFMPEG when quit. It's very important for user to simple the problem domain, for instance, user can modify the config item fps from 20 to 25, but it's very complex for user to modify the FFMPEG param -v:f which maybe changed for different version.

![Media Stream Servers Feature](http://winlinvip.github.io/srs.release/wiki/images/compare/300.feature.png)

## Deploy

SRS support all linux, including x86/x64/arm/mips cpu.

![Media Stream Servers Deploy](http://winlinvip.github.io/srs.release/wiki/images/compare/400.deploy.png)

## Arch

First generation server arch: FMS/Helix/Wowza, multiple thread, single process, sync and blocking io.

Second generation server arch: nginx/crtmpd, single thread, single/multiple process, async and none-blocking io.

Third generation server arch: go/eventlet/srs, single thread, single/multiple process, async and none-blocking io.

![Media Stream Servers Architecture](http://winlinvip.github.io/srs.release/wiki/images/compare/500.arch.png)

## CDN/VDN

Special key features for cdn(content delivery network) or vdn(video delivery network).

![Media Stream Servers CDN/VDN](http://winlinvip.github.io/srs.release/wiki/images/compare/600.cdn.png?version=1.0)

## Code

Code lines and comments lines, maybe means nothing, whatever, it just a data of server comparation.

![Media Stream Servers Code](http://winlinvip.github.io/srs.release/wiki/images/compare/700.code.png?v=3)

## SRS history

Cook needs time, the server development also needs long long time to code, refine and evolve. The releases and milestone makes the Marathon easier.

![Media Stream Servers SRS History](http://winlinvip.github.io/srs.release/wiki/images/compare/800.srs.higtory.png?v=1)

## FMS PK SRS

FMS is a media streaming server of Adobe, which created RTMP protocol. FMS is a very important one, and it evolved from 1.0 to 5.0. FMS(Flash Media Server) 5.0 renamed to AMS(Adobe Media Server).

FMS better than SRS for:
* Features：FMS supports RTMP/RTMPE/RTMPS/RTMPT/RTMFP protocol, AMF0/AMF3 codec, SharedObject, HLS/HDS, Live/Vod, ServerSide ActionScript1.0, FlashP2P, Windows/Linux; FMS supports everything whatever you know. SRS only support RTMP/AMF0/HLS/Live/linux.
* Dev Resource: The developer resource of Adobe FMS is very large, maybe 100+. SRS only 2 primary authors.

SRS better than SRS for:
* Simple: SRS focus on small problem domain, which is the most complex for all software(see OOAD). Because of lack of deveoper resource, SRS only provides features which is the most popular for internet. SRS is simple for and only for problem domain is simplified.
* High Performance: FMS is not low performance, but the tech using by FMS is not modern high performance arch. The "modern" arch is single-thread, async and none-blocking, which introduced by epoll of linux, for example, nginx. FMS use multiple thread arch, about 265 threads running for FMS.
* Only Linux: Why support windows for servers? Linux is not good enough as a server platform?
* Easy Config: the config is very complex of FMS, for the xml is ugly and complex to read and maintain. Why app for a live server? App is no use for a pure live server. SRS is vhost based config.
* Reload：FMS does not support FMS, while reload is very very important for service.
* Fast Restart: SRS restart very fast, <100ms, for there is no lock to wait and no resource to release. FMS need about 60s to restart. Restart is a very common feature when got unknown problem.
* Tracable Log: Although FMS provides log, but only access and error log, it is nothing to tell us why connect failed, we can not finger out the whole log of a specified connection from connect to play then close. SRS use session based log to grep by the connection id, which is send to flash client and edge node. SRS use tracable log, when we got any connection id, everything we got, that is, the source id for this connection, the id on the upnode, etc.
* High Available: No backup server for edge, when fault or upnode server crash. SRS can fetch from multiple upnode server, when one failed, rollback to the next immediately.
* For all Linux: FMS only run on Centos5 64bits, AMF can run on Centos5/6 32/64bits. SRS is ok for all Linux which has epoll, for x86/64/arm/mips cpu, that is, for Centos4/5/6 or Ubuntu or Debian or whatever linux.
* Simple URL: It's simple to guess the HLS by the RTMP of SRS, vice versa. For example, RTMP is rtmp://server/live/livestream, then HLS is http://server/live/livestream.m3u8. What about FMS? Nobody can finger it out for rtmp and hds/hls is very complex.
* Transcode: SRS support live stream transcode by fork a FFMPEG process.
* DVR: SRS support dvr with some plans. FMS need some server-side scripts which always introduce bugs.
* OpenSource: SRS use MIT license. FMS is not open source. If you want to master one server, only SRS is possible.

## Wowza PK SRS

Wowza is a greate media streaming server too. SRS provides some features like wowza, for instance, transcoding and dvr. 

Wowza better than SRS for:
* More features: like FMS, wowza supports lots of features, maybe more than FMS.

SRS better than Wowza for:
* High performance by c++: Wowza developed by java. However, the performance of java is not the bottleneck.
* Deploy Simple: SRS only needs 2 files, srs and srs.conf. For Wowza, you need to installed jdk, set the JAVA_HOME env. SRS install zip <= 3MB, Wowza maybe n*100MB.
* Low Memory: Java application needs more memory for gc is not smart enough. SRS needs nKB memory, while wowza is nGB. It maybe ok for x86/x64 servers, but it's a serious problem for embeded os, for instance arm/mips.
* Single OS: SRS only supports linux, to simplify the problem domain. While Wowza needs the jdk to process this problem.
* Config Simple: SRS use the modern nginx config style. Wowza use ugly XML config file.
* Support Reload：SRS supports reload, important feature for CDN. Wowza cann't.
* Tracable Log: SRS provides full log and api. Commercial software rarely provides full log.
* High Available: When server crash or shutdown, SRS will switch to another one, the fault backup feature. Wowza maybe provide this similar feature by a plugin.
* MIT OpenSource Code: SRS is open source under MIT license. If possible, user can maintain and create private version base on SRS.

## NginxRtmp PK SRS

NginxRTMP is absolutely modern media streaming server, fast and graceful, capable and vigorous, all features are important.

nginx-rtmp release 0.0.1 at 2012.3.17, while 1.0 at 2013.5.27. See：[nginx-rtmp release 0.0.1](https://github.com/arut/nginx-rtmp-module/releases/tag/v0.0.1)。

nginx-rtmp(1.1.4) has 68883 lines code. SRS(1.0.0) is 43700 lines. Both support rtmp/hls/http, live, x86/x64/arm/mips linux, origin, dvr and http-api/http-callback/reload. NginxRTMP supports multiple process, DASH. SRS supports edge/vhost, transcode/ingest/forward, tracable log, EN/CH wiki.

Remark: NginxRTMP and NGINX code lines. NginxRTMP(+nothing is 30957 lines)(+nginx is 131534 lines)(+core+event is 68883 lines).

The 22% code of SRS is comments, while nginx+nginx-rtmp is 3%.

nginx-rtmp better than SRS for:
* Greate Author: Anyone who can write module of nginx is awesome. At 2012, I try to write a media server for a CDN, use linux/epoll/multiple process/single thread/async none-blocking socket, not base on nginx but directly write base on socket, it's very hard to process the state of RTMP, so I use ST(state-threads).
* Multiple Process: SRS only single process, maybe support in future.

SRS better than nginx-rtmp for:
* Simple: The high performance of nginx is single thread, async and none-blocking socket. SRS use ST, which also use the same arch. Futhermore, ST provides coroutine thread to simplify the code.
* Vhost：The vhost is a very very important feature for CDN to serve multiple customers. Many many CDN who using nginx-rtmp develop the vhost themselves, and introduce bugs then crash again and again, because nginx-rtmp is too complex.
* RTMP Edge: Although NginxRTMP provides pull and push, but it's similar to ingest and forward of SRS. The rtmp edge is a feature of FMS, similar to a proxy cache of origin.
* Transcode: User can use exec to start FFMPEG process, but SRS provides more: SRS can manage the FFMPEG process, restart it when crash; SRS provides simple config for complex ffmpeg params, for instance, vbitrate to set the bitrate in kbps.
* Less code: SRS(1.0.0) is 43700 lines with 22% comments; NginxRTMP(1.1.4) is 68883 lines with 3% comments. That is, SRS only 34k lines, NginxRTMP 66k.
* Chinese+English Wiki: SRS provides both Chinese and English wiki, about 58*2 pages, while NginxRTMP with less wiki.

I also forked NginxRTMP, it's a greate project and I study a lot from NginxRTMP. The complex is a challenge using async none-blocking socket and state space of RTMP protocol. The complex also introduce some bug to crash NginxRTMP, the following is some bug fix for keywork "crash":

![nginx-rtmp crash](http://winlinvip.github.io/srs.release/wiki/images/nginx-rtmp-1.1.4-crash.png)

Comparing the code to send the ConnectApp response, SRS is more readable to NginxRTMP:

![nginx-pk-srs.send-conn-response](http://winlinvip.github.io/srs.release/wiki/images/nginx-pk-srs.send-conn-response.png)

SRS focus on high level packets and objects, while nginx confused by set high level data, change global config, invoke low level async send methods.

## Red5 PK SRS

Need to compare with Red5, which only supports about 100 connections.

## crtmpd PK SRS

CRTMPD(CrtmpServer) a RTMP server writen by c++. SRS also writen by c++. It seems crtmpd use c++ to writen c code. CRTMPD never think in c++, so the language introduced more complex. It's maybe better if CRTMPD is writen by c.

CRTMPD(svnversion 811) is 93450 lines, while SRS(1.0.0) is 43700 lines. CRTMPD 17% is comments, while SRS is 22%.

```bash
# CRTMPD
python ~/srs/research/code-statistic/csr.py ~/git/crtmpserver/sources/ *.h,*.cpp .svn,tests
#total:93450 code:77559 comments:15891(17.00%) block:13123 line:2768
```

```bash
#NGINX1.5(event,core)
python ~/srs/research/code-statistic/csr.py ~/tools/nginx-rtmp/nginx-1.5.7/src/ *.h,*.c http,mail,misc,os
#total:37678 code:36034 comments:1644(4.36%) block:1644 line:0
```

```bash
#NGINX-RTMP1.1.4
python ~/srs/research/code-statistic/csr.py ~/tools/nginx-rtmp/nginx-rtmp-module-1.1.4/ *.h,*.c
#total:30957 code:30011 comments:946(3.06%) block:946 line:0
```

```bash
# NGINX(event,core)+NGINX-RTMP
python ~/srs/research/code-statistic/csr.py ~/tools/nginx-rtmp/mixed/ *.h,*.c
#total:68883 code:66285 comments:2598(3.77%) block:2598 line:0
```

```bash
# ST
python ~/srs/research/code-statistic/csr.py ~/srs/objs/st-1.9/ *.h,*.c examples,extensions,LINUX
#total:4839 code:4085 comments:754(15.58%) block:754 line:0
```

```bash
# SRS
python ~/srs/research/code-statistic/csr.py ~/srs/src *.*pp utest,upp
total:43726 code:33984 comments:9742(22.28%) block:7160 line:2582
```

CRTMPD seems support lua, which I think it's not a good idea. The opensource team always add more and more features to it, it becomes more and more complex, are these features really used by user? I do not think so.

The performance of CRTMPD is good, without multiple process support.

## Others

The above is the media servers I know. SRS is ok for me, to keep it small grow and simple.

Winlin 2014.10
