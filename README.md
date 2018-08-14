# 简介



# 构建状态
| travis-ci |
|----------|
| [![Build Status](https://travis-ci.com/phantom9999/torrent_suit.svg?branch=master)](https://travis-ci.com/phantom9999/torrent_suit) |


# bbts-agent
## 依赖
- uri
- sqlite
- plugin
- libtorrent





# bbts-tracker
## 依赖
- minihttpd
- protocol
- clientmock


## 配置文件
- tracker.conf
- redis.conf
- log.conf

tracker.conf:

```
port       : 18695
httpd_port : 8523
thread_num : 39
log_conf   : "tracker_log.conf"
redis_conf : "redis.conf"
monitor_cycle_second  : 10
info_hash_expire_time : 3600
remote_peers_syncronizer_num : 3
server_send_timeout   : 3000
server_recv_timeout   : 3000
server_accept_timeout : 3000
task_expire_time      : 3000
```

redis.conf:

```
max_queue_length : 50000
timeout: 10000
passwd: "passwd"
database: "1"

host_pair {
    master_hostname : "master-redis-host-name"
    master_port: 6379
    master_weight : 1000
    slave_hostname : "slave-redis-host-name" # could be same with master
    slave_port: 6379
    slave_weight : 1000
}
```


log.conf:

```
log_dir: logs

```








