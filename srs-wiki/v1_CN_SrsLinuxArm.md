# SRS for linux-arm

arm芯片上，如何使用SRS？一般arm上的硬件可以获取到h.264裸码流。有几个方案：
* arm推送RTMP到SRS：从arm上将h.264裸码流包装成flv流，使用[srs-librtmp](v1_CN_SrsLibrtmp)，或者librtmp，将flv格式的包以RTMP发送到SRS。
* arm推送h.264裸码流到SRS（目前还不支持）：可以使用自定义协议，可以不必使用RTMP这么复杂的协议，使用socket将h.264裸码流发送到SRS，SRS服务器端将裸码流打包成RTMP分发。
* arm上运行SRS：在arm上运行SRS，使用上面两个方案将h.264裸码流推送到arm上的SRS。客户端或者RTMP边缘直接从arm上的SRS源站取RTMP流。

## Why run SRS on ARM?

ARM跑SRS主要原因：
* arm设备，像摄像头，比较多，譬如一万个摄像头，如果有中心服务器，都往上面推，中心服务器就完蛋了。
* 用户看摄像头时，一般需要装插件，一般摄像头只能出rtmp。所以没法用浏览器flash直接看。所以arm上跑个srs，就可以让用户直接打开。
* arm上跑srs，每个摄像头都是服务器，相当于分布式服务器。不过都是源站。

SRS在ARM上主要是源站：
* 只需要st，ssl，提供基本的RTMP流源站即可。
* 不需要http-parser，nginx，ffmpeg，api-server，边缘，其他都不需要。
* 支持RTMP/HLS，RTMP需要ssl，HLS不需要额外的支持，只是切片成文件。

## Ubuntu Cross Build SRS

srs使用的默认gcc/g++编译出来的srs无法在arm下使用，必须使用arm编译器。

推荐在Ubuntu12下交叉编译，Ubuntu12对交叉编译支持得比较完善。CentOS6不太好使。

以Ubuntu12为例，arm编译器的安装：

```bash
sudo aptitude install -y gcc-arm-linux-gnueabi g++-arm-linux-gnueabi
```

编译工具对比：

| x86 | armhf(v7cpu) |
| --- | ------- |
| gcc | arm-linux-gnueabi-gcc |
| g++ | arm-linux-gnueabi-g++ |
| ar | arm-linux-gnueabi-ar |
| as | arm-linux-gnueabi-as |
| ld | arm-linux-gnueabi-ld |
| ranlib | arm-linux-gnueabi-ranlib |
| strip | arm-linux-gnueabi-strip | 

交叉编译SRS：

```bash
./configure --with-arm-ubuntu12 && make
```

其中，
* --with-arm-ubuntu12：必选，指定为arm编译。注意目前只支持ubuntu，CentOS的交叉环境不好搭。
* --with-ssl：默认开启，可关闭。支持复杂握手。参考：[握手协议](v1_CN_RTMPHandshake)
* --with-hls：默认开启，可关闭。支持将RTMP流切成HLS片。注意不会编译nginx，在i386/x86_64平台上srs会编译nginx用于分发。
* --with-http-server: 默认开启，可关闭。开启[内嵌的http服务器分发hls](v1_CN_HTTPServer)。
* --with-http-api: 默认开启，可关闭。支持[http api](v1_CN_HTTPApi)。
* --with-librtmp：默认关闭，可开启。编译客户端库，arm客户端可以调用这个库将流推送到srs。

编译成功后，srs即为arm上可运行：

```bash
winlin@winlin-VirtualBox:~/srs$ file objs/srs
objs/srs: ELF 32-bit LSB executable, ARM, version 1 (SYSV), statically linked, 
for GNU/Linux 2.6.31, BuildID[sha1]=0xfba434cea50d6b02fd6e21ce67f01c39772c724b, 
not stripped
```

备注：在x86和arm平台切换时，譬如之前是为arm编译的，现在为x86平台编译，不需要手动删除东西，直接执行configure就可以，脚本会自动判断。

备注：srs都是使用静态链接，不依赖st/ssl，链接.a库。

## Use Other Cross build tools

ubuntu12默认的arm交叉编译工具是arm7，如何使用自己的交叉编译工具：
* 修改configure后编译。
* 导出环境变量，然后configure和编译。

<strong>第一种方法，修改configure并编译</strong>：

