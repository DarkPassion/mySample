# HTTP API

SRS provides HTTP api, to external application to manage SRS, and support crossdomain for js.

The following is a SRS server api of ossrs.net:

![SRS](http://winlinvip.github.io/srs.release/wiki/images/demo.api.png?v1)

## Design Priciple

The HTTP API of SRS follows the simple priciple:

* Only provides API in json format, both request and json are json.
* No html, access the api return json format.
* When error, response in HTTP status or code in json.

## Build

To enable http api, configure SRS with `--with-http-api`, 
read [configure](v3_EN_Build)

```bash
./configure --with-http-api && make
```

## Config

The config also need to enable it:

```bash
listen              1935;
# system statistics section.
# the main cycle will retrieve the system stat,
# for example, the cpu/mem/network/disk-io data,
# the http api, for instance, /api/v1/summaries will show these data.
# @remark the heartbeat depends on the network,
#       for example, the eth0 maybe the device which index is 0.
stats {
    # the index of device ip.
    # we may retrieve more than one network device.
    # default: 0
    network         0;
    # the device name to stat the disk iops.
    # ignore the device of /proc/diskstats if not configed.
    disk            sda sdb xvda xvdb;
}
# api of srs.
# the http api config, export for external program to manage srs.
# user can access http api of srs in browser directly, for instance, to access by:
#       curl http://192.168.1.170:1985/api/v1/reload
# which will reload srs, like cmd killall -1 srs, but the js can also invoke the http api,
# where the cli can only be used in shell/terminate.
http_api {
    # whether http api is enabled.
    # default: off
    enabled         on;
    # the http api listen entry is <[ip:]port>
    # for example, 192.168.1.100:1985
    # where the ip is optional, default to 0.0.0.0, that is 1985 equals to 0.0.0.0:1985
    # default: 1985
    listen          1985;
    # whether enable crossdomain request.
    # default: on
    crossdomain     on;
    # the HTTP RAW API is more powerful api to change srs state and reload.
    raw_api {
        # whether enable the HTTP RAW API.
        # default: off
        enabled             off;
        # whether enable rpc reload.
        # default: off
        allow_reload        off;
        # whether enable rpc query.
        # default: off
        allow_query         off;
        # whether enable rpc update.
        # default: off
        allow_update        off;
    }
}
vhost __defaultVhost__ {
}
```

The `http_api` enable the HTTP API, and `stats` used for SRS to stat the system info, including:

* network: Used for heartbeat to report the network info, where heartbeat used to report system info. Please read [Heartbeat](https://github.com/simple-rtmp-server/srs/wiki/v1_EN_Heartbeat)
* disk: Used to stat the specified disk iops. You can use command `cat /proc/diskstats` to get the right disk names, for instance, xvda.

## Start

Start SRS: `./objs/srs -c http-api.conf`

Access api, open the url in web browser: [http://192.168.1.170:1985/api/v1](http://192.168.1.170:1985/api/v1)

Remark: Please use your server ip instead.

## Performance

The HTTP api supports 370 request per seconds.

## Access Api

Use web brower, or curl, or other http library.

SRS provides api urls list, no need to remember:
* code, an int error code. 0 is success.
* urls, the url lists, can be access.
* data, the last level api serve data.

Root directory:

```bash
# curl http://192.168.1.102:1985/
    "urls": {
        "api": "the api root"
    }
```

The urls is the apis to access:

```bash
# curl http://192.168.1.102:1985/api/
    "urls": {
        "v1": "the api version 1.0"
    }
```

Go on:

```bash
# curl http://192.168.1.102:1985/api/v1/
    "urls": {
        "versions": "the version of SRS",
        "authors": "the primary authors and contributors"
    }
```

Go on:

```bash
# curl http://192.168.1.102:1985/api/v1/versions
        "major": 0,
        "minor": 9,
        "revision": 43,
        "version": "0.9.43"
```

Or:

```bash
# curl http://192.168.1.102:1985/api/v1/authors
        "primary_authors": "winlin,wenjie.zhao",
        "contributors_link": "https://github.com/simple-rtmp-server/srs/blob/master/AUTHORS.txt",
        "contributors": "winlin<winlin@vip.126.com> wenjie.zhao<740936897@qq.com> xiangcheng.liu<liuxc0116@foxmail.com> naijia.liu<youngcow@youngcow.net> alcoholyi<alcoholyi@qq.com> "
```

The Api of SRS is self-describes api.

## Error Code

SRS response error in both HTTP status or HTTP body.

For example, SRS response HTTP error, where HTTP status not 200:

```
winlin:~ winlin$ curl -v http://127.0.0.1:1985 && echo ""
< HTTP/1.1 404 Not Found
< Connection: Keep-Alive
< Content-Length: 9
< Content-Type: text/plain; charset=utf-8
< Server: SRS/2.0.184
< 
Not Found
```

For example, SRS response code not 0 when HTTTP Status 200:

```
winlin:~ winlin$ curl -v http://127.0.0.1:1985/api/v1/tests/errors && echo ""
< HTTP/1.1 200 OK
< Connection: Keep-Alive
< Content-Length: 12
< Content-Type: application/json
< Server: SRS/2.0.184
< 
{"code":100}
```

User should handle these two error style.

## Crossdomain

SRS HTTP API supports js crossdomain, so the html/js can invoke http api of srs。

SRS support two main CROS styles:

* OPTIONS: JQuery can directly access the CROS, where the brower will send an OPTIONS first, then the API request.
* JSONP: JQuery/Angularjs can send JSONP CROS request to SRS API, where specifes the function name by QueryString `callback`.
* JSONP-DELETE: JSONP only support GET, so we use the `method` in QueryString to override the HTTP method for JSONP.

For example, the JSONP crossdomain request:

```
GET http://localhost:1985/api/v1/vhosts/?callback=JSON_CALLBACK
JSON_CALLBACK({"code":0,"server":13449})
GET http://localhost:1985/api/v1/vhosts/100?callback=JSON_CALLBACK&method=DELETE
JSON_CALLBACK({"code":0})
```

## Server ID

Each response of api contains a `server` field, which identify the server. When ServerID changed, SRS already restarted, all information before is invalid.

## API Nevigation

SRS provides the nevigation of APIs.

User can access the `http://192.168.1.102:1985/api/v1`, where:

| API | Example  | Description |
| --- | -------- | ---------   |
| server | 4481  | The identity of SRS   |
| versions | /api/v1/versions  | the version of SRS |
| summaries | /api/v1/summaries | the summary(pid, argv, pwd, cpu, mem) of SRS |
| rusages  | /api/v1/rusages | the rusage of SRS |
| self_proc_stats | /api/v1/self_proc_stats | the self process stats |
| system_proc_stats | /api/v1/system_proc_stats | the system process stats |
| meminfos | /api/v1/meminfos | the meminfo of system |
| authors | /api/v1/authors | the license, copyright, authors and contributors |
| features | /api/v1/features | the supported features of SRS |
| requests | /api/v1/requests | the request itself, for http debug |
| vhosts | /api/v1/vhosts | manage all vhosts or specified vhost |
| streams | /api/v1/streams | manage all streams or specified stream |
| clients | /api/v1/clients | manage all clients or specified client, default query top 10 clients |
| configs | /api/v1/configs | RAW API for CUID the configs |

## Summaries

User can get the system summaries, for instance, the memory, cpu, network, load usage.

Please access the url `http://192.168.1.170:1985/api/v1/summaries`

## Vhosts

SRS provides http api to query all vhosts.

The http api vhost url: `http://192.168.1.102:1985/api/v1/vhosts`

To process specified vhost by id, for instance `http://192.168.1.102:1985/api/v1/vhosts/3756`

## Streams

SRS provides http api to query all streams.

The http api stream url: `http://192.168.1.102:1985/api/v1/streams`

To process specified stream by id, for instance `http://192.168.1.102:1985/api/v1/streams/3756`

## Clients

SRS provides http api to query clients, default to get top 10 clients.

The http api client url: `http://192.168.1.102:1985/api/v1/clients`

To process specified client by id, for instance `http://192.168.1.102:1985/api/v1/clients/3756`

## Kickoff Client

SRS provides HTTP RESTful api to kickoff user:

```
DELETE /api/v1/clients/{id}
```

User can get the id of client to kickoff:

```
GET /api/v1/clients
```

User can get the id of publish client from streams api:

```
GET /api/v1/streams
or GET /api/v1/streams/6745
```

The client cid is the info from stream api `stream.publish.cid`:

```
1. GET http://192.168.1.170:1985/api/v1/streams/6745
2. Response stream.publish.cid:
stream: {
    publish: {
        active: true,
        cid: 107
    }
}
3. DELETE http://192.168.1.170:1985/api/v1/clients/107
```

Remark: User can use [HTTP REST Tool](http://ossrs.net/srs.release/http-rest/index.html) to send a request.

Remark: User can use linux tool `curl` to start HTTP request. For example:

```
curl -v -X GET http://192.168.1.170:1985/api/v1/clients/426 && echo ""
curl -v -X DELETE http://192.168.1.170:1985/api/v1/clients/426 && echo ""
```

## Persistence Config

User can send signal `SIGUSR1` to SRS to persistence config to file.

1. To reformat config without comments.
2. To support HTTP RAW API, please read [HTTP RAW API][HttpRawAPI].

For example:

```
killall -s SIGUSR1 srs
killall -30 srs
```

The config persistence file maybe:

```
listen 1935;
max_connections 1000;
daemon off;
srs_log_tank console;
pithy_print_ms 1000;
http_api {
    enabled on;
    listen 1985;
}
http_server {
    enabled on;
    listen 8080;
}
stream_caster {
    enabled off;
    caster flv;
    output rtmp://127.0.0.1/[app]/[stream];
    listen 8936;
}
vhost __defaultVhost__ {
    ingest livestream {
        enabled on;
        input {
            type file;
            url doc/source.200kbps.768x320.flv;
        }
        ffmpeg ./objs/ffmpeg/bin/ffmpeg;
        engine {
            enabled off;
            output rtmp://127.0.0.1:[port]/live?vhost=[vhost]/livestream;
        }
    }
    http_remux {
        enabled off;
        mount [vhost]/[app]/[stream].flv;
        hstrs on;
    }
}
```

## HTTP RAW API

SRS supports powerful HTTP RAW API, while other server only support `Read API`, for instance, to get the stat of server. SRS supports `Write API`, which can `Reload` or change server state.

<b>Remark:</b> User must enable the HTTP RAW API, in config section `http_api` to enable the `http_api.raw_api.enabled`, or SRS will response error code 1061.

```
http_api {
    enabled         on;
    listen          1985;
    raw_api {
        enabled             on;
        allow_reload        on;
        allow_query         on;
        allow_update        on;
    }
}
```

The supported HTTP RAW APi of SRS is:

* [Raw][raw-raw]: To query the HTTP RAW API config.
* [Reload][raw-reload]: To reload the SRS.
* [Query][raw-query]: Query the global and vhost config of SRS.
* [Update][raw-update]: Update the global and vhost config of SRS.
* [Vhost][raw-vhost]: Vhost is a subset of update.
* [DVR][raw-dvr]: DVR is a subset of update.

### Raw

| Key | DESC | 
| ---- | ---- |
| feature | Query the HTTP RAW API info. |
| url  | `/api/v1/raw?rpc=raw` |
| curl | `curl http://127.0.0.1:1985/api/v1/raw?rpc=raw` |
| config | No config |
| params | No params|

### RAW Reload

| Key | DESC | 
| ---- | ---- |
| feature | Reload is the same to `killall -1 srs` to reload the config |
| url  | `/api/v1/raw?rpc=reload` |
| curl | `curl http://127.0.0.1:1985/api/v1/raw?rpc=reload` |
| params | No params |

### RAW Query

| Key | DESC | 
| ---- | ---- |
| feature | Query the global config of SRS |
| url  | `/api/v1/raw?rpc=query&scope=global` |
| curl | `curl 'http://127.0.0.1:1985/api/v1/raw?rpc=query&scope=global'` |
| config | `allow_query on;`|
| params | `scope=global`, to query global config of SRS|

| Key | DESC | 
| ---- | ---- |
| feature | Query the minimal global config of SRS |
| url  | `/api/v1/raw?rpc=query&scope=minimal` |
| curl | `curl 'http://127.0.0.1:1985/api/v1/raw?rpc=query&scope=minimal'` |
| config | `allow_query on;`|
| params | `scope=minimal`, to query minimal global config of SRS|

| Key | DESC | 
| ---- | ---- |
| feature | Query specified vhost config. |
| url  | `/api/v1/raw?rpc=query&scope=vhost&vhost=__defaultVhost__` |
| curl | `curl 'http://127.0.0.1:1985/api/v1/raw?rpc=query&scope=vhost&vhost=__defaultVhost__'` |
| config | `allow_query on;`|
| params | `scope=vhost&vhost=xxx`, query specified vhost.|

### RAW Update

| Key | DESC | 
| ---- | ---- |
| feature | Update global listen port |
| url  | `/api/v1/raw?rpc=update&scope=listen&value=1935,1936` |
| curl | `curl 'http://127.0.0.1:1985/api/v1/raw?rpc=update&scope=listen&value=1935,1936'` |
| config | `allow_update on;`|
| params | `scope=listen&value=1935,1936`, specifies the listen port list|
| require | Int port list split by comma, for instance, 1935,1936,1937 |

| Key | DESC | 
| ---- | ---- |
| feature | Update the pid file of SRS |
| url  | `/api/v1/raw?rpc=update&scope=pid&value=./objs/srs.pid` |
| curl | `curl 'http://127.0.0.1:1985/api/v1/raw?rpc=update&scope=pid&value=./objs/srs.pid'` |
| config | `allow_update on;`|
| params | `scope=pid&value=./objs/srs.pid`, specifies the new pid file for SRS|
| require | File path must starts with ./, /tmp or /var, and ends with .pid, for instance, /var/srs.pid |

| Key | DESC | 
| ---- | ---- |
| feature | Update the global RTMP chunk_size |
| url  | `/api/v1/raw?rpc=update&scope=chunk_size&value=60000` |
| curl | `curl 'http://127.0.0.1:1985/api/v1/raw?rpc=update&scope=chunk_size&value=60000'` |
| config | `allow_update on;`|
| params | `scope=chunk_size&value=60000`, specifies the new global RTMP chunk_size |
| require | chunk_size must be integer in [128, 65535], for instance, 60000 |

| Key | DESC | 
| ---- | ---- |
| feature | Update the global log dir for ffmpeg |
| url  | `/api/v1/raw?rpc=update&scope=ff_log_dir&value=./objs` |
| curl | `curl 'http://127.0.0.1:1985/api/v1/raw?rpc=update&scope=ff_log_dir&value=./objs'` |
| config | `allow_update on;`|
| params | `scope=ff_log_dir&value=./objs`, specifies the new global ff_log_dir|
| require | ff_log_dir must starts with ./, /tmp/ or /var/, for instance, ./objs |

| Key | DESC | 
| ---- | ---- |
| feature | Update the global log tank for SRS |
| url  | `/api/v1/raw?rpc=update&scope=srs_log_tank&value=file` |
| curl | `curl 'http://127.0.0.1:1985/api/v1/raw?rpc=update&scope=srs_log_tank&value=file'` |
| config | `allow_update on;`|
| params | `scope=srs_log_tank&value=file`, specifies the new log tank. |
| require | srs_log_tank must be file or console, for instance, file |

| Key | DESC | 
| ---- | ---- |
| feature | Update the global log level for SRS|
| url  | `/api/v1/raw?rpc=update&scope=srs_log_level&value=trace` |
| curl | `curl 'http://127.0.0.1:1985/api/v1/raw?rpc=update&scope=srs_log_level&value=trace'` |
| config | `allow_update on;`|
| params | `scope=srs_log_level&value=trace`, speicifies the new log level |
| require | srs_log_level must be verbose,info,trace,warn,error, for inance, trace |

| Key | DESC | 
| ---- | ---- |
| feature | Update the global log file path for SRS.|
| url  | `/api/v1/raw?rpc=update&scope=srs_log_file&value=./objs/srs.log` |
| curl | `curl 'http://127.0.0.1:1985/api/v1/raw?rpc=update&scope=srs_log_file&value=./objs/srs.log'` |
| config | `allow_update on;`|
| params | `scope=srs_log_file&value=./objs/srs.log`, specifies the new log file |
| require | srs_log_file must be *.log and in ./, /var/ or /tmp/, for instance, ./objs/srs.log |

| Key | DESC | 
| ---- | ---- |
| feature | Update the max connections for SRS, both RTMP and HTTP.|
| url  | `/api/v1/raw?rpc=update&scope=max_connections&value=1000` |
| curl | `curl 'http://127.0.0.1:1985/api/v1/raw?rpc=update&scope=max_connections&value=1000'` |
| config | `allow_update on;`|
| params | `scope=max_connections&value=1000`, specifies the new max connections|
| require | max_connections must be interger and in [10, 65535], for instance,  1000 |

| Key | DESC | 
| ---- | ---- |
| feature | Whether enable the utc time for log and path which use time|
| url  | `/api/v1/raw?rpc=update&scope=utc_time&value=false` |
| curl | `curl 'http://127.0.0.1:1985/api/v1/raw?rpc=update&scope=utc_time&value=false'` |
| config | `allow_update on;`|
| params | `scope=utc_time&value=false`, specifies whether enable the utc time |
| require | utc_time must be true or false, for instance, false |

| Key | DESC | 
| ---- | ---- |
| feature | Update the global pithy print time in ms |
| url  | `/api/v1/raw?rpc=update&scope=pithy_print_ms&value=10000` |
| curl | `curl 'http://127.0.0.1:1985/api/v1/raw?rpc=update&scope=pithy_print_ms&value=10000'` |
| config | `allow_update on;`|
| params | `scope=pithy_print_ms&value=10000`, specifies the new pithy print time |
| require | pithy_print_ms must in [100,300000], in ms, for insance 10000 |

## RAW Vhost

The vhost raw apis is a subset of update.

| Key | DESC | 
| ---- | ---- |
| feature | Add a none exists vhost. |
| url  | `/api/v1/raw?rpc=update&scope=vhost&value=ossrs.net&param=create` |
| curl | `curl 'http://127.0.0.1:1985/api/v1/raw?rpc=update&scope=vhost&value=ossrs.net&param=create'` |
| config | `allow_update on;`|
| params | `scope=vhost&value=ossrs.net&param=create` to create a vhost |
| require | The vhost to crote must not be exists.|

| Key | DESC | 
| ---- | ---- |
| feature | Update the name of vhost. |
| url  | `/api/v1/raw?rpc=update&scope=vhost&value=ossrs.net&param=update&data=new.ossrs.net` |
| curl | `curl 'http://127.0.0.1:1985/api/v1/raw?rpc=update&scope=vhost&value=ossrs.net&param=update&data=new.ossrs.net'` |
| config | `allow_update on;`|
| params | `scope=vhost&value=ossrs.net&param=update&data=new.ossrs.net` to update the name of vhost |
| require | The vhost to update must exists and disabled. |

| Key | DESC | 
| ---- | ---- |
| feature | Disable the vhost|
| url  | `/api/v1/raw?rpc=update&scope=vhost&value=ossrs.net&param=disable` |
| curl | `curl 'http://127.0.0.1:1985/api/v1/raw?rpc=update&scope=vhost&value=ossrs.net&param=disable'` |
| config | `allow_update on;`|
| params | `scope=vhost&value=ossrs.net&param=disable` to disable the vhost|
| require | The vhost to disable must exists and enabled.  |

| Key | DESC | 
| ---- | ---- |
| feature | Enable the vhost|
| url  | `/api/v1/raw?rpc=update&scope=vhost&value=ossrs.net&param=enable` |
| curl | `curl 'http://127.0.0.1:1985/api/v1/raw?rpc=update&scope=vhost&value=ossrs.net&param=enable'` |
| config | `allow_update on;`|
| params | `scope=vhost&value=ossrs.net&param=enable` to enable the vhost|
| require | The vhost to enable must exists and disabled. |

## RAW DVR

The DVR is a subset of Update.

| Key | DESC | 
| ---- | ---- |
| feature | Enable the DVR of stream of vhost |
| url  | `/api/v1/raw?rpc=update&scope=dvr&value=ossrs.net&param=enable&data=live/livestream` |
| curl | `curl 'http://127.0.0.1:1985/api/v1/raw?rpc=update&scope=dvr&value=ossrs.net&param=enable&data=live/livestream'` |
| config | `allow_update on;`|
| params | `scope=dvr&value=ossrs.net&param=enable&data=live/livestream` to enable the DVR of stream|
| require | The DVR of vhost must enabled.|

| Key | DESC | 
| ---- | ---- |
| feature | Disable the DVR of stream of vhost|
| url  | `/api/v1/raw?rpc=update&scope=dvr&value=ossrs.net&param=disable&data=live/livestream` |
| curl | `curl 'http://127.0.0.1:1985/api/v1/raw?rpc=update&scope=dvr&value=ossrs.net&param=disable&data=live/livestream'` |
| config | `allow_update on;`|
| params | `scope=dvr&value=ossrs.net&param=disable&data=live/livestream` to disable the DVR of stream |
| require | The DVR of vhost must enabled. |

Winlin 2015.8

[HttpRawAPI]: https://github.com/simple-rtmp-server/srs/issues/319
[raw-raw]: https://github.com/simple-rtmp-server/srs/wiki/v3_EN_HTTPApi#raw
[raw-reload]: https://github.com/simple-rtmp-server/srs/wiki/v3_EN_HTTPApi#reload
[raw-query]: https://github.com/simple-rtmp-server/srs/wiki/v3_EN_HTTPApi#raw-query
[raw-update]: https://github.com/simple-rtmp-server/srs/wiki/v3_EN_HTTPApi#raw-update
[raw-vhost]: https://github.com/simple-rtmp-server/srs/wiki/v3_EN_HTTPApi#raw-vhost
[raw-dvr]: https://github.com/simple-rtmp-server/srs/wiki/v3_EN_HTTPApi#raw-dvr
