if (GFLAGS_NAMESPACE)
    message(${GFLAGS_NAMESPACE})
    add_definitions(-DGFLAGS_NAMESPACE=${GFLAGS_NAMESPACE})
endif()

if("$ENV{SCMPF_MODULE_VERSION}" STREQUAL "")
    set(BBTS_TRACKER_VERSION "\"1.0.0\"")
else()
    set(BBTS_TRACKER_VERSION "\"$ENV{SCMPF_MODULE_VERSION}\"")
endif()

add_definitions(-DHAVE_NETINET_IN_H
                -DHAVE_NETDB_H=1
                -DBBTS_TRACKER_VERSION=${BBTS_TRACKER_VERSION})
if(BUILD_BAIDU)
    add_definitions(-DBUILD_BAIDU)
endif()
#set(CMAKE_CXX_FLAGS "-g -fPIC -Wall -pipe -W -fpermissive -Wno-unused-function -Wno-unused-parameter -Wno-invalid-offsetof -Winline -Wpointer-arith -Wwrite-strings -Woverloaded-virtual -ftemplate-depth-128 -Wreorder -Wswitch -Wformat")

SET(default_file_permissions OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ)
SET(default_excute_permissions OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