```bash
SrsArmCC="arm-linux-gnueabi-gcc"
SrsArmGCC="arm-linux-gnueabi-gcc"
SrsArmCXX="arm-linux-gnueabi-g++"
SrsArmAR="arm-linux-gnueabi-ar"
SrsArmLD="arm-linux-gnueabi-ld"
SrsArmRANDLIB="arm-linux-gnueabi-ranlib"
```

修改后和前面一样运行configure和make：

```bash
./configure --with-arm-ubuntu12 --with-ssl --with-hls --with-librtmp && make
```

<strong>第二种方法，导出环境变量并编译：</strong>

```bash
export SrsArmCC="arm-linux-gnueabi-gcc" &&
export SrsArmGCC="arm-linux-gnueabi-gcc" &&
export SrsArmCXX="arm-linux-gnueabi-g++" &&
export SrsArmAR="arm-linux-gnueabi-ar" &&
export SrsArmLD="arm-linux-gnueabi-ld" &&
export SrsArmRANDLIB="arm-linux-gnueabi-ranlib" &&
./configure --with-arm-ubuntu12 --with-ssl --with-hls --with-librtmp && make
```

拷贝到目标机器上执行即可。

## RaspberryPi

可以在arm环境下直接编译，如果可以的话，譬如raspbian。

