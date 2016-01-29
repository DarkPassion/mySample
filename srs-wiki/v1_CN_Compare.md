# 产品对比

对比几个知名的商业流媒体服务器，知名的开源流媒体服务器，以星级评定。难免有主观因素，以及不熟悉该产品的因素，有异议可以在群里面告知。

SRS聚焦在某些方面，这些都是由[产品定位](v1_CN_Product)决定的，有所不为才能有所为。

SRS优势的基础在于基础架构，采用ST轻量线程，SRS和其他流媒体系统的根本区别（类似于GO）就是轻量线程，参考[SRS架构](v1_CN_Architecture)。

详细的功能列表，以及研发计划，参考：[产品功能列表](v1_CN_Features)

## 术语表

下面对比用到的术语：
* RTMPS/E/T：RTMPS/E是加密协议，RTMPT是HTTP穿墙协议
* DASH：各路诸侯对抗Apple的HLS提出的类似协议
* 转换Vhost：上行publish加速往往使用特殊设备和域名，需要转换vhost为下行域名，在CDN/VDN中常用
* 配置友好：FMS/Helix/Wowza的配置是XML时代产物，不是给人看的，极其不友好。Nginx配置是当代产物，简单明了易读。
* 启动脚本：以linux服务启动的脚本，譬如init.d下面的脚本
* 源站配套系统：其他辅助系统，譬如VMS、转码、编码、收录、P2P等等。
* 扩展脚本语言：FMS有AS，crtmpd/nginx有lua，扩展语言难以调试，代码量较多时问题频繁发生，不觉得是个好东西
* 单线程：单线程能支持10K级别并发，往往采用非阻塞异步机制
* 轻量线程：轻量线程架构能使用“类同步”（本质异步）结构简化逻辑
* 代码维护性：代码量，有效注释，注释百分比，逻辑复杂性，业务复杂性
* 可追溯日志：客户端/边缘若知道自己在服务器的id，那么就很容易追溯整个流

## History

国家没有历史就没有文化，产品没有历史就没有品牌。开源软件也需要历史，只有不断更新和完善的产品才是好产品，尤其是资源紧缺的开源产品。

