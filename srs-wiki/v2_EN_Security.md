# Security

SRS provides simple security strategy to allow or deny specifies clients.

## Config

The config for security of vhost:

```
vhost your_vhost {
    # security for host to allow or deny clients.
    # @see https://github.com/simple-rtmp-server/srs/issues/211   
    security {
        # whether enable the security for vhost.
        # default: off
        enabled         on;
        # the security list, each item format as:
        #       allow|deny    publish|play    all|<ip>
        # for example:
        #       allow           publish     all;
        #       deny            publish     all;
        #       allow           publish     127.0.0.1;
        #       deny            publish     127.0.0.1;
        #       allow           play        all;
        #       deny            play        all;
        #       allow           play        127.0.0.1;
        #       deny            play        127.0.0.1;
        # SRS apply the following simple strategies one by one:
        #       1. allow all if security disabled.
        #       2. default to deny all when security enabled.
        #       3. allow if matches allow strategy.
        #       4. deny if matches deny strategy.
        allow           play        all;
        allow           publish     all;
    }
}
```

Please see `conf/security.deny.publish.conf` for detail.

## Kickoff Client

SRS provides api to kickoff user, read [wiki](https://github.com/simple-rtmp-server/srs/wiki/v2_EN_HTTPApi#kickoff-client).

## Bug

The bug about this feature, read [#211](https://github.com/simple-rtmp-server/srs/issues/211)

## Reload

When reload the security config, it only effects the new clients.

Winlin 2015.1
