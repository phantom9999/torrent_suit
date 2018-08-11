if("$ENV{SCMPF_MODULE_VERSION}" STREQUAL "")
    set(GINGKO_VERSION "\"1.0.0\"")
else()
    set(GINGKO_VERSION "\"$ENV{SCMPF_MODULE_VERSION}\"")
endif()

add_definitions(-D_XOPEN_SOURE=500
                -D_GNU_SOURCE
                -DHAVE_NETINET_IN_H
                -DHAVE_NETDB_H=1
                -DGINGKO_VERSION=${GINGKO_VERSION}
# todo: 禁用
                -DTORRENT_USE_OPENSSL
        #-DTORRENT_DISABLE_ENCRYPTION=1
                -DTORRENT_NO_DEPRECATE=1

                -DTORRENT_DISABLE_GEO_IP=1
                -DTORRENT_DISABLE_DHT=1
                -DBOOST_ASIO_HASH_MAP_BUCKETS=1021
                -DBOOST_ASIO_SEPARATE_COMPILATION=1
                -DBOOST_ASIO_HAS_EPOLL=1
                #-DTORRENT_DEBUG=1
                -DBOOST_EXCEPTION_DISABLE=1
                -DBOOST_ASIO_ENABLE_CANCELIO=1)

#set(CMAKE_CXX_FLAGS "-g -fPIC -ggdb -Wall -pipe -fpermissive -Wno-unused-function -Wno-unused-parameter -Wno-invalid-offsetof -Winline -Wpointer-arith -Wwrite-strings -Woverloaded-virtual -ftemplate-depth-128 -Wreorder -Wswitch -Wformat -Wno-deprecated-declarations")



set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
