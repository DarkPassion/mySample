# Delivery HTTP Flv Stream

## HTTP FLV VOD Stream

About the HTTP flv vod stream, read [v2_EN_FlvVodStream](v2_EN_FlvVodStream)

## HTTP FLV Live Stream

SRS supports remux the rtmp stream to http flv stream, when publish rtmp stream on SRS, SRS will mount a http flv url and when user access the flv url, SRS will remux the rtmp stream to user.

The config to delivery HTTP flv live stream:

```
http_server {
    enabled         on;
    listen          8080;
}
vhost your_vhost {
    http_remux {
        enabled     on;
        mount       [vhost]/[app]/[stream].flv;
    }
}
```

@remark For more information about config, read [following detail config](v2_EN_DeliveryHttpStream#http-live-stream-config).

## HTTP TS Live Stream

SRS supports remux the rtmp stream to http ts stream, when publish rtmp stream on SRS, SRS will mount a http ts url and when user access the ts url, SRS will remux the rtmp stream to user.

The config to delivery HTTP ts live stream:

```
http_server {
    enabled         on;
    listen          8080;
}
vhost your_vhost {
    http_remux {
        enabled     on;
        mount       [vhost]/[app]/[stream].ts;
    }
}
```

@remark For more information about config, read [following detail config](v2_EN_DeliveryHttpStream#http-live-stream-config).

## HTTP Mp3 Live Stream

SRS support remux the rtmp stream to http mp3 stream, drop video and mount mp3 http url, SRS will delivery mp3 stream when user access it.

The config to delivery HTTP mp3 live stream:

```
http_server {
    enabled         on;
    listen          8080;
}
vhost your_vhost {
    http_remux {
        enabled     on;
        fast_cache  30;
        mount       [vhost]/[app]/[stream].mp3;
    }
}
```

@remark For more information about config, read [following detail config](v2_EN_DeliveryHttpStream#http-live-stream-config).

## HTTP Aac Live Stream

SRS support remux the rtmp stream to http aac stream, drop video and mount aac http url, SRS will delivery aac stream when user access it.

The config to delivery HTTP ac live stream:

```
http_server {
    enabled         on;
    listen          8080;
}
vhost your_vhost {
    http_remux {
        enabled     on;
        fast_cache  30;
        mount       [vhost]/[app]/[stream].aac;
    }
}
```

@remark For more information about config, read [following detail config](v2_EN_DeliveryHttpStream#http-live-stream-config).

## HTTP Live Stream Config

The config for HTTP Flv/Mp3/Aac Live Stream, use different extension to apply different stream:

```
vhost your_vhost {
    # http flv/mp3/aac/ts stream vhost specified config
    http_remux {
        # whether enable the http live streaming service for vhost.
        # default: off
        enabled     on;
        # the fast cache for audio stream(mp3/aac),
        # to cache more audio and send to client in a time to make android(weixin) happy.
        # @remark the flv/ts stream ignore it
        # @remark 0 to disable fast cache for http audio stream.
        # default: 0
        fast_cache  30;
        # the stream mout for rtmp to remux to live streaming.
        # typical mount to [vhost]/[app]/[stream].flv
        # the variables:
        #       [vhost] current vhost for http live stream.
        #       [app] current app for http live stream.
        #       [stream] current stream for http live stream.
        # @remark the [vhost] is optional, used to mount at specified vhost.
        # the extension:
        #       .flv mount http live flv stream, use default gop cache.
        #       .ts mount http live ts stream, use default gop cache.
        #       .mp3 mount http live mp3 stream, ignore video and audio mp3 codec required.
        #       .aac mount http live aac stream, ignore video and audio aac codec required.
        # for example:
        #       mount to [vhost]/[app]/[stream].flv
        #           access by http://ossrs.net:8080/live/livestream.flv
        #       mount to /[app]/[stream].flv
        #           access by http://ossrs.net:8080/live/livestream.flv
        #           or by http://192.168.1.173:8080/live/livestream.flv
        #       mount to [vhost]/[app]/[stream].mp3
        #           access by http://ossrs.net:8080/live/livestream.mp3
        #       mount to [vhost]/[app]/[stream].aac
        #           access by http://ossrs.net:8080/live/livestream.aac
        #       mount to [vhost]/[app]/[stream].ts
        #           access by http://ossrs.net:8080/live/livestream.ts
        # @remark the port of http is specified by http_server section.
        # default: [vhost]/[app]/[stream].flv
        mount       [vhost]/[app]/[stream].flv;
        # whether http stream trigger rtmp stream source when no stream available,
        # for example, when encoder has not publish stream yet,
        # user can play the http flv stream and wait for stream.
        # default: on
        hstrs       on;
    }
}
```

Remark: Use forward+vhost to support multiple http live stream for a stream.

Remark: The http server config section, read [HTTP Server](v2_EN_HTTPServer#config)

## HSTRS

HSTRS(http stream trigger rtmp source) used for http-flv cluster and the http-flv standby to wait for encoder to publish.

HSTRS should set `hstrs` to "on" of `http_remux`, default is "on".

For more information, read https://github.com/simple-rtmp-server/srs/issues/324

## About HTTP FLV

This section descrbes the HTTP FLV live streaming.

How to use multiple process for HTTP FLV? You can use HTTP reverse proxy, and SRS start the go-sharp project, which can proxy in load balance mode and detect the status of SRS. For go-sharp, read [go-sharp][go-sharp]

### What is HTTP FLV

All http streaming is a http flv url, for instance `http://ossrs.net:8081/live/livestream.flv`, but the delivery methods maybe different:

1. FLV file, HTTP progressiv streaming. User can download and play the file which is serve by nginx, but user cannot seek to the part that not downloaded.
1. FLV pseudo streaming. This is the common used HTTP FLV streaming, to support to seek to the part which is not download, the server must accept a param, for instance .flv?start= to specifies the start time to play. This is HTTP FLV pseudo stream, but it's vod not live streaming.
1. FLV live streaming. The HTTP FLV streaming of SRS, is absolute live streaming, which is similar to RTMP for cluster, low latency, fault-backup and gop cache. And the HTTP FLV also similar to HTTP which can traverse firewall, HTTP 302 and simple. Because of SRS implements the HTTP and RTMP protocol, SRS remux the HTTP FLV stream on the edge server, and still use RTMP in cluster inside. The server which implements the RTMP and HTTP, only SRS and nginx-rtmp, but nginx-rtmp does not support HTTP FLV.

In a word, the HTTP FLV of SRS is enhanced RTMP, absolute realtime media streaming delivery.

### Confuse HTTP FLV

The HTTP FLV of SRS is not:

1. RTMPT: The rtmpt is similar to HTTP FLV of SRS, but rtmpt is rtmp over HTTP, not FLV over HTTP.
2. HDL/HFL: There exists another HXX stream, the latence maybe different, for SRS use RTMP to delivery HTTP FLV.
3. HDS: Absolutely not HTTP FLV, HDS is similar to HLS.

### Why HTTP FLV

Why SRS supports HTTP FLV?

1. The internet realtime live streaming, generally use RTMP for low latence, while HTTP FLV latency equals to RTMP.
1. Firewall traverse: Many firewall may block RTMP, but never HTTP.
1. For GSLB: The HTTP FLV can support HTTP 302, to help GSLB to correct the stupid DNS problem.
1. Fault tolerance: The HTTP FLV of SRS similar to RTMP, support mulitple origin and fault backup.
1. Common Protocol: The HTTP FLV is ok for flash and other player.
1. Simple: FLV is the most simple media stream fomat, HTTP is the standard protocol of internet, they make thinks simple.

## Sample

The config sample, read https://github.com/simple-rtmp-server/srs/issues/293#issuecomment-70449126

To config HTTP FLV, read [Sample](v2_EN_SampleHttpFlv)

Winlin 2015.1

[go-sharp]: https://github.com/simple-rtmp-server/go-sharp
