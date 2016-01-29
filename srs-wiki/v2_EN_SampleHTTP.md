# SRS HTTP server deploy example

SRS embeded HTTP server, to delivery HLS and files.

Note: Never use SRS HTTP server for online service.

<strong>Suppose the server ip is 192.168.1.170</strong>

<strong>Step 1, get SRS.</strong> For detail, read [GIT](v1_EN_Git)

```bash
git clone https://github.com/simple-rtmp-server/srs
cd simple-rtmp-server/trunk
```

Or update the exists code:

```bash
git pull
```

<strong>Step 2, build SRS.</strong> For detail, read [Build](v2_EN_Build)

```bash
./configure --disable-all --with-hls --with-ssl --with-http-server && make
```

<strong>Step 3, config srs.</strong> For detail, read [HLS](v2_EN_DeliveryHLS) and [HTTP Server](v2_EN_HTTPServer)

Save bellow as config, or use `conf/http.hls.conf`:

```bash
# conf/http.hls.conf
listen              1935;
max_connections     1000;
http_server {
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
}
```

Note: The hls_path must exists, srs never create it. For detail, read [HLS](v2_EN_DeliveryHLS)

<strong>Step 4, start srs.</strong> For detail, read [HLS](v2_EN_DeliveryHLS) and [SRS HTTP Server](v2_EN_HTTPServer)

```bash
./objs/srs -c conf/http.hls.conf
```

<strong>Step 5, start encoder.</strong> For detail, read [HLS](v2_EN_DeliveryHLS)

Use FFMPEG to publish stream:

```bash
    for((;;)); do \
        ./objs/ffmpeg/bin/ffmpeg -re -i ./doc/source.200kbps.768x320.flv \
        -vcodec copy -acodec copy \
        -f flv -y rtmp://192.168.1.170/live/livestream; \
        sleep 1; \
    done
```

Or use FMLE(which support h.264+aac) to publish, read [Transcode2HLS](v2_EN_SampleTranscode2HLS)：

```bash
FMS URL: rtmp://192.168.1.170/live
Stream: livestream
```

The streams on SRS:
* RTMP: `rtmp://192.168.1.170/live/livestream`
* HLS: `http://192.168.1.170:8080/live/livestream.m3u8`

<strong>Step 6, play RTMP.</strong> For detail, read [HLS](v2_EN_DeliveryHLS)

RTMP url is: `rtmp://192.168.1.170:1935/live/livestream`

User can use vlc to play the RTMP stream.

Or, use online SRS player: [srs-player][srs-player]

Note: Please replace all ip 192.168.1.170 to your server ip.

<strong>Step 7, play HLS.</strong> For detail, read [HLS](v2_EN_DeliveryHLS)

HLS url: `http://192.168.1.170:8080/live/livestream.m3u8`

User can use vlc to play the HLS stream.

Or, use online SRS player: [jwplayer][jwplayer]

Note: Please replace all ip 192.168.1.170 to your server ip.

Winlin 2014.11

[nginx]: http://192.168.1.170:8080/nginx.html
[srs-player]: http://winlinvip.github.io/srs.release/trunk/research/players/srs_player.html?vhost=__defaultVhost__&autostart=true&server=192.168.1.170&app=live&stream=livestream&port=1935
[srs-player-19350]: http://winlinvip.github.io/srs.release/trunk/research/players/srs_player.html?vhost=__defaultVhost__&autostart=true&server=192.168.1.170&app=live&stream=livestream&port=19350
[srs-player-ff]: http://winlinvip.github.io/srs.release/trunk/research/players/srs_player.html?vhost=__defaultVhost__&autostart=true&server=192.168.1.170&app=live&stream=livestream_ff
[jwplayer]: http://winlinvip.github.io/srs.release/trunk/research/players/jwplayer6.html?vhost=__defaultVhost__&hls_autostart=true&server=192.168.1.170&app=live&stream=livestream&hls_port=8080
[jwplayer-ff]: http://winlinvip.github.io/srs.release/trunk/research/players/jwplayer6.html?vhost=__defaultVhost__&hls_autostart=true&server=192.168.1.170&app=live&stream=livestream_ff&hls_port=8080
