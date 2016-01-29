# HLS deploy example

To deploy HLS on SRS:

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
./configure --disable-all --with-ssl --with-hls --with-nginx && make
```

<strong>Step 3, start nginx to delivery hls(m3u8+ts).</strong> For detail, read [HLS](v1_EN_DeliveryHLS)

```bash
sudo ./objs/nginx/sbin/nginx
```

Note: Ensure nginx is ok, to access [nginx][nginx].

<strong>Step 4, config srs.</strong> For detail, read [HLS](v1_EN_DeliveryHLS)

Save bellow as config, or use `conf/hls.conf`:

```bash
# conf/hls.conf
listen              1935;
max_connections     1000;
vhost __defaultVhost__ {
    hls {
        enabled         on;
        hls_path        ./objs/nginx/html;
        hls_fragment    10;
        hls_window      60;
    }
}
```

Note: The hls_path must exists, srs never create it. For detail, 
read [HLS](v1_EN_DeliveryHLS)

<strong>Step 5, start srs.</strong> For detail, read [HLS](v1_EN_DeliveryHLS)

```bash
./objs/srs -c conf/hls.conf
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

Or use FMLE(which support h.264+aac) to publish, read 
[Transcode2HLS](v1_EN_SampleTranscode2HLS)：

```bash
FMS URL: rtmp://192.168.1.170/live
Stream: livestream
```

The stream in SRS:
* RTMP url：`rtmp://192.168.1.170/live/livestream`
* HLS url： `http://192.168.1.170/live/livestream.m3u8`

<strong>Step 7, play RTMP stream.</strong> For detail, read [HLS](v1_EN_DeliveryHLS)

RTMP url is: `rtmp://192.168.1.170:1935/live/livestream`

User can use vlc to play the RTMP stream.

Or, use online SRS player: [srs-player][srs-player]

Note: Please replace all ip 192.168.1.170 to your server ip.

<strong>Step 8, play HLS stream.</strong> For detail, read [HLS](v1_EN_DeliveryHLS)

HLS url： `http://192.168.1.170/live/livestream.m3u8`

User can use vlc to play the HLS stream.

Or, use online SRS player：[jwplayer][jwplayer]

Note: Please replace all ip 192.168.1.170 to your server ip.

Note: VLC can not play the pure audio stream, while jwplayer can.

For detail about pure audio HLS, read [HLS audio only][HLS-Audio-Only]

Winlin 2014.11

[nginx]: http://192.168.1.170:8080/nginx.html
[srs-player]: http://winlinvip.github.io/srs.release/trunk/research/players/srs_player.html?vhost=__defaultVhost__&autostart=true&server=192.168.1.170&app=live&stream=livestream&port=1935
[srs-player-19350]: http://winlinvip.github.io/srs.release/trunk/research/players/srs_player.html?vhost=__defaultVhost__&autostart=true&server=192.168.1.170&app=live&stream=livestream&port=19350
[srs-player-ff]: http://winlinvip.github.io/srs.release/trunk/research/players/srs_player.html?vhost=__defaultVhost__&autostart=true&server=192.168.1.170&app=live&stream=livestream_ff
[jwplayer]: http://winlinvip.github.io/srs.release/trunk/research/players/jwplayer6.html?vhost=__defaultVhost__&hls_autostart=true&server=192.168.1.170&app=live&stream=livestream&hls_port=8080
[jwplayer-ff]: http://winlinvip.github.io/srs.release/trunk/research/players/jwplayer6.html?vhost=__defaultVhost__&hls_autostart=true&server=192.168.1.170&app=live&stream=livestream_ff&hls_port=8080
[HLS-Audio-Only]: https://github.com/simple-rtmp-server/srs/wiki/v1_EN_DeliveryHLS#hlsaudioonly
