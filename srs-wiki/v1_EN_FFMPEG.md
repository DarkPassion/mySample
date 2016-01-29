# Live Streaming Transcode

SRS can transcode the RTMP stream and output to any RTMP server, typically itself.

## Use Scenario

The important use scenaio of FFMPEG:
* One in N out: Publish a high resolution video with big bitrate, for intance, h.264 5Mbps 1080p. Then use FFMPEG to transcode to multiple bitrates, for example, 1080p/720p/576p, the 576p is for mobile devices.
* Support multiple screen: The stream published by flash is in h264/vp6/mp3/speex codec. Use FFMPEG to transcode to HLS(h264+aac) for IOS/Android.
* Stream filters: For example, add logo to stream. SRS support all filters of FFMPEG.

## Workflow

The workflow of SRS transcoding:

1. Encoder publish RTMP to SRS.
1. SRS fork process for FFMPEG when transcode configed.
1. The forked FFMPEG transcode stream and publish to SRS or other servers.

## Transcode Config

The SRS transcoding feature can apply on vhost, app or specified stream.

```bash
listen              1935;
vhost __defaultVhost__ {
    # apply on vhost, for all streams.
    transcode {
        enabled     on;
        ffmpeg      ./objs/ffmpeg/bin/ffmpeg;
        engine ff {
            # whether the engine is enabled
            # default: off.
            enabled         on;
            # ffmpeg filters, follows the main input.
            vfilter {
            }
            # video encoder name. can be:
            # libx264: use h.264(libx264) video encoder.
            # copy: donot encoder the video stream, copy it.
            # vn: disable video output.
            vcodec          libx264;
            # video bitrate, in kbps
            vbitrate        1500;
            # video framerate.
            vfps            25;
            # video width, must be even numbers.
            vwidth          768;
            # video height, must be even numbers.
            vheight         320;
            # the max threads for ffmpeg to used.
            vthreads        12;
            # x264 profile, @see x264 -help, can be:
            # high,main,baseline
            vprofile        main;
            # x264 preset, @see x264 -help, can be: 
            # ultrafast,superfast,veryfast,faster,fast
            # medium,slow,slower,veryslow,placebo
            vpreset         medium;
            # other x264 or ffmpeg video params
            vparams {
            }
            # audio encoder name. can be:
            # libaacplus: use aac(libaacplus) audio encoder.
            # copy: donot encoder the audio stream, copy it.
            # an: disable audio output.
            acodec          libaacplus;
            # audio bitrate, in kbps. [16, 72] for libaacplus.
            abitrate        70;
            # audio sample rate. for flv/rtmp, it must be:
            # 44100,22050,11025,5512
            asample_rate    44100;
            # audio channel, 1 for mono, 2 for stereo.
            achannels       2;
            # other ffmpeg audio params
            aparams {
            }
            # output stream. variables:
            # [vhost] the input stream vhost.
            # [port] the intput stream port.
            # [app] the input stream app.
            # [stream] the input stream name.
            # [engine] the tanscode engine name.
            output          rtmp://127.0.0.1:[port]/[app]?vhost=[vhost]/[stream]_[engine];
        }
    }
}
```

The config apply to all streams of this vhost, for example:
* Publish stream to: rtmp://dev:1935/live/livestream
* Play the origin stream: rtmp://dev:1935/live/livestream
* Play the transcoded stream: rtmp://dev:1935/live/livestream_ff

The output url contains some variables:
* [vhost] The input stream vhost, for instance, dev.ossrs.net
* [port] The input stream port, for instance, 1935
* [app] The input stream app, for instance, live
* [stream] The intput stream name, for instance, livestream
* [engine] The transcode engine name, which follow the keyword engine, for instance, ff

Add the app or app/stream when need to apply transcode to app or stream:

```bash
listen              1935;
vhost __defaultVhost__ {
    # Transcode all streams of app "live"
    transcode live {
    }
}
```

Or for stream:

```bash
listen              1935;
vhost __defaultVhost__ {
    # Transcode stream name "livestream" and app is "live"
    transcode live/livestream{
    }
}
```

## Transcode Rulers

All params of SRS transcode is for FFMPEG, and SRS rename some parameters:

| SRS | FFMPEG | Exammple | Description |
| ---- | ------ | ------- | ------------ |
| vcodec | vcodec | ffmpeg ... -vcodec libx264 ... | The codec to use.|vbitrate | b:v | ffmpeg ... -b:v 500000 ... | The bitrate in kbps(for SRS) or bps(for FFMPEG) to output transcode stream.|
| vfps | r | ffmpeg ... -r 25 ... | The output framerate.|
| vwidth/vheight | s | ffmpeg ... -s 400x300 -aspect 400:300 ... | The output video size, the width x height and the aspect set to width:height.|
| vthreads | threads | ffmpeg ... -threads 8 ... | The encode thread for x264.|
| vprofile | profile:v | ffmpeg ... -profile:v high ... | The profile for x264.|
| vpreset | preset | ffmpeg ... -preset medium ... | The preset for x264.|
| acodec | acodec | ffmpeg ... -acodec libaacplus ... | The codec for audio.|
| abitrate | b:a | ffmpeg ... -b:a 70000 ... | The bitrate in kbps(for SRS) and bps(for FFMPEG) for output audio. For libaacplus：16-72k|
| asample_rate | ar | ffmpeg ... -ar 44100 ... | The audio sample rate.|
| achannels | ac | ffmpeg ... -ac 2 ... | THe audio channel. |

