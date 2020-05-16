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
    set(BOOST_ROOT /usr/local/Cellar/boost/1.67.0_1)
endif()

#set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREAD ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(
        Boost COMPONENTS
        system
        filesystem
        thread
        log
        regex
        unit_test_framework
        test_exec_monitor
        program_options
        REQUIRED
)

if (Boost_FOUND)
    #message(STATUS"Boost_INCLUDE_DIRS:${Boost_INCLUDE_DIRS}")
    #message(STATUS"Boost_LIBRARIES:${Boost_LIBRARIES}")
    #message(STATUS"Boost_VERSION:${Boost_VERSION}")
    #include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})
else()
    message(FATAL_ERROR "boost not found.")
endif ()


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
        ${LIBEVENT_H}
        ${THRIFT_H}
        ${PROTOBUF_H}
        ${SNAPPY_H}
        ${LOG4CPP_H}
        ${Boost_INCLUDE_DIRS}
        ${SQLITE_H}
        ${HIREDIS_H}
)


add_compile_options(
        -g -fPIC -ggdb -w -Wall -pipe -fpermissive -ftemplate-depth-128
        -Wno-unused-function -Wno-unused-parameter -Wno-invalid-offsetof
        -Wno-deprecated-declarations
        -Winline -Wpointer-arith -Wwrite-strings -Woverloaded-virtual
        -Wreorder -Wswitch -Wformat
)




