# Kafka

SRS可以将信息以JSON形式发送到KAFKA集群，Spark等大数据系统从Kafka获取数据并处理。

## Config

SRS支持Kafka相关的配置如下：

```
# Apache Kafka is a high-throughput distributed messaging system.
# SRS is a Kafka producer to send message to kafka.
# @see https://kafka.apache.org/documentation.html#introduction
kafka {
    # whether enabled kafka.
    # default: off
    enabled         off;
    # the broker list, broker is <ip:port>
    # and use space to specify multple brokers.
    # for example, 127.0.0.1:9092 127.0.0.1:9093
    brokers         127.0.0.1:9092;
    # the kafka topic to use.
    # default: srs
    topic           srs;
}
```

启动后，SRS将连接到Kafka集群获取Metadata信息。

## Events

目前SRS会将如下事件写入Kafka集群：

1. accept: 当收到客户端连接时。
1. close: 当关闭客户端连接时。

Winlin 2015.9.22
