#!/bin/bash

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

export CMAKE_CXX_COMPILER="ccache"
export CMAKE_CXX_COMPILER_ARG1="g++"
export CMAKE_C_COMPILER="ccache"
export CMAKE_C_COMPILER_ARG1="gcc"

