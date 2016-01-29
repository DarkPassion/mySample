# DVR

SRS supports DVR RTMP stream to flv file.

## Build

Use `--with-dvr` to enable dvr, while `--without-dvr` to disable it.

For information about the dvr option, read 
[Build](v1_EN_Build)

## Config

The difficult of DVR is about the flv name, while SRS use app/stream+random name.
User can use http-callback to rename, for example, when DVR reap flv file.

Config for DVR:

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

The plan of DVR used to reap flv file:

* session: When start publish, open flv file, close file when unpublish.
* segment: Reap flv file by the dvr_duration and dvr_wait_keyframe.
* time_jitter: The time jitter algorithm to use.

The config file can also use `conf/dvr.segment.conf` or `conf/dvr.session.conf`.

## Reload

The changing of dvr and reload will restart the dvr, that is, to close current dvr file then apply new config.

Winlin 2014.11
