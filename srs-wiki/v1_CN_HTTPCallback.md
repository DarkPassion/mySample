# HTTP回调和服务器定制

SRS不支持服务器脚本（参考：[服务器端脚本](v1_CN_ServerSideScript)），服务器端定制有一个重要的替代功能，就是HTTP回调。譬如当客户端连接到SRS时，回调指定的http地址，这样可以实现验证功能。

关于Token认证，即基于http回调的认证，参考：[Token Authentication](v1_CN_DRM#token-authentication)

## Build

HTTP回调功能提供了编译选项，可以开启或者关闭这个功能。参考：[Build](v1_CN_Build)

## Events

SRS的回调事件包括：

| 事件 | 数据 | 说明 |
| --- | ---- | ---- |
| on_connect|{<br/> "action": "on_connect",<br/> "client_id": 1985,<br/> "ip": "192.168.1.10", <br/> "vhost": "video.test.com", <br/> "app": "live",<br/> "tcUrl": "rtmp://x/x?key=xxx",<br/> "pageUrl": "http://x/x.html"<br/> } | 当客户端连接到指定的vhost和app时| 
| on_close|{<br/> "action": "on_close",<br/> "client_id": 1985,<br/> "ip": "192.168.1.10", <br/> "vhost": "video.test.com", <br/> "app": "live"<br/> } | 当客户端关闭连接，或者SRS主动关闭连接时| 
| on_publish|{<br/> "action": "on_publish",<br/> "client_id": 1985,<br/> "ip": "192.168.1.10", <br/> "vhost": "video.test.com", <br/> "app": "live",<br/> "stream": "livestream"<br/> } | 当客户端发布流时，譬如flash/FMLE方式推流到服务器| 
| on_unpublish|{<br/> "action": "on_unpublish",<br/> "client_id": 1985,<br/> "ip": "192.168.1.10", <br/> "vhost": "video.test.com", <br/> "app": "live",<br/> "stream": "livestream"<br/> } | 当客户端停止发布流时| 
| on_play|{<br/> "action": "on_play",<br/> "client_id": 1985,<br/> "ip": "192.168.1.10", <br/> "vhost": "video.test.com", <br/> "app": "live",<br/> "stream": "livestream"<br/> } | 当客户端开始播放流时| 
| on_stop|{<br/> "action": "on_stop",<br/> "client_id": 1985,<br/> "ip": "192.168.1.10", <br/> "vhost": "video.test.com", <br/> "app": "live",<br/> "stream": "livestream"<br/> } | 当客户端停止播放时。备注：停止播放可能不会关闭连接，还能再继续播放。| 

其中，
* 事件：发生该事件时，即回调指定的HTTP地址。
* HTTP地址：可以支持多个，以空格分隔，SRS会依次回调这些接口。
* 数据：SRS将数据POST到HTTP接口。
* 返回值：SRS要求HTTP服务器返回HTTP200并且response内容为整数错误码（0表示成功），其他错误码会断开客户端连接。

## Config SRS

以on_connect为例，当用户连接到vhost/app时，验证客户端的ip，配置文件如下：

```bash
# the listen ports, split by space.
listen              1935;
vhost __defaultVhost__ {
    http_hooks {
        # whether the http hooks enalbe.
        # default off.
        enabled         on;
        # when client connect to vhost/app, call the hook,
        # the request in the POST data string is a object encode by json:
        #       {
        #           "action": "on_connect",
        #           "client_id": 1985,
        #           "ip": "192.168.1.10", "vhost": "video.test.com", "app": "live",
        #           "tcUrl": "rtmp://video.test.com/live?key=d2fa801d08e3f90ed1e1670e6e52651a",
        #           "pageUrl": "http://www.test.com/live.html"
        #       }
        # if valid, the hook must return HTTP code 200(Stauts OK) and response
        # an int value specifies the error code(0 corresponding to success):
        #       0
        # support multiple api hooks, format:
        #       on_connect http://xxx/api0 http://xxx/api1 http://xxx/apiN
        on_connect      http://127.0.0.1:8085/api/v1/clients;
    }
}
```

备注：可以参考conf/full.conf配置文件中的hooks.callback.vhost.com实例。

## 默认的HTTP服务器

SRS自带了一个默认的处理HTTP Callback的服务器，启动时需要指定端口，譬如8085端口。

启动方法：`python research/api-server/server.py 8085`

启动日志如下：

```bash
[winlin@dev6 srs]$ python research/api-server/server.py 8085
[2014-02-27 09:42:25][trace] api server listen at port: 8085, static_dir: /home/winlin/git/simple-rtmp-server/trunk/research/api-server/static-dir
[2014-02-27 09:42:25][trace] start cherrypy server
[27/Feb/2014:09:42:25] ENGINE Listening for SIGHUP.
[27/Feb/2014:09:42:25] ENGINE Listening for SIGTERM.
[27/Feb/2014:09:42:25] ENGINE Listening for SIGUSR1.
[27/Feb/2014:09:42:25] ENGINE Bus STARTING
[27/Feb/2014:09:42:25] ENGINE Started monitor thread '_TimeoutMonitor'.
[27/Feb/2014:09:42:25] ENGINE Started monitor thread 'Autoreloader'.
[27/Feb/2014:09:42:25] ENGINE Serving on 0.0.0.0:8085
[27/Feb/2014:09:42:25] ENGINE Bus STARTED
```

## 推流和播放

推流到SRS时，会调用HTTP接口：
```bash
[2014-02-27 09:41:33][trace] post to clients, req={"action":"on_connect","client_id":4,"ip":"192.168.1.179","vhost":"__defaultVhost__","app":"live","pageUrl":""}
[2014-02-27 09:41:33][trace] srs on_connect: client id=4, ip=192.168.1.179, vhost=__defaultVhost__, app=live, pageUrl=
127.0.0.1 - - [27/Feb/2014:09:41:33] "POST /api/v1/clients HTTP/1.1" 200 1 "" "srs(simple rtmp server)0.9.2"
```

播放SRS的流时，也会调用HTTP接口：
```bash
[2014-02-27 09:41:50][trace] post to clients, req={"action":"on_connect","client_id":5,"ip":"192.168.1.179","vhost":"__defaultVhost__","app":"live","pageUrl":"http://dev.chnvideo.com:3080/players/rtmp/"}
[2014-02-27 09:41:50][trace] srs on_connect: client id=5, ip=192.168.1.179, vhost=__defaultVhost__, app=live, pageUrl=http://dev.chnvideo.com:3080/players/rtmp/
127.0.0.1 - - [27/Feb/2014:09:41:50] "POST /api/v1/clients HTTP/1.1" 200 1 "" "srs(simple rtmp server)0.9.2"
```

Winlin 2014.2
