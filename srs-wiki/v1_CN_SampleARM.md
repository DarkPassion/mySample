# ARM上部署SRS实例

SRS可以在SRS上作为服务器运行，播放器可以从arm设备上取流播放；arm也可以作为客户端推流到外部srs服务器，使用srs-librtmp库。

编译和搭建ARM环境，详细信息可以参考：[SrsLinuxArm](v1_CN_SrsLinuxArm)，本文举例说明部署的实例步骤。

<strong>假设编译为Ubuntu12的IP是：192.168.1.170</strong><br/>
<strong>假设ARM虚拟设备：1935映射到Ubuntu12的19350端口，22映射到2200端口。即访问Ubuntu的19350就是访问ARM虚拟机的1935（流媒体）；访问Ubuntu的2200就是访问ARM虚拟机的22（SSH登录）。</strong> 
详细信息可以参考：[SrsLinuxArm](v1_CN_SrsLinuxArm)

## Ubuntu12交叉编译SRS

<strong>第一步，获取SRS。</strong>详细参考[GIT获取代码](v1_CN_Git)

```bash
git clone https://github.com/simple-rtmp-server/srs
cd simple-rtmp-server/trunk
```

或者使用git更新已有代码：

```bash
git pull
```

<strong>第二步，编译SRS。</strong>详细参考：[SrsLinuxArm](v1_CN_SrsLinuxArm)

```bash
./configure --arm && make
```

<strong>第三步，将SRS发送到ARM虚拟机。</strong>详细参考：[SrsLinuxArm](v1_CN_SrsLinuxArm)

```bash
# 密码为：root
scp -P 2200 objs/srs  root@localhost:~
scp -P 2200 conf/rtmp.conf root@localhost:~
```

## ARM上启动SRS

登录到Ubuntu的2200端口，就是登录到了ARM虚拟设备。

<strong>第四步，启动SRS。</strong>详细参考：[SrsLinuxArm](v1_CN_SrsLinuxArm)

```bash
./objs/srs -c conf/rtmp.conf
```

<strong>第五步，启动推流编码器。</strong>详细参考：[SrsLinuxArm](v1_CN_SrsLinuxArm)

使用FFMPEG命令推流：

```bash
    for((;;)); do \
        ./objs/ffmpeg/bin/ffmpeg -re -i ./doc/source.200kbps.768x320.flv \
        -vcodec copy -acodec copy \
        -f flv -y rtmp://192.168.1.170:19350/live/livestream; \
        sleep 1; \
    done
```

或使用FMLE推流：

```bash
FMS URL: rtmp://192.168.1.170:19350/live
Stream: livestream
```

## 用户机器

在用户的Windows机器上观看流。

<strong>第六步，观看RTMP流。</strong>

RTMP流地址为：`rtmp://192.168.1.170:19350/live/livestream`

可以使用VLC观看。

或者使用在线SRS播放器播放：[srs-player][srs-player]

备注：请将所有实例的IP地址192.168.1.170都换成部署的服务器IP地址。


Winlin 2014.3

[nginx]: http://192.168.1.170:8080/nginx.html
[srs-player]: http://winlinvip.github.io/srs.release/trunk/research/players/srs_player.html?vhost=__defaultVhost__&autostart=true&server=192.168.1.170&app=live&stream=livestream&port=1935
[srs-player-19350]: http://winlinvip.github.io/srs.release/trunk/research/players/srs_player.html?vhost=__defaultVhost__&autostart=true&server=192.168.1.170&app=live&stream=livestream&port=19350
[srs-player-ff]: http://winlinvip.github.io/srs.release/trunk/research/players/srs_player.html?vhost=__defaultVhost__&autostart=true&server=192.168.1.170&app=live&stream=livestream_ff
[jwplayer]: http://winlinvip.github.io/srs.release/trunk/research/players/jwplayer6.html?vhost=__defaultVhost__&hls_autostart=true&server=192.168.1.170&app=live&stream=livestream&hls_port=8080
