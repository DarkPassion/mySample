# SRS Architecture

SRS is a high performance server, single thread, async and none-blocking using epoll. Similar to NGINX, but without multiple process right now.

SRS use coroutine library state-threads, which use select, poll or epoll. ST(state-threads) is a server arch for internet application, see: [StateThreads](http://blog.csdn.net/win_lin/article/details/8242653).

The information about setjmp/longjmp, and why st must alloc stack for each thread, read [st(state-threads) coroutine and setjmp/longjmp](http://blog.csdn.net/win_lin/article/details/40948277)

The information about stack of st, and how to switch between coroutines, and the cycle of coroutine, read [st(state-threads) stack and coroutine cycle](http://blog.csdn.net/win_lin/article/details/40978665)

ST mirror, see: [ST](https://github.com/winlinvip/state-threads)

BTW: SRS multiple process arch is in plan, maybe support in 2015.

Winlin 2014.10
