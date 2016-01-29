# TimeJitter

This article describes the timestamp correct of SRS.

## RTMP Monotonically Increase Timestamp

RTMP requires the timestamp is mono-inc(monotonically increase). The mono-inc is the 
timestamp of packet is always larger.

RTMP requires the stream is mono-inc. The audio is mono-inc, 
video is mono-inc, and stream mixed audio with video is mono-inc. 

What happens when not mono-inc? Some server will disconnect connection, flash maybe
failed to play stream.

## Timestamp Jitter

SRS will ensure the stream timestamp is mono-inc. When delta of packets too large, set to 40ms(fps 25).

Some components use the timestamp jitter:
* RTMP delivery: The timestamp jitter algorithm can set by vhost `time_jitter`.
* DVR: The timestamp jitter algorithm can set by dvr `time_jitter`.
* HLS: Always disable the timestamp is mono-inc, use `off` timestamp jitter algorithm.
* Forward: Always disable the timestamp is mono-inc, use `off` timestamp jitter algorithm.

You can disable the timestamp jitter algorithm when your encoder can not ensure the 
video+autio mono-inc, some encoder can ensure video mono-inc and audio mono-inc.

## Config

Config the timestamp jitter in vhost for RTMP delivery:

```bash
vhost jitter.srs.com {
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

Config timestamp jitter for dvr:

```
vhost dvr.srs.com {
    # dvr RTMP stream to file,
    # start to record to file when encoder publish,
    # reap flv according by specified dvr_plan.
    # http callbacks:
    # @see http callback on_dvr_hss_reap_flv on http_hooks section.
    dvr {
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
}
```

## ATC

When [RTMP ATC](v1_EN_RTMP-ATC) is on,
RTMP always disable the time_jitter.

Winlin 2014.11
