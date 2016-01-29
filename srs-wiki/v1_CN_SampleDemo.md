# Demo的部署

SRS的DEMO的部署，提供网页演示播放/推送RTMP到SRS，播放HLS，转码，视频会议等。

<strong>注意：强烈建议初学者不要动demo，只要看demo就好；建议学完[Usage][Usage]的每个单独的使用，然后再动demo。</strong>

<strong>假设服务器的IP是：192.168.1.170</strong>

<strong>第一步，设置客户端hosts，注意是客户端（需要将demo.srs.com这个域名/vhost解析到服务器）</strong>

```bash
# edit the folowing file:
# linux: /etc/hosts
# windows: C:\Windows\System32\drivers\etc\hosts
# where server ip is 192.168.1.170
192.168.1.170 demo.srs.com
```

<strong>第二步，获取SRS。</strong>详细参考[GIT获取代码](v1_CN_Git)

```bash
git clone https://github.com/simple-rtmp-server/srs
cd simple-rtmp-server/trunk
```

或者使用git更新已有代码：

```bash
git pull
```

<strong>第三步，编译SRS。</strong>详细参考[Build](v1_CN_Build)

```bash
bash scripts/build.sh
```

<strong>第三步，启动SRS的DEMO，启动ffmpeg/api-server/srs等。</strong>

```bash
bash scripts/run.sh
```

访问SRS的DEMO：[http://demo.srs.com:8085](http://demo.srs.com:8085)

<strong>第四步，停止SRS的DEMO，停止ffmpeg/api-server/srs等。</strong>

```bash
bash scripts/stop.sh
```

备注：DEMO的所有链接都是可以点的，所有功能都是可用的。

备注：请将所有实例的IP地址192.168.1.170都换成部署的服务器IP地址。

备注：DEMO不依赖nginx，使用cherrypy做演示网站以及分发HLS，依赖FFMPEG需要演示转码。

Winlin 2014.3

[nginx]: http://192.168.1.170:8080/nginx.html
[srs-player]: http://winlinvip.github.io/srs.release/trunk/research/players/srs_player.html?vhost=__defaultVhost__&autostart=true&server=192.168.1.170&app=live&stream=livestream&port=1935
[srs-player-19350]: http://winlinvip.github.io/srs.release/trunk/research/players/srs_player.html?vhost=__defaultVhost__&autostart=true&server=192.168.1.170&app=live&stream=livestream&port=19350
[srs-player-ff]: http://winlinvip.github.io/srs.release/trunk/research/players/srs_player.html?vhost=__defaultVhost__&autostart=true&server=192.168.1.170&app=live&stream=livestream_ff
[jwplayer]: http://winlinvip.github.io/srs.release/trunk/research/players/jwplayer6.html?vhost=__defaultVhost__&hls_autostart=true&server=192.168.1.170&app=live&stream=livestream&hls_port=8080
