#! /bin/bash
set -x

zypper install -y \
    libboost_system1_66_0-devel \
    libboost_program_options1_66_0-devel \
    libboost_log1_66_0-devel \
    libboost_filesystem1_66_0-devel \
    libboost_thread1_66_0-devel \
    libboost_regex1_66_0-devel \
    libboost_test1_66_0-devel \
    libevent-devel \
    sqlite3-devel \
    libthrift-devel \
    hiredis-devel \
    protobuf-devel \
    snappy-devel \
    log4cpp-devel \
    libopenssl-devel \
    cmake make thrift

sh proto_gen.sh ;
cmake . -DBOOST_TEST_DYN_LINK=1; 
make -j4

