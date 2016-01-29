# SRS Linux Service

There are many ways to startup SRS:
* Directly run srs at the trunk/objs, and need start again when system restart.
* Linux service, the init.d scirpt at `simple-rtmp-server/trunk/etc/init.d/srs`, and user can add to linux service when linked to the /etc/init.d/srs then add as service `/sbin/chkconfig --add srs`.

The SRS release binary can be downloaded from release site, we can install as system service, see: [Github: release](http://winlinvip.github.io/srs.release) or [Mirror for China: release](http://www.ossrs.net)

## Directly startup SRS

We donot need to add to linux service to directly start SRS:

```bash
cd /home/winlin/_git/simple-rtmp-server/trunk &&
./etc/init.d/srs start
```

or

```bash
cd /home/winlin/_git/simple-rtmp-server/trunk &&
./objs/srs -c conf/srs.conf
```

## LinuxService

Install and startup SRS as linux system service:
* Build SRS: the install script will modify the INSTALL ROOT of init.d script.
* Link to init.d: link the `trunk/etc/init.d/srs` to `/etc/init.d/srs`
* Add to linux service: use /sbin/chkconfig for Centos.

<strong>Step1:</strong> Build and Install SRS

Intall SRS when build ok:

```bash
make && sudo make install
```

the install of make will install srs to the prefix dir, default to `/usr/local/srs`, which is specified by configure, for instance, ```./configure --prefix=`pwd`/_release``` set the install dir to _release of current dir to use `make install` without sudo.

<strong>Step2:</strong> Link script to init.d:

```bash
sudo ln -sf \
    /usr/local/srs/etc/init.d/srs \
    /etc/init.d/srs
```

<strong>Step3:</strong>Add as linux service:

```bash
#centos 6
sudo /sbin/chkconfig --add srs
```

or

```bash
#ubuntu12
sudo update-rc.d srs defaults
```

## Use init.d script

Get the status of SRS:

```bash
/etc/init.d/srs status
```

Start SRS：

```bash
/etc/init.d/srs start
```

Stop SRS：

```bash
/etc/init.d/srs stop
```

Restart SRS：

```bash
/etc/init.d/srs restart
```

Reload SRS：

```bash
/etc/init.d/srs reload
```

## Install SRS-API

SRS will install api to `/usr/local/srs`, which is specified by configure.

The api is the api-server of SRS, which is used for SRS to callback the http api. The http callback need to open the feature `--with-http-callback`, and SRS use python(cherrypy) for server.py, which user can directly run.

The api-server tp provide http service, and the pages for demo of SRS including the player, encoder and the meeting demo of SRS. When api-server start, we can access `http://192.168.1.170:8085`, the demo of SRS. See: [Usage: Demo](v1_EN_SampleDemo).

<strong>Step1:</strong> Enable the demo feature:

```bash
./configure --with-hls --with-ffmpeg --with-http-callback --with-ffmpeg
```

<strong>Step2:</strong> Install api:

```bash
make && sudo make install-api
```

Start api-server:

```bash
/usr/local/srs/etc/init.d/srs-api start
```

See: [Usage: Demo](v1_EN_SampleDemo)

<strong>Step3:</strong> Link api script:

```bash
sudo ln -sf \
    /usr/local/srs/etc/init.d/srs-api \
    /etc/init.d/srs-api
```

<strong>Step4:</strong> Add to linux service:

```bash
#centos 6
sudo /sbin/chkconfig --add srs-api
```

or

```bash
#ubuntu12
sudo update-rc.d srs-api defaults
```

<strong>Step5:</strong> Manage the SRS-api service:

Get the SRS-api status:

```bash
/etc/init.d/srs-api status
```

Start SRS-api：

```bash
/etc/init.d/srs-api start
```

Stop SRS-api：

```bash
/etc/init.d/srs-api stop
```

Restart SRS-api：

```bash
/etc/init.d/srs-api restart
```

Winlin 2014.10
