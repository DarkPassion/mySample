# Ingest采集实例

SRS启动后，自动启动Ingest开始采集file/stream/device，并将流推送到SRS。详细规则参考：[Ingest](v1_CN_Ingest)，本文列出了具体的部署的实例。

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
./configure --disable-all --with-ssl --with-ffmpeg --with-ingest && make
```

<strong>第三步，编写SRS配置文件。</strong>详细参考[Ingest](v1_CN_Ingest)

将以下内容保存为文件，譬如`conf/ingest.conf`，服务器启动时指定该配置文件(srs的conf文件夹有该文件)。

```bash
# conf/ingest.conf
listen              1935;
max_connections     1000;
vhost __defaultVhost__ {
    ingest livestream {
        enabled      on;
        input {
            type    file;
            url     ./doc/source.200kbps.768x320.flv;
        }
        ffmpeg      ./objs/ffmpeg/bin/ffmpeg;
        engine {
            enabled          off;
            output          rtmp://127.0.0.1:[port]/live?vhost=[vhost]/livestream;
        }
    }
}
```

<strong>第四步，启动SRS。</strong>详细参考[Ingest](v1_CN_Ingest)

```bash
./objs/srs -c conf/ingest.conf
```

涉及的流包括：
* 采集的流：rtmp://192.168.1.170:1935/live/livestream

<strong>第五步，观看RTMP流。</strong>详细参考[Ingest](v1_CN_Ingest)

RTMP流地址为：`rtmp://192.168.1.170/live/livestream`

可以使用VLC观看。

或者使用在线SRS播放器播放：[srs-player][srs-player]

备注：请将所有实例的IP地址192.168.1.170都换成部署的服务器IP地址。

Winlin 2014.4

[nginx]: http://192.168.1.170:8080/nginx.html
[srs-player]: http://winlinvip.github.io/srs.release/trunk/research/players/srs_player.html?vhost=__defaultVhost__&autostart=true&server=192.168.1.170&app=live&stream=livestream&port=1935
[srs-player-19350]: http://winlinvip.github.io/srs.release/trunk/research/players/srs_player.html?vhost=__defaultVhost__&autostart=true&server=192.168.1.170&app=live&stream=livestream&port=19350
[srs-player-ff]: http://winlinvip.github.io/srs.release/trunk/research/players/srs_player.html?vhost=__defaultVhost__&autostart=true&server=192.168.1.170&app=live&stream=livestream_ff
[jwplayer]: http://winlinvip.github.io/srs.release/trunk/research/players/jwplayer6.html?vhost=__defaultVhost__&hls_autostart=true&server=192.168.1.170&app=live&stream=livestream&hls_port=8080
