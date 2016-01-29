# Delivery RTMP

The kernel feature of SRS is RTMP.

The RTMP and HLS, see: [HLS](v1_EN_DeliveryHLS)

For information about compare the RTMP and HLS, read [RTMP PK HLS](v1_EN_RTMP.PK.HTTP).

For information about how to deploy SRS to support RTMP, read [Usage: RTMP](v1_EN_SampleRTMP).

## Use Scenario

RTMP the first supported video stream for flash, and the best realtime stream on PC flash:
* No plugin: The flash plugin already installed on almost all PC, no need other plugin to play the RTMP stream.
* Fast develop player: Only need 7lines to play a RTMP stream for flash. It is very complex to play the HLS or HDS on PC flash.
* Industrial-strength: RTMP server and flash player is very stable for RTMP.
* Realtime Stream for internet: RTMP latency in 0.8-3s, can be used in live show or meeting.
* The actual standard for encoder output for internet: RTMP is the actual standard protocol, for all encoder to publish stream to internet server, server will mux the RTMP to HLS, HDS or DASH.

SRS live streaming use RTMP as kernel protocol.

SRS vod streaming is in plan and use HTTP protocol.

## FlashRTMP

RTMP is the acient and the first video streaming protocol supported by flash, which is MacroMedia flash player, then rename to Adobe Flash player. Few lines code to play the RTMP stream:

```bash
var conn = new NetConnection();
var stream = new NetStream(conn);
var video = new Video();
this.addChild(video);
video.attachNetStream(stream);
conn.connect("rtmp://192.168.1.170/live");
stream.play("livestream");
```

## Config RTMP stream

Config SRS to support RTMP:

```bash
listen              1935;
max_connections     1000;
vhost __defaultVhost__ {
}
```

Start server: `./objs/srs -c conf/rtmp.conf`

## Publish RTMP

Use encoder, for instance, FMLE to publish RTMP stream. FMLE use `FMS URL` to input the vhost/app, and the `Stream` to input the stream name:

```bash
# For RTMP url: rtmp://192.168.1.170/live/livestream
FMS URL: rtmp://192.168.1.170/live
Stream: livestream
```

The RTMP url, see: [RTMP URL&Vhost](v1_EN_RtmpUrlVhost)

The deploy sample, see: [Usage: RTMP](v1_EN_SampleRTMP)

The FMLE publish example:
![FMLE publish RTMP to SRS](http://winlinvip.github.io/srs.release/wiki/images/FMLE.png)

## Play RTMP Stream

User can use flash player or vlc to play RTMP stream, for example, the RTMP url: `rtmp://192.168.1.170/live/livestream`

## Config Low Latency for RTMP

The latency of RTMP is in 1-3s, the config see: [Low Latency](v1_EN_LowLatency)

Winlin 2014.11
