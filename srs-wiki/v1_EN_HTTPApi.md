# HTTP API

SRS provides HTTP api, to external application to manage SRS, and support crossdomain for js.

The following is a SRS server api of ossrs.net:

![SRS](http://winlinvip.github.io/srs.release/wiki/images/demo.api.png?v1)

## Design Priciple

The HTTP API of SRS follows the simple priciple:

* Only provides API in json format, both request and json are json.
* No html, access the api return json format.

## Build

To enable http api, configure SRS with `--with-http-api`, 
read [configure](v1_EN_Build)

```bash
./configure --with-http-api && make
```

The config also need to enable it:

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

Start SRS: `./objs/srs -c http-api.conf`

Access api, open the url in web browser: [http://192.168.1.170:1985/api/v1](http://192.168.1.170:1985/api/v1)

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
{

    "code": 0,
    "urls": {
        "api": "the api root"
    }

}
```

The urls is the apis to access:

```bash
# curl http://192.168.1.102:1985/api/
{

    "code": 0,
    "urls": {
        "v1": "the api version 1.0"
    }

}
```

Go on:

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

Go on:

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

Or:

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

The Api of SRS is self-describes api.

## Error Code

When error for http server, maybe return an error page, SRS always return the json result.

For example, the 404 `not found` api `http://192.168.1.102:1985/apis`:

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

While the http header is 404:

```bash
HTTP/1.1 404 Not Found
Server: SRS/0.9.43
Content-Type: application/json;charset=utf-8
Allow: DELETE, GET, HEAD, OPTIONS, POST, PUT
Content-Length: 81
```

Not all HTTP protocol is supported by SRS.

Winlin 2014.11
