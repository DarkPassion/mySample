## Exec

## NGINX RTMP EXEC

NGINX-RTMP支持的EXEC方式，参考[nginx exec][ne]，SRS只支持常用的几种。下面是exec的支持情况：

1. exec/exec_publish: 当发布流时调用，支持。
1. exec_pull: 不支持。
1. exec_play: 不支持。
1. exec_record_done: 不支持。

## Config

SRS EXEC的配置参考`conf/exec.conf`，如下：

```
vhost __defaultVhost__ {
    # the exec used to fork process when got some event.
    exec {
        # whether enable the exec.
        # default: off.
        enabled     off;
        # when publish stream, exec the process with variables:
        #       [vhost] the input stream vhost.
        #       [port] the intput stream port.
        #       [app] the input stream app.
        #       [stream] the input stream name.
        #       [engine] the tanscode engine name.
        # other variables for exec only:
        #       [url] the rtmp url which trigger the publish.
        #       [tcUrl] the client request tcUrl.
        #       [swfUrl] the client request swfUrl.
        #       [pageUrl] the client request pageUrl.
        # @remark empty to ignore this exec.
        publish     ./objs/ffmpeg/bin/ffmpeg -f flv -i [url] -c copy -y ./[stream].flv;
    }
}
```

Winlin 2015.08

[ne]: https://github.com/arut/nginx-rtmp-module/wiki/Directives#exec
