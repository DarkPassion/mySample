# HTTP接口

SRS提供HTTP接口，供外部程序管理服务器，并支持跨域（js可以直接控制和获取服务器的各种信息）。

下面是ossrs.net一台演示SRS的二维码，使用微信扫描即可看到这台服务器的版本
（可以做更高级的功能，扫描即可发现bug，扫描即可管理，扫描即可重启，等等）：

![SRS](http://winlinvip.github.io/srs.release/wiki/images/demo.api.png?v1)

## 设计原则

SRS的HTTP接口遵循最简单原则，主要包括：
* 只提供json数据格式接口，要求请求和响应的数据全都是json。
* 不提供html数据，譬如运行SRS后，浏览器打开HTTP接口或HTTP服务地址，看到的是json，不是html。

## 编译和启动

SRS需要打开HTTPApi选项，参考：[configure选项](v1_CN_Build)

```bash
./configure --with-http-api && make
```

配置文件需要开启http-api：

```bash
# http-api.conf
listen              1935;
http_api {
    enabled         on;
    listen          1985;
}
vhost __defaultVhost__ {
}
```

启动服务器：`./objs/srs -c http-api.conf`

访问api：浏览器打开地址[http://192.168.1.170:1985/api/v1](http://192.168.1.170:1985/api/v1)

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
# curl http://192.168.1.102:1985/
{

    "code": 0,
    "urls": {
        "api": "the api root"
    }

}
```

返回的urls表示子链接可以访问。接着访问：

```bash
# curl http://192.168.1.102:1985/api/
{

    "code": 0,
    "urls": {
        "v1": "the api version 1.0"
    }

}
```

继续：

```bash
# curl http://192.168.1.102:1985/api/v1/
{

    "code": 0,
    "urls": {
        "versions": "the version of SRS",
        "authors": "the primary authors and contributors"
    }

}
```

继续：

```bash
# curl http://192.168.1.102:1985/api/v1/versions
{

    "code": 0,
    "data": {
        "major": 0,
        "minor": 9,
        "revision": 43,
        "version": "0.9.43"
    }

}
```

或者：

```bash
# curl http://192.168.1.102:1985/api/v1/authors
{

    "code": 0,
    "data": {
        "primary_authors": "winlin,wenjie.zhao",
        "contributors_link": "https://github.com/simple-rtmp-server/srs/blob/master/AUTHORS.txt",
        "contributors": "winlin<winlin@vip.126.com> wenjie.zhao<740936897@qq.com> xiangcheng.liu<liuxc0116@foxmail.com> naijia.liu<youngcow@youngcow.net> alcoholyi<alcoholyi@qq.com> "
    }

}
```

SRS的API属于“自解释型，HTTP RESTful API”

## 错误码

当HTTP错误时，譬如404，默认的HTTP服务器会返回错误页面，SRS返回的永远是程序能解析的json。

譬如，浏览器打开地址`http://192.168.1.102:1985/apis`，api多写了个s，是404，服务器返回：

```bash
{

    "code": 804,
    "data": {
        "status_code": 404,
        "reason_phrase": "Not Found",
        "url": "/apis"
    }

}
```

查看HTTP的响应头为：

```bash
HTTP/1.1 404 Not Found
Server: SRS/0.9.43
Content-Type: application/json;charset=utf-8
Allow: DELETE, GET, HEAD, OPTIONS, POST, PUT
Content-Length: 81
```

SRS提供HTTP服务的基本原则是支持少量的HTTP协议，并且只提供给程序读的信息。尽量保证提供的信息都是可读的json，除非连不上服务器，或者服务器崩溃，否则数据都是json。

Winlin 2014.4
