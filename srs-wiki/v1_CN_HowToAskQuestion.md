[**HOME**](Home) > [**CN(1.0)**](v1_CN_Home) > [**HowToAskQuestion**](v1_CN_HowToAskQuestion)

为了减少沟通成本，帮助大家直接解决问题，提问前请阅读这个wiki，帮助大家提出更好的问题，更快速高效的解决问题。

提问前请先在[wiki][wiki]中找您需要的答案，如果wiki没有在[issues][issues]中找找，如果还是没有就可以在[群里][group]问问，群里也不知道就提[issues][new]。

## 常见问题

提问前需要做什么？确保别人已经碰到过类似的问题，或者wiki中已经说明了，这种问题就不要提了。

下面是一些基本的问题：

* 如何编译：参考[Build](v1_CN_Build)
* SRS需要什么硬件环境：参考[Build](v1_CN_Build)
* SRS需要什么软件环境：参考[Build](v1_CN_Build)
* 为何看不到流？可能是防火墙问题，参考[Build](v1_CN_Build)
* 如何搭建集群：参考[Cluster](v1_Cluster)
* 如何测SRS性能：参考[Performance](v1_CN_Performance)
* SRS的授权是MIT？参考[License](https://github.com/simple-rtmp-server/srs/blob/master/LICENSE)
* 如何看SRS的DEMO？参考[Readme](https://github.com/simple-rtmp-server/srs/tree/1.0release#usagesimple)
* 如何一步一步编译SRS的DEMO？参考[Readme](https://github.com/simple-rtmp-server/srs/tree/1.0release#usagedetail)
* SRS的主要作者是谁？参考[Readme](https://github.com/simple-rtmp-server/srs/tree/1.0release#authors)
* SRS的架构是什么？参考[Readme](https://github.com/simple-rtmp-server/srs/tree/1.0release#architecture)
* SRS的功能有哪些，哪些是开发中的？参考[Readme](https://github.com/simple-rtmp-server/srs/tree/1.0release#summary)
* SRS发布的版本有哪些？参考[Readme](https://github.com/simple-rtmp-server/srs/tree/1.0release#releases)
* SRS和Nginx-Rtmp/CRtmpServer/Red5/Wowza/FMS/Helix相比，优势在哪里？参考[Readme](https://github.com/simple-rtmp-server/srs/tree/1.0release#compare)
* SRS开发人员每天都在做些什么？参考[Reaame](https://github.com/simple-rtmp-server/srs/tree/1.0release#history)

如果上面都不是你要提的问题，查看[Wiki](https://github.com/simple-rtmp-server/srs/wiki)，若Wiki都翻遍了，还是没有，那就参考下面的提问要求提问吧。

## 提问范例

提问时，需要收集以下重要信息：
* 问题描述：先描述问题。
* 运行环境：操作系统（位数，版本），服务器多少台，服务器IP等信息
* 网络架构：编码器如何推流到SRS，SRS如何分发到播放器。
* SRS的版本：是master分支最新代码，还是某个版本。执行命令获取SRS的版本：`./objs/srs -v`
* 编码器推流方式：不要说用ffmpeg推流，要说明具体的方式。
* SRS的配置文件：请不要说我是参考的哪个wiki，因为wiki上的大家也记不住，直接把配置文件内容粘贴出来。
* SRS的启动脚本：请不要参考的README的启动方式，给出具体的启动方式。
* 客户端播放的方式：不要说客户端播放不了，应该说明详细的播放方式，以及日志。
* SRS服务器日志：把SRS服务器日志发出来，可以和配置一起打个包。

举个实际的例子：

    提交bug，或者问问题时，说明以下情况：
    * 系统：什么操作系统？32位还是64位？编译器版本多少？
    * 编码器：用什么编码器？版本是什么？编码参数是什么？流地址是什么？
    * 服务器：用SRS什么版本？配置是什么？日志是什么？
    * 客户端：用什么客户端？版本是什么？
    * 问题和重现步骤：问题是什么？重现步骤是什么？

这个问题就很快能得到排查，开发人员能按照重现步骤进行复现。

Winlin 2014.1

[wiki]: https://github.com/simple-rtmp-server/srs/wiki/Home
[issues]: https://github.com/simple-rtmp-server/srs/issues?q=
[group]: https://github.com/simple-rtmp-server/srs/wiki/v1_CN_Contact
[new]: https://github.com/simple-rtmp-server/srs/issues/new
