# 心跳

嵌入式设备上，只能运行SRS时，其他的业务系统可能需要知道这个设备的ip等信息，SRS可以以http方式主动汇报给api服务器。

## 编译

要求编译时支持http-parser，即开启了下面任何一个选项即支持：
* --with-http-api HTTP接口。
* --with-http-server HTTP服务器。
* --with-http-callback HTTP回调。

## 配置

在全局配置以下信息即可以开启心跳：
```bash
# heartbeat to api server
# @remark, the ip report to server, is retrieve from system stat,
#       which need the config item stats.network_device_index.
heartbeat {
    # whether heartbeat is enalbed.
    # default: off
    enabled         on;
    # the interval seconds for heartbeat,
    # recommend 0.3,0.6,0.9,1.2,1.5,1.8,2.1,2.4,2.7,3,...,6,9,12,....
    # default: 9.9
    interval        9.3;
    # when startup, srs will heartbeat to this api.
    # @remark: must be a restful http api url, where SRS will POST with following data:
    #   {
    #       "device_id": "my-srs-device",
    #       "ip": "192.168.1.100"
    #   }
    # default: http://127.0.0.1:8085/api/v1/servers
    url             http://127.0.0.1:8085/api/v1/servers;
    # the id of devide.
    device_id       "my-srs-device";
    # whether report with summaries
    # if true, put /api/v1/summaries to the request data:
    #   {
    #       "summaries": summaries object.
    #   }
    # @remark: optional config.
    # default: off
    summaries       off;
}
# system statistics section.
# the main cycle will retrieve the system stat,
# for example, the cpu/mem/network/disk-io data,
# the http api, for instance, /api/v1/summaries will show these data.
# @remark the heartbeat depends on the network_device_index,
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
```

注意：enabled默认是off的，必须打开才行。

注意：需要配置stats中的设备索引，获取正确的ip。

默认SRS会将设备内网IP(在stats中配置设备的索引)，设备名POST到url指定的地址。

Winlin 2014.5
