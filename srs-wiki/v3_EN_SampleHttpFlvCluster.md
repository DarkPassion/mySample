# SRS HTTP FLV cluster deploy example

About the HTTP FLV cluster of SRS, read [HTTP FLV](v2_EN_DeliveryHttpStream#about-http-flv)

How to use multiple process for HTTP FLV? You can use HTTP reverse proxy, and SRS start the go-sharp project, which can proxy in load balance mode and detect the status of SRS. For go-sharp, read [go-sharp][go-sharp]

This example show how to deploy three SRS instance, listen at different port at a machine(user can deploy each to different machine, use same port), while one is origin server, another two are edge servers. We can publish RTMP to origin or edge, and play the RTMP/FLV at any edge. The latency is same to RTMP, 0.8-1s.

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

<strong>Step 2, build SRS.</strong> For detail, read [Build](v3_EN_Build)

```bash
./configure --disable-all \
    --with-hls --with-ssl --with-http-server \
    --with-http-api && 
make
```

<strong>Step 3, config origin srs.</strong> For detail, read [HTTP FLV](v2_EN_DeliveryHttpStream)

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

<strong>Step 4, config edge srs.</strong> For detail, read [HTTP FLV](v2_EN_DeliveryHttpStream)

Save bellow as config, or use `conf/http.flv.live.edge1.conf` or `conf/http.flv.live.edge2.conf`:

```bash
# conf/http.flv.live.edge1.conf
listen              19351;
max_connections     1000;
pid                 objs/srs.flv.19351.pid;
srs_log_file        objs/srs.flv.19351.log;
http_server {
    enabled         on;
    listen          8081;
    dir             ./objs/nginx/html;
}
vhost __defaultVhost__ {
    cluster {
        mode            remote;
        origin          127.0.0.1;
    }
    http_remux {
        enabled     on;
        mount       [vhost]/[app]/[stream].flv;
        hstrs       on;
    }
}
```

<strong>Step 5, start srs.</strong> For detail, read [HTTP FLV](v2_EN_DeliveryHttpStream)

```bash
./objs/srs -c conf/http.flv.live.conf &&
./objs/srs -c conf/http.flv.live.edge1.conf &&
./objs/srs -c conf/http.flv.live.edg2.conf
```

<strong>Step 6, start encoder.</strong> For detail, read read [HTTP FLV](v2_EN_DeliveryHttpStream)

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

The streams on SRS origin:
* RTMP: `rtmp://192.168.1.170/live/livestream`
* HTTP FLV: `http://192.168.1.170:8080/live/livestream.flv`

The streams on SRS edge1:
* RTMP: `rtmp://192.168.1.170:19351/live/livestream`
* HTTP FLV: `http://192.168.1.170:8081/live/livestream.flv`

The streams on SRS edge2:
* RTMP: `rtmp://192.168.1.170:19352/live/livestream`
* HTTP FLV: `http://192.168.1.170:8082/live/livestream.flv`

<strong>Step 7, play RTMP.</strong> For detail, read [HTTP FLV](v2_EN_DeliveryHttpStream)

Origin RTMP url is: `rtmp://192.168.1.170:1935/live/livestream`, User can use vlc to play the RTMP stream. Or, use online SRS player: [srs-player][srs-player]

Edge1 RTMP url is: `rtmp://192.168.1.170:19351/live/livestream`, User can use vlc to play the RTMP stream. Or, use online SRS player: [srs-player-19351][srs-player-19351]

Edge2 RTMP url is: `rtmp://192.168.1.170:19352/live/livestream`, User can use vlc to play the RTMP stream. Or, use online SRS player: [srs-player-19352][srs-player-19352]

Note: Please replace all ip 192.168.1.170 to your server ip.

<strong>Step 8, play HTTP FLV.</strong> For detail, read [HTTP FLV](v2_EN_DeliveryHttpStream)

Origin HTTP FLV url: `http://192.168.1.170:8080/live/livestream.flv`, User can use vlc to play the HLS stream. Or, use online SRS player(you must input the flv url): [jwplayer-flv][jwplayer-flv]

Edge1 HTTP FLV url: `http://192.168.1.170:8081/live/livestream.flv`, User can use vlc to play the HLS stream. Or, use online SRS player(you must input the flv url): [jwplayer-flv-8081][jwplayer-flv-8081]

Edge2 HTTP FLV url: `http://192.168.1.170:8082/live/livestream.flv`, User can use vlc to play the HLS stream. Or, use online SRS player(you must input the flv url): [jwplayer-flv-8082][jwplayer-flv-8082]

Note: Please replace all ip 192.168.1.170 to your server ip.

Winlin 2014.11

[nginx]: http://192.168.1.170:8080/nginx.html
[srs-player]: http://winlinvip.github.io/srs.release/trunk/research/players/srs_player.html?vhost=__defaultVhost__&autostart=true&server=192.168.1.170&app=live&stream=livestream&port=1935
[srs-player-19350]: http://winlinvip.github.io/srs.release/trunk/research/players/srs_player.html?vhost=__defaultVhost__&autostart=true&server=192.168.1.170&app=live&stream=livestream&port=19350
[srs-player-19351]: http://winlinvip.github.io/srs.release/trunk/research/players/srs_player.html?vhost=__defaultVhost__&autostart=true&server=192.168.1.170&app=live&stream=livestream&port=19351
[srs-player-19352]: http://winlinvip.github.io/srs.release/trunk/research/players/srs_player.html?vhost=__defaultVhost__&autostart=true&server=192.168.1.170&app=live&stream=livestream&port=19352
[srs-player-ff]: http://winlinvip.github.io/srs.release/trunk/research/players/srs_player.html?vhost=__defaultVhost__&autostart=true&server=192.168.1.170&app=live&stream=livestream_ff
[jwplayer]: http://winlinvip.github.io/srs.release/trunk/research/players/jwplayer6.html?vhost=__defaultVhost__&hls_autostart=true&server=192.168.1.170&app=live&stream=livestream&hls_port=8080
[jwplayer-ff]: http://winlinvip.github.io/srs.release/trunk/research/players/jwplayer6.html?vhost=__defaultVhost__&hls_autostart=true&server=192.168.1.170&app=live&stream=livestream_ff&hls_port=8080
[jwplayer-flv]: http://www.ossrs.net/players/jwplayer6.html?vhost=192.168.1.170&stream=livestream.flv&server=192.168.1.170&port=1935&hls_autostart=true
[jwplayer-flv-8081]: http://www.ossrs.net/players/jwplayer6.html?vhost=192.168.1.170&stream=livestream.flv&server=192.168.1.170&port=1935&hls_port=8081&hls_autostart=true
[jwplayer-flv-8082]: http://www.ossrs.net/players/jwplayer6.html?vhost=192.168.1.170&stream=livestream.flv&server=192.168.1.170&port=1935&hls_port=8082&hls_autostart=true
[go-sharp]: https://github.com/simple-rtmp-server/go-sharp
