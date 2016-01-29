# HTTP callback

SRS does not support server-side script, but support
http-callback, read [ServerSide script](v1_EN_ServerSideScript).

About the token authentication which is base on http callback, read [Token Authentication](v1_EN_DRM#token-authentication)

## Compile

Build SRS with or without http callback, read
[Build](v1_EN_Build)

## HTTP callback events

SRS can call the http callback, for events:

| Event | Data | Description |
| --- | ---- | ---- |
| on_connect|{<br/> "action": "on_connect",<br/> "client_id": 1985,<br/> "ip": "192.168.1.10", <br/> "vhost": "video.test.com", <br/> "app": "live",<br/> "tcUrl": "rtmp://x/x?key=xxx",<br/> "pageUrl": "http://x/x.html"<br/> } | When a client connects to the specified vhost and app.| 
| on_close|{<br/> "action": "on_close",<br/> "client_id": 1985,<br/> "ip": "192.168.1.10", <br/> "vhost": "video.test.com", <br/> "app": "live"<br/> } | When a client closes a connection, or the server drops a connection.| 
| on_publish|{<br/> "action": "on_publish",<br/> "client_id": 1985,<br/> "ip": "192.168.1.10", <br/> "vhost": "video.test.com", <br/> "app": "live",<br/> "stream": "livestream"<br/> } | When a client publishes a stream, for example, using flash or FMLE to publish a stream to the server.| 
| on_unpublish|{<br/> "action": "on_unpublish",<br/> "client_id": 1985,<br/> "ip": "192.168.1.10", <br/> "vhost": "video.test.com", <br/> "app": "live",<br/> "stream": "livestream"<br/> } | When a client stops publishing a stream.| 
| on_play|{<br/> "action": "on_play",<br/> "client_id": 1985,<br/> "ip": "192.168.1.10", <br/> "vhost": "video.test.com", <br/> "app": "live",<br/> "stream": "livestream"<br/> } | When a client starts playing a stream.| 
| on_stop|{<br/> "action": "on_stop",<br/> "client_id": 1985,<br/> "ip": "192.168.1.10", <br/> "vhost": "video.test.com", <br/> "app": "live",<br/> "stream": "livestream"<br/> } | When a client stops playback.| 

Note:
* Event: When this event occur, callback the specified HTTP url.
* HTTP url: Can be multiple urls, splits by space, SRS will notice all one by one.
* Data: SRS will POST the data to specified HTTP api.
* Return Code: SRS requires the response is an int, indicates the error, 0 is success.
SRS will disconnect the connection when response is not 0, or http status is not 200.

## Config SRS

For example, when user connect at vhost/app, verify the ip of client:

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

Note: For more information, read conf/full.conf the section hooks.callback.vhost.com

## SRS HTTP callback Server

SRS provides a default HTTP callback server, use cherrypy.

To start it: `python research/api-server/server.py 8085`

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

## Publish and Play

Publish stream to SRS, SRS will call the http callback:

```bash
[2014-02-27 09:41:33][trace] post to clients, req={"action":"on_connect","client_id":4,"ip":"192.168.1.179","vhost":"__defaultVhost__","app":"live","pageUrl":""}
[2014-02-27 09:41:33][trace] srs on_connect: client id=4, ip=192.168.1.179, vhost=__defaultVhost__, app=live, pageUrl=
127.0.0.1 - - [27/Feb/2014:09:41:33] "POST /api/v1/clients HTTP/1.1" 200 1 "" "srs(simple rtmp server)0.9.2"
```

Play stream on SRS, SRS will call the http callback:

```bash
[2014-02-27 09:41:50][trace] post to clients, req={"action":"on_connect","client_id":5,"ip":"192.168.1.179","vhost":"__defaultVhost__","app":"live","pageUrl":"http://dev.chnvideo.com:3080/players/rtmp/"}
[2014-02-27 09:41:50][trace] srs on_connect: client id=5, ip=192.168.1.179, vhost=__defaultVhost__, app=live, pageUrl=http://dev.chnvideo.com:3080/players/rtmp/
127.0.0.1 - - [27/Feb/2014:09:41:50] "POST /api/v1/clients HTTP/1.1" 200 1 "" "srs(simple rtmp server)0.9.2"
```

Winlin 2014.11
