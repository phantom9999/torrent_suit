cmake_minimum_required(VERSION 2.8)
project(gko_suit)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_SUPPRESS_REGENERATION 1)
set(CMAKE_USE_RELATIVE_PATHS ON)
set(CMAKE_BUILD_TYPE "Debug")
set(default_file_permissions OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ)
set(default_excute_permissions OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)


set(
        third
        /third-party/boost
        /third-party/gflags
        /third-party/glog
        /third-party/hiredis
        /third-party/json-cpp
        /third-party/libcurl
        /third-party/libevent
        /third-party/protobuf
        /third-party/thrift
        /third-party/log4cpp
        /third-party/gtest
        #/third-party/torrent
        /third-party/hadoop
        /third-party/jdk
)

set(include_dir "${PROJECT_SOURCE_DIR}/third-party/torrent/include")
set(lib_dir "${PROJECT_SOURCE_DIR}/third-party/torrent/lib")

foreach (item ${third})
    set(TMP_INCLUDE ${PROJECT_SOURCE_DIR}${item}/include)
    set(TMP_LIB ${PROJECT_SOURCE_DIR}${item}/lib)
    message(${TMP_INCLUDE})
    message(${TMP_LIB})
    set(include_dir "${include_dir}:${TMP_INCLUDE}")
    set(lib_dir "${lib_dir}:${TMP_LIB}")
    #include_directories(${TMP_INCLUDE})
    #link_directories(${TMP_LIB})
endforeach ()

#set(CMAKE_INCLUDE_PATH ${include_dir})
#set(CMAKE_LIBRARY_PATH ${lib_dir})

set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ${include_dir})
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ${lib_dir})


message(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH})
message(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH})


install(DIRECTORY conf DESTINATION conf
        FILE_PERMISSIONS ${default_file_permissions}
        DIRECTORY_PERMISSIONS ${default_excute_permissions}
        PATTERN .svn EXCLUDE)


install(DIRECTORY bin DESTINATION bin
        FILE_PERMISSIONS ${default_excute_permissions}
        DIRECTORY_PERMISSIONS ${default_excute_permissions}
        PATTERN .svn EXCLUDE)

include_directories(.)
include_directories(libtorrent_0_16_5/include)

add_subdirectory(bbts-agent)
add_subdirectory(bbts-tracker)
add_subdirectory(clientmock)
add_subdirectory(file_utils)
add_subdirectory(libtorrent_0_16_5)
add_subdirectory(minihttpd)

add_subdirectory(sqlite)
#add_subdirectory(tracker-protocol)
add_subdirectory(plugin)








