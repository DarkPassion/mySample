# SRS ARM deploy example

SRS can deploy on ARM linux. SRS provides srs-librtmp as client library for ARM.

Compile and build ARM, read [SrsLinuxArm](v1_EN_SrsLinuxArm),
this artical describes how to deploy.

<strong>Suppose the IP of ubuntu12: 192.168.1.170</strong><br/>
<strong>Suppose the ARM device running in VirtualBox 1935 mapped to Ubuntu12 19350, 22 mapped to 2200.
That is, we can access Ubuntu12 19350 to access the ARM 1935, while the Ubuntu 2200 for ARM 22.</strong>
For more information, read [SrsLinuxArm](v1_EN_SrsLinuxArm)

## Ubuntu12 cross build SRS

<strong>Step 1, get SRS.</strong> For detail, read [GIT](v1_EN_Git)

```bash
git clone https://github.com/simple-rtmp-server/srs
cd simple-rtmp-server/trunk
```

Or update the exists code:

```bash
git pull
```

<strong>Step 2, build SRS.</strong> For detail, read [SrsLinuxArm](v1_EN_SrsLinuxArm)

```bash
./configure --arm && make
```

<strong>Step 3, send SRS to ARM virtual machine.</strong> For detail, read [SrsLinuxArm](v1_EN_SrsLinuxArm)

```bash
# Password is：root
scp -P 2200 objs/srs  root@localhost:~
scp -P 2200 conf/rtmp.conf root@localhost:~
```

## Start SRS on ARM

Login to Ubuntu 2200, we are on ARM:

<strong>Step 4, start SRS.</strong> For detail, read [SrsLinuxArm](v1_EN_SrsLinuxArm)

```bash
./objs/srs -c conf/rtmp.conf
```

<strong>Step 5, start encoder.</strong> For detail, read [SrsLinuxArm](v1_EN_SrsLinuxArm)

Use FFMPEG to publish stream:

```bash
    for((;;)); do \
        ./objs/ffmpeg/bin/ffmpeg -re -i ./doc/source.200kbps.768x320.flv \
        -vcodec copy -acodec copy \
        -f flv -y rtmp://192.168.1.170:19350/live/livestream; \
        sleep 1; \
    done
```

Or use FMLE to publish stream:

```bash
FMS URL: rtmp://192.168.1.170:19350/live
Stream: livestream
```

## User Machine

Play RTMP stream on user machine.

<strong>Step 6, play RTMP stream.</strong>

RTMP url is: `rtmp://192.168.1.170:19350/live/livestream`

User can use vlc to play the RTMP stream.

Or, use online SRS player: [srs-player][srs-player]

Note: Please replace all ip 192.168.1.170 to your server ip.

Winlin 2014.11

[nginx]: http://192.168.1.170:8080/nginx.html
[srs-player]: http://winlinvip.github.io/srs.release/trunk/research/players/srs_player.html?vhost=__defaultVhost__&autostart=true&server=192.168.1.170&app=live&stream=livestream&port=1935
[srs-player-19350]: http://winlinvip.github.io/srs.release/trunk/research/players/srs_player.html?vhost=__defaultVhost__&autostart=true&server=192.168.1.170&app=live&stream=livestream&port=19350
[srs-player-ff]: http://winlinvip.github.io/srs.release/trunk/research/players/srs_player.html?vhost=__defaultVhost__&autostart=true&server=192.168.1.170&app=live&stream=livestream_ff
[jwplayer]: http://winlinvip.github.io/srs.release/trunk/research/players/jwplayer6.html?vhost=__defaultVhost__&hls_autostart=true&server=192.168.1.170&app=live&stream=livestream&hls_port=8080
[jwplayer-ff]: http://winlinvip.github.io/srs.release/trunk/research/players/jwplayer6.html?vhost=__defaultVhost__&hls_autostart=true&server=192.168.1.170&app=live&stream=livestream_ff&hls_port=8080
