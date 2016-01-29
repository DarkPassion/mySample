# RTMP低延时配置

配置SRS为Realtime模式，使用RTMP可以将延迟降低到0.8-3秒，可以应用到对实时性要求不苛刻的地方，
譬如视频会议（其实视频会议，以及人类在开会的时候，正常时候是会有人讲，有人在听在想，然后换别人讲，
其实1秒左右延迟没有问题的，除非要吵架，就需要0.3秒左右的延迟）。

配置最低延迟的服务器详细信息可以参考：[LowLatency](v1_CN_LowLatency)，本文举例说明部署的实例步骤。

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

<strong>第二步，编译SRS。</strong>详细参考[Build](v1_CN_Build)

```bash
./configure --disable-all --with-ssl && make
```

<strong>第三步，编写SRS配置文件。</strong>详细参考[LowLatency](v1_CN_LowLatency)

将以下内容保存为文件，譬如`conf/realtime.conf`，服务器启动时指定该配置文件(srs的conf文件夹有该文件)。

```bash
# conf/realtime.conf
listen              1935;
max_connections     1000;
vhost __defaultVhost__ {
    gop_cache       off;
    queue_length    10;
}
```

<strong>第四步，启动SRS。</strong>详细参考[LowLatency](v1_CN_LowLatency)

```bash
./objs/srs -c conf/realtime.conf
```

<strong>第五步，启动推流编码器。</strong>详细参考[LowLatency](v1_CN_LowLatency)

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

备注：测量延迟，可以使用FMLE推流时，将智能手机的秒表功能打开，用FMLE摄像头对着秒表，然后对比FMLE的摄像头的图像，和服务器分发的头像的延迟，就知道精确的延迟多大。
参考：[延迟的测量](http://blog.csdn.net/win_lin/article/details/12615591)，如下图所示：
![latency](http://img.blog.csdn.net/20131011134922187?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lubGludmlw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

<strong>第六步，观看RTMP流。</strong>详细参考[LowLatency](v1_CN_LowLatency)

RTMP流地址为：`rtmp://192.168.1.170/live/livestream`

注意：不要使用VLC观看，<strong>VLC的延迟会很大</strong>，虽然VLC能看到流。

或者使用在线SRS播放器播放：[srs-player][srs-player]

备注：请将所有实例的IP地址192.168.1.170都换成部署的服务器IP地址。

Winlin 2014.3

[nginx]: http://192.168.1.170:8080/nginx.html
[srs-player]: http://winlinvip.github.io/srs.release/trunk/research/players/srs_player.html?vhost=__defaultVhost__&autostart=true&server=192.168.1.170&app=live&stream=livestream&port=1935
[srs-player-19350]: http://winlinvip.github.io/srs.release/trunk/research/players/srs_player.html?vhost=__defaultVhost__&autostart=true&server=192.168.1.170&app=live&stream=livestream&port=19350
[srs-player-ff]: http://winlinvip.github.io/srs.release/trunk/research/players/srs_player.html?vhost=__defaultVhost__&autostart=true&server=192.168.1.170&app=live&stream=livestream_ff
[jwplayer]: http://winlinvip.github.io/srs.release/trunk/research/players/jwplayer6.html?vhost=__defaultVhost__&hls_autostart=true&server=192.168.1.170&app=live&stream=livestream&hls_port=8080
