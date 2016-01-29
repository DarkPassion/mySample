# SRS Embeded HTTP Server

SRS内嵌了一个web服务器，支持api和简单的文件分发。

部署和使用SRS的内嵌http服务器，参考：[Usage: HTTP](v1_CN_SampleHTTP)

SRS的内置HTTP服务器属于实验功能；建议不要当作web服务器，主要是提供API服务；建议使用nginx等成熟web服务器分发文件。

## Use Scenario

它的定位很简单：智能手机上的摄像头。

Nginx/Apache/lighthttpd等众多HTTP server大佬就是专业的单反，老长老长镜头了。
难道有了单反智能手机上就不能有摄像头？不会吧！而且必须有。所以不是要和nginx拼个你死我活，
定位不一样，就像fms内嵌apache一样（不过fms嵌得很烂），真的有必要而且方便。

为何srs不内嵌一个nginx呢？智能手机上能内嵌一个单反长镜头么？我去，那是怪物吧。
nginx14万行代码，巨大无比，srs才2万行，如何能内嵌呢？最核心的原因是：srs需要提供http的api，
方便外部管理和调用；这点往往都毫无异议，但是提到srs要内嵌web服务器，就都炸开锅啦。
OK，其实就是http的api稍微扩展点，支持读文件后发送给客户端。

srs会一如既往的保持最简单，http的代码不会有多少行，功能不会有几个，就支持简单的文件分发就足够了。可以：
* 只需要部署一个服务器就可以分发RTMP和HLS。
* SRS对于HLS/HDS/DASH的支持会更完善。
* SRS可以支持点播，动态转封装等。
* SRS依然可以用nginx作为反向代理，或者禁用这个选项，使用nginx分发。

实际上，RTMP协议本身比HTTP复杂很多，所以st来做http分发，没有任何不可以的地方，更何况只是做部分。所以，淡定～

## Config

需要配置全局的HTTP端口和根目录的路径。

```bash
# embeded http server in srs.
# the http streaming config, for HLS/HDS/DASH/HTTPProgressive
# global config for http streaming, user must config the http section for each vhost.
# the embed http server used to substitute nginx in ./objs/nginx,
# for example, srs runing in arm, can provides RTMP and HTTP service, only with srs installed.
# user can access the http server pages, generally:
#       curl http://192.168.1.170:80/srs.html
# which will show srs version and welcome to srs.
# @remeark, the http embeded stream need to config the vhost, for instance, the __defaultVhost__
# need to open the feature http of vhost.
http_stream {
    # whether http streaming service is enabled.
    # default: off
    enabled         on;
    # the http streaming port
    # @remark, if use lower port, for instance 80, user must start srs by root.
    # default: 8080
    listen          8080;
    # the default dir for http root.
    # default: ./objs/nginx/html
    dir             ./objs/nginx/html;
}
```

同时，vhost上可以指定虚拟目录（默认根目录）。

```bash
vhost __defaultVhost__ {
    # http vhost specified config
    http {
        # whether enable the http streaming service for vhost.
        # default: off
        enabled     on;
        # the virtual directory root for this vhost to mount at
        # for example, if mount to /hls, user access by http://server/hls
        # default: /
        mount       /hls;
        # main dir of vhost,
        # to delivery HTTP stream of this vhost.
        # default: ./objs/nginx/html
        dir         ./objs/nginx/html;
    }
}
```

注意：vhost的http配置是可选的，就算不配置，http根目录依然可以访问。

## MIME

支持少量的MIME，见下表。| 

| 文件扩展名 | Content-Type |
| -------- | ------------ |
| .ts  | | Content-Type: video/MP2T;charset=utf-8 |
| .m3u8 | Content-Type: application/x-mpegURL;charset=utf-8 |
| .json | Content-Type: application/json;charset=utf-8 |
| .css | Content-Type: text/css;charset=utf-8 |
| .swf | Content-Type: application/x-shockwave-flash;charset=utf-8 |
| .js | Content-Type: text/javascript;charset=utf-8 |
| .xml | Content-Type: text/xml;charset=utf-8 |
| 其他 | Content-Type: text/html;charset=utf-8 |

## Method

支持的Method包括：
* GET: 下载文件。

Winlin 2014.4
