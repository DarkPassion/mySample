# Kafka

SRS support to write events to Kafka cluster, from which Spark can consume data.

## Config

The config for SRS to support Kafka:

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

SRS will fetch metadata from Kafka when startup.

## Events

SRS will write following events to Kafka:

1. accept: When got a fresh client.
1. close: When close a client.

Winlin 2015.9.22
