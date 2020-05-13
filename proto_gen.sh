#!/usr/bin/env bash
set -x

# agent
cd bbts-agent/proto
thrift --gen cpp -out . TorrentProvider.thrift
thrift --gen cpp -out . transfer_server.thrift
thrift --gen cpp -out . stat.thrift
thrift --gen cpp -out . tracker.thrift
thrift --gen cpp -out . announce.thrift
cd -

# tracker
cd tracker-protocol
thrift --gen cpp -out . tracker.thrift
thrift --gen cpp -out . inner.thrift
thrift --gen cpp -out . announce.thrift
cd -

rm `find -name *skeleton.cpp`



# pb_config
cd pb_config
protoc --cpp_out=. *.proto
cd -





