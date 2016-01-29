# gprof性能分析

SRS支持gprof性能分析。

## SRS性能分析

SRS使用gprof分析的步骤如下：
* 配置：`./configure --with-gprof`
* 编译：`make`
* 直接启动即可：`rm -f gmon.out; ./objs/srs -c conf/console.conf`
* 退出SRS：`killall -2 srs # or CTRL+C to stop gprof`
* 生成gprof报告：`gprof -b ./objs/srs gmon.out > gprof.srs.log && rm -f gmon.out`

可以打开`gprof.srs.log`查看性能报告，譬如([raspberry-pi下30个连接的性能数据](v1_CN_RaspberryPi)）：
```bash
Each sample counts as 0.01 seconds.
  %   cumulative   self              self     total           
 time   seconds   seconds    calls   s/call   s/call  name    
 49.99     39.28    39.28                             writev
  5.42     43.54     4.26                             epoll_ctl
  3.16     46.02     2.48                             read
  2.41     47.91     1.89   265797     0.00     0.00  SrsProtocol::send_message(ISrsMessage*)
  1.93     49.43     1.52                             __gnu_mcount_nc
  1.82     50.86     1.43                             epoll_wait
  1.67     52.17     1.31       33     0.04     0.42  SrsRtmpConn::playing(SrsSource*)
  1.49     53.34     1.17                             heap_insert
  1.37     54.42     1.08                             heap_delete
  1.31     55.45     1.03                             _st_epoll_dispatch
  0.97     56.21     0.76                             st_writev
  0.84     56.87     0.66                             _st_epoll_pollset_del
  0.71     57.43     0.56   295144     0.00     0.00  SrsProtocol::recv_interlaced_message
```

注意：性能分析在不同的CPU压力下，不同的流条件，结果都不一样。所以需要在和实际应用较为接近的流码率，客户端连接数，机器型号和配置等等，环境一致的数据给出的报告，才能给改进有参考价值；否则可能改进后发现根本没有用（没有跑那个分支）。

## 图形化

gprof还能图形化，可以将结果绘制成调用图：
* 编译SRS：参考上一节`SRS性能分析`。
* 编译图形化工具：`(cd 3rdparty/gprof && bash build_gprof2dot.sh)`
* 启动服务器（按CTRL+C或者killall -2 srs生成数据）：`./objs/srs -c conf/console.conf`
* 生成gprof数据：`gprof -b ./objs/srs gmon.out > gprof.srs.log`
* 将报表转换为图片：`./3rdparty/gprof/gprof2dot.py gprof.srs.log | dot -Tpng -o winlin.png`

图片中一眼就能看出性能有问题的函数([raspberry-pi下30个连接的性能数据](v1_CN_RaspberryPi)），参考下图：
![SRS-gprof性能分析图片](http://winlinvip.github.io/srs.release/wiki/images/gprof-raspberry-pi.png)

备注：有时候dot会占用N多内存，而且还出不了图片，这个应该是dot的bug。这时候赶快`killall -9 dot`，不要等了。有可能是因为性能数据太少，一般让SRS的CPU跑高点（5%以上），gprof的数据较多时，不会出现这种情况；若出现了，就重新gprof采样就好了。

Winlin 2014.3
