# SRS-HTTP-FLV集群部署实例

SRS支持HTTP FLV直播流集群分发，详细参考[HTTP FLV](v2_CN_DeliveryHttpStream#about-http-flv)

SRS的HTTP FLV边缘只能使用单进程，如何做到多进程呢？可以使用HTTP反向代理，SRS提供了go-sharp，支持根据SRS边缘的负载均衡以及心跳检测。参考：[go-sharp][go-sharp]

本例子部署了三个SRS，侦听不同的端口（实际可以部署到不同的机器，侦听同一个端口），一个作为Origin源站，两个作为Edge边缘。推RTMP流到源站或者边缘后，可以通过任何Edge观看，和RTMP的效果一样，集群的延迟在0.8-3秒。

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

<strong>第二步，编译SRS。</strong>详细参考[Build](v3_CN_Build)

```bash
./configure --disable-all \
    --with-hls --with-ssl --with-http-server \
    --with-http-api && 
make
```

<strong>第三步，编写SRS源站配置文件。</strong>详细参考[HTTP FLV](v2_CN_DeliveryHttpStream)

将以下内容保存为文件，譬如`conf/http.flv.live.conf`，服务器启动时指定该配置文件(srs的conf文件夹有该文件)。

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

<strong>第四步，编写SRS边缘配置文件。</strong>详细参考[HTTP FLV](v2_CN_DeliveryHttpStream)

将以下内容保存为文件，譬如`conf/http.flv.live.edge1.conf`和`conf/http.flv.live.edge2.conf`，服务器启动时指定该配置文件(srs的conf文件夹有该文件)。

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

<strong>第五步，启动SRS。</strong>详细参考[HTTP FLV](v2_CN_DeliveryHttpStream)

```bash
./objs/srs -c conf/http.flv.live.conf &&
./objs/srs -c conf/http.flv.live.edge1.conf &&
./objs/srs -c conf/http.flv.live.edg2.conf
```

<strong>第六步，启动推流编码器。</strong>详细参考[HTTP FLV](v2_CN_DeliveryHttpStream)

使用FFMPEG命令推流：

```bash
    for((;;)); do \
        ./objs/ffmpeg/bin/ffmpeg -re -i ./doc/source.200kbps.768x320.flv \
        -vcodec copy -acodec copy \
        -f flv -y rtmp://192.168.1.170/live/livestream; \
        sleep 1; \
    done
```

或使用支持FMLE推流：

```bash
FMS URL: rtmp://192.168.1.170/live
Stream: livestream
```

源站，生成的流地址为：
* RTMP流地址为：`rtmp://192.168.1.170/live/livestream`
* HTTP FLV: `http://192.168.1.170:8080/live/livestream.flv`

边缘1，生成的流地址为：
* RTMP流地址为：`rtmp://192.168.1.170:19351/live/livestream`
* HTTP FLV: `http://192.168.1.170:8081/live/livestream.flv`

边缘2，生成的流地址为：
* RTMP流地址为：`rtmp://192.168.1.170:19352/live/livestream`
* HTTP FLV: `http://192.168.1.170:8082/live/livestream.flv`

<strong>第七步，观看RTMP流。</strong>详细参考[HTTP FLV](v2_CN_DeliveryHttpStream)

源站RTMP流地址为：`rtmp://192.168.1.170/live/livestream`，可以使用VLC观看，或者使用在线SRS播放器播放：[srs-player][srs-player]

边缘1的RTMP流地址为：`rtmp://192.168.1.170:19351/live/livestream`，可以使用VLC观看，或者使用在线SRS播放器播放：[srs-player-19351][srs-player-19351]

边缘2的RTMP流地址为：`rtmp://192.168.1.170:19352/live/livestream`，可以使用VLC观看，或者使用在线SRS播放器播放：[srs-player-19352][srs-player-19352]

备注：请将所有实例的IP地址192.168.1.170都换成部署的服务器IP地址。

<strong>第八步，观看HTTP流。</strong>详细参考[HTTP FLV](v2_CN_DeliveryHttpStream)

源站HTTP FLV流地址为： `http://192.168.1.170:8080/live/livestream.flv`，可以使用VLC观看，或者使用在线SRS播放器播放：[jwplayer-flv][jwplayer-flv]

边缘1的HTTP FLV流地址为： `http://192.168.1.170:8081/live/livestream.flv`，可以使用VLC观看，或者使用在线SRS播放器播放：[jwplayer-flv-8081][jwplayer-flv-8081]

边缘2的HTTP FLV流地址为： `http://192.168.1.170:8082/live/livestream.flv`，可以使用VLC观看，或者使用在线SRS播放器播放：[jwplayer-flv-8081][jwplayer-flv-8082]

备注：请将所有实例的IP地址192.168.1.170都换成部署的服务器IP地址。

Winlin 2014.4

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
