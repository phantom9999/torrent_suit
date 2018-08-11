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



aux_source_directory(. AGENT_SRC)
aux_source_directory(agent AGENT_SRC)
aux_source_directory(proto AGENT_SRC)
aux_source_directory(tool AGENT_SRC)


include(${PROJECT_SOURCE_DIR}/config_agent.cmake)
add_executable(gko-agent ${AGENT_SRC})

target_link_libraries(
        gko-agent
        -Xlinker "-("
        uri_agent
        pb_conf
        common_lib
        torrent-rasterbar
        ${SQLITE_LIB}
        ${LOG4CPP_LIB}
#        ${LIBTORRENT_LIB}
        ${BOOST_SYSTEM_LIB}
        ${BOOST_THREAD_LIB}
        ${BOOST_REGEX_LIB}
        ${THRIFT_LIB}
        ${PROTOBUF_LIB}
        ${SNAPPY_LIB}
        pthread dl rt ssl crypto
        -Xlinker "-)"
)



set_target_properties(
        gko-agent PROPERTIES
        INSTALL_RPATH ".:$ENV{HADOOP_HOME}/lib/native:$ENV{JAVA_HOME}/jre/lib/amd64/server"
)

install(
        TARGETS gko-agent
        RUNTIME DESTINATION bin
)
