# 依赖包括:
# - boost
# - libevent 包管理
# - thrift
# - protobuf
# - snappy
# - log4cpp
# - sqlite 包管理
# - hiredis
# - jdk
# - hadoop
# - zlib 包管理
# - openssl 包管理
#

link_directories(.)

set(Boost_USE_STATIC_LIBS ON)
#set(Boost_USE_MULTITHREAD ON)
#set(Boost_USE_STATIC_RUNTIME OFF)
find_package(
        Boost COMPONENTS
        system
        filesystem
        thread
        log
        regex
        unit_test_framework
#        test_exec_monitor
        program_options
        REQUIRED
)

#add_definitions(
#        -DBOOST_ALL_DYN_LINK
#)

if (Boost_FOUND)
    #message(STATUS"Boost_INCLUDE_DIRS:${Boost_INCLUDE_DIRS}")
    #message(STATUS"Boost_LIBRARIES:${Boost_LIBRARIES}")
    #message(STATUS"Boost_VERSION:${Boost_VERSION}")
    #include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})
    include_directories(${Boost_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "boost not found.")
endif ()


find_path(LIBEVENT_H NAMES event.h)
find_library(LIBEVENT_LIB NAMES event)
if(NOT LIBEVENT_H OR NOT LIBEVENT_LIB)
    message(FATAL_ERROR "libevent not found.")
else()
    include_directories(${LIBEVENT_H})
endif()


find_path(THRIFT_H NAMES thrift/Thrift.h)
find_library(THRIFT_LIB NAMES thrift)
find_library(THRIFT_NB_LIB NAMES thriftnb)
if(NOT THRIFT_H OR NOT THRIFT_LIB OR NOT THRIFT_NB_LIB)
    message(FATAL_ERROR "thrift not found.")
else()
    include_directories(${THRIFT_H})
endif()

find_package(Protobuf)
if (NOT Protobuf_FOUND)
    message(FATAL_ERROR "protobuf not found.")
else()
    include_directories(${Protobuf_INCLUDE_DIRS})
endif()

find_path(SNAPPY_H NAMES snappy.h)
find_library(SNAPPY_LIB NAMES snappy)
if(NOT SNAPPY_H OR NOT SNAPPY_LIB)
    message(FATAL_ERROR "snappy not found.")
else()
    include_directories(${SNAPPY_H})
endif()

find_path(SQLITE_H NAMES sqlite3.h)
find_library(SQLITE_LIB NAMES sqlite3)
if (NOT SQLITE_H OR NOT SQLITE_LIB)
    message(FATAL_ERROR "sqlite not found")
else()
    include_directories(${SQLITE_H})
endif()

find_path(HIREDIS_H NAMES hiredis/hiredis.h)
find_library(HIREDIS_LIB NAMES hiredis)
if(NOT HIREDIS_H OR NOT HIREDIS_LIB)
    message(FATAL_ERROR "hiredis not found.")
else()
    include_directories(${HIREDIS_H})
endif()

find_package(OpenSSL)
if (NOT OPENSSL_FOUND)
    message(FATAL_ERROR "openssl not found.")
else()
    include_directories(${OPENSSL_INCLUDE_DIR})
endif()

find_package(ZLIB)
if(NOT ZLIB_FOUND)
    message(FATAL_ERROR "zlib not found.")
else()
    include_directories(${ZLIB_INCLUDE_DIRS})
endif()

find_path(FORMAT_H NAMES fmt/format.h)
find_library(FORMAT_LIB NAMES libfmt.a)
if(NOT FORMAT_H OR NOT FORMAT_LIB)
    message(FATAL_ERROR "fmt not found.")
else()
    include_directories(${FORMAT_H})
endif()

add_compile_options(
        -g -fPIC -ggdb -w -Wall -pipe -fpermissive -ftemplate-depth-128
        -Wno-unused-function -Wno-unused-parameter -Wno-invalid-offsetof
        -Wno-deprecated-declarations
        -Winline -Wpointer-arith -Wwrite-strings -Woverloaded-virtual
        -Wreorder -Wswitch -Wformat
)




