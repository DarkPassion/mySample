# HttpServer

本文描述了SRS的HTTP模块的结构和原理，侧重介绍了如何使用和定制SRS的HTTP模块。

## HTTP Arch

本节介绍了SRS的HTTP涉及的主要结构（类和函数），以及启动的流程，连接处理的流程，HTTP请求的解析和处理过程。

## HTTP Stream Arch

本节介绍了SRS的HTTP Stream涉及的主要结构（类和函数），HTTP的hijack，动态mount。

## Example

本节以一个实例，介绍如何在SRS基础上新增一个HTTP服务，这个服务侦听端口9000，将服务器的`__defaultVhost__/live/livestream`的流，分发给所有客户端(VLC)，流格式是HTTP FLV和TS。

服务器配置（实例没有新增配置，都是写死的，譬如端口9000，实时转的流地址，流的封装）：
```
listen              1935;
max_connections     1000;
daemon              off;
srs_log_tank        console;
vhost __defaultVhost__ {
}
```

譬如，推流到服务器：
```
ffmpeg -re -i doc/source.*.flv -c copy -f flv -y rtmp://127.0.0.1/live/livestream
```

客户端VLC可以播放下面两个流(播放地址是固定的stream.flv和stream.ts)：
```
http://server:9000/stream.flv
http://server:9000/stream.ts
```

FlyBird, 2015.8
