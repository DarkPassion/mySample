# HTTPCallback

SRS does not support server-side scripting, instead it supports
HTTP callbacks; read [ServerSide script](v1_EN_ServerSideScript).

For token authentication based on HTTP callbacks, read [Token Authentication](v3_EN_DRM#token-authentication)

## Compile

Use `--with-http-callback` to enable HttpCallback, while `--without-http-callback` to disable it.

For more information, read [Build](v3_EN_Build)

## Configuring SRS

The config for HTTP hooks is:

```
vhost your_vhost {
    http_hooks {
        # Whether http hooks are enabled.
        # Default: off
        enabled         on;

        # on_connect Hook
        #
        # When clients connect to the vhost/app, call this hook, with
        # the request's POST data containing a JSON-encoded object like this:
        #       {
        #           "action": "on_connect",
        #           "client_id": 1985,
        #           "ip": "192.168.1.10", "vhost": "video.test.com", "app": "live",
        #           "tcUrl": "rtmp://video.test.com/live?key=d2fa801d08e3f90ed1e1670e6e52651a",
        #           "pageUrl": "http://www.test.com/live.html"
        #       }
        # If valid, the hook must return HTTP code 200 (Status OK) and its response,
        # an int value, specifies the error code (0 corresponding to success):
        #       0
        # Multiple API hooks are supported in this format:
        #       on_connect http://xxx/api0 http://xxx/api1 http://xxx/apiN
        on_connect      http://127.0.0.1:8085/api/v1/clients http://localhost:8085/api/v1/clients;

        # on_close Hook
        #
        # When clients close/disconnect from the vhost/app/stream, call this hook.
        # The request's POST data will be a JSON object like the following:
        #       {
        #           "action": "on_close",
        #           "client_id": 1985,
        #           "ip": "192.168.1.10", "vhost": "video.test.com", "app": "live"
        #       }
        # If valid, the hook must return HTTP code 200 (Status OK) with its response,
        # an int value, specifying the error code (0 corresponding to success):
        #       0
        # Multiple API hooks are supported in this format:
        #       on_close http://xxx/api0 http://xxx/api1 http://xxx/apiN
        on_close        http://127.0.0.1:8085/api/v1/clients http://localhost:8085/api/v1/clients;

        # on_publish Hook
        #
        # When a client (encoder) publishes to the vhost/app/stream, call this hook.
        # The request's POST data will be a JSON object similar to the following:
        #       {
        #           "action": "on_publish",
        #           "client_id": 1985,
        #           "ip": "192.168.1.10", "vhost": "video.test.com", "app": "live",
        #           "stream": "livestream"
        #       }
        # If valid, the hook must return HTTP code 200 (Status OK) and its response,
        # an int value, specifies the error code (0 corresponding to success):
        #       0
        # Multiple API hooks are supported in this format:
        #       on_publish http://xxx/api0 http://xxx/api1 http://xxx/apiN
        on_publish      http://127.0.0.1:8085/api/v1/streams http://localhost:8085/api/v1/streams;

        # on_unpublish Hook
        #
        # When a client (encoder) stops publishing to the vhost/app/stream, call this hook.
        # The request's POST data will be a JSON object like this:
        #       {
        #           "action": "on_unpublish",
        #           "client_id": 1985,
        #           "ip": "192.168.1.10", "vhost": "video.test.com", "app": "live",
        #           "stream": "livestream"
        #       }
        # If valid, the hook must return HTTP code 200 (Status OK) and its response,
        # an int value, specifies the error code (0 corresponding to success):
        #       0
        # Multiple API hooks are supported with this format:
        #       on_unpublish http://xxx/api0 http://xxx/api1 http://xxx/apiN
        on_unpublish    http://127.0.0.1:8085/api/v1/streams http://localhost:8085/api/v1/streams;

        # on_play Hook
        #
        # When client start to play the vhost/app/stream, call this hook,
        # with the request's POST data a JSON object similar to:
        #       {
        #           "action": "on_play",
        #           "client_id": 1985,
        #           "ip": "192.168.1.10", "vhost": "video.test.com", "app": "live",
        #           "stream": "livestream",
        #           "pageUrl": "http://www.test.com/live.html"
        #       }
        # If valid, the hook must return HTTP code 200 (Status OK) and its response,
        # an int value, specifies the error code (0 corresponding to success):
        #       0
        # Multiple API hooks are supported with this format:
        #       on_play http://xxx/api0 http://xxx/api1 http://xxx/apiN
        on_play         http://127.0.0.1:8085/api/v1/sessions http://localhost:8085/api/v1/sessions;

        # on_stop
        #
        # When a client stops playing the vhost/app/stream, call this hook,
        # with the request's POST data a JSON object as follows:
        #       {
        #           "action": "on_stop",
        #           "client_id": 1985,
        #           "ip": "192.168.1.10", "vhost": "video.test.com", "app": "live",
        #           "stream": "livestream"
        #       }
        # If valid, the hook must return HTTP code 200 (Status OK) and its response,
        # an int value, specifies the error code (0 corresponding to success):
        #       0
        # Multiple API hooks are supported with this format:
        #       on_stop http://xxx/api0 http://xxx/api1 http://xxx/apiN
        on_stop         http://127.0.0.1:8085/api/v1/sessions http://localhost:8085/api/v1/sessions;

        # on_dvr Hook
        #
        # When SRS reap a DVR file, call this hook,
        # with the request's POST data a JSON object like this:
        #       {
        #           "action": "on_dvr",
        #           "client_id": 1985,
        #           "ip": "192.168.1.10", "vhost": "video.test.com", "app": "live",
        #           "stream": "livestream",
        #           "cwd": "/usr/local/srs",
        #           "file": "./objs/nginx/html/live/livestream.1420254068776.flv"
        #       }
        # If valid, the hook must return HTTP code 200 (Status OK) and its response,
        # an int value, specifies the error code (0 corresponding to success):
        #       0
        on_dvr          http://127.0.0.1:8085/api/v1/dvrs http://localhost:8085/api/v1/dvrs;
    }
}
```

Note: For more information, read the section hooks.callback.vhost.com in conf/full.conf 

## HTTP Callback Events

SRS can call HTTP callbacks for events:

| Event | Data | Description |
| --- | ---- | ---- |
| on_connect|{<br/> "action": "on_connect",<br/> "client_id": 1985,<br/> "ip": "192.168.1.10", <br/> "vhost": "video.test.com", <br/> "app": "live",<br/> "tcUrl": "rtmp://x/x?key=xxx",<br/> "pageUrl": "http://x/x.html"<br/> } | When a client connects to the specified vhost and app.| 
| on_close|{<br/> "action": "on_close",<br/> "client_id": 1985,<br/> "ip": "192.168.1.10", <br/> "vhost": "video.test.com", <br/> "app": "live", <br/> "send_bytes": 10240, <br/> "recv_bytes": 10240<br/> } | When a client closes a connection, or the server drops a connection.| 
| on_publish|{<br/> "action": "on_publish",<br/> "client_id": 1985,<br/> "ip": "192.168.1.10", <br/> "vhost": "video.test.com", <br/> "app": "live",<br/> "stream": "livestream"<br/> } | When a client publishes a stream, for example, using flash or FMLE to publish a stream to the server.| 
| on_unpublish|{<br/> "action": "on_unpublish",<br/> "client_id": 1985,<br/> "ip": "192.168.1.10", <br/> "vhost": "video.test.com", <br/> "app": "live",<br/> "stream": "livestream"<br/> } | When a client stops publishing a stream.| 
| on_play|{<br/> "action": "on_play",<br/> "client_id": 1985,<br/> "ip": "192.168.1.10", <br/> "vhost": "video.test.com", <br/> "app": "live",<br/> "stream": "livestream",<br/> "pageUrl": "http://www.test.com/live.html"<br/> } | When a client starts playing a stream.| 
| on_stop|{<br/> "action": "on_stop",<br/> "client_id": 1985,<br/> "ip": "192.168.1.10", <br/> "vhost": "video.test.com", <br/> "app": "live",<br/> "stream": "livestream"<br/> } | When a client stops playback.| 
| on_dvr|{<br/> "action": "on_dvr",<br/> "client_id": 1985,<br/> "ip": "192.168.1.10", <br/> "vhost": "video.test.com", <br/> "app": "live",<br/> "stream": "livestream",<br/> "cwd": "/opt",<br/> "file": "./l.xxx.flv"<br/> } | When reap a DVR file.|

Notes:
* Event: When this event occurs, call back to the specified HTTP URL.
* HTTP URL: Can be multiple URLs, split by spaces, SRS will notify all one by one.
* Data: SRS will POST the data to specified HTTP API.
* Return Code: SRS requires that the response is an int indicating the error, 0 is success.
SRS will disconnect the connection when the response is not 0, or HTTP status is not 200.

## SRS HTTP Callback Server

SRS provides a default HTTP callback server, using cherrypy.

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

When publishing a stream to SRS, SRS will call the registered HTTP callback:

```bash
[2014-02-27 09:41:33][trace] post to clients, req={"action":"on_connect","client_id":4,"ip":"192.168.1.179","vhost":"__defaultVhost__","app":"live","pageUrl":""}
[2014-02-27 09:41:33][trace] srs on_connect: client id=4, ip=192.168.1.179, vhost=__defaultVhost__, app=live, pageUrl=
127.0.0.1 - - [27/Feb/2014:09:41:33] "POST /api/v1/clients HTTP/1.1" 200 1 "" "srs(simple rtmp server)0.9.2"
```

When playing a stream on SRS, SRS will call the registered HTTP callback:

```bash
[2014-02-27 09:41:50][trace] post to clients, req={"action":"on_connect","client_id":5,"ip":"192.168.1.179","vhost":"__defaultVhost__","app":"live","pageUrl":"http://dev.chnvideo.com:3080/players/rtmp/"}
[2014-02-27 09:41:50][trace] srs on_connect: client id=5, ip=192.168.1.179, vhost=__defaultVhost__, app=live, pageUrl=http://dev.chnvideo.com:3080/players/rtmp/
127.0.0.1 - - [27/Feb/2014:09:41:50] "POST /api/v1/clients HTTP/1.1" 200 1 "" "srs(simple rtmp server)0.9.2"
```

## Snapshot

The HttpCallback can used to snapshot, please read [snapshot](https://github.com/simple-rtmp-server/srs/wiki/v3_EN_Snapshot#httpcallback)

Winlin 2015.1