![Media Stream Servers History](http://winlinvip.github.io/srs.release/wiki/images/compare/100.release.png)

## Protocol

协议是服务器的基础，协议决定了关键应用场景，譬如毫秒级别延时只能用udp，秒级别延迟用RTMP，十秒级别可以用HLS。

可惜技术并不能十全十美，RTMP在PC上支持完善是因为adobe的flash支持得好，HLS在IOS上完善是因为apple支持得好，不代表RTMP协议就简单。

所以什么协议都支持的服务器往往失去了重要的方向和定位，互联网上最重要的莫过于HTTP，即HLS协议，因此目前HLS大行其道。而实际PC平台浏览器中的的重要协议，是RTMP。

![Media Stream Servers Protocol](http://winlinvip.github.io/srs.release/wiki/images/compare/200.protocol.png)

## Feature

流媒体不仅仅是能播放就可以，特别对于流媒体业务运营。配置几个参数就能转码，和写复杂的FFMPEG参数完全不一样，技术上容易1倍，到最终用户使用时会方便千百倍。

![Media Stream Servers Feature](http://winlinvip.github.io/srs.release/wiki/images/compare/300.feature.png)

## Deploy

部署不重要么？如果你有老旧的机器，运行着老系统，就不会那么认为了，不是所有系统都能更新到CENTOS6的，程序员往往喜欢写不能碰的软件，特别是系统，特别是早期的程序员。如果加上现在广泛应用的ARM呢？能在ARM上运行，不仅仅是很酷吧!

![Media Stream Servers Deploy](http://winlinvip.github.io/srs.release/wiki/images/compare/400.deploy.png)

## Arch

体系结构其实是时代特征，FMS/Helix/Wowza一看就是一个时代的，nginx/rtmpd/srs是这个时代的架构。

![Media Stream Servers Architecture](http://winlinvip.github.io/srs.release/wiki/images/compare/500.arch.png)

## CDN/VDN

对于CDN/VDN，往往有很多特殊的要求，这些只有在CDN里面的运维最清楚。CDN的软件系统，应该做到不需要运维半夜三更职守升级，这个很容易？你没有在CDN混过吧。

![Media Stream Servers CDN/VDN](http://winlinvip.github.io/srs.release/wiki/images/compare/600.cdn.png?version=1.0)

## Code

代码行数往往没有什么意义，软件的基础是代码，所以比较下代码行数也没有关系。如果代码行数相差不大，功能也差不多，那没有什么奇怪的；如果功能少一倍，代码行数差不多，我会选择行数少的；如果功能少一倍，代码却多一倍，只有脑袋有问题的才会选择前者吧。

![Media Stream Servers Code](http://winlinvip.github.io/srs.release/wiki/images/compare/700.code.png?v=3)

## SRS history

SRS的里程碑，服务器开发不是百米冲刺，而是马拉松，绝对的马拉松。

![Media Stream Servers SRS History](http://winlinvip.github.io/srs.release/wiki/images/compare/800.srs.higtory.png?v=1)

## FMS PK SRS

FMS是adobe的流媒体服务器，RTMP协议就是adobe提出来的，FMS一定是重量级的产品。

FMS比SRS优点
* 功能全面：支持RTMP/RTMPE/RTMPS/RTMPT/RTMFP流协议，AMF0/AMF3编码，SharedObject协议，HLS/HDS协议，直播和点播，支持服务器脚本，支持多码率，支持Windows和Linux平台。能想到的好像都能支持。SRS呢？可怜兮兮的只有RTMP/AMF0/HLS/直播/linux。
* 研发资源充分：adobe做FMS的多少人，看那个样子真是不少，还得不断改进和推出新版本。这个也算一个优势，不过也难讲，windows也不少人，对比起linux，服务器还是比不过后者。

SRS比FMS优点
* 简单：SRS聚焦在更小的问题域上，而问题域是所有软件复杂性的根本原因之一（参考OOAD）。因为缺乏研发资源，SRS只提供互联网流媒体所使用最广泛的功能。因为，而且只因为简化了问题域，SRS才如此简单。
* 更高性能：FMS的性能不算瓶颈，不过FMS一个进程开启246个线程的架构设计来看，FMS就是一个旧时代的产物。
* 不跨平台：FMS支持跨平台，在我看来不过是多此一举，服务器为何要跑在windows上面？大约只是为了自宫练宝典。正是SRS不跨平台，才得以略去很多麻烦事。
* 配置简单：FMS的配置巨麻烦无比，xml也是上一代技术的产物，真心xml作为配置是个不好用的东西。何况FMS的配置巨繁琐，创建vhost还得创建一个目录，拷贝配置，创建app也要建立目录，且小心了，别改错了。SRS呢？根本不用创建app，为什么要创建app？！创建vhost只要在配置文件加一行就搞定。
* 支持Reload：FMS没有Reload，所以某CDN公司的运维就很苦了，白天不能动FMS，不能加新客户，那会导致FMS重启。只能半夜三更起来，操作完了还要阿弥陀佛，因为研发一般这时候睡觉了，除了问题就只能自求多福。SRS呢？直接Reload就能生效，不影响在线用户，想怎么改都行。
* 快速重启：c/c++程序嘛，总会出问题，解决这种问题，简单的方式就是看门狗重启。FMS惨了，重启要1分钟，我去，1分钟没有流啊，客户都要找上门赔钱了，不行的。SRS重启多长时间？以毫秒计算。
* 可追溯日志：FMS的日志，就是没有什么用的东西，想知道某个IP的客户端为何死活播放不了？想知道有哪些客户端延迟较大？想知道目前服务器吞吐带宽？做梦吧，adobe根本没打算给出来，或许他们自己也不知道，哈哈。SRS呢？首先，记录完整日志，都有错误码，而且有client id，可以查询到某个客户端的整个信息（要在那么多客户端中找出一个，不简单）。其次，使用pithy print，做到智能化打印，SRS的日志输出还是比较能给人看的，不多不少。最后，SRS提供cli，能吐出json数据，想查带宽？想查流信息？cli都可以搞定，而且是json，现代系统标准接口。
* 支持热备：FMS竟然没有热备？是的，不是adobe不行，几乎都不会考虑热备。SRS边缘可以回多个源站，一个挂了切另外一个。FMS如何做？得改配置，重启，等等，重启不是要一分钟吗？是的，简单来讲，FMS不支持热备。
* 适应性强：FMS适应性如何不强？FMS4只能运行在Centos5 64位，FMS5要好点也好不到哪里去。SRS呢？估计linux-arm上都能跑，更别提什么linux发行版，什么机器，什么内存，都行！如果你有大量旧机器要跑流媒体？可以，上SRS吧。
* url格式简单：这个也算？我觉得很算。看过FMS的RTMP对应的HDS/HLS流吧？多么长的地址，多么恐怖的adbevent！谁要是能立马配置FMS的HLS，然后输入地址播放，那真的是神。SRS呢？把rtmp换成http，后面加上.m3u8就是HLS，多么简单！
* 支持转码：FMS无法对直播流在服务器端转码。SRS使用ffmpeg做了支持。adobe是大公司，应该是没有办法使用ffmpeg转码。
* 支持录制：FMS的录制是在FMLE上有个DVR的按钮，然后配置服务器端脚本实现，不靠谱。SRS的录制和时移只会做一部分，但是也会比那种脚本方案要靠谱很多（脚本不可能不出问题，亲身经历）。
* 开源代码：FMS最重要一点，不提供代码，有bug？找adobe。想要定制？找adobe。那基本上就不要有那个念想了。SRS代码都是开源的，SRS作者水平一般，所以写出来代码就需要很小心，尽量写得清楚，不然自己看不懂，哈哈。

## Wowza PK SRS

Wowza也是个很了不起的产品，据说公司快上市了，Wowza和SRS在功能上很像，不过wowza也是在功能方面比SRS多很多。

Wowza比SRS优点
* 功能全面：和FMS类似，Wowza支持的功能很多，估计比FMS不会少。

SRS比Wowza优点
* c++高性能：wowza是java的，想不通为何用java做服务器，性能肯定不高。不过实测发现没有想象的那么低，当然比起NGINX还是很低。低性能的问题就是延迟会大。不过不是所有流媒体客户都关心延迟，所以Wowza这点不算硬伤。
* 部署简单：wowza依赖于jdk，还得设置环境变量，我去。wowza的安装包也很大，jdk的也很大，都在100MB+，真的不方便。SRS多大？3MB左右，不依赖与任何外部库。一个srs，一个conf，就能跑起来了。wowza需要多少东西。。。
* 内存少：其实java都是这样，内存居高不下，没有办法，gc肯定没有那么智能。wowza吃内存是以GB算的，SRS是以KB算的，在某些没有10GB内存的服务器上，还是不要跑wowza得好。虽说内存便宜，如果内存没法那么大呢？譬如arm？
* 不跨平台：wowza使用java跨平台，技术就是这样，有好处就会有代价。SRS打死也不会考虑做非linux平台，目的就是简单。
* 配置简单：java的配置，xml，蛋疼，不喜欢所有java的配置，譬如tomcat之类。nginx的配置文件绝对是极品，是的，SRS就是抄袭的nginx的配置部分的代码。
* 支持Reload：wowza也没有听说过reload这回事吧，这个功能上用起来真是很重要，不知道为何大家都不支持。同样的，nginx的reload多么强大。reload也不是多么难的事情，srs也支持reload，这个不是从nginx抄袭的，自己写额。
* 可追溯日志：商业软件都是一副德行，生怕别人看懂日志，提供的接口也很封闭。SRS当然不会了，原因是SRS没有那么多精力做方案，只好提供接口给大家控制和使用。
* 支持热备：wowza的热备似乎是个插件，也有可能没有，这点不太确定。不过SRS原生支持热备，发生故障时切换时间以毫秒计算，也就是上层服务器没有流了，马上切换到其他服务器，用户不会断也不会有感觉。
* 开源代码：wowza也是没有代码的，比FMS好的是它提供了plugin方案。等等，plugin方案和nginx的模块，哪个好？当然是后者，后者直接编译进去，接口都可见，甚至把nginx自己代码改了都可以。SRS不支持nginx的模块，原因是觉得那个太麻烦，本身代码就没有多少，不如直接改。

## NginxRtmp PK SRS

可以说，nginx-rtmp是最现代化的流服务器，几乎无可挑剔，所以现在崛起也很快。主要得益于nginx的基础做得好。

nginx-rtmp是2012.3.17发布的0.0.1（1.0是在2013.5.27），基本上那个时候开始做的。参考：[nginx-rtmp release 0.0.1](https://github.com/arut/nginx-rtmp-module/releases/tag/v0.0.1)。

nginx-rtmp(1.1.4版本)的代码行数是30957行代码，和SRS(0.9.90 33679行，另外st有4839行)是差不多的，功能和SRS差不多吗？

可惜，nginx-rtmp不能单独运行，得基于nginx运行。nginx1.5的代码是141534行，核心的服务器部分（core, event)是37678行。也就是说，nginx-rtmp实际上是68883行，是SRS（38610行）的1.784倍，功能能有SRS的2倍吗？这就是ST强大的地方吧。

SRS的注释（可使用工具research/code-statistic/csr.py统计）是5944行，占总代码行数的17.65%。ST的注释是754行，占代码总行数比例为15.58%。合在一起是6698行，占总数的17.39%。

Nginx的注释是1644行，占代码总行数的4.36%。NginxRTMP的注释是946行，占代码总行数的3.06%。混合在一起的行数是2598行，占总行数的3.77%。

nginx-rtmp比SRS优点
* 作者牛逼：能在nginx上写rtmp扩展的人，真心是牛逼。SRS作者以前做过类似的事情，不是在nginx上，是照着nginx的底层结构，用linux/epoll/多进程单线程/非阻塞异步socket实现RTMP协议，发现越到后面那个异步状态处理越麻烦。不得不承认，nginx-rtmp作者能力比SRS作者能力高出N个数量级。
* 支持多进程：nginx的多进程是个硬指标。SRS有研发计划，但目前还没有支持多进程（多进程不Simple），好消息是在不久将来，SRS就可以在这点上不成为弱点了。

SRS比nginx-rtmp优点
* 简单：nginx高性能，原因是直接使用异步非阻塞socket。SRS本质上也是，所以说和nginx同源架构，但是在另外一个牛人的指点下，用了st这个库，避免了异步socket那些状态处理。所以SRS看起来的逻辑很简单。我一直以为，nginx-rtmp最大的挑战就是不稳定，太复杂了，越发展应该是越明显，不过nginx-rtmp作者很强大，这个就很难估计了。
* Vhost：nginx-rtmp作者估计没有用过FMS，因为FMS的Vhost在客户较多时，会很有用（是个必选），也可能是支持vhost会导致RTMP状态更多吧。总之，没有vhost，对于CDN这种公司，有很多客户用同一套流媒体时，是不行的（如何计费呢？）
* RTMP边缘：或许nginx-rtmp的pull和push也算边缘，但是实际上不完全是，边缘应该只需要知道源站的ip，所有信息都从源站取。这样对于大规模部署很方便。另外和上面一点相关，配置应该基于vhost，而不是app，app是不需要配置的，只有vhost才需要，配置vhost后随便客户推什么流上来啦。
* 转码：nginx-rtmp其实也可以用ffmpeg转码，也是用ffmpeg，不过稍微没有往前走一步。nginx-rtmp的转码是通过事件，类似SRS的HTTP callback，在连接上来时转码。SRS往前走了一步，在配置文件里配置转码信息，SRS会自动管理进程，kill或者重启。使用起来还是方便不少的。
* 代码少：nginx-rtmp是基于nginx的，nginx是web服务器，专业的牛逼的web服务器。所以nginx-rtmp的代码总数是16万行，而srs只有2万行。如果要在arm上编译，还是srs稍微瘦一点。如果打算维护，还是维护2万行代码的产品会好些。
* 中文文档：SRS中文文档基本覆盖了SRS的功能，而且会根据大家的问题更新，还是很适合中文水平不错的人。同时，SRS也提供英文文档。

我也fork了nginx-rtmp代码，RTMP和HLS部分都是参考了nginx-rtmp，大牛还是大牛啊。nginx-rtmp 1.1.4的一些提交，还是在fix crash，直接异步的方式做RTMP还是比较难的：

![nginx-rtmp crash](http://winlinvip.github.io/srs.release/wiki/images/nginx-rtmp-1.1.4-crash.png)

对比下代码，响应connect-app这个包的发送的代码：

![nginx-pk-srs.send-conn-response](http://winlinvip.github.io/srs.release/wiki/images/nginx-pk-srs.send-conn-response.png)

这个就是同步和异步socket的区别，以及问题的分解导致的一致性（组包和发包两个层次，而不是nginx那样设置数据，更改全局配置，调用发送函数），对象层次的互动和数据操作（或者说数据隐藏和层次化，和数据结构）这两个编程方法的区别。

## Red5 PK SRS

Red5就算了，100个连接就不行了，有wowza的java的弱点，也没有特别的优点，就不要pk了。同是开源软件，相煎何太急。

## crtmpd PK SRS

crtmpd（rtmpserver），c++的RTMP服务器，但是SRS也是C++的，私下以为crtmpd是以c的思维习惯来写c++代码，就像c++作者讲的，拿着c++这个电钻当铁锤锤钉子————不仅仅没有效果，还可能会砸到自己的手。

crtmp(svnversion为811版本）的代码行数是93450行代码，是SRS(0.9.90 38518行，其中st有4839行)的2.426倍，功能不会比SRS多3倍吧？这就是ST强大的地方。

SRS的注释（可使用工具research/code-statistic/csr.py统计）是5944行，占总代码行数的17.65%。ST的注释是754行，占代码总行数比例为15.58%。合在一起是6698行，占总数的17.39%。

CRTMPD的注释是15891行，占代码总行数的17.00%。

```bash
# CRTMPD
python ~/srs/research/code-statistic/csr.py ~/git/crtmpserver/sources/ *.h,*.cpp .svn,tests
#total:93450 code:77559 comments:15891(17.00%) block:13123 line:2768
```

```bash
#NGINX1.5(event,core)
python ~/srs/research/code-statistic/csr.py ~/tools/nginx-rtmp/nginx-1.5.7/src/ *.h,*.c http,mail,misc,os
#total:37678 code:36034 comments:1644(4.36%) block:1644 line:0
```

```bash
#NGINX-RTMP1.1.4
python ~/srs/research/code-statistic/csr.py ~/tools/nginx-rtmp/nginx-rtmp-module-1.1.4/ *.h,*.c
#total:30957 code:30011 comments:946(3.06%) block:946 line:0
```

```bash
# NGINX(event,core)+NGINX-RTMP
python ~/srs/research/code-statistic/csr.py ~/tools/nginx-rtmp/mixed/ *.h,*.c
#total:68883 code:66285 comments:2598(3.77%) block:2598 line:0
```

```bash
# ST
python ~/srs/research/code-statistic/csr.py ~/srs/objs/st-1.9/ *.h,*.c examples,extensions,LINUX
#total:4839 code:4085 comments:754(15.58%) block:754 line:0
```

```bash
# SRS
python ~/srs/research/code-statistic/csr.py ~/srs/src *.*pp utest,upp
#total:33679 code:27735 comments:5944(17.65%) block:4126 line:1818
```

而且，crtmpd还支持lua，这个是开源软件的通病，喜欢什么都往里面加，窃以为不可取。所以有人抱怨说crtmpd太大，是的，大得不得了。

我测试过crtmpd性能，还是不错的，应该和SRS差不多。可惜crtmpd走的是单进程方向，各种扩展和协议，没有支持多进程和边缘集群方向，所以大家道不同不相为谋，也没有什么好比较的了。

## 其他

以上就是我所知道的流媒体服务器，特别是是直播流媒体服务器，目前来看SRS还是相当让我满意的。

如果你希望知道其他服务器和SRS的PK结果，在QQ群里告知我，我看看然后加上。如果还不错，可以PK一下的话。

Winlin 2014.5
