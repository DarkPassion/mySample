[**HOME**](Home) > [**EN(2.0)**](v2_EN_Home) > [**Flash/Android/IOS P2P**](v2_EN_WebP2P)

Does SRS plan to support RTMFP or P2P? Let's talk about SRS and media P2P system.

## Summary

SRS never supports the tranditional P2P, for instance p2p over ed2k network. There are three types of modern media P2P for WEB:

1. Flash P2P：Developed by Adobe, for meida P2P between flash players.
1. Android P2P：The media P2P for Android app. The media over HTML5 web pages is impossible for P2P.
1. IOS P2P：The media P2P for IOS app. The media over HTML5 HLS is impossible for P2P.

These types P2P system：

1. Only for media(video and audio) P2P, not about file or application data.
1. Use common internet protocol to delivery the media stream, for example, flv, mp4 or HLS. Use CDN to delivery the media source.
1. No additional plugin. For example, FlashP2P run in flash player without other AcitveX plugin.

In short terms, the WebP2P involes Flash, Android and IOS P2P. We introduce a possible arch for WebP2P.

## Structure

The arch for a WebP2P may built by the bellow systems:

1. Client SDK: The P2P system must provide SDK for client for player or app. For example, AS client SDK of FlashP2P.
1. API Cluster: The API used to authenticate, server resources request, media stream identify and realtime scheduler.
1. RTMFP Cluster：The RTMFP or other P2P basic protocol cluster used to finish the NAT traverse and neccessary data exchange.
1. Tracker Cluster: The Tracker used to provide peers discovery service.
1. Pingback Cluster: The Pingback used to collect realtime data from all clients, analysis it and provide the best information for API cluster.
1. Media Origin Cluster: The Media Origin Cluster used to output the media data for p2p system to delivery and clients to play, from which CDN delivery the media streams.

The system can build over SRS is: Media Origin Cluster, RTMFP Cluster. Others are over HTTP protocol and depends on complex logic, which can build by Python or GO.

And GO or Spark can used to build the Pingback realtime analysis system. 

## SRS for WebP2P

Have SRS already support RTMFP or Media Origin Cluster? YES, SRS is ok for Media Origin Cluster for Bravo P2P system. And NO for SRS not and maybe never support RTMFP.

SRS can output the HLS stream following the Bravo P2P system, please read [BravoP2P][BravoP2P]. That is, your media stream can use Flash P2P feature directly, when you already use SRS for your origin server.

Why SRS maybe never support RTMFP Cluster:

1. The RTMFP Cluster over SRS is developed by Chnvideo company. Its copyright is belong to Bravo System.
1. The difference of RTMFP and SRS is huge, and the P2P system is not only RTMFP.
1. The goal of SRS is for common solution. And SRS3 plan to support DRAGON tech, while SRS4 plan for Spark. The P2P system is not on roadmap before SRS5.

So, SRS never support P2P before SRS3(2016) and SRS4(2017).

## Flash P2P

The FlashP2P is developed by Adobe.

It's ok for commercial service over FlashP2P on PC. There are some commercial FlashP2P systems: [Bravo Chnvideo][BravoP2P], Cloudacc, yunfan.

## Android P2P

Coming soon.

## IOS P2P

Coming soon.

## Challenge

The challenges of WebP2P:

1. CDN friendly: It's very hard to coroprate with CDN for WebP2P System. CDN system is hard to support private protocol, so the largest challenge is use common media stream, for example, HLS for WebP2P, to make CDN happy.
1. Fluency: The tranditional client P2P can be paused to cache more data from p2p network. It's impossible for WebP2P, for anything on Web can not be wait for 7s+.
1. Realtime GSLB: The lifetime of peer of WebP2P maybe very short, so the data of realtime of P2P system is very large, and we must build the GSLB system overy the realtime analysis.
1. Load-balance and Fault-tollerance: The load-balance and fault-tollerance is very important for p2p network, for it takes a long time to recover from crash of server. For example, the RTMFP protocol introduced the Redirect for this.

## BuyService OR Develop

Should user develop a WebP2P system from scratch? Or but service of online commercial WebP2P system?

Develop it when your team atleast can master SRS, which takes little time of mine. Strongly recommend not to try to develop the WebP2P, which takes fulltime of me and my dev team for 3years+, buy the service is ok.

Winlin 2015.5

[BravoP2P]: http://www.chnvideo.com
