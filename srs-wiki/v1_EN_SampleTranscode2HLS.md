# Transcode for HLS deploy example

HLS required h.264+aac, user can transcode for other codecs.

Pure audio HLS, read [HLS audio only][HLS-Audio-Only]

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
./configure --disable-all \
    --with-ssl --with-hls --with-nginx \
    --with-ffmpeg --with-transcode && make
```

<strong>Step 3, start nginx to delivery hls(m3u8+ts).</strong> For detail, read [HLS](v1_EN_DeliveryHLS)

```bash
sudo ./objs/nginx/sbin/nginx
```

Note: Ensure nginx is ok, to access [nginx][nginx].

<strong>Step 4, config srs.</strong> For detail, read [HLS](v1_EN_DeliveryHLS)

Save bellow as config, or use `conf/transcode2hls.audio.only.conf`:

```bash
# conf/transcode2hls.audio.only.conf
listen              1935;
max_connections     1000;
vhost __defaultVhost__ {
    hls {
        enabled         on;
        hls_path        ./objs/nginx/html;
        hls_fragment    10;
        hls_window      60;
    }
    transcode {
        enabled     on;
        ffmpeg      ./objs/ffmpeg/bin/ffmpeg;
        engine ff {
            enabled         on;
            vcodec          copy;
            acodec          libaacplus;
            abitrate        45;
            asample_rate    44100;
            achannels       2;
            aparams {
            }
            output          rtmp://127.0.0.1:[port]/[app]?vhost=[vhost]/[stream]_[engine];
        }
    }
}
```

<strong>Step 5, strat SRS.</strong> For detail, read [HLS](v1_EN_DeliveryHLS)

```bash
./objs/srs -c conf/transcode2hls.audio.only.conf
```

<strong>Step 6, start encoder.</strong> For detail, read [HLS](v1_EN_DeliveryHLS)

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

The stream in SRS:
* RTMP URL: `rtmp://192.168.1.170/live/livestream`
* Transcode RTMP: `rtmp://192.168.1.170/live/livestream_ff`
* Transcode HLS: `http://192.168.1.170/live/livestream_ff.m3u8`

Note: we can use another vhost to output HLS, other codecs transcode then output to this vhost.

<strong>Step 7, play RTMP.</strong> For detail, read [HLS](v1_EN_DeliveryHLS)

RTMP url is: `rtmp://192.168.1.170:1935/live/livestream_ff`

User can use vlc to play the RTMP stream.

Or, use online SRS player: [srs-player-ff][srs-player-ff]

Note: Please replace all ip 192.168.1.170 to your server ip.

<strong>Step 8, play HLS.</strong> For detail, read [HLS](v1_EN_DeliveryHLS)

HLS url: `http://192.168.1.170/live/livestream_ff.m3u8`

User can use vlc to play the HLS stream.

Or, use online SRS player: [jwplayer-ff][jwplayer-ff]

Note: Please replace all ip 192.168.1.170 to your server ip.

Winlin 2014.11

[nginx]: http://192.168.1.170:8080/nginx.html
[srs-player]: http://winlinvip.github.io/srs.release/trunk/research/players/srs_player.html?vhost=__defaultVhost__&autostart=true&server=192.168.1.170&app=live&stream=livestream&port=1935
[srs-player-19350]: http://winlinvip.github.io/srs.release/trunk/research/players/srs_player.html?vhost=__defaultVhost__&autostart=true&server=192.168.1.170&app=live&stream=livestream&port=19350
[srs-player-ff]: http://winlinvip.github.io/srs.release/trunk/research/players/srs_player.html?vhost=__defaultVhost__&autostart=true&server=192.168.1.170&app=live&stream=livestream_ff
[jwplayer]: http://winlinvip.github.io/srs.release/trunk/research/players/jwplayer6.html?vhost=__defaultVhost__&hls_autostart=true&server=192.168.1.170&app=live&stream=livestream&hls_port=8080
[jwplayer-ff]: http://winlinvip.github.io/srs.release/trunk/research/players/jwplayer6.html?vhost=__defaultVhost__&hls_autostart=true&server=192.168.1.170&app=live&stream=livestream_ff&hls_port=8080
[HLS-Audio-Only]: https://github.com/simple-rtmp-server/srs/wiki/v1_EN_DeliveryHLS#hlsaudioonly
