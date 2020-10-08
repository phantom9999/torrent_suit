#! /bin/bash
set -x

sh proto_gen.sh ;
cmake .;
make -j4

