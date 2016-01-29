# SRS Arch

SRS最关键是Simple，最简单的方案就是最佳方案；这个文章记录了SRS关键的Simple方案，也就是50%代码完成200%功能，100%代码完成400%功能的要点。

## State Threads

ST带来的问题简化，在一个状态空间时至少一个数量级；多个状态空间时就是百个数量级，譬如edge回源，http-flv和hstrs。在网络服务器中st的思路是与众不同，也是很巧妙的思路。

SRS是单进程使用epoll进行异步socket操作的高性能服务器，架构和nginx同源（同为非阻塞、异步、单线程），除了nginx是多进程SRS是单进程之外。

SRS没有直接使用epoll进行状态处理，而是使用了协程库state-threads，简称st（详细介绍可以看文章：[高性能、高并发、高扩展性和可读性的网络服务器架构：StateThreads](http://blog.csdn.net/win_lin/article/details/8242653)）。

关于setjmp和longjmp，以及为何st必须自己分配stack，参考[st(state-threads) coroutine和setjmp/longjmp的关系](http://blog.csdn.net/win_lin/article/details/40948277)

关于st如何分配栈，以及进行协程切换，以及协程的生命周期，参考：[st(state-threads)线程和栈分析](http://blog.csdn.net/win_lin/article/details/40978665)

ST参考：[ST](https://github.com/winlinvip/state-threads)

## HLS Fault Tolerance Backup

关于HLS热备，在流媒体中是比较难以处理的一个问题，具体可以参考[hls][hls]。所谓HLS热备，是指编码器（它自己可以热备）输出两路相同的RTMP流给两个不同地方的机房的流媒体服务器，然后这两个服务器生成的切片一样，这样任何一个机房宕机都不会影响hls流的生成。

比较典型的做法，是利用ATC时间戳，然后按照指定的规则生成切片。这样两个服务器输出的切片完全一样。

悟空提出了更好的方案，即使用standby方案替代完全热备：

1. 编码器输出相同的流给异地机房的两个流媒体服务器，两个服务器之间保持通信。
1. 两个服务器一个时刻只有一个输出HLS切片，另外一个standby。
1. 当另外一个服务器挂掉时，standby的立刻开始切片。
1. 两个服务器的切片都输出到一个存储。

这个方案确实很巧妙，使用另外不同的思路解决切片同步的问题。虽然在服务器切换时会有点切片间隙或不同步，但实际上并不会有大的影响。这点点瑕疵，至少简化了几个数量级的难度，是我见过所有HLS热备中最牛逼的一个方案~

这个方案可以加以改进，不依赖存储，不需要服务器做同步，更加简单：

1. 编码器输出同样的流给服务器，两台服务器同时切片，并调用http回调。
1. 用GO写一个web服务器，接收回调，并且这两个go服务器之间互相通信。
1. 两个go服务器保持切片同步到两个目录，并且选举出主备服务器和目录。
1. 只有两个go服务器使用主服务器的切片提供服务，即两个go都使用主或备目录服务。

## Multiple Processes PlanA

SRS支持多进程吗？不支持。SRS能支持多进程吗？可以的。简单讲有几个因素不支持多进程：

1. 单进程9k并发够用了。
1. 多进程比单进程还是麻烦点。
1. RTMP302会更方便，只是需要播放器支持。

在SRS1时，杰哥就提出过多进程的方案，这个方案比任何流媒体服务器多进程方案都要牛逼！简单讲，就是master进程作为源站origin服务器，然后fork多个边缘edge服务器，这些edge服务器通过lo网卡回本地的origin。这个方案牛逼的地方包括：

1. 没有引入新的角色，只是fork进程，使用SRS本来的origin-edge结构合并回源。
1. 不transfer fd，也就是edge直接处理连接，不用在进程间调度。

如果你觉得9k的并发还不够，建议自己使用杰哥提供的这个多进程方案。

## Multiple Processes PlanB

2015年5月国外的一个朋友提到，为何不直接做TCP级别的反向代理，实现SRS的多进程呢？这样SRS不用改，多进程也非常简单。

也就是说，可以单独开启一个项目[srs-dolphin][srs-dolphin]，类似于[go-sharp][go-sharp]（HTTP级别的FLV代理），但是和sharp不一样的是dolphin是TCP代理，dolphin的主要流程是：

1. dolphin配置时指定侦听的端口，以及开启的核心数，譬如4核心侦听1935。dolphin会给每个核自动分配端口，譬如1936-1939。
1. dolphin启动后，绑定指定的服务端口譬如1935，然后每个核启动一个进程共享这个服务端口。
1. dolphin启动对应数目的SRS，譬如4核心就启动4个SRS，然后侦听对应的分配的端口，譬如1936-1939。
1. dolphin多个子进程接收到客户端连接后，进行策略选择一个SRS，进行TCP代理。这样RTMP和HTTP-FLV都能支持。

这样，dolphin就形成了以下的结构，以侦听1935为例，开启4进程：

dolphin(master)   => 4 X dolphin(proxer)  => 4 X SRS

很显然RTMP和HTTP-FLV边缘适合这种结构做多进程，源站需要保证流的唯一性，需要做hash之类，也可以基于这个结构做。

这个结构最大的优势，在于SRS不需要做结构的变动，启动时也只是换成了srs-dolphin启动，其他的都不变。

这个结构，和杰哥的PlanA结构的区别在于，dolphin没有开启源站合并回源，所以多核会增加回源带宽。不过可以通过适当的调度策略，跑满了一个进程再跑下一个，缓解这个问题。

Winlin 2015.5

[hls]: https://github.com/simple-rtmp-server/srs/issues/351
[srs-dolphin]: https://github.com/simple-rtmp-server/srs-dolphin
[go-sharp]: https://github.com/simple-rtmp-server/go-sharp
