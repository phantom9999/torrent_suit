#! /bin/bash
set -x

sh proto_gen.sh ;
cmake3 .;
make -j4

