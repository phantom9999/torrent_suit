#!/usr/bin/env bash

home=/home/travis
home=/Users/zhangxiao

conan_dir_list=`find $home/.conan -name include | grep package`

for conan_dir in ${conan_dir_list[*]}
do
    dep_dir=`dirname ${conan_dir}`
    CMAKE_INCLUDE_PATH=${CMAKE_INCLUDE_PATH}:${dep_dir}/include
    CMAKE_LIBRARY_PATH=${CMAKE_LIBRARY_PATH}:${dep_dir}/lib
done

export CMAKE_INCLUDE_PATH
export CMAKE_LIBRARY_PATH


mkdir build
cd build
cmake ..
make -j4
