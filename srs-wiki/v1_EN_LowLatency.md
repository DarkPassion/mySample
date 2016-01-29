# Low Latency Live Stream

The RTMP and HLS can cover all requires for internet live stream,
read  [DeliveryHLS](v1_EN_DeliveryHLS),
while RTMP is designed for low latency live stream.

The deploy for low latency, read [Usage: Realtime](v1_EN_SampleRealtime)

## Use Scenario

The low latency use scenario:
* Live show.
* Video meeting.
* Other, for example, monitor, education.

## Latency

RTMP is design for low latency:
* Adobe flash player is good at play RTMP stream.
* RTMP is stable enough for longtime publish and play on PC.
* Low latency, about 0.8-3s.
* For RTMP is base on TCP, the latency maybe very large for network issue.

## Benchmark

We use the clock of mobile phone to test the latency,
read [RTMP latency benchmark](http://blog.csdn.net/win_lin/article/details/12615591)

When netowork is ok:
* RTMP can ensure 0.8-3s latency.
* The RTMP cluster add 0.3s latency for each level.
* The latency of nginx-rtmp is larger than SRS, maybe the cache or multiple process issue.
* The gop cache always make the latency larger, but SRS can disable the gop cache.
* The bufferTime of flash client should set to small, see NetStream.bufferTime.

## GOP-Cache

The gop is the gop between two I frame.

SRS use gop-cache to cache the last gop for the live stream,
when client play stream, SRS can send the last gop to client
to enable the client to start play immediately.

Config of srs:

```bash
# the listen ports, split by space.
listen              1935;
vhost __defaultVhost__ {
    # whether cache the last gop.
    # if on, cache the last gop and dispatch to client,
    #   to enable fast startup for client, client play immediately.
    # if off, send the latest media data to client,
    #   client need to wait for the next Iframe to decode and show the video.
    # set to off if requires min delay;
    # set to on if requires client fast startup.
    # default: on
    gop_cache       on;
}
```

Read about the min.delay.com in `conf/full.conf`.

## Low Latency config

Recoment to use the bellow config for low latency application:

```bash
# the listen ports, split by space.
listen              1935;
vhost __defaultVhost__ {
    # whether cache the last gop.
    # if on, cache the last gop and dispatch to client,
    #   to enable fast startup for client, client play immediately.
    # if off, send the latest media data to client,
    #   client need to wait for the next Iframe to decode and show the video.
    # set to off if requires min delay;
    # set to on if requires client fast startup.
    # default: on
    gop_cache       off;
    # the max live queue length in seconds.
    # if the messages in the queue exceed the max length, 
    # drop the old whole gop.
    # default: 30
    queue_length    10;
}
```

## Benchmark

SRS: 0.9.55

Encoder: FMLE, video(h264, profile=baseline, level=3.1, keyframe-frequency=5seconds), fps=15, input=640x480, 
output(500kbps, 640x480), no audio output.

Network: Publish to aliyun qindao server.

SRS config:

```bash
listen              1935;
vhost __defaultVhost__ {
    enabled         on;
    gop_cache       off;
    hls {
        enabled         on;
        hls_path        ./objs/nginx/html;
        hls_fragment    5;
        hls_window      20;
    }
}
```

Latency: RTMP 2s, HLS 24s.

Read: ![RTMP-HLS-latency](http://winlinvip.github.io/srs.release/wiki/images/rtmp-hls-latency.png)

## Edge benchmark

SRS RTMP cluster almost not add more latency.

Read ![Edge-latency](http://winlinvip.github.io/srs.release/wiki/images/edge-latency.png)

Winlin 2014.11
