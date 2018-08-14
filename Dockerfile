FROM opensuse

MAINTAINER phantom9999

RUM zypper install -y boost_1_58_0-devel libevent-devel sqlite3-devel libthrift-devel hiredis-devel protobuf-devel snappy-devel log4cpp-devel libopenssl-devel cmake make



ADD . /root/torrent
RUN cd /root/torrent/; sh proto_gen.sh ;cmake . -DBOOST_TEST_DYN_LINK=1; make -j4


