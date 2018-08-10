#!/usr/bin/env bash
# cmake -D CMAKE_CXX_COMPILER="ccache" -D CMAKE_CXX_COMPILER_ARG1="g++" -D CMAKE_C_COMPILER="ccache" -D CMAKE_C_COMPILER_ARG1="gcc" -DGFLAGS_NAMESPACE='google'

export CMAKE_CXX_COMPILER="ccache"
export CMAKE_CXX_COMPILER_ARG1="g++"
export CMAKE_C_COMPILER="ccache"
export CMAKE_C_COMPILER_ARG1="gcc"

# -D GFLAGS_NAMESPACE

workspace=$(cd $(dirname $0) && pwd)
third_party=( \
    boost \
    protobuf \
    libevent \
    snappy \
    thrift \
    log4cpp \
    hadoop \
    glog \
    gflags \
    hiredis \
    gtest \
    sqlite \
)
dep_dir=${workspace}/third-party

CMAKE_INCLUDE_PATH=''
CMAKE_LIBRARY_PATH=''

CMAKE_INCLUDE_PATH=${dep_dir}/jdk/include
CMAKE_LIBRARY_PATH=${dep_dir}/jdk/lib

for libdir in ${third_party[*]}
do
    CMAKE_INCLUDE_PATH=${CMAKE_INCLUDE_PATH}:${dep_dir}/${libdir}/include
    CMAKE_LIBRARY_PATH=${CMAKE_LIBRARY_PATH}:${dep_dir}/${libdir}/lib
done

echo export CMAKE_INCLUDE_PATH=${CMAKE_INCLUDE_PATH}
echo export CMAKE_LIBRARY_PATH=${CMAKE_LIBRARY_PATH}





