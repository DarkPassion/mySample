# HTTP API

SRS提供HTTP接口，供外部程序管理服务器，并支持跨域（js可以直接控制和获取服务器的各种信息）。

下面是ossrs.net一台演示SRS的二维码，使用微信扫描即可看到这台服务器的版本：

![SRS](http://winlinvip.github.io/srs.release/wiki/images/demo.api.png?v1)

## 设计原则

SRS的HTTP接口遵循最简单原则，主要包括：
* 只提供json数据格式接口，要求请求和响应的数据全都是json。
* 不提供html数据，譬如运行SRS后，浏览器打开HTTP接口或HTTP服务地址，看到的是json，不是html。
* 发生错误时，支持HTTP错误码，或者json中的code错误码。

## Build

SRS需要打开HTTPApi选项，参考：[configure选项](v3_CN_Build)

```bash
./configure --with-http-api && make
```

## Config

配置文件需要开启http-api：

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

其中，`http_api`开启了HTTP API，`stats`配置了SRS后台统计的信息，包括：

* network: 这个配置了heartbeat使用的网卡ip，即SRS主动汇报的网卡信息。参考[Heartbeat](https://github.com/simple-rtmp-server/srs/wiki/v1_CN_Heartbeat)
* disk: 这个配置了需要统计的磁盘的IOPS，可以通过`cat /proc/diskstats`命令获得名称，譬如阿里云的磁盘名称叫xvda.

## Start

启动服务器：`./objs/srs -c http-api.conf`

访问api：浏览器打开地址[http://192.168.1.170:1985/api/v1](http://192.168.1.170:1985/api/v1)

请注意：请将`192.168.1.170`或者下面提到的任何服务器IP，换成您的服务器的IP。

## 性能

机器：虚拟机CentOS6-64位，4CPU，T430笔记本，VirtualBox

10%CPU，10000次请求，27秒，平均370次请求/秒，30毫秒一个请求

```bash
top - 09:59:49 up 3 days, 50 min,  4 users,  load average: 0.00, 0.00, 0.00
Tasks: 140 total,   1 running, 139 sleeping,   0 stopped,   0 zombie
Cpu(s): 11.6%us, 20.0%sy,  0.0%ni, 66.7%id,  0.0%wa,  0.0%hi,  1.8%si,  0.0%st
Mem:   2055440k total,   990148k used,  1065292k free,   228544k buffers
Swap:  2064376k total,        0k used,  2064376k free,   486620k cached
  PID USER      PR  NI  VIRT  RES  SHR S %CPU %MEM    TIME+  COMMAND
29696 winlin    20   0 15872 1592 1360 S  9.3  0.1   0:14.21 ./objs/srs -c console.conf
```

```bash
[winlin@dev6 srs]$ time for((i=0;i<10000;i++)); do curl http://127.0.0.1:1985/api >/dev/null 2>&1; done

real	0m27.375s
user	0m8.223s
sys	0m16.289s
```

## 访问api

直接在浏览器中就可以访问，或者用curl发起http请求。

SRS提供了api的面包屑，可以从根目录开始导航，不需要任何记忆。一般的字段包括：
* code表示错误码，按照linux惯例，0表示成功。
* urls表示是面包屑导航，该api下面的子api（链接）。
* data表示最后一级提供服务的api，返回的数据。

另外，提供服务的api按照HTTP RESTful规则是复数，譬如versions/authors，表示资源。HTTP的各种方法表示操作，譬如GET查询，PUT更新，DELETE删除。参考：[Redmine HTTP Rest api](http://www.redmine.org/projects/redmine/wiki/Rest_api)

根目录：

```bash
# curl http://192.168.1.170:1985/
    "urls": {
        "api": "the api root"
    }
```

返回的urls表示子链接可以访问。接着访问：

```bash
# curl http://192.168.1.170:1985/api/
    "urls": {
        "v1": "the api version 1.0"
    }
```

继续：

```bash
# curl http://192.168.1.170:1985/api/v1/
    "urls": {
        "versions": "the version of SRS",
        "authors": "the primary authors and contributors"
    }
```

继续：

```bash
# curl http://192.168.1.170:1985/api/v1/versions
        "major": 0,
        "minor": 9,
        "revision": 43,
        "version": "0.9.43"
```

或者：

```bash
# curl http://192.168.1.170:1985/api/v1/authors
        "primary_authors": "winlin,wenjie.zhao",
        "contributors_link": "https://github.com/simple-rtmp-server/srs/blob/master/AUTHORS.txt",
        "contributors": "winlin<winlin@vip.126.com> wenjie.zhao<740936897@qq.com> xiangcheng.liu<liuxc0116@foxmail.com> naijia.liu<youngcow@youngcow.net> alcoholyi<alcoholyi@qq.com> "
```

SRS的API属于“自解释型，HTTP RESTful API”

## Error Code

SRS可能返回HTTP错误，即Status不等于200；或者在HTTP Status为200时，响应的json的code不为0.

譬如，返回HTTP错误：

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

譬如，HTTP200时内容中code不等于0：

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

用户应该处理这两种错误。

## Crossdomain

SRS HTTP API支持跨域，js可以直接调用srs的http api。

SRS支持两种跨域方式：

* OPTIONS: jquery可以直接跨域请求API，浏览器会发送一个OPTIONS跨域请求，SRS允许跨域后，浏览器再次发起API请求。
* JSONP: jquery/angularjs可以发起JSONP跨域请求，服务器会将响应作为js文件，内容是调用一个函数，函数名由QueryString中的callback指定。
* JSONP-DELETE: JSONP只能GET，因此DELETE方法是由QueryString的method指定的。

JSONP实例，例如：

```
GET http://localhost:1985/api/v1/vhosts/?callback=JSON_CALLBACK
JSON_CALLBACK({"code":0,"server":13449})
GET http://localhost:1985/api/v1/vhosts/100?callback=JSON_CALLBACK&method=DELETE
JSON_CALLBACK({"code":0})
```

## Server ID

SRS返回的api中都会带有`server`的信息，即Server的ID，用来标识服务器。客户端在获取信息时，必须检查ServerID是否改变，改变时就是服务器重启，之前所有的数据都应该作废了。

## API Nevigation

SRS提供了API的导航，即所有支持的API及描述。

地址是：`http://192.168.1.170:1985/api/v1`，主要包含的子api有：

| API | Example  | Description |
| --- | -------- | ---------   |
| server | 4481  | 服务器标识   |
| versions | /api/v1/versions  | 获取服务器版本信息 |
| summaries | /api/v1/summaries | 获取服务器的摘要信息 |
| rusages  | /api/v1/rusages | 获取服务器资源使用信息 |
| self_proc_stats | /api/v1/self_proc_stats | 获取服务器进程信息 |
| system_proc_stats | /api/v1/system_proc_stats | 获取服务器所有进程情况 |
| meminfos | /api/v1/meminfos | 获取服务器内存使用情况 |
| authors | /api/v1/authors | 获取作者、版权和License信息 |
| features | /api/v1/features | 获取系统支持的功能列表 |
| requests | /api/v1/requests | 获取请求的信息，即当前发起的请求的详细信息 |
| vhosts | /api/v1/vhosts | 获取服务器上的vhosts信息 |
| streams | /api/v1/streams | 获取服务器的streams信息 |
| clients | /api/v1/clients | 获取服务器的clients信息，默认获取前10个 |
| configs | /api/v1/configs | CUID配置，RAW API |

## Summaries

SRS提供系统的摘要信息接口，譬如当前的内存、CPU、网络、负载使用率。

地址为：`http://192.168.1.170:1985/api/v1/summaries`

## Vhosts

SRS提供获取所有vhost的接口，vhost中的server为srs的id，用来标识是否服务器重启了。

地址为：`http://192.168.1.170:1985/api/v1/vhosts`

还可以继续处理某个vhost的信息，譬如`http://192.168.1.170:1985/api/v1/vhosts/3756`

## Streams

SRS提供获取所有stream的接口，stream中的server为srs的id，用来标识是否服务器重启了。vhost为stream所属的vhost的id。

地址为：`http://192.168.1.170:1985/api/v1/streams`

还可以继续处理某个stream的信息，譬如`http://192.168.1.170:1985/api/v1/streams/3756`

## Clients

SRS提供查询客户端信息的接口，和Vhosts或Streams不一样的是，Clients查询时需要指定start和count(默认start为0，count为10，即查询头10个clients)。

地址为：`http://192.168.1.170:1985/api/v1/clients`

还可以继续处理某个client的信息，譬如`http://192.168.1.170:1985/api/v1/clients/3756`

## Kickoff Client

可以踢掉连接的用户，SRS提供了HTTP RESTful接口：

```
DELETE /api/v1/clients/{id}
```

可以先查询到需要踢掉的Client的ID：

```
GET /api/v1/clients
```

若需要踢掉推流的Client，可以从streams接口中查询推流client的id：

```
GET /api/v1/streams
or GET /api/v1/streams/6745
```

流信息中的`stream.publish.cid`就是推流的客户端id：

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

备注：HTTP请求可以使用[HTTP REST Tool](http://ossrs.net/srs.release/http-rest/index.html)

备注：HTTP请求还可以使用Linux的工具`curl`，常见的请求如下：

```
curl -v -X GET http://192.168.1.170:1985/api/v1/clients/426 && echo ""
curl -v -X DELETE http://192.168.1.170:1985/api/v1/clients/426 && echo ""
```

## Persistence Config

可以发送信号`SIGUSR1`给SRS，用来将当前系统的配置写入配置文件。主要用于：

1. 去掉注释，没有用的配置，格式化配置。
2. 支持HTTP API写入配置，通过HTTP API改变配置后写入然后Reload，参考：[HTTP RAW API][HttpRawAPI].

命令实例：

```
killall -s SIGUSR1 srs
killall -30 srs
```

写出来的配置文件可能是：

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

SRS支持RAW API，一般的服务器只能提供读(Read)形式的API，譬如获取系统状态之类，但是SRS提供写(Write)形式的API，譬如Reload和修改系统配置等所有改写系统的行为。

<b>注意:</b> 必须在`http_api`配置中，开启`http_api.raw_api.enabled`才能允许HTTP RAW API，否则会返回错误代码是1061。

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

SRS支持的HTTP RAW API包括：

* [Raw][raw-raw]: 查看HTTP RAW API的配置。
* [Reload][raw-reload]: 支持reload配置。
* [Query][raw-query]: 查询全局和Vhost配置。
* [Update][raw-update]: 更新全局和Vhost配置。
* [Vhost][raw-vhost]: Vhost操作是Update的子集。
* [DVR][raw-dvr]: DVR操作是Update的子集。

### Raw

| Key | DESC | 
| ---- | ---- |
| feature | 查询服务器端HTTP RAW API的配置 |
| url  | `/api/v1/raw?rpc=raw` |
| curl | `curl http://127.0.0.1:1985/api/v1/raw?rpc=raw` |
| config | 不需要 |
| params | 无参数 |

### RAW Reload

| Key | DESC | 
| ---- | ---- |
| feature | 可以重新加载配置，和`killall -1 srs`的效果是一样的 |
| url  | `/api/v1/raw?rpc=reload` |
| curl | `curl http://127.0.0.1:1985/api/v1/raw?rpc=reload` |
| config | `allow_reload on;`|
| params | 无参数 |

### RAW Query

| Key | DESC | 
| ---- | ---- |
| feature | 查询服务器全局配置 |
| url  | `/api/v1/raw?rpc=query&scope=global` |
| curl | `curl 'http://127.0.0.1:1985/api/v1/raw?rpc=query&scope=global'` |
| config | `allow_query on;`|
| params | `scope=global`，查询服务器的全局配置|

| Key | DESC | 
| ---- | ---- |
| feature | 查询服务器最小全局配置 |
| url  | `/api/v1/raw?rpc=query&scope=minimal` |
| curl | `curl 'http://127.0.0.1:1985/api/v1/raw?rpc=query&scope=minimal'` |
| config | `allow_query on;`|
| params | `scope=minimal`，查询服务器的最小全局配置|

| Key | DESC | 
| ---- | ---- |
| feature | 查询服务器指定的Vhost配置 |
| url  | `/api/v1/raw?rpc=query&scope=vhost&vhost=__defaultVhost__` |
| curl | `curl 'http://127.0.0.1:1985/api/v1/raw?rpc=query&scope=vhost&vhost=__defaultVhost__'` |
| config | `allow_query on;`|
| params | `scope=vhost&vhost=xxx`，查询服务器的指定的Vhost的配置|

### RAW Update

| Key | DESC | 
| ---- | ---- |
| feature | 更新服务器侦听端口 |
| url  | `/api/v1/raw?rpc=update&scope=listen&value=1935,1936` |
| curl | `curl 'http://127.0.0.1:1985/api/v1/raw?rpc=update&scope=listen&value=1935,1936'` |
| config | `allow_update on;`|
| params | `scope=listen&value=1935,1936`，指定侦听的端口列表|
| require | 参数必须是整数端口列表，多个端口时以逗号分割，譬如：1935,1936,1937 |

| Key | DESC | 
| ---- | ---- |
| feature | 更新服务器PID文件 |
| url  | `/api/v1/raw?rpc=update&scope=pid&value=./objs/srs.pid` |
| curl | `curl 'http://127.0.0.1:1985/api/v1/raw?rpc=update&scope=pid&value=./objs/srs.pid'` |
| config | `allow_update on;`|
| params | `scope=pid&value=./objs/srs.pid`，指定新的PID文件|
| require | 文件路径必须以./，/tmp/或/var/开头，并且扩展名必须是.pid，譬如：/var/srs.pid |

| Key | DESC | 
| ---- | ---- |
| feature | 设置RTMP全局chunk_size |
| url  | `/api/v1/raw?rpc=update&scope=chunk_size&value=60000` |
| curl | `curl 'http://127.0.0.1:1985/api/v1/raw?rpc=update&scope=chunk_size&value=60000'` |
| config | `allow_update on;`|
| params | `scope=chunk_size&value=60000`，指定新的全局chunk_size|
| require | chunk_size必须是数字，并且在[128, 65535]中，譬如：60000 |

| Key | DESC | 
| ---- | ---- |
| feature | 设置ffmpeg的全局日志路径 |
| url  | `/api/v1/raw?rpc=update&scope=ff_log_dir&value=./objs` |
| curl | `curl 'http://127.0.0.1:1985/api/v1/raw?rpc=update&scope=ff_log_dir&value=./objs'` |
| config | `allow_update on;`|
| params | `scope=ff_log_dir&value=./objs`，指定新的全局ff_log_dir|
| require | ff_log_dir必须以./, /tmp/或/var/开头。譬如：./objs |

| Key | DESC | 
| ---- | ---- |
| feature | 设置SRS的日志容器|
| url  | `/api/v1/raw?rpc=update&scope=srs_log_tank&value=file` |
| curl | `curl 'http://127.0.0.1:1985/api/v1/raw?rpc=update&scope=srs_log_tank&value=file'` |
| config | `allow_update on;`|
| params | `scope=srs_log_tank&value=file`，设置新的日志容器|
| require | srs_log_tank必须是file或console。譬如：file |

| Key | DESC | 
| ---- | ---- |
| feature | 设置SRS的日志级别|
| url  | `/api/v1/raw?rpc=update&scope=srs_log_level&value=trace` |
| curl | `curl 'http://127.0.0.1:1985/api/v1/raw?rpc=update&scope=srs_log_level&value=trace'` |
| config | `allow_update on;`|
| params | `scope=srs_log_level&value=trace`，设置新的日志级别|
| require | srs_log_level必须是verbose,info,trace,warn,error。譬如：trace |

| Key | DESC | 
| ---- | ---- |
| feature | 设置SRS的日志文件路径|
| url  | `/api/v1/raw?rpc=update&scope=srs_log_file&value=./objs/srs.log` |
| curl | `curl 'http://127.0.0.1:1985/api/v1/raw?rpc=update&scope=srs_log_file&value=./objs/srs.log'` |
| config | `allow_update on;`|
| params | `scope=srs_log_file&value=./objs/srs.log`，设置新的日志文件路径|
| require | srs_log_file必须是.log类型，并且在./, /var/, /tmp/开头。譬如：./objs/srs.log |

| Key | DESC | 
| ---- | ---- |
| feature | 设置SRS能服务的最大连接数，包含RTMP和HTTP连接|
| url  | `/api/v1/raw?rpc=update&scope=max_connections&value=1000` |
| curl | `curl 'http://127.0.0.1:1985/api/v1/raw?rpc=update&scope=max_connections&value=1000'` |
| config | `allow_update on;`|
| params | `scope=max_connections&value=1000`，设置新的最大连接数|
| require | max_connections必须是整型，并且在[10, 65535]范围内。譬如：1000 |

| Key | DESC | 
| ---- | ---- |
| feature | 是否开启utc时间，影响日志和包含时间的路径，譬如DVR和HLS|
| url  | `/api/v1/raw?rpc=update&scope=utc_time&value=false` |
| curl | `curl 'http://127.0.0.1:1985/api/v1/raw?rpc=update&scope=utc_time&value=false'` |
| config | `allow_update on;`|
| params | `scope=utc_time&value=false`，设置是否开启utc时间|
| require | utc_time是bool，必须是true或false。譬如：false |

| Key | DESC | 
| ---- | ---- |
| feature | 设置全局的pithy打印时间间隔|
| url  | `/api/v1/raw?rpc=update&scope=pithy_print_ms&value=10000` |
| curl | `curl 'http://127.0.0.1:1985/api/v1/raw?rpc=update&scope=pithy_print_ms&value=10000'` |
| config | `allow_update on;`|
| params | `scope=pithy_print_ms&value=10000`，设置新的pithy打印时间间隔|
| require | pithy_print_ms单位是毫秒，在[100,300000]范围内。譬如：10000 |

## RAW Vhost

Vhost操作是Update的一个子集。

| Key | DESC | 
| ---- | ---- |
| feature | 新增vhost，指定vhost名称|
| url  | `/api/v1/raw?rpc=update&scope=vhost&value=ossrs.net&param=create` |
| curl | `curl 'http://127.0.0.1:1985/api/v1/raw?rpc=update&scope=vhost&value=ossrs.net&param=create'` |
| config | `allow_update on;`|
| params | `scope=vhost&value=ossrs.net&param=create`，创建新的vhost|
| require | 新创建的vhost必须是不存在的。注意禁用的vhost也算是存在的vhost。|

| Key | DESC | 
| ---- | ---- |
| feature | 修改禁用的vhost名称|
| url  | `/api/v1/raw?rpc=update&scope=vhost&value=ossrs.net&param=update&data=new.ossrs.net` |
| curl | `curl 'http://127.0.0.1:1985/api/v1/raw?rpc=update&scope=vhost&value=ossrs.net&param=update&data=new.ossrs.net'` |
| config | `allow_update on;`|
| params | `scope=vhost&value=ossrs.net&param=update&data=new.ossrs.net`，修改vhost名称为新名称|
| require | 需要修改的vhost必须存在，并且是禁用状态。|

| Key | DESC | 
| ---- | ---- |
| feature | 禁用vhost|
| url  | `/api/v1/raw?rpc=update&scope=vhost&value=ossrs.net&param=disable` |
| curl | `curl 'http://127.0.0.1:1985/api/v1/raw?rpc=update&scope=vhost&value=ossrs.net&param=disable'` |
| config | `allow_update on;`|
| params | `scope=vhost&value=ossrs.net&param=disable`，禁用vhost |
| require | 要禁用的vhost必须存在并且是启用状态。 |

| Key | DESC | 
| ---- | ---- |
| feature | 启用vhost|
| url  | `/api/v1/raw?rpc=update&scope=vhost&value=ossrs.net&param=enable` |
| curl | `curl 'http://127.0.0.1:1985/api/v1/raw?rpc=update&scope=vhost&value=ossrs.net&param=enable'` |
| config | `allow_update on;`|
| params | `scope=vhost&value=ossrs.net&param=enable`，启用vhost|
| require | 要启用的vhost必须存在并且是禁用状态。 |

## RAW DVR

DVR操作是Update的一个子集。

| Key | DESC | 
| ---- | ---- |
| feature | 开启Vhost的某个流的DVR|
| url  | `/api/v1/raw?rpc=update&scope=dvr&value=ossrs.net&param=enable&data=live/livestream` |
| curl | `curl 'http://127.0.0.1:1985/api/v1/raw?rpc=update&scope=dvr&value=ossrs.net&param=enable&data=live/livestream'` |
| config | `allow_update on;`|
| params | `scope=dvr&value=ossrs.net&param=enable&data=live/livestream`，对Vhost的Stream开启DVR|
| require | 必须Vhost的DVR是开启状态。|

| Key | DESC | 
| ---- | ---- |
| feature | 关闭Vhost的某个流的DVR|
| url  | `/api/v1/raw?rpc=update&scope=dvr&value=ossrs.net&param=disable&data=live/livestream` |
| curl | `curl 'http://127.0.0.1:1985/api/v1/raw?rpc=update&scope=dvr&value=ossrs.net&param=disable&data=live/livestream'` |
| config | `allow_update on;`|
| params | `scope=dvr&value=ossrs.net&param=disable&data=live/livestream`，对Vhost的Stream关闭DVR|
| require | 必须Vhost的DVR是开启状态。|

Winlin 2015.8

[HttpRawAPI]: https://github.com/simple-rtmp-server/srs/issues/319
[raw-raw]: https://github.com/simple-rtmp-server/srs/wiki/v3_CN_HTTPApi#raw
[raw-reload]: https://github.com/simple-rtmp-server/srs/wiki/v3_CN_HTTPApi#reload
[raw-query]: https://github.com/simple-rtmp-server/srs/wiki/v3_CN_HTTPApi#raw-query
[raw-update]: https://github.com/simple-rtmp-server/srs/wiki/v3_CN_HTTPApi#raw-update
[raw-vhost]: https://github.com/simple-rtmp-server/srs/wiki/v3_CN_HTTPApi#raw-vhost
[raw-dvr]: https://github.com/simple-rtmp-server/srs/wiki/v3_CN_HTTPApi#raw-dvr
