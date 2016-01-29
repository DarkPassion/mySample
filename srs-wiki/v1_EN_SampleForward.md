# Forward deploy example

SRS can forward stream to other RTMP server.

<strong>Suppose the server ip is 192.168.1.170</strong>

Forward will copy streams to other RTMP server:
* Master: Encoder publish stream to master, which will forward to slave.
* Slave: Slave forward stream to slave.

We use master to listen at 1935, and slave listen at 19350.

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

<strong>Step 3, config master SRS.</strong> For detail, read [Forward](v1_EN_Forward)

Save bellow as config, or use `conf/forward.master.conf`:

```bash
# conf/forward.master.conf
listen              1935;
max_connections     1000;
pid                 ./objs/srs.master.pid;
srs_log_tank        file;
srs_log_file        ./objs/srs.master.log;
vhost __defaultVhost__ {
    forward         127.0.0.1:19350;
}
```

<strong>Step 4, start master SRS.</strong> For detail, read [Forward](v1_EN_Forward)

```bash
./objs/srs -c conf/forward.master.conf
```

<strong>Step 5, config slave SRS.</strong> For detail, read [Forward](v1_EN_Forward)

Save bellow as config, or use `conf/forward.slave.conf`:

```bash
# conf/forward.slave.conf
listen              19350;
pid                 ./objs/srs.slave.pid;
srs_log_tank        file;
srs_log_file        ./objs/srs.slave.log;
vhost __defaultVhost__ {
}
```

<strong>Step 6, start slave SRS.</strong> For detail, read [Forward](v1_EN_Forward)

```bash
./objs/srs -c conf/forward.slave.conf
```

Note: Ensure the master and slave is ok, no error in log.

```bash
[winlin@dev6 srs]$ sudo netstat -anp|grep srs
tcp        0      0 0.0.0.0:1935                0.0.0.0:*                   LISTEN      7826/srs            
tcp        0      0 0.0.0.0:19350               0.0.0.0:*                   LISTEN      7834/srs
```

<strong>Step 7, start encoder.</strong> For detail, read [Forward](v1_EN_Forward)

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
* Stream publish by encoder: rtmp://192.168.1.170:1935/live/livestream
* The stream forward by master SRS: rtmp://192.168.1.170:19350/live/livestream
* Play stream on master: rtmp://192.168.1.170/live/livestream
* Play strema on slave: rtmp://192.168.1.170:19350/live/livestream

<strong>Step 8, play the stream on master.</strong> For detail, read [Forward](v1_EN_Forward)

RTMP url is: `rtmp://192.168.1.170:1935/live/livestream`

User can use vlc to play the RTMP stream.

Or, use online SRS player: [srs-player][srs-player]

Note: Please replace all ip 192.168.1.170 to your server ip.

<strong>Step 9, play the stream on slave.</strong> For detail, read [Forward](v1_EN_Forward)

RTMP url is: `rtmp://192.168.1.170:19350/live/livestream`

User can use vlc to play the RTMP stream.

Or, use online SRS player: [srs-player-19350][srs-player-19350]

Note: Please replace all ip 192.168.1.170 to your server ip.

Winlin 2014.11

[nginx]: http://192.168.1.170:8080/nginx.html
[srs-player]: http://winlinvip.github.io/srs.release/trunk/research/players/srs_player.html?vhost=__defaultVhost__&autostart=true&server=192.168.1.170&app=live&stream=livestream&port=1935
[srs-player-19350]: http://winlinvip.github.io/srs.release/trunk/research/players/srs_player.html?vhost=__defaultVhost__&autostart=true&server=192.168.1.170&app=live&stream=livestream&port=19350
[srs-player-ff]: http://winlinvip.github.io/srs.release/trunk/research/players/srs_player.html?vhost=__defaultVhost__&autostart=true&server=192.168.1.170&app=live&stream=livestream_ff
[jwplayer]: http://winlinvip.github.io/srs.release/trunk/research/players/jwplayer6.html?vhost=__defaultVhost__&hls_autostart=true&server=192.168.1.170&app=live&stream=livestream&hls_port=8080
[jwplayer-ff]: http://winlinvip.github.io/srs.release/trunk/research/players/jwplayer6.html?vhost=__defaultVhost__&hls_autostart=true&server=192.168.1.170&app=live&stream=livestream_ff&hls_port=8080
