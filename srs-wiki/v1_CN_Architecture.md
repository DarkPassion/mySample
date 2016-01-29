# SRS Arch

SRS最关键是Simple，最简单的方案就是最佳方案；这个文章记录了SRS关键的Simple方案，也就是50%代码完成200%功能，100%代码完成400%功能的要点。

## State Threads

ST带来的问题简化，在一个状态空间时至少一个数量级；多个状态空间时就是百个数量级，譬如edge回源，http-flv和hstrs。在网络服务器中st的思路是与众不同，也是很巧妙的思路。

SRS是单进程使用epoll进行异步socket操作的高性能服务器，架构和nginx同源（同为非阻塞、异步、单线程），除了nginx是多进程SRS是单进程之外。

SRS没有直接使用epoll进行状态处理，而是使用了协程库state-threads，简称st（详细介绍可以看文章：[高性能、高并发、高扩展性和可读性的网络服务器架构：StateThreads](http://blog.csdn.net/win_lin/article/details/8242653)）。

关于setjmp和longjmp，以及为何st必须自己分配stack，参考[st(state-threads) coroutine和setjmp/longjmp的关系](http://blog.csdn.net/win_lin/article/details/40948277)

关于st如何分配栈，以及进行协程切换，以及协程的生命周期，参考：[st(state-threads)线程和栈分析](http://blog.csdn.net/win_lin/article/details/40978665)

ST参考：[ST](https://github.com/winlinvip/state-threads)

Winlin 2014.2

[hls]: https://github.com/simple-rtmp-server/srs/issues/351
