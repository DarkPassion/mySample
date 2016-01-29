# SRS for linux-arm

How to run SRS on ARM pcu?

* Application on ARM to publish stream to SRS, use [srs-librtmp](v1_EN_SrsLibrtmp).
* ARM directly publish h.264 stream to SRS(Not support yet).
* Run SRS on ARM: Client can play strema on ARm.

## Why run SRS on ARM?

The use scenario:

* There are lots of ARM device, and the center server cannot accept so many streams.
* User can directly play stream on ARM when ARM running SRS server.
* SRS can output RTMP and HLS, for both PC and mobile(Android+IOS).

## Ubuntu Cross Build SRS

Install the ARM cross build tool for SRS:

```bash
sudo aptitude install -y gcc-arm-linux-gnueabi g++-arm-linux-gnueabi
```

The tools compare:

| x86 | armhf(v7cpu) |
| --- | ------------- |
| gcc | arm-linux-gnueabi-gcc |
| g++ | arm-linux-gnueabi-g++ |
| ar | arm-linux-gnueabi-ar |
| as | arm-linux-gnueabi-as |
| ld | arm-linux-gnueabi-ld |
| ranlib | arm-linux-gnueabi-ranlib |
| strip | arm-linux-gnueabi-strip | 

Cross build:

```bash
./configure --with-arm-ubuntu12 && make
```

The bellow features can used for ARM-SRS:
* --with-arm-ubuntu12: To use the cross build tools for ubuntu.
* --with-ssl: To support complex handshake, read [Handshake](v1_EN_RTMPHandshake)
* --with-hls: To support HLS.
* --with-http-server: To support embeded server to delivery HLS, read [SRS HTTP SERVER](v1_EN_HTTPServer)。
* --with-http-api: To support http api, read [http api](v1_EN_HTTPApi)。
* --with-librtmp: The client library.

Copy the binary to ARM and run:

```bash
winlin@winlin-VirtualBox:~/srs$ file objs/srs
objs/srs: ELF 32-bit LSB executable, ARM, version 1 (SYSV), statically linked, 
for GNU/Linux 2.6.31, BuildID[sha1]=0xfba434cea50d6b02fd6e21ce67f01c39772c724b, 
not stripped
```

## Use Other Cross build tools

SRS supports to modify the Default cross build tools:
* Modify the `configure` then build.
* Export evn variables, then build.

<strong>Modify the configure</strong>：

```bash
SrsArmCC="arm-linux-gnueabi-gcc"
SrsArmGCC="arm-linux-gnueabi-gcc"
SrsArmCXX="arm-linux-gnueabi-g++"
SrsArmAR="arm-linux-gnueabi-ar"
SrsArmLD="arm-linux-gnueabi-ld"
SrsArmRANDLIB="arm-linux-gnueabi-ranlib"
```

Then configure and build:

```bash
./configure --with-arm-ubuntu12 --with-ssl --with-hls --with-librtmp && make
```

<strong>Export env variables</strong>

```bash
export SrsArmCC="arm-linux-gnueabi-gcc" &&
export SrsArmGCC="arm-linux-gnueabi-gcc" &&
export SrsArmCXX="arm-linux-gnueabi-g++" &&
export SrsArmAR="arm-linux-gnueabi-ar" &&
export SrsArmLD="arm-linux-gnueabi-ld" &&
export SrsArmRANDLIB="arm-linux-gnueabi-ranlib" &&
./configure --with-arm-ubuntu12 --with-ssl --with-hls --with-librtmp && make
```

## RaspberryPi

SRS can be build on ARM os, for example, raspbian:
* Install the lsb_release, which is used by package: `sudo aptitude install -y lsb_release`
* Install zip, for package: `sudo aptitude install -y zip`
* Build SRS: `./configure --pi && make`
* Package SRS: `./scripts/package.sh --pi`

The package, for instance, `objs/SRS-RaspberryPi7-armv6l-0.9.37.zip`,
can be unzip and install.

## Armel and Armhf

The error `Illegal instruction` is the cpu not compatible,
the cross build target cpu too high, read 
[http://stackoverflow.com/questions/14253626/arm-cross-compiling](http://stackoverflow.com/questions/14253626/arm-cross-compiling)

The bellow is a simple test program:

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

To get the target cpu:

```bash
arm-linux-gnueabi-readelf --file-header --arch-specific test
Result:
  Machine:                           ARM
File Attributes
  Tag_CPU_name: "7-A"
  Tag_CPU_arch: v7
```

So, the ubuntu12 cross build target cpu is cpuv7(debian armhf).

The debian armel, cpu is armv5:

```bash
root@debian-armel:~# cat /proc/cpuinfo 
Processor	: ARM926EJ-S rev 5 (v5l)
CPU revision	: 5
```

While the debian armhf, cpu is armv7:

```bash
root@debian-armhf:~# cat /proc/cpuinfo 
Processor	: ARMv7 Processor rev 0 (v7l)
CPU architecture: 7
```

Run test program on armhf:

```bash
root@debian-armhf:~# ./test 
hello, arm!
```

## The SRS License for ARM

SRS is MIT license, ok for ARM.

## ST-ARM-BUG-FIX

I fixed a bug for ARM in st, the setjmp.h layout changed:
* i386 offset of sp is 4：env[0].__jmp_buf[4]=(long)sp
* x86_64 offset of sp is 6：env[0].__jmp_buf[6]=(long)sp
* armhf(v7cpu) sp offset is 8, but st use 20.

Read [bug #182](https://github.com/simple-rtmp-server/srs/issues/182), 
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

When st_init(), we should never fork process,
read [bug #190](https://github.com/simple-rtmp-server/srs/issues/190)

Winlin 2014.11
