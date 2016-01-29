# SRS系统服务

SRS提供多种启动的方式，包括：
* 在git目录直接启动，简单直接，但系统重启后需要手动启动。
* 系统服务，init.d脚本：SRS提供`simple-rtmp-server/trunk/etc/init.d/srs`脚本，可以作为CentOS或者Ubuntu的系统服务自动启动。

可以直接下载release的binary，提供了几个常见系统的安装包，安装程序会安装系统服务，直接以系统服务启动即可。参考：[Github: release](http://winlinvip.github.io/srs.release)或者[国内镜像: release](http://ossrs.net/)

## 直接启动

若不需要添加到系统服务，每次重启后需要手动启动SRS，可以直接在srs的trunk目录执行脚本：

```bash
cd /home/winlin/_git/simple-rtmp-server/trunk &&
./etc/init.d/srs start
```

## LinuxService

SRS作为系统服务启动，需要以下几步：
* 安装srs：安装脚本会自动修改init.d脚本，将`ROOT="./"`改为安装目录。
* 链接安装目录的`init.d/srs`到`/etc/init.d/srs`
* 添加到系统服务，CentOS和Ubuntu方法不一样。

<strong>Step1:</strong> 安装SRS

编译SRS后，可执行命令安装SRS：

```bash
make && sudo make install
```

安装命令会将srs默认安装到`/usr/local/srs`中，可以在configure时指定其他目录，譬如```./configure --prefix=`pwd`/_release```可以安装到当前目录的_release目录（可以不用sudo安装，直接用`make install`即可安装。

<strong>Step2:</strong> 链接脚本：

```bash
sudo ln -sf \
    /usr/local/srs/etc/init.d/srs \
    /etc/init.d/srs
```

备注：若SRS安装到其他目录，将`/usr/local/srs`替换成其他目录。

备注：也可以使用其他的名称，譬如`/etc/init.d/srs`，可以任意名称，启动时也用该名称。

<strong>Step3:</strong>添加服务：

```bash
#centos 6
sudo /sbin/chkconfig --add srs
```

或者

```bash
#ubuntu12
sudo update-rc.d srs defaults
```

## 使用init.d脚本管理SRS

查看SRS状态：

```bash
/etc/init.d/srs status
```

启动SRS：

```bash
/etc/init.d/srs start
```

停止SRS：

```bash
/etc/init.d/srs stop
```

重启SRS：

```bash
/etc/init.d/srs restart
```

Reload SRS：

```bash
/etc/init.d/srs reload
```

## 安装SRS-API

SRS支持安装SRS到`/usr/local/srs`目录，用户在configure时可以修改这个目录。

SRS的API即api-server，SRS可以调用api-server提供的http接口，需要打开`--with-http-callback`支持。SRS使用python(cherrypy)编写的server.py，直接运行脚本。

API服务器主要提供http调用服务，提供demo运行的页面，播放器和编码器视频会议等DEMO。也就是说，启动api之后，可以访问`http://192.168.1.170:8085`即可以看到DEMO的页面，播放器/编码器/视频会议都可以使用，不过有些DEMO的功能不支持（譬如测速，演示流等，参考[Usage: Demo](v1_CN_SampleDemo)）。

<strong>Step1:</strong> 配置时打开demo支持：

```bash
./configure --with-hls --with-ffmpeg --with-http-callback --with-ffmpeg
```

<strong>Step2:</strong> 安装api：

```bash
make && sudo make install-api
```

安装后，可以启动api：

```bash
/usr/local/srs/etc/init.d/srs-api start
```

即可以观看demo的页面。推流需要自己手动推流。若需要观看所有演示，直接用脚本启动，参考：[Usage: Demo](v1_CN_SampleDemo)

注意：安装srs-api适用于使用播放器的客户，使用推流编码器的客户，以及使用视频会议demo的客户。总之只会启动api-server，所以默认那些演示流是不会起来的。

注意：也可以直接在srs的git目录启动`./etc/init.d/simple-rtmp-servr-api start`，不必安装。

注意：也可以以系统服务方式启动api-server，参考前面srs以系统服务方式启动的例子。

<strong>Step3:</strong> 软链api脚本：

```bash
sudo ln -sf \
    /usr/local/srs/etc/init.d/srs-api \
    /etc/init.d/srs-api
```

<strong>Step4:</strong> 加入服务：

```bash
#centos 6
sudo /sbin/chkconfig --add srs-api
```

或者

```bash
#ubuntu12
sudo update-rc.d srs-api defaults
```

<strong>Step5:</strong> 管理SRS-api服务：

查看SRS-api状态：

```bash
/etc/init.d/srs-api status
```

启动SRS-api：

```bash
/etc/init.d/srs-api start
```

停止SRS-api：

```bash
/etc/init.d/srs-api stop
```

重启SRS-api：

```bash
/etc/init.d/srs-api restart
```

Winlin 2014.3
