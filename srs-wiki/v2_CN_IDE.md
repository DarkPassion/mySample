# 集成开发环境

xcode开发SRS，参考：[SRS for osx](https://github.com/simple-rtmp-server/srs/issues/155#issuecomment-77995950).

windows/linux下开发srs的IDE：[http://blog.csdn.net/win_lin/article/details/39579799](http://blog.csdn.net/win_lin/article/details/39579799)

我也比较过其他的IDE，可以参考：[开源日志：关于IDE/源代码编辑](http://blog.csdn.net/win_lin/article/details/8142981)

## QtCreator

SRS添加了QT的配置文件，使用QT打开`simple-rtmp-server/trunk/src/qt/srs/srs-qt.pro`即可。

## JetBrains

JetBrains终于放出来c++的IDE了。目前是60天的预览版，下载：[http://www.jetbrains.com/clion/](http://www.jetbrains.com/clion/)

CLion的主要亮点：

1. windows下linux程序的IDE。别纠缠vs是不是王中之王，用vs打开srs代码错误就一坨一坨的，没法正常使用。
1. 可以忽略编译，当作编辑器使用。windows下的linux代码无法编译过，mingw有时也不好使，但是CLion可以当作编辑器使用。
1. 支持基本功能：函数跳转，类跳转，头文件跳转，hpp和cpp直接跳转，智能提示，没用的宏定义的提示。
1. 支持FindUsage：函数或者类在什么地方使用了，这个功能对于代码阅读和修改很有用。
1. 支持Refactor：Rename，Extract，Move，ChangeSignature，PullMemberUp/Down众多减少苦力的功能。
1. 还有个牛逼的东西，选中后按CTRL+F，自动高亮。这个是非常非常常用的功能，比notepad++好。upp就是没有这个愁死我了。
1. InspectCode，代码检查，分析代码潜在的问题，譬如我检查srs有：一千个拼写问题，没有用到的代码2百行，类型检查1百个，声明问题2个。
术业有专攻，JetBrains的IDE做得非常之用心，只要CLion出来，我就果断抛弃upp。

使用方法：OpenProject，选择srs的trunk目录下的CMakeLists.txt，点OK即可。

linux版的，在centos下也挺好用，需要下载cmake2.8：[http://www.cmake.org/files/v2.8/cmake-2.8.12.2.tar.gz](http://www.cmake.org/files/v2.8/cmake-2.8.12.2.tar.gz)和gdb：[http://ftp.gnu.org/gnu/gdb/gdb-7.7.tar.gz](http://ftp.gnu.org/gnu/gdb/gdb-7.7.tar.gz)，然后指定toolchain为：/usr/local/bin/cmake和/usr/local/bin/gdb，因为centos6默认的cmake和gdb都太老了。

另外，还需要java虚拟机，可以下载jdk7之类：jdk-7u45-linux-x64.tar.gz，然后编辑环境脚本：`sudo vi /etc/profile`，在文件尾部加入：
```
# winlin
JAVA_HOME=/usr/local/jdk1.7.0_45
PATH=$PATH:$JAVA_HOME/bin
CLASSPATH=.:$JAVA_HOME/bin/dt.jar:$JAVA_HOME/lib/tools.jar
export JAVA_HOME
export PATH
export CLASSPATH
```
假设jdk7解压在/usr/local/jdk1.7.0_45的话。解压clion后，执行bin/clion.sh脚本即可，在centos图形界面下执行。

## UPP开发SRS

SRS使用UPP的开发，好处是：
* 只需要一个配置文件，不会在源码生成乱七八糟的文件。像vs之类就讨厌这个。
* 不编译，只用来编辑代码，因为windows下面编译过不了。在linux编译和调试。
* hpp和cpp之间跳转方便，可以方便的在类成员函数和实现之间跳转。
* 解析宏定义，类定义和函数，模板函数等等，解析都正确，不像SourceInsight解析不对。

下面讲讲使用过程：

<strong>第一步，下载和安装upp</strong>

http://sourceforge.net/project/downloading.php?group_id=93970&filename=upp-win32-5485.exe

<strong>第二步，打开UPP后，新建assembly</strong>

![新建assembly](http://winlinvip.github.io/srs.release/wiki/images/upp/001.new.assembly.png)

<strong>第三步，导入srs</strong>

![导入srs](http://winlinvip.github.io/srs.release/wiki/images/upp/002.setup.assembly.png)

![导入srs](http://winlinvip.github.io/srs.release/wiki/images/upp/003.open.assembly.png)

<strong>OK</strong>

![导入srs](http://winlinvip.github.io/srs.release/wiki/images/upp/004.ok.png)

<strong>设置智能补全键，按CTRL+J出现说明</strong>4

默认的智能补全键是CTRL+空格键，这个组合键实际上用来切换输入法。

打开 Setup=>Keyboard shortcuts=>Ide，找到Assist并设置（在最后），设置如下图：

![智能补全键](http://winlinvip.github.io/srs.release/wiki/images/upp/005.assist.shortcuts.png)

注意：记得把Navigate的快件键改为CTRL+F1（CTRL+J，在前面点位置），否则Assist的新快捷键不会生效。

非常简单！

Winlin 2015.3
