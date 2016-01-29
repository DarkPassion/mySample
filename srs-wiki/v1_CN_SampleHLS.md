# HLS部署实例

SRS支持HLS的详细步骤。

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
./configure --disable-all --with-ssl --with-hls --with-nginx && make
```

<strong>第三步，启动分发hls（m3u8/ts）的nginx。详细参考[HLS分发](v1_CN_DeliveryHLS)

```bash
sudo ./objs/nginx/sbin/nginx
```

备注：为了突出HLS的配置，我们在HLS的实例中没有使用SRS内置的HTTP Server，可以配置几行就可以不用nginx。参考：[Usage: HTTP](v1_CN_SampleHTTP)

备注：请确定nginx已经启动，可以访问[nginx][nginx]，若能看到`nginx is ok`则没有问题。

<strong>第四步，编写SRS配置文件。</strong>详细参考[HLS分发](v1_CN_DeliveryHLS)

将以下内容保存为文件，譬如`conf/hls.conf`，服务器启动时指定该配置文件(srs的conf文件夹有该文件)。

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

备注：hls_path必须存在，srs只会自动创建${hls_path}下的app的目录。参考：[HLS分发: HLS流程](v1_CN_DeliveryHLS)

<strong>第五步，启动SRS。</strong>详细参考[HLS分发](v1_CN_DeliveryHLS)

```bash
./objs/srs -c conf/hls.conf
```

<strong>第六步，启动推流编码器。</strong>详细参考[HLS分发](v1_CN_DeliveryHLS)

使用FFMPEG命令推流：

```bash
    for((;;)); do \
        ./objs/ffmpeg/bin/ffmpeg -re -i ./doc/source.200kbps.768x320.flv \
        -vcodec copy -acodec copy \
        -f flv -y rtmp://192.168.1.170/live/livestream; \
        sleep 1; \
    done
```

或使用支持h.264+aac的FMLE推流（若不支持h.264+aac，则可以使用srs转码，参考[Transcode2HLS](v1_CN_SampleTranscode2HLS)）：

```bash
FMS URL: rtmp://192.168.1.170/live
Stream: livestream
```

生成的流地址为：
* RTMP流地址为：`rtmp://192.168.1.170/live/livestream`
* HLS流地址为： `http://192.168.1.170/live/livestream.m3u8`

<strong>第七步，观看RTMP流。</strong>详细参考[HLS分发](v1_CN_DeliveryHLS)

RTMP流地址为：`rtmp://192.168.1.170/live/livestream`

可以使用VLC观看。

或者使用在线SRS播放器播放：[srs-player][srs-player]

备注：请将所有实例的IP地址192.168.1.170都换成部署的服务器IP地址。

<strong>第八步，观看HLS流。</strong>详细参考[HLS分发](v1_CN_DeliveryHLS)

HLS流地址为： `http://192.168.1.170/live/livestream.m3u8`

可以使用VLC观看。

或者使用在线SRS播放器播放：[jwplayer][jwplayer]

备注：请将所有实例的IP地址192.168.1.170都换成部署的服务器IP地址。

注意：VLC无法观看纯音频流，jwplayer可以观看。

分发纯音频流参考：[HLS audio only][HLS-Audio-Only]

## Q&A

<strong>RTMP流能看，HLS看不了</strong>
* 确认nginx启动并且可以访问：`nginx is ok`页面能访问。
* 确认m3u8文件能下载：浏览器打开`http://192.168.1.170/live/livestream.m3u8`，ip地址换成你服务器的IP地址。
* 若m3u8能下载，可能是jwplayer的问题，使用vlc播放地址：`http://192.168.1.170/live/livestream.m3u8`，ip地址换成你服务器的IP地址。
* 若VLC不能播放，将m3u8下载后，用文本编辑器打开，将m3u8文件内容发到QQ群中，或者贴到issue中。寻求帮助。
* 还有可能是编码问题，参考下面的“RTMP流和HLS流内容不一致”

<strong>RTMP流内容和HLS流内容不一致</strong>
* 一般这种问题出现在使用上面的例子推流，然后换成别的编码器推流，或者换个文件推流。
* 可能是流的编码不对（推流时使用FMLE），HLS需要h.264+aac，需要转码，参考只转码音频[Transcode2HLS](v1_CN_SampleTranscode2HLS)

Winlin 2014.3

[nginx]: http://192.168.1.170:8080/nginx.html
[srs-player]: http://winlinvip.github.io/srs.release/trunk/research/players/srs_player.html?vhost=__defaultVhost__&autostart=true&server=192.168.1.170&app=live&stream=livestream&port=1935
[srs-player-19350]: http://winlinvip.github.io/srs.release/trunk/research/players/srs_player.html?vhost=__defaultVhost__&autostart=true&server=192.168.1.170&app=live&stream=livestream&port=19350
[srs-player-ff]: http://winlinvip.github.io/srs.release/trunk/research/players/srs_player.html?vhost=__defaultVhost__&autostart=true&server=192.168.1.170&app=live&stream=livestream_ff
[jwplayer]: http://winlinvip.github.io/srs.release/trunk/research/players/jwplayer6.html?vhost=__defaultVhost__&hls_autostart=true&server=192.168.1.170&app=live&stream=livestream&hls_port=8080
[HLS-Audio-Only]: https://github.com/simple-rtmp-server/srs/wiki/v1_CN_DeliveryHLS#hlsaudioonly
