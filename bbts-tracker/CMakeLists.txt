aux_source_directory(. GKO3_TRACKER_SRC)
aux_source_directory(tracker GKO3_TRACKER_SRC)
aux_source_directory(proto GKO3_TRACKER_SRC)

INCLUDE(${PROJECT_SOURCE_DIR}/config_tracker.cmake)

ADD_EXECUTABLE(gko3-tracker ${GKO3_TRACKER_SRC})

TARGET_LINK_LIBRARIES(gko3-tracker
        gko3-protocol
        pb_conf
        common_lib
        ${THRIFT_NB_LIB}
        ${THRIFT_LIB}
        minihttpd
        ${HIREDIS_LIB}
        ${GLOG_LIB}
        ${PROTOBUF_LIB}
        ${LIBEVENT_LIB}
        ${BOOST_SYSTEM_LIB}
        ${BOOST_THREAD_LIB}
        ${BOOST_OPTIONS_LIB}
        pthread
        rt)

INSTALL(TARGETS gko3-tracker RUNTIME DESTINATION bin)
