# LiveShow

SRS provdies a server with public ip for all to test.

## SRS Website

Thanks JOHNNY who contribute the dns name [ossrs.net](http://www.ossrs.net).

## Virtual Live Stream

There is 100GB data on the public server, anyone can upload movies and I will check it and add to the virtual live stream.

You can:
* Upload by ftp: `ftp://ftpuser:srs@www.ossrs.net`, recomment to use ftp tools, using utf-8 encoding.
* System auto transcode: SRS use [transcoding cluster](http://www.ossrs.net:1971/) to transcode the uploaded movies to h.264+aac in mp4 format. The cluster is provided by [chnvideo.com](http://chnvideo.com).
* Check: I will check the movie at [SRS preview](http://www.ossrs.net/srs-preview/)
* Publish: When check ok, I will add to the playlist of virtual live stream of SRS.
* Play: User can play the virtual live stream of SRS.

You can play the virtual live stream of SRS：
* [RTMP: speedup by verycloud](http://182.92.80.26:8085/players/srs_player.html?vhost=srs-rtmp.verycloud.cn&server=srs-rtmp.verycloud.cn&stream=livestream&autostart=true)
* [Origin RTMP, directly play on public server of SRS](http://182.92.80.26:8085/players/srs_player.html?stream=livestream&autostart=true)
* [Origin HLS, directly play on public server of SRS](http://182.92.80.26:8085/players/jwplayer6.html?stream=livestream&hls_autostart=true&hls_port=80)

Thanks [verycloud.cn](http://verycloud.cn/) to  | speedup the virtual live stream of SRS.

## Usage For Public server

Thanks all to donate SRS, I spent these money to buy the virtual public server. You can:
* Test to play or publish the RTMP stream to SRS.
* Use the demo of SRS on public server.
* Do performance test.
* Auto build and test.
* SRS release server, to download the SRS releases.
* To download SRS files.

## Machine Info

The public server list：

| Provider | Internet IP | City | Open? | Bandwidth | Disk | Link |
| ---- | ---- | ---- | ----- | --- | ---- | ---- |
| <a href="http://www.aliyun.com/product/ecs/">Aliyun</a> | 182.92.80.26 | Beijing | Open | 5Mbps | 100GB | <a href="http://182.92.80.26" target="_blank">SRS</a> |

## SRS Config

The public server use default vhost, contact me if need add your vhost:

```
listen              1935;
http_api {
    enabled         on;
    listen          1985;
}
http_stream {
    enabled         on;
    listen          8080;
    dir             ./objs/nginx/html;
}
vhost __defaultVhost__ {
    hls {
        enabled         on;
        hls_path        ./objs/nginx/html;
        hls_fragment    10;
        hls_window      60;
    }
    ingest livestream {
        enabled      on;
        input {
            type    file;
            url     ./doc/source.200kbps.768x320.flv;
        }
        ffmpeg      ./objs/srs_ingest_flv_ssl;
        engine {
            enabled          off;
            output          rtmp://127.0.0.1:[port]/live?vhost=[vhost]/livestream;
        }
    }
}
```

To play the virtual live stream：[http://182.92.80.26:8085/players/srs_player.html?vhost=182.92.80.26&stream=livestream&autostart=true](http://182.92.80.26:8085/players/srs_player.html?vhost=182.92.80.26&stream=livestream&autostart=true)

## SRS

![SRS](http://182.92.80.26:8085/srs/wiki/images/srs.qq.jpg)

Winlin 2014.10
