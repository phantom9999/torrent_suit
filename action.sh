#! /bin/bash
set -e

sh proto_gen.sh ;
cmake . -DBOOST_TEST_DYN_LINK=1; 
make -j4

