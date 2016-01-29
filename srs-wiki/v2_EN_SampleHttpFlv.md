# SRS HTTP FLV deploy example

About the HTTP FLV of SRS, read [HTTP FLV](v2_EN_DeliveryHttpStream#about-http-flv)

How to use multiple process for HTTP FLV? You can use HTTP reverse proxy, and SRS start the go-sharp project, which can proxy in load balance mode and detect the status of SRS. For go-sharp, read [go-sharp][go-sharp]

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
./configure --disable-all \
    --with-hls --with-ssl --with-http-server \
    --with-http-api && 
make
```

<strong>Step 3, config srs.</strong> For detail, read [HTTP FLV](v2_EN_DeliveryHttpStream)

Save bellow as config, or use `conf/http.flv.live.conf`:

```bash
# conf/http.flv.live.conf
listen              1935;
max_connections     1000;
http_server {
    enabled         on;
    listen          8080;
    dir             ./objs/nginx/html;
}
vhost __defaultVhost__ {
    http_remux {
        enabled     on;
        mount       [vhost]/[app]/[stream].flv;
        hstrs       on;
    }
}
```

<strong>Step 4, start srs.</strong> For detail, read [HTTP FLV](v2_EN_DeliveryHttpStream)

```bash
./objs/srs -c conf/http.flv.live.conf
```

<strong>Step 5, start encoder.</strong> For detail, read read [HTTP FLV](v2_EN_DeliveryHttpStream)

Use FFMPEG to publish stream:

```bash
    for((;;)); do \
        ./objs/ffmpeg/bin/ffmpeg -re -i ./doc/source.200kbps.768x320.flv \
        -vcodec copy -acodec copy \
        -f flv -y rtmp://192.168.1.170/live/livestream; \
        sleep 1; \
    done
```

Or use FMLE to publish：

```bash
FMS URL: rtmp://192.168.1.170/live
Stream: livestream
```

The streams on SRS:
* RTMP: `rtmp://192.168.1.170/live/livestream`
* HTTP FLV: `http://192.168.1.170:8080/live/livestream.flv`

<strong>Step 6, play RTMP.</strong> For detail, read [HTTP FLV](v2_EN_DeliveryHttpStream)

RTMP url is: `rtmp://192.168.1.170:1935/live/livestream`, User can use vlc to play the RTMP stream. Or, use online SRS player: [srs-player][srs-player]

Note: Please replace all ip 192.168.1.170 to your server ip.

<strong>Step 7, play HTTP FLV.</strong> For detail, read [HTTP FLV](v2_EN_DeliveryHttpStream)

HTTP FLV url: `http://192.168.1.170:8080/live/livestream.flv`, User can use vlc to play the HLS stream. Or, use online SRS player(you must input the flv url): [jwplayer-flv][jwplayer-flv]

Note: Please replace all ip 192.168.1.170 to your server ip.

Winlin 2014.11

[nginx]: http://192.168.1.170:8080/nginx.html
[srs-player]: http://winlinvip.github.io/srs.release/trunk/research/players/srs_player.html?vhost=__defaultVhost__&autostart=true&server=192.168.1.170&app=live&stream=livestream&port=1935
[srs-player-19350]: http://winlinvip.github.io/srs.release/trunk/research/players/srs_player.html?vhost=__defaultVhost__&autostart=true&server=192.168.1.170&app=live&stream=livestream&port=19350
[srs-player-ff]: http://winlinvip.github.io/srs.release/trunk/research/players/srs_player.html?vhost=__defaultVhost__&autostart=true&server=192.168.1.170&app=live&stream=livestream_ff
[jwplayer]: http://winlinvip.github.io/srs.release/trunk/research/players/jwplayer6.html?vhost=__defaultVhost__&hls_autostart=true&server=192.168.1.170&app=live&stream=livestream&hls_port=8080
[jwplayer-ff]: http://winlinvip.github.io/srs.release/trunk/research/players/jwplayer6.html?vhost=__defaultVhost__&hls_autostart=true&server=192.168.1.170&app=live&stream=livestream_ff&hls_port=8080
[jwplayer-flv]: http://www.ossrs.net/players/jwplayer6.html?vhost=192.168.1.170&stream=livestream.flv&server=192.168.1.170&port=1935&hls_autostart=true
[go-sharp]: https://github.com/simple-rtmp-server/go-sharp
