# DVR录制

SRS支持将RTMP流录制成flv文件。

## Build

DVR的编译选项为`--with-dvr`，关闭DVR的选项为`--without-dvr`。

参考：[Build](v1_CN_Build)

## Config

DVR的难点在于写入flv和文件命名，SRS的做法是随机生成文件名，用户可以使用http-callback方式，使用外部程序记录这个文件名，或者改成自己要的文件命名方式。

当然也可以修改SRS代码，这种做法不推荐，c操作文件名比较麻烦。还是用外部辅助系统做会很方便。

DVR的配置文件说明：

```bash
    # dvr RTMP stream to file,
    # start to record to file when encoder publish,
    # reap flv according by specified dvr_plan.
    dvr {
        # whether enabled dvr features
        # default: off
        enabled         on;
        # the dvr output path.
        # the app dir is auto created under the dvr_path.
        # for example, for rtmp stream:
        #   rtmp://127.0.0.1/live/livestream
        #   http://127.0.0.1/live/livestream.m3u8
        # where dvr_path is /dvr, srs will create the following files:
        #   /dvr/live       the app dir for all streams.
        #   /dvr/live/livestream.{time}.flv   the dvr flv file.
        # @remark, the time use system timestamp in ms, user can use http callback to rename it.
        # in a word, the dvr_path is for vhost.
        # default: ./objs/nginx/html
        dvr_path        ./objs/nginx/html;
        # the dvr plan. canbe:
        #   session reap flv when session end(unpublish).
        #   segment reap flv when flv duration exceed the specified dvr_duration.
        # default: session
        dvr_plan        session;
        # the param for plan(segment), in seconds.
        # default: 30
        dvr_duration    30;
        # the param for plan(segment),
        # whether wait keyframe to reap segment,
        # if off, reap segment when duration exceed the dvr_duration,
        # if on, reap segment when duration exceed and got keyframe.
        # default: on
        dvr_wait_keyframe       on;
        # about the stream monotonically increasing:
        #   1. video timestamp is monotonically increasing, 
        #   2. audio timestamp is monotonically increasing,
        #   3. video and audio timestamp is interleaved monotonically increasing.
        # it's specified by RTMP specification, @see 3. Byte Order, Alignment, and Time Format
        # however, some encoder cannot provides this feature, please set this to off to ignore time jitter.
        # the time jitter algorithm:
        #   1. full, to ensure stream start at zero, and ensure stream monotonically increasing.
        #   2. zero, only ensure sttream start at zero, ignore timestamp jitter.
        #   3. off, disable the time jitter algorithm, like atc.
        # default: full
        time_jitter             full;
    }
```

DVR的计划即决定什么时候关闭flv文件，打开新的flv文件，主要的录制计划包括：

* session：按照session来关闭flv文件，即编码器停止推流时关闭flv，整个session录制为一个flv。
* segment：按照时间分段录制，flv文件时长配置为dvr_duration和dvr_wait_keyframe。注意：若不按关键帧切flv（即dvr_wait_keyframe配置为off），所以会导致后面的flv启动时会花屏。
* time_jitter: 时间戳抖动算法。full使用完全的时间戳矫正；zero只是保证从0开始；off不矫正时间戳。

参考`conf/dvr.segment.conf`和`conf/dvr.session.conf`配置实例。

## Reload

改变dvr配置后reload，会导致dvr重启，即关闭当前dvr文件后重新应用dvr配置。

Winlin 2014.4
