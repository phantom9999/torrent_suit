FROM fedora

MAINTAINER phantom9999


RUN dnf -y install boost-devel.x86_64 libevent-devel.x86_64 gflags-devel.x86_64 glog-devel.x86_64 thrift-devel.x86_64 hiredis-devel.x86_64 protobuf-devel.x86_64 snappy-devel.x86_64 log4cpp-devel.x86_64 java-1.8.0-openjdk-devel.x86_64 hadoop-devel.x86_64 cmake gcc-c++ openssl-devel.x86_64 gtest-devel.x86_64

ADD . /root/torrent
RUN cd /root/torrent/; cmake . ; make -j4


