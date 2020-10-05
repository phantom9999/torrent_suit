# proto目录下的文件:
#       proto/configure.pb
#       proto/message.pb
#       proto/routing.pb
#       proto/StatAnnounce
#       proto/stat_constants
#       proto/stat_types
#       proto/TorrentProvider_constants
#       proto/TorrentProvider_types
#        proto/TorrentProviderService
#        proto/transfer_server_constants
#        proto/transfer_server_types
#        proto/TransferServer
#        proto/tracker_constants
#        proto/tracker_types
#        proto/announce_constants
#        proto/announce_types
#        proto/Announce
#
#

include(${PROJECT_SOURCE_DIR}/config_agent.cmake)

aux_source_directory(. GKO_SRC)
aux_source_directory(agent AGENT_SRC)
aux_source_directory(proto GKO_SRC)
aux_source_directory(tool CLIENT_SRC)
aux_source_directory(torrent_plugins GKO_SRC)


add_library(gko3 ${GKO_SRC})
add_executable(bbts-agent ${AGENT_SRC})
add_executable(bbts-client ${CLIENT_SRC})


target_link_libraries(
        bbts-agent
        -Xlinker "-("
        gko3
        uri_agent
        pb_conf
        common_lib
        torrent-rasterbar
        ${SQLITE_LIB}
        ${Boost_LIBRARIES}
        ${THRIFT_LIB}
        ${Protobuf_LIBRARIES}
        ${SNAPPY_LIB}
        ${ZLIB_LIBRARIES}
        ${OPENSSL_LIBRARIES}
        ${FORMAT_LIB}
        pthread dl rt
        -Xlinker "-)"
)

target_link_libraries(
        bbts-client
        -Xlinker "-("
        gko3
        uri_agent
        pb_conf
        common_lib
        torrent-rasterbar
        ${SQLITE_LIB}
        ${Boost_LIBRARIES}
        ${THRIFT_LIB}
        ${Protobuf_LIBRARIES}
        ${SNAPPY_LIB}
        ${ZLIB_LIBRARIES}
        ${OPENSSL_LIBRARIES}
        ${FORMAT_LIB}
        pthread dl rt
        -Xlinker "-)"
)


set_target_properties(
        bbts-agent PROPERTIES
        INSTALL_RPATH ".:$ENV{HADOOP_HOME}/lib/native:$ENV{JAVA_HOME}/jre/lib/amd64/server"
)

set_target_properties(
        bbts-client PROPERTIES
        INSTALL_RPATH ".:$ENV{HADOOP_HOME}/lib/native:$ENV{JAVA_HOME}/jre/lib/amd64/server"
)

install(
        TARGETS bbts-agent bbts-client
        RUNTIME DESTINATION bin
)
