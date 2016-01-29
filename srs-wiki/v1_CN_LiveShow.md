# LiveShow

公用机器是SRS提供给所有人使用的服务器。

## SRS Website

感谢JOHNNY贡献了[ossrs.net](http://www.ossrs.net)域名

## Virtual Live Stream

大家可以上传电影到SRS服务器，有100GB空间，上传的电影经过转码，我审核通过后，会自动加入播放列表，随机轮流播放。

主要流程：
* 上传： `ftp://ftpuser:srs@www.ossrs.net` 推荐使用ftp工具上传，编码使用utf-8。
* 系统自动转码：[SRS使用的转码集群](http://www.ossrs.net:1971/) SRS官方服务器的虚拟直播流的转码集群，由[chnvideo.com](http://chnvideo.com)供应。
* 人工审查：[SRS官方视频审查](http://www.ossrs.net/srs-preview/) SRS官方服务器的虚拟直播流的审查地址
* 发布：审查通过后通知我将视频发布到虚拟直播流。
* 观看：默认直播流在顺序播放完后，会将新的文件加入轮播列表。

观看地址：
* [源站RTMP](http://182.92.80.26:8085/players/srs_player.html?stream=livestream&autostart=true)
* [源站HLS](http://182.92.80.26:8085/players/jwplayer6.html?stream=livestream&hls_autostart=true&hls_port=80)

感谢[verycloud.cn](http://verycloud.cn/)给SRS轮播流提供加速。

## Usage For Public server

SRS感谢捐赠，我用这些捐赠购买公网阿里云的虚拟主机。这些机器的用处：
* 大家测试和学习使用
* 部署SRS的demo环境
* 做性能测试
* 自动编译和回归测试
* 版本发布
* 发行版本下载

## Machine Info

这些机器的列表：

| 供应商 | 公网IP | 地域 | 开放 | 带宽 | 磁盘 | 链接 |
| ---- | ---- | ---- | ----- | --- | ---- | ---- |
| <a href="http://www.aliyun.com/product/ecs/">阿里云</a> | 182.92.80.26 | 北京 | 开放 | 5Mbps | 100GB | <a href="http://182.92.80.26" target="_blank">SRS</a>|

## SRS Config

公用机器上配置了默认vhost，有需要可以联系我添加新的vhost。

```
listen              1935;
http_api {
    enabled         on;
    listen          1985;
}
http_stream {
    enabled         on;
    listen          8080;
    dir             ./objs/nginx/html;
}
vhost __defaultVhost__ {
    hls {
        enabled         on;
        hls_path        ./objs/nginx/html;
        hls_fragment    10;
        hls_window      60;
    }
    ingest livestream {
        enabled      on;
        input {
            type    file;
            url     ./doc/source.200kbps.768x320.flv;
        }
        ffmpeg      ./objs/srs_ingest_flv_ssl;
        engine {
            enabled          off;
            output          rtmp://127.0.0.1:[port]/live?vhost=[vhost]/livestream;
        }
    }
}
```

实例流的播放地址：[http://182.92.80.26:8085/players/srs_player.html?vhost=182.92.80.26&stream=livestream&autostart=true](http://182.92.80.26:8085/players/srs_player.html?vhost=182.92.80.26&stream=livestream&autostart=true)

## SRS

![SRS](http://182.92.80.26:8085/srs/wiki/images/srs.qq.jpg)

Winlin 2014.5
