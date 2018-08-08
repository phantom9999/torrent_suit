#!/usr/bin/env bash

set -x

home=$(cd ~ && pwd)

conan_dir_list=`find $home/.conan -name include | grep package`
echo ${conan_dir_list}

for conan_dir in ${conan_dir_list[*]}
do
    dep_dir=`dirname ${conan_dir}`
    CMAKE_INCLUDE_PATH=${CMAKE_INCLUDE_PATH}:${dep_dir}/include
    CMAKE_LIBRARY_PATH=${CMAKE_LIBRARY_PATH}:${dep_dir}/lib
done

export CMAKE_INCLUDE_PATH
export CMAKE_LIBRARY_PATH

echo ${CMAKE_INCLUDE_PATH}
echo ${CMAKE_LIBRARY_PATH}

[ ! -d build ] || rm -rf build 
mkdir build
cd build
cmake -D CMAKE_CXX_COMPILER="ccache" -D CMAKE_CXX_COMPILER_ARG1="g++" -D CMAKE_C_COMPILER="ccache" -D CMAKE_C_COMPILER_ARG1="gcc" ..
make -j4