There are more parameter of SRS:
* vfilter：Parameters added before the vcodec, for the FFMPEG filters.
* vparams：Parameters added after the vcodec, for the video transcode parameters.
* aparams：Parameters added after the acodec and before the -y, for the audio transcode parameters.

These parameters will generate by the sequence:

```bash
ffmpeg -f flv -i <input_rtmp> {vfilter} -vcodec ... {vparams} -acodec ... {aparams} -f flv -y {output}
```

The actual parameters to fork FFMPEG can find the log by keyword `start transcoder`:

```bash
[2014-02-28 21:38:09.603][4][trace][start] start transcoder, 
log: ./objs/logs/encoder-__defaultVhost__-live-livestream.log, 
params: ./objs/ffmpeg/bin/ffmpeg -f flv -i 
rtmp://127.0.0.1:1935/live?vhost=__defaultVhost__/livestream 
-vcodec libx264 -b:v 500000 -r 25.00 -s 768x320 -aspect 768:320 
-threads 12 -profile:v main -preset medium -acodec libaacplus 
-b:a 70000 -ar 44100 -ac 2 -f flv 
-y rtmp://127.0.0.1:1935/live?vhost=__defaultVhost__/livestream_ff 
```

## FFMPEG Log Path

When FFMPEG process forked, SRS will redirect the stdout and stderr to the log file, for instance, `./objs/logs/encoder-__defaultVhost__-live-livestream.log`, sometimes the log file is very large. User can add parameter to vfilter to tell FFMPEG to generate less log:

```bash
listen              1935;
vhost __defaultVhost__ {
    transcode {
        enabled     on;
        ffmpeg      ./objs/ffmpeg/bin/ffmpeg;
        engine ff {
            enabled         on;
            vfilter {
                # -v quiet
                v           quiet;
            }
            vcodec          libx264;
            vbitrate        500;
            vfps            25;
            vwidth          768;
            vheight         320;
            vthreads        12;
            vprofile        main;
            vpreset         medium;
            vparams {
            }
            acodec          libaacplus;
            abitrate        70;
            asample_rate    44100;
            achannels       2;
            aparams {
            }
            output          rtmp://127.0.0.1:[port]/[app]?vhost=[vhost]/[stream]_[engine];
        }
    }
}
```

That is, add parameter `-v quiet` to FFMPEG.

## Copy Without Transcode

Set the vcodec/acodec to copy, FFMPEG will demux and mux without transcode, like the forward of SRS. User can copy video and transcode audio, for example, when the flash publish stream with h264+speex.

```bash
listen              1935;
vhost __defaultVhost__ {
    transcode {
        enabled     on;
        ffmpeg      ./objs/ffmpeg/bin/ffmpeg;
        engine ff {
            enabled         on;
            vcodec          copy;
            acodec          libaacplus;
            abitrate        70;
            asample_rate    44100;
            achannels       2;
            aparams {
            }
            output          rtmp://127.0.0.1:[port]/[app]?vhost=[vhost]/[stream]_[engine];
        }
    }
}
```

Or, copy video and audio:
```bash
listen              1935;
vhost __defaultVhost__ {
    transcode {
        enabled     on;
        ffmpeg      ./objs/ffmpeg/bin/ffmpeg;
        engine ff {
            enabled         on;
            vcodec          copy;
            acodec          copy;
            output          rtmp://127.0.0.1:[port]/[app]?vhost=[vhost]/[stream]_[engine];
        }
    }
}
```

## Drop Video or Audio

FFMPEG can drop video or audio stream by config the vcodec to vn and acodec to an. For example:

```bash
listen              1935;
vhost __defaultVhost__ {
    transcode {
        enabled     on;
        ffmpeg      ./objs/ffmpeg/bin/ffmpeg;
        engine vn {
            enabled         on;
            vcodec          vn;
            acodec          libaacplus;
            abitrate        45;
            asample_rate    44100;
            achannels       2;
            aparams {
            }
            output          rtmp://127.0.0.1:[port]/[app]?vhost=[vhost]/[stream]_[engine];
        }
    }
}
```

The config above will output pure audio in aac codec.

## Other Transcode Config

There are lots of vhost in conf/full.conf for transcode, or refer to the FFMPEG:
* mirror.transcode.srs.com
* drawtext.transcode.srs.com 
* crop.transcode.srs.com
* logo.transcode.srs.com 
* audio.transcode.srs.com
* copy.transcode.srs.com
* all.transcode.srs.com
* ffempty.transcode.srs.com 
* app.transcode.srs.com 
* stream.transcode.srs.com 
* vn.transcode.srs.com

## Transcode on ARM cpu

SRS can fork the FFMEPG on ARM, see: [Raspberry pi Transcode](v1_EN_ARMTranscode)

Note: To use your own tool, you can disable the ffmpeg by `--without-ffmpeg`, but must open the transcode `--with-transcode`, see: [Build](v1_EN_Build)

## FFMPEG Transcode the Stream by Flash encoder

The flash web page can encode and publish RTMP stream to server, and the audio codec must be speex, nellymoser or pcma/pcmu.

The flash will disable audio when no audio when publish, so FFMPEG may cannot discovery the audio in stream and disable the audio.

## FFMPEG

FFMPEG links:
* [ffmpeg.org](http://ffmpeg.org)
* [ffmpeg CLI](http://ffmpeg.org/ffmpeg.html)
* [ffmpeg filters](http://ffmpeg.org/ffmpeg-filters.html)
* [ffmpeg codecs](http://ffmpeg.org/ffmpeg-codecs.html)

Winlin 2014.10
