# Bandwidth Test

When publish stream very slow to server, or play always buffer, maybe the bandwidth to server is low.

SRS provides api and tools to test the bandwidth to SRS.

## SRS Server Config

The config of SRS use a specified vhost for bandwidth test, for example, `conf/bandwidth.conf`:

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

Where:
* key: The bandwidth test key, to prevent DOS attack.
* interval: The interval in seconds, can be float, SRS will disconnect when interval too short.
* limit_kbps: The limit max bandwidth can be test by client.

<strong>Suppose the server ip is 192.168.1.170</strong>

## Flash Bandwidth Test

When SRS started, open the url in web browser: [http://winlinvip.github.io/srs.release/trunk/research/players/srs_bwt.html?server=192.168.1.170](http://winlinvip.github.io/srs.release/trunk/research/players/srs_bwt.html?server=192.168.1.170)

Note: Replace the ip 192.168.1.170 to your server.

When test finished, the result is:

```bash
检测结束: 192.168.1.170 上行(upload): 1965 kbps 下行(download): 3631 kbps 测试时间: 6.0 秒
server:SRS 0.9.156 (github.com/winlinvip/simple-rtmp-server), 
authors:winlin,wenjie.zhao, srs_id:123, srs_pid:32057, ip:192.168.1.170
```

## Bandwidth Test Api

SRS provides as and js library, to test the bandwidth of server.

The as library, user can directly use `SrsBandwidth.as`:
* The as library object [SrsBandwidth.as](https://github.com/simple-rtmp-server/srs/blob/master/trunk/research/players/srs_bwt/src/SrsBandwidth.as)
* The as active object [srs_bwt.as](https://github.com/simple-rtmp-server/srs/blob/master/trunk/research/players/srs_bwt/src/srs_bwt.as), the example to how to use `SrsBandwidth.as`.

The js library, copy the `srs_bwt.swf` and `srs.bandwidth.js`:
* The js library object [srs.bandwidth.js](https://github.com/simple-rtmp-server/srs/blob/master/trunk/research/players/srs_bwt/src/srs.bandwidth.js)
* The js active object, the html page [srs_bwt.html](https://github.com/simple-rtmp-server/srs/blob/master/trunk/research/players/srs_bwt.html), the example to how to use `srs.bandwidth.js`.

Note: The js library depends on the swf exported js functions. The RTMP packets is sent by the [srs_bwt.swf](https://github.com/simple-rtmp-server/srs/blob/master/trunk/research/players/srs_bwt/release/srs_bwt.swf)

## Linux Bandwidth Tool

SRS also provides linux tool to teset the bandwidth:

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

Print text and json:
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

Only show the stderr(json) info:
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
