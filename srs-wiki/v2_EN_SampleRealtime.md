# RTMP low latency deploy example

The SRS realtime(low latency) mode can decrease the latency to 0.8-3s.
For detail about latency, read [LowLatency](v2_EN_LowLatency).

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
./configure --disable-all --with-ssl && make
```

<strong>Step 3, config srs.</strong> For detail, read [LowLatency](v2_EN_LowLatency)

Save bellow as config, or use `conf/realtime.conf`:

```bash
# conf/realtime.conf
listen              1935;
max_connections     1000;
vhost __defaultVhost__ {
    gop_cache       off;
    queue_length    10;
    min_latency     on;
    mr {
        enabled     off;
    }
    mw_latency      100;
    tcp_nodelay     on;
}
```

<strong>Step 4, start srs.</strong> For detail, read [LowLatency](v2_EN_LowLatency)

```bash
./objs/srs -c conf/realtime.conf
```

<strong>Step 5, start encoder.</strong> For detail, read [LowLatency](v2_EN_LowLatency)

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

Note: To measure the latency, can use the clock of mobile phone.
![latency](http://img.blog.csdn.net/20131011134922187?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lubGludmlw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

<strong>Step 5, play RTMP.</strong> For detail, read [LowLatency](v2_EN_LowLatency)

RTMP url is: `rtmp://192.168.1.170:1935/live/livestream`

User can use vlc to play the RTMP stream.

Or, use online SRS player: [srs-player][srs-player]

Note: Please replace all ip 192.168.1.170 to your server ip.

Winlin 2014.12

[nginx]: http://192.168.1.170:8080/nginx.html
[srs-player]: http://winlinvip.github.io/srs.release/trunk/research/players/srs_player.html?vhost=__defaultVhost__&autostart=true&server=192.168.1.170&app=live&stream=livestream&port=1935
[srs-player-19350]: http://winlinvip.github.io/srs.release/trunk/research/players/srs_player.html?vhost=__defaultVhost__&autostart=true&server=192.168.1.170&app=live&stream=livestream&port=19350
[srs-player-ff]: http://winlinvip.github.io/srs.release/trunk/research/players/srs_player.html?vhost=__defaultVhost__&autostart=true&server=192.168.1.170&app=live&stream=livestream_ff
[jwplayer]: http://winlinvip.github.io/srs.release/trunk/research/players/jwplayer6.html?vhost=__defaultVhost__&hls_autostart=true&server=192.168.1.170&app=live&stream=livestream&hls_port=8080
[jwplayer-ff]: http://winlinvip.github.io/srs.release/trunk/research/players/jwplayer6.html?vhost=__defaultVhost__&hls_autostart=true&server=192.168.1.170&app=live&stream=livestream_ff&hls_port=8080

