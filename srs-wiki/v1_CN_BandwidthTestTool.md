# 带宽测试

视频很卡，播放不了，缓冲区突然很大，推流上不来，都有可能是带宽过低，SRS支持测试客户端到服务器的带宽。

## SRS配置

SRS配置一般是单独加一个vhost支持测速。SRS的配置`conf/bandwidth.conf`。譬如：

```bash
listen              1935;
vhost __defaultVhost__ {
}

vhost bandcheck.srs.com {
    enabled         on;
    chunk_size      65000;
    bandcheck {
        enabled         on;
        key             "35c9b402c12a7246868752e2878f7e0e";
        interval        30;
        limit_kbps      4000;
    }
}
```

其中：
* key：服务器的key，若客户端给出的key和配置的不一致，断开连接。
* interval：测速的间隔，单位为秒，可为小数。若连续发起测速，时间间隔小于interval，服务器拒绝连接。
* limit_kbps：测速的最大带宽，即可以测出来的最大带宽，防止服务器收到攻击。

<strong>假设服务器的IP是：192.168.1.170</strong>

## Flash测速工具

启动后用带宽测试客户端就可以查看：[http://winlinvip.github.io/srs.release/trunk/research/players/srs_bwt.html?server=192.168.1.170](http://winlinvip.github.io/srs.release/trunk/research/players/srs_bwt.html?server=192.168.1.170)

备注：请将所有实例的IP地址192.168.1.170都换成部署的服务器IP地址。

检测完毕后会提示带宽，譬如：

```bash
检测结束: 192.168.1.170 上行: 1965 kbps 下行: 3631 kbps 测试时间: 6.0 秒
server:SRS 0.9.156 (github.com/winlinvip/simple-rtmp-server), 
authors:winlin,wenjie.zhao, srs_id:123, srs_pid:32057, ip:192.168.1.170
```

## 测速库

我提供了AS和JS的库，可以直接调用来和服务器测速。

AS的库，直接拷贝文件`SrsBandwidth.as`到工程，调用即可（参考注释说明）：
* AS库对象：[SrsBandwidth.as](https://github.com/simple-rtmp-server/srs/blob/master/trunk/research/players/srs_bwt/src/SrsBandwidth.as)
* AS调用对象（主对象）：[srs_bwt.as](https://github.com/simple-rtmp-server/srs/blob/master/trunk/research/players/srs_bwt/src/srs_bwt.as)，如何调用`SrsBandwidth.as`的实例。

JS的库，需要拷贝`srs_bwt.swf`和`srs.bandwidth.js`，调用方法参考js说明：
* JS库对象：[srs.bandwidth.js](https://github.com/simple-rtmp-server/srs/blob/master/trunk/research/players/srs_bwt/src/srs.bandwidth.js)
* JS调用对象（页面）：[srs_bwt.html](https://github.com/simple-rtmp-server/srs/blob/master/trunk/research/players/srs_bwt.html)，如何调用`srs.bandwidth.js`的实例。

备注：JS需要调用swf导出的js函数，由Flash发送RTMP包测速，因此js库依赖于as。可以导入Flex工程自己编译，或者使用已经编译好的[srs_bwt.swf](https://github.com/simple-rtmp-server/srs/blob/master/trunk/research/players/srs_bwt/release/srs_bwt.swf)

## Linux工具测速

另外，SRS还提供了带宽检测命令行工具：

```bash

[winlin@dev6 srs]$ cd objs/research/librtmp/
[winlin@dev6 librtmp]$ ./srs_bandwidth_check 
RTMP bandwidth check/test with server.
Usage: ./srs_bandwidth_check <rtmp_url>
   rtmp_url     RTMP bandwidth url to check. format: rtmp://server:port/app?key=xxx,vhost=xxx
For example:
   ./srs_bandwidth_check rtmp://127.0.0.1:1935/app?key=35c9b402c12a7246868752e2878f7e0e,vhost=bandcheck.srs.com
   ./srs_bandwidth_check rtmp://127.0.0.1:1935/app?key=35c9b402c12a7246868752e2878f7e0e,vhost=bandcheck.srs.com>/dev/null
@remark, output text to stdout, while json to stderr.
```

直接执行将打印文本和json信息：
```
[winlin@dev6 librtmp]$ ./srs_bandwidth_check rtmp://127.0.0.1:1935/app?key=35c9b402c12a7246868752e2878f7e0e,vhost=bandcheck.srs.com
RTMP bandwidth check/test with server.
srs(simple-rtmp-server) client librtmp library.
version: 0.9.158
bandwidth check/test url: rtmp://127.0.0.1:1935/app?key=35c9b402c12a7246868752e2878f7e0e,vhost=bandcheck.srs.com
simple handshake success
connect vhost/app success
bandwidth check/test success

SRS 0.9.158 (github.com/winlinvip/simple-rtmp-server), winlin,wenjie.zhao
127.0.0.1, 0.9.158, srs_pid=15264, srs_id=107
duration: 6395ms(3165+3148)
play: 3578kbps
publish: 4035kbps

terminate with ret=0

{"code":0,
"srs_server":"SRS 0.9.158 (github.com/winlinvip/simple-rtmp-server)", 
"srs_primary_authors":"winlin,wenjie.zhao", 
"srs_server_ip":"127.0.0.1", 
"srs_version":"0.9.158", 
"srs_pid":15264, 
"srs_id":107, 
"duration":6395, 
"play_duration":3165, 
"play_kbps":3148, 
"publish_kbps":3578}
```

可以只打印json信息，将stdout定向到/dev/null：
```
[winlin@dev6 librtmp]$ ./srs_bandwidth_check rtmp://127.0.0.1:1935/app?key=35c9b402c12a7246868752e2878f7e0e,vhost=bandcheck.srs.com>/dev/null 
{"code":0,
"srs_server":"SRS 0.9.158 (github.com/winlinvip/simple-rtmp-server)", 
"srs_primary_authors":"winlin,wenjie.zhao", 
"srs_server_ip":"127.0.0.1", 
"srs_version":"0.9.158", 
"srs_pid":15264, 
"srs_id":109, 
"duration":6354, 
"play_duration":3092, 
"play_kbps":3177, 
"publish_kbps":3662}
```

Winlin 2014.11
