#!/usr/bin/env bash
# 依赖boost
# 来源: https://github.com/arvidn/libtorrent/archive/libtorrent_0_16_5.tar.gz
# 补丁: libtorrent-0.16.5.patch
# 打补丁命令: patch -p1 < ../libtorrent-0.16.5.patch
# 生成 configure的脚本: autoreconf --verbose --install --force
# 编译参数:
# --with-boost=$(BOOST) \
#			--enable-debug=no \
#			--disable-dht \
#			--disable-geoip \
#			--disable-deprecated-functions \
#			--enable-examples=no \
#			--disable-encryption \
#			--enable-tests=no \
#			--enable-static=yes \
#			--enable-shared=no \
#			--prefix=$(PREFIX) \
#
#
#
#