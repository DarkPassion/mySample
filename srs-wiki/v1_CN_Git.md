[**HOME**](Home) > [**CN(1.0)**](v1_CN_Home) > [**Git**](v1_CN_Git)

# GIT

如何选择SRS的稳定版本？如何更新代码？

## Fast Checkout

很多人希望缩减仓库大小，很多doc和3rdparty中的第三方代码压缩包。我在这里集中说明一次，为何需要把doc收集那么全？这个不言自明，srs中23%的代码都是注释，注释会说明是哪个文档的哪一页，难道还需要再下载一次这个文档吗？为何需要把依赖的第三方代码放进来，譬如ffmpeg和nignx还有openssl？谁会无聊到再执行几个命令去下载？

为何不做这些改变？这些是次要复杂度，仓库多大对于代码质量没有任何影响。而且更重要的是，国内很多git镜像站点，SRS是同步更新的，阿里云提供服务的oschina，我git clone一次是40秒左右。难道一定得是3秒clone下来才叫做满意吗？不要纠缠这个问题了。

如何使用国内镜像站点clone，然后把服务器设置为github上？这样和直接从github上clone一模一样了。执行下面的命令就可以了：

```
git clone https://git.oschina.net/winlinvip/srs.oschina.git && cd srs.oschina &&
git remote set-url origin https://github.com/simple-rtmp-server/srs.git
```

其他国内镜像参考：https://github.com/simple-rtmp-server/srs/tree/develop#mirrors

## Checkout Branch

有些功能只有SRS2.0有，SRS1.0没有，请注意看wiki是v1还是v2的。

如果是1.0的功能，更新代码后要改变到1.0分支：

```
git pull && git checkout 1.0release
```

如果是2.0的功能，更新代码后要改变到2.0分支：

```
git pull && git checkout 2.0release
```

如果是3.0的功能，更新代码后要改变到3.0分支（没有单独的3.0release分支就是develop）：

```
git pull && git checkout develop
```

注意：master是作为稳定版本分支。

## SRS Branches

稳定的SRS版本会新建branch，譬如1.0release。1.0release是1.0的release，有最新的bug fix。

Master分支是主要的release，目前就是1.0。对比1.0release分支，master分支bug fix频率低，每隔1-2个月会集中merge 1.0release的bug fix到master。

develop是开发版，目前是2.0。

所以稳定度master >= 1.0release >> develop.

## Get SRS Code

注意：中国国内访问github慢，可以使用oschina或csdn镜像，参考下面的其他镜像。

你可以从github获取SRS的代码，需要先安装git（如何安装git可以百度下）。

安装好git后，执行以下命令获取SRS最新代码：

```bash
git clone https://github.com/simple-rtmp-server/srs
```

其他镜像git地址: https://github.com/simple-rtmp-server/srs/tree/1.0release#mirrors

## Update SRS Code

SRS包含的软件很多，不需要额外下载其他的包就可以编译。所以第一次从github上下载后，不需要每次都git clone，用以下命令即可更新：

```bash
git pull
```

另外，不需要删除objs后编译srs，只需要make就可以编译。若make出错，则执行configure之后再make，可能有配置项更新。

## Fetch and Push

具体参考：[Pro Git](http://git-scm.com/book/zh/v2/GitHub-%E5%AF%B9%E9%A1%B9%E7%9B%AE%E5%81%9A%E5%87%BA%E8%B4%A1%E7%8C%AE)

## Patch

可以生成patch后发送邮件给`winlin at vip.126.com`，例如针对SRS2的patch：

```
git format-patch origin/2.0release
```

Winlin 2014.3
