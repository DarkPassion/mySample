# RTMP cluster deploy example

RTMP cluster is the kernel feature of SRS.

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

<strong>Step 2, build SRS.</strong> For detail, read [Build](v1_EN_Build)

```bash
./configure --disable-all --with-ssl && make
```

<strong>Step 3, config origin srs.</strong> For detail, read [RTMP](v1_EN_DeliveryRTMP)

Save bellow as config, or use `conf/origin.conf`:

```bash
# conf/origin.conf
listen              19350;
max_connections     1000;
pid                 objs/origin.pid;
srs_log_file        ./objs/origin.log;
vhost __defaultVhost__ {
}
```

<strong>Step 4, config edge srs.</strong> For detail, read [RTMP](v1_EN_DeliveryRTMP)

Save bellow as config, or use `conf/edge.conf`:

```bash
# conf/edge.conf
listen              1935;
max_connections     1000;
pid                 objs/edge.pid;
srs_log_file        ./objs/edge.log;
vhost __defaultVhost__ {
    mode            remote;
    origin          127.0.0.1:19350;
}
```

<strong>Step 5, start srs.</strong> For detail, read [RTMP](v1_EN_DeliveryRTMP)

```bash
./objs/srs -c conf/origin.conf &&
./objs/srs -c conf/edge.conf
```

<strong>Step 6, start encoder.</strong> For detail, read [RTMP](v1_EN_DeliveryRTMP)

Use FFMPEG to publish stream:

```bash
    for((;;)); do \
        ./objs/ffmpeg/bin/ffmpeg -re -i ./doc/source.200kbps.768x320.flv \
        -vcodec copy -acodec copy \
        -f flv -y rtmp://192.168.1.170/live/livestream; \
        sleep 1; \
    done
```

Or use FMLE to publish:

```bash
FMS URL: rtmp://192.168.1.170/live
Stream: livestream
```

<strong>Step 7, play RTMP.</strong> For detail, read [RTMP](v1_EN_DeliveryRTMP)

Origin RTMP url is: `rtmp://192.168.1.170:19350/live/livestream`, User can use vlc to play the RTMP stream. Or, use online SRS player: [srs-player-19350][srs-player-19350]

Edge RTMP url is: `rtmp://192.168.1.170:1935/live/livestream`, User can use vlc to play the RTMP stream. Or, use online SRS player: [srs-player][srs-player]

Note: Please replace all ip 192.168.1.170 to your server ip.

Winlin 2014.11

[nginx]: http://192.168.1.170:8080/nginx.html
[srs-player]: http://winlinvip.github.io/srs.release/trunk/research/players/srs_player.html?vhost=__defaultVhost__&autostart=true&server=192.168.1.170&app=live&stream=livestream&port=1935
[srs-player-19350]: http://winlinvip.github.io/srs.release/trunk/research/players/srs_player.html?vhost=__defaultVhost__&autostart=true&server=192.168.1.170&app=live&stream=livestream&port=19350
[srs-player-ff]: http://winlinvip.github.io/srs.release/trunk/research/players/srs_player.html?vhost=__defaultVhost__&autostart=true&server=192.168.1.170&app=live&stream=livestream_ff
[jwplayer]: http://winlinvip.github.io/srs.release/trunk/research/players/jwplayer6.html?vhost=__defaultVhost__&hls_autostart=true&server=192.168.1.170&app=live&stream=livestream&hls_port=8080
[jwplayer-ff]: http://winlinvip.github.io/srs.release/trunk/research/players/jwplayer6.html?vhost=__defaultVhost__&hls_autostart=true&server=192.168.1.170&app=live&stream=livestream_ff&hls_port=8080
