cmake_minimum_required(VERSION 3.0)
project(gko_suit)


#set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_SUPPRESS_REGENERATION 1)
set(CMAKE_USE_RELATIVE_PATHS ON)
set(CMAKE_BUILD_TYPE "Debug")
set(default_file_permissions OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ)
set(default_excute_permissions OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

include(deps.cmake)

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
add_subdirectory(tracker-protocol)
#add_subdirectory(plugin)
add_subdirectory(uri)
add_subdirectory(pb_config)
add_subdirectory(common)

add_subdirectory(test)










