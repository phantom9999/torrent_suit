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
)
dep_dir=${workspace}/../gko-third-party

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