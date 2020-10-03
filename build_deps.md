## boost
sh bootstrap.sh
./b2 variant=release link=static threading=multi runtime-link=static -j8 install


## ccache


## log4cpp


## protobuf
./configure --enable-shared=no
make -j8
make install

## snappy
cmake .
make -j8
make install

## thrift
./configure --enable-shared=no --without-python
make -j8
make install
