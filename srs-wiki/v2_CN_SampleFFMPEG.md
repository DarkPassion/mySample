# FFMPEG转码部署实例

FFMPEG对RTMP直播流转码，SRS在收到编码器推送的直播流后，可以对直播流进行转码，输出RTMP流到服务器（也可以到SRS自己）。
详细规则参考：[FFMPEG](v2_CN_FFMPEG)，本文列出了具体的部署的实例。

<strong>假设服务器的IP是：192.168.1.170</strong>

<strong>第一步，获取SRS。</strong>详细参考[GIT获取代码](v1_CN_Git)

```bash
git clone https://github.com/simple-rtmp-server/srs
cd simple-rtmp-server/trunk
```

或者使用git更新已有代码：

```bash
git pull
```

<strong>第二步，编译SRS。</strong>详细参考[Build](v2_CN_Build)

```bash
./configure --disable-all --with-ssl --with-ffmpeg --with-transcode && make
```

<strong>第三步，编写SRS配置文件。</strong>详细参考[FFMPEG](v2_CN_FFMPEG)

将以下内容保存为文件，譬如`conf/ffmpeg.transcode.conf`，服务器启动时指定该配置文件(srs的conf文件夹有该文件)。

```bash
# conf/ffmpeg.transcode.conf
listen              1935;
max_connections     1000;
vhost __defaultVhost__ {
    transcode {
        enabled     on;
        ffmpeg      ./objs/ffmpeg/bin/ffmpeg;
        engine ff {
            enabled         on;
            vfilter {
            }
            vcodec          libx264;
            vbitrate        500;
            vfps            25;
            vwidth          768;
            vheight         320;
            vthreads        12;
            vprofile        main;
            vpreset         medium;
            vparams {
            }
            acodec          libaacplus;
            abitrate        70;
            asample_rate    44100;
            achannels       2;
            aparams {
            }
            output          rtmp://127.0.0.1:[port]/[app]?vhost=[vhost]/[stream]_[engine];
        }
    }
}
```

<strong>第四步，启动SRS。</strong>详细参考[FFMPEG](v2_CN_FFMPEG)

```bash
./objs/srs -c conf/ffmpeg.conf
```

<strong>第五步，启动推流编码器。</strong>详细参考[FFMPEG](v2_CN_FFMPEG)

使用FFMPEG命令推流：

```bash
    for((;;)); do \
        ./objs/ffmpeg/bin/ffmpeg -re -i ./doc/source.200kbps.768x320.flv \
        -vcodec copy -acodec copy \
        -f flv -y rtmp://192.168.1.170/live/livestream; \
        sleep 1; \
    done
```

或使用FMLE推流：

```bash
FMS URL: rtmp://192.168.1.170/live
Stream: livestream
```

涉及的流包括：
* 编码器推送流：rtmp://192.168.1.170:1935/live/livestream
* 观看原始流：rtmp://192.168.1.170:1935/live/livestream
* 观看转码流：rtmp://192.168.1.170:1935/live/livestream_ff

<strong>第六步，观看RTMP流。</strong>详细参考[FFMPEG](v2_CN_FFMPEG)

RTMP流地址为：`rtmp://192.168.1.170/live/livestream`

可以使用VLC观看。

或者使用在线SRS播放器播放：[srs-player][srs-player]

备注：请将所有实例的IP地址192.168.1.170都换成部署的服务器IP地址。

<strong>第七步，观看FFMPEG转码的RTMP流。</strong>详细参考[FFMPEG](v2_CN_FFMPEG)

RTMP流地址为：`rtmp://192.168.1.170/live/livestream_ff`

可以使用VLC观看。

或者使用在线SRS播放器播放：[srs-player-ff][srs-player-ff]

备注：请将所有实例的IP地址192.168.1.170都换成部署的服务器IP地址。

Winlin 2014.3

[nginx]: http://192.168.1.170:8080/nginx.html
[srs-player]: http://winlinvip.github.io/srs.release/trunk/research/players/srs_player.html?vhost=__defaultVhost__&autostart=true&server=192.168.1.170&app=live&stream=livestream&port=1935
[srs-player-19350]: http://winlinvip.github.io/srs.release/trunk/research/players/srs_player.html?vhost=__defaultVhost__&autostart=true&server=192.168.1.170&app=live&stream=livestream&port=19350
[srs-player-ff]: http://winlinvip.github.io/srs.release/trunk/research/players/srs_player.html?vhost=__defaultVhost__&autostart=true&server=192.168.1.170&app=live&stream=livestream_ff
[jwplayer]: http://winlinvip.github.io/srs.release/trunk/research/players/jwplayer6.html?vhost=__defaultVhost__&hls_autostart=true&server=192.168.1.170&app=live&stream=livestream&hls_port=8080