我个人买了个树莓派（应该是B型），在京东上买的，树莓派基本上跑起来很容易。参考：[RaspberryPi](http://blog.csdn.net/win_lin/article/details/22468687)

我用的是respberrypi最新的img，运行起来后发现什么都有。gcc/g++/gdb/git，应有尽有。发现release的那些包，ubuntu12下面编译出来的包是armv7的，raspberrypi是armv6的体系，所以跑不了。

干脆就git clone了一个srs，在pi下面直接编译，速度是慢点，但以后就快了，然打包放到release中，其他pi就不用自己编译了。

RaspberryPi默认推荐的系统是debian，即Raspbian。官方的源在国内很慢，推荐用东软或者清华的源：

```bash
sudo cp /etc/apt/sources.list /etc/apt/sources.list.old
sudo vi /etc/apt/sources.list
```

系统默认的源是：

```bash
deb http://mirrordirector.raspbian.org/raspbian/ wheezy main contrib non-free rpi
```

可以改为下表的Source：

| 说明 | Source |
| --- | ------- |
| 东软 | deb http://mirrors.neusoft.edu.cn/raspbian/raspbian wheezy main contrib non-free rpi |
| 清华 | deb http://mirrors.tuna.tsinghua.edu.cn/raspbian/raspbian/ wheezy main contrib non-free rpi |
| 成都凝聚工作室 | deb http://raspbian.cnssuestc.org/raspbian/ wheezy main contrib non-free rpi |
| 华中科大 | deb http://mirrors.hustunique.com/raspbian/raspbian wheezy main contrib non-free rpi |
| 中科大 | deb http://mirrors.ustc.edu.cn/raspbian/raspbian/ wheezy main contrib non-free rpi |
| 中山大学 | deb http://mirror.sysu.edu.cn/raspbian/ wheezy main contrib non-free rpi |
| 新加坡大学 | deb http://mirror.nus.edu.sg/raspbian/raspbian wheezy main contrib non-free rpi | 

然后更新安装源：

```bash
sudo apt-get update
```

若需要在RaspberryPi下编译srs，需要：
* 安装lsb_release，package打包需要：`sudo aptitude install -y lsb_release`
* 安装zip，打包需要：`sudo aptitude install -y zip`
* 编译srs：`./configure --pi && make`
* 打包srs：`./scripts/package.sh --pi`

就可以生成安装包，譬如`objs/SRS-RaspberryPi7-armv6l-0.9.37.zip`。

## Armel and Armhf

有时候总是碰到`Illegal instruction`，那是编译器的目标CPU太高，虚拟机的CPU太低。
参考：[http://stackoverflow.com/questions/14253626/arm-cross-compiling](http://stackoverflow.com/questions/14253626/arm-cross-compiling)

写一个简单的测试程序，测试编译环境：

```cpp
/*
 arm-linux-gnueabi-g++ -o test test.cpp -static
 arm-linux-gnueabi-strip test
*/
#include <stdio.h>

int main(int argc, char** argv) {
    printf("hello, arm!\n");
    return 0;
}
```

编译出test后，使用工具查看目标CPU：

```bash
arm-linux-gnueabi-readelf --file-header --arch-specific test
运行结果如下：
  Machine:                           ARM
File Attributes
  Tag_CPU_name: "7-A"
  Tag_CPU_arch: v7
```

可见Ubuntu12的交叉环境是cpuv7（debian armhf），所以arm虚拟机需要是v7的。

若使用debian armel，cpu是v5的，信息如下：

```bash
root@debian-armel:~# cat /proc/cpuinfo 
Processor	: ARM926EJ-S rev 5 (v5l)
CPU revision	: 5
```

若使用debian armhf，cpu是v7的，信息如下：

```bash
root@debian-armhf:~# cat /proc/cpuinfo 
Processor	: ARMv7 Processor rev 0 (v7l)
CPU architecture: 7
```

将测试程序编译后scp到arm虚拟机，可以运行：

```bash
root@debian-armhf:~# ./test 
hello, arm!
```

## Install QEMU

qemu可以模拟arm的环境，可以在CentOS/Ubuntu下先编译安装qemu（yum/aptitude安装的好像不全）。

qemu依赖于SDL（图形界面），SDL依赖于图形linux，所以最好用ubuntu桌面版和CentOS开发版，进入图形界面后安装sdl。
* CentOS SDL安装：`sudo yum install -y SDL-devel`
* Ubuntu SDL安装：`sudo aptitude install -y libsdl1.2-dev`

然后下载qemu，好像网站被墙了，所以可以用迅雷下，它的p2p网络会去找这个文件：

```html
http://wiki.qemu-project.org/download/qemu-1.7.0.tar.bz2
```

编译和安装qemu

```bash
tar xf qemu-1.7.0.tar.bz2 && cd qemu-1.7.0 && ./configure && make && sudo make install
```

qemu两个重要的工具：

```bash
/usr/local/bin/qemu-img
/usr/local/bin/qemu-system-arm
```

## Start ARM VM

网络安装很慢，而且有时候安装失败。可以直接使用别人提供的已经安装好的镜像，QEMU的磁盘文件和引导镜像。

首先是下载编译qemu，参考前面一章。

下载已经安装好的镜像(armhf对应ubuntu12交叉环境），引导内核：
* [vmlinuz-3.2.0-4-vexpress](http://people.debian.org/~aurel32/qemu/armhf/vmlinuz-3.2.0-4-vexpress)
* [initrd.img-3.2.0-4-vexpress](http://people.debian.org/~aurel32/qemu/armhf/initrd.img-3.2.0-4-vexpress)
* [debian_wheezy_armhf_standard.qcow2](http://people.debian.org/~aurel32/qemu/armhf/debian_wheezy_armhf_standard.qcow2)

下载后直接启动qemu：

```bash
qemu-system-arm -M vexpress-a9 -kernel vmlinuz-3.2.0-4-vexpress \
    -initrd initrd.img-3.2.0-4-vexpress -drive if=sd,file=debian_wheezy_armhf_standard.qcow2 \
    -append "root=/dev/mmcblk0p2"
```

登录信息：
* ROOT密码：root
* 用户名：user
* 用户密码：user

## ARM VM Network

arm虚拟机如何对外提供服务？桥接的方式很麻烦，有一种简单的方式，就是[端口转发](http://en.wikibooks.org/wiki/QEMU/Networking)，启动qemu时指定宿主host的端口和虚拟机的端口绑定，这样就可以访问宿主的端口来访问虚拟机了。譬如：

```bash
qemu-system-arm -M vexpress-a9 -kernel vmlinuz-3.2.0-4-vexpress \
    -initrd initrd.img-3.2.0-4-vexpress -drive if=sd,file=debian_wheezy_armhf_standard.qcow2 \
    -append "root=/dev/mmcblk0p2" \
    -redir tcp:8000::80 -redir tcp:4450::445 -redir tcp:2200::22 -redir tcp:19350::1935
```

备注：注意，端口不能太小，譬如800:80就不行。

启动后，连接宿主的2200就可以登录到arm虚拟机。访问宿主的8000就是访问arm的80，访问19350就是访问arm的流媒体1935。445是samba端口，宿主可以将arm的共享挂载到自己的目录，对外提供共享。

其实，只要能开放22和服务端口（1935），就可以啦，可以通过scp拷贝文件。从宿主host拷贝文件到arm虚拟机：

```bash
scp -P 2200 objs/srs root@localhost:~
```

输入密码就可以拷贝过去，结果如下：

```bash
root@debian-armel:~# uname -a
Linux debian-armel 3.2.0-4-versatile #1 Debian 3.2.51-1 armv5tejl GNU/Linux
root@debian-armel:~# ls -lh
total 3.3M
-rwxr-xr-x 1 root root 3.3M Mar 15 19:01 srs
root@debian-armel:~# file srs
srs: ELF 64-bit LSB executable, x86-64, version 1 (GNU/Linux), dynamically linked (uses shared 
libs), for GNU/Linux 2.6.24, BuildID[sha1]=0x678e75d2547bc219be05864ef6582a3a7a4ad734, 
not stripped
```

若srs编译时指定arm，则可以启动，推流和观看宿主的19350，就是arm提供服务了。

## The SRS License for ARM

ARM设备大多是消费类产品，所以对于依赖的软件授权（License）很敏感，nginx-rtmp/crtmpserver都是GPL授权，
对于需要目标用户在国外的ARM设备还是SRS的MIT-License更商业友好。

License也是很多ARM厂商考虑SRS的原因。

## Debian ARM VM

除了直接使用已有的镜像，还可以通过网络安装（但网络比较慢，安装过程容易出错，不推荐）。

创建虚拟机使用的硬盘：

```bash
qemu-img create -f raw hda.img 4G
```

安装方式，可以选择网络安装，下载网络安装镜像后启动。

网络安装，先下载内核镜像：
* [initrd.gz](http://ftp.de.debian.org/debian/dists/stable/main/installer-armel/current/images/versatile/netboot/initrd.gz)
* [vmlinuz-3.2.0-4-versatile](http://ftp.de.debian.org/debian/dists/stable/main/installer-armel/current/images/versatile/netboot/vmlinuz-3.2.0-4-versatile)

开始安装：

```bash
qemu-system-arm -machine versatilepb -kernel vmlinuz-3.2.0-4-versatile \
    -hda hda.img -initrd initrd.gz -append "root=/dev/ram" -m 256
```

安装过程中，镜像源选择China或者Taiwan。

安装完成后，需要从hda.img硬盘中将启动映像拷贝出来，下次qemu就从它开始引导。

```bash
file -s hda.img 
#hda.img: x86 boot sector; 
#    partition 1: ID=0x83, starthead 32, startsector 2048, 7936000 sectors; 
#    partition 2: ID=0x5, starthead 63, startsector 7940094, 446466 sectors, code offset 0xb8
#第一扇区的起始地址是2048，挂载映像到文件夹：
mkdir -p disk && sudo mount ./hda.img ./disk -o offset=$((2048*256))
#取出启动内核：
cp disk/boot/initrd.img-3.2.0-4-versatile .
#使用新启动内核启动：
qemu-system-arm -machine versatilepb -kernel vmlinuz-3.2.0-4-versatile \
    -hda hda.img -initrd initrd.img-3.2.0-4-versatile -m 256 -append "root=/dev/sda1"
```

## ST-ARM-BUG-FIX

st在arm上有个bug，原因是setjmp.h的布局变了。st在setjmp后，开辟新的stack空间，所以需要将sp设置为新开辟的空间。
* i386的sp偏移量是4：env[0].__jmp_buf[4]=(long)sp
* x86_64的sp偏移量是6：env[0].__jmp_buf[6]=(long)sp
* armhf(v7cpu)的sp偏移量是8，但是st写的是20，所以就崩溃了。

实际上是因为i386和x86_64的setjmp是汇编写的，st自己实现的，所以这个偏移量是有效的。
但是对于arm，st是使用的默认的setjmp，即libc的函数，而libc的这个jmpbuf是有变更的，
在2.4以上甚至都不是公开的了（这是为何st要自己实现的原因），具体参考：
[bug #182](https://github.com/simple-rtmp-server/srs/issues/182), 
[bug #190](https://github.com/simple-rtmp-server/srs/issues/190)

```bash
// md.h
        #elif defined(__i386__)
            #if defined(__GLIBC__) && __GLIBC__ >= 2
                #define MD_GET_SP(_t) (_t)->context[0].__jmpbuf[4]
        #elif defined(__amd64__) || defined(__x86_64__)
            #define MD_GET_SP(_t) (_t)->context[0].__jmpbuf[6]
        #elif defined(__arm__)
            #if defined(__GLIBC__) && __GLIBC__ >= 2
                #define MD_GET_SP(_t) (_t)->context[0].__jmpbuf[20]
```

x86_64的setjmp的env参数的布局：[参考资料](https://gfiber.googlesource.com/kernel/prism/+/dbb415ed05d5cea3d84dec3400669fb4f9b4c727%5E/arch/um/sys-x86_64/setjmp.S)

```bash
# The jmp_buf is assumed to contain the following, in order:
#       %rbx                 7
#       %rsp (post-return)   6
#       %rbp                 5
#       %r12                 4
#       %r13                 3
#       %r14                 2
#       %r15                 1
#       <return address>     0
// 从下往上数，sp是第6。所以st写的6是对的。
```

arm直接看头文件的说明：

```bash
// /usr/arm-linux-gnueabi/include/bits/setjmp.h
#ifndef _ASM
/* The exact set of registers saved may depend on the particular core
   in use, as some coprocessor registers may need to be saved.  The C
   Library ABI requires that the buffer be 8-byte aligned, and
   recommends that the buffer contain 64 words.  The first 28 words
   are occupied by v1-v6, sl, fp, sp, pc, d8-d15, and fpscr.  (Note
   that d8-15 require 17 words, due to the use of fstmx.)  */
typedef int __jmp_buf[64] __attribute__((__aligned__ (8)));
#endif
//布局应该是：words=ints
0-5: v1-v6 
6: sl
7: fp
8: sp
9: pc
10-26: d8-d15 17words
27: fpscr
//所以应该sp是env[8]，设置它就对了。
```

修正方法，srs已经打了patch，会向st提交：

```bash
#define MD_GET_SP(_t) (_t)->context[0].__jmpbuf[20]
```

改为：

```bash
#define MD_GET_SP(_t) (_t)->context[0].__jmpbuf[8]
```

运行结果：

```bash
root@debian-armhf:~# uname -a
Linux debian-armhf 3.2.0-4-vexpress #1 SMP Debian 3.2.51-1 armv7l GNU/Linux
root@debian-armhf:~# file srs
srs: ELF 32-bit LSB executable, ARM, version 1 (SYSV), statically linked, 
for GNU/Linux 2.6.31, BuildID[sha1]=0xba18634b92775bdb1314ea02db23b1e233d30df3, 
not stripped
root@debian-armhf:~# ./srs -c conf/srs.conf 
[2014-03-16 09:48:30.114][error][0][0] end of file. ret=409
[2014-03-16 09:48:30.120][trace][1][0] server started, listen at port=1935, fd=3
[2014-03-16 09:48:30.126][trace][2][0] thread cycle start
[2014-03-16 09:48:31.344][trace][3][11] get peer ip success. ip=192.168.2.101, send_to=30000000, recv_to=30000000
[2014-03-16 09:48:31.355][trace][3][11] simple handshake success.
[2014-03-16 09:48:31.363][trace][3][11] rtmp connect app success. 
tcUrl=rtmp://192.168.2.111:19350/live, pageUrl=, swfUrl=rtmp://192.168.2.111:19350/live, 
schema=rtmp, vhost=__defaultVhost__, port=19350, app=live
[2014-03-16 09:48:31.376][trace][3][11] set ack window size to 2500000
[2014-03-16 09:48:31.381][trace][3][11] identify ignore messages except AMF0/AMF3 command message. type=0x5
[2014-03-16 09:48:31.387][trace][3][11] identify client success. type=FMLEPublish, stream_name=livestream
[2014-03-16 09:48:31.393][trace][3][11] set output chunk size to 60000
[2014-03-16 09:48:31.393][trace][3][11] set chunk_size=60000 success
[2014-03-16 09:48:32.888][trace][3][11] <- time=1065756, obytes=4168, ibytes=4973, okbps=22, ikbps=26
[2014-03-16 09:48:32.896][trace][3][11] dispatch metadata success.
[2014-03-16 09:48:32.898][trace][3][11] process onMetaData message success.
[2014-03-16 09:48:32.935][trace][3][11] set input chunk size to 157
[2014-03-16 09:48:33.895][trace][3][11] <- time=1066860, obytes=4168, ibytes=49187, okbps=13, ikbps=156
[2014-03-16 09:48:35.391][trace][3][11] <- time=1067984, obytes=4168, ibytes=82561, okbps=8, ikbps=164
```

备注：另外，ST还有个bug，多进程启动时，st只能在fork之后启动。否则会有问题。

see: https://github.com/simple-rtmp-server/srs/issues/190

Winlin 2014.2
