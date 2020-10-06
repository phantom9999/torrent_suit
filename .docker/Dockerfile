FROM centos:7

RUN yum install -y cmake gcc-c++ make autoconf automake libtool flex byacc zlib-devel openssl-devel sqlite-devel; \
        yum clean all; \
        rm -rf /var/cache/yum/*;


#RUN tar -xzf openssl-1.0.1g.tar.gz; cd openssl-1.0.1g; make -j8 && make install; cd ..; rm -rf openssl-1.0.1g;

RUN tar -xf snappy-1.1.8.tar.gz; \
    cd snappy-1.1.8; \
    cmake .; \
    make -j8 && make install; \
    cd ..; rm -rf snappy-1.1.8;

RUN tar -xf libevent-2.1.12-stable.tar.gz; \
    cd libevent-2.1.12-stable; \
    ./configure --enable-shared=no --with-pic; \
    make -j8 && make install; \
    cd ..; rm -rf libevent-2.1.12-stable;

RUN tar -xf protobuf-3.7.0.tar.gz; \
    cd protobuf-3.7.0; \
    sh autogen.sh; \
    ./configure --with-pic --enable-shared=no; \
    make -j8 && make install; \
    cd ..; rm -rf protobuf-3.7.0;

RUN tar -xf boost_1_58_0.tar.gz \
    cd boost_1_58_0; \
    sh bootstrap.sh; \
    ./b2 variant=release link=static threading=multi runtime-link=shared -j8 install \
    cd ..; rm -rf boost_1_58_0;

RUN tar -xf thrift-0.10.0.tar.gz; \
    cd thrift-0.10.0; \
    sh bootstrap.sh; \
    ./configure --with-perl=no --with-python=no --with-pic --enable-shared=no; \
    make -j8 && make install; \
    cd ..; rm -rf thrift-0.10.0;

RUN tar -xf fmt-7.0.3.tar.gz; \
    cd fmt-7.0.3; \
    cmake -DFMT_TEST=NO .; make -j8 && make install; \
    cd ..; rm -rf fmt-7.0.3;

RUN tar -xf hiredis-0.13.3.tar.gz \
    cd hiredis-0.13.3; \
    make -j8 && make install; \
    rm /usr/local/lib/libhiredis.so.0.13 /usr/local/lib/libhiredis.so; \
    cd ..; rm -rf hiredis-0.13.3;


