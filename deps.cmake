# 依赖包括:
# - boost
# - libevent
# - thrift
# - protobuf
# - snappy
# - log4cpp
# - glog
# - sqlite
# - hiredis
# - jdk
# - hadoop
#


if (APPLE)
    set(OPENSSL_ROOT_DIR /usr/local/Cellar/openssl/1.0.2o_2)
endif()

find_path(BOOST_H NAMES boost/version.hpp)
if (NOT BOOST_H)
    message(FATAL_ERROR "header not found")
endif()

find_library(BOOST_SYSTEM_LIB NAMES boost_system)
if (NOT BOOST_SYSTEM_LIB)
    message(FATAL_ERROR "BOOST_SYSTEM_LIB not found")
endif()

find_library(BOOST_THREAD_LIB NAMES boost_thread)
if (NOT BOOST_THREAD_LIB)
    message(FATAL_ERROR "BOOST_THREAD_LIB not found")
endif()

find_library(BOOST_REGEX_LIB NAMES boost_regex)
if (NOT BOOST_REGEX_LIB)
    message(FATAL_ERROR "BOOST_REGEX_LIB not found")
endif()

find_library(BOOST_TEST_LIB NAMES boost_unit_test_framework)
if (NOT BOOST_TEST_LIB)
    message(FATAL_ERROR "BOOST_TEST_LIB not found")
endif()

find_library(BOOST_OPTIONS_LIB NAMES boost_program_options)
if (NOT BOOST_OPTIONS_LIB)
    message(FATAL_ERROR "BOOST_OPTIONS_LIB not found")
endif()

#find_library(BOOST_LOG_LIB NAMES boost_log)
#if (NOT BOOST_LOG_LIB)
#    message(FATAL_ERROR "BOOST_LOG_LIB not found")
#endif()

find_path(LIBEVENT_H NAMES event.h)
find_library(LIBEVENT_LIB NAMES event)
if(NOT LIBEVENT_H OR NOT LIBEVENT_LIB)
    message(FATAL_ERROR "libevent not found.")
endif()


find_path(THRIFT_H NAMES thrift/Thrift.h)
find_library(THRIFT_LIB NAMES thrift)
find_library(THRIFT_NB_LIB NAMES thriftnb)
if(NOT THRIFT_H OR NOT THRIFT_LIB OR NOT THRIFT_NB_LIB)
    message(FATAL_ERROR "thrift not found.")
endif()

find_path(PROTOBUF_H NAMES google/protobuf/message.h)
find_library(PROTOBUF_LIB NAMES protobuf)
if(NOT PROTOBUF_H OR NOT PROTOBUF_LIB)
    message(FATAL_ERROR "protobuf not found.")
endif()

find_path(SNAPPY_H NAMES snappy.h)
find_library(SNAPPY_LIB NAMES snappy)
if(NOT SNAPPY_H OR NOT SNAPPY_LIB)
    message(FATAL_ERROR "snappy not found.")
endif()



find_path(LOG4CPP_H NAMES log4cpp/Category.hh)
find_library(LOG4CPP_LIB NAMES log4cpp)
if(NOT LOG4CPP_H OR NOT LOG4CPP_LIB)
    message(FATAL_ERROR "log4cpp not found")
endif()

find_path(GLOG_H NAMES glog/logging.h)
find_library(GLOG_LIB NAMES glog)
if(NOT GLOG_H OR NOT GLOG_LIB)
    message(FATAL_ERROR "glog not found.")
endif()

#find_path(GFLAGS_H NAMES gflags/gflags.h)
#find_library(GFLAGS_LIB NAMES gflags)
#if(NOT GFLAGS_H OR NOT GFLAGS_LIB)
#    message(FATAL_ERROR "gflags not found.")
#endif()

find_path(SQLITE_H NAMES sqlite3.h)
find_library(SQLITE_LIB NAMES sqlite3)
if (NOT SQLITE_H OR NOT SQLITE_LIB)
    message(FATAL_ERROR "sqlite not found")
endif()


find_path(HIREDIS_H NAMES hiredis/hiredis.h)
find_library(HIREDIS_LIB NAMES hiredis)
if(NOT HIREDIS_H OR NOT HIREDIS_LIB)
    message(FATAL_ERROR "hiredis not found.")
endif()



include_directories(
        ${BOOST_H}
        ${LIBEVENT_H}
        ${THRIFT_H}
        ${PROTOBUF_H}
        ${SNAPPY_H}
        ${LOG4CPP_H}
        ${GLOG_H}
        ${GFLAGS_H}
        ${GLOG_H}
        ${SQLITE_H}
        ${HIREDIS_H}
)


add_compile_options(
        -g -fPIC -ggdb -Wall -pipe -fpermissive -ftemplate-depth-128
        -Wno-unused-function -Wno-unused-parameter -Wno-invalid-offsetof
        -Wno-deprecated-declarations
        -Winline -Wpointer-arith -Wwrite-strings -Woverloaded-virtual
        -Wreorder -Wswitch -Wformat
)




