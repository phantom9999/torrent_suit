aux_source_directory(. GKO3_TRACKER_SRC)
aux_source_directory(tracker GKO3_TRACKER_SRC)
aux_source_directory(proto GKO3_TRACKER_SRC)

INCLUDE(${PROJECT_SOURCE_DIR}/config_tracker.cmake)

ADD_EXECUTABLE(bbts-tracker ${GKO3_TRACKER_SRC})

TARGET_LINK_LIBRARIES(bbts-tracker
        -Xlinker "-("
        gko3-protocol
        pb_conf
        common_lib
        ${THRIFT_NB_LIB}
        ${THRIFT_LIB}
        minihttpd
        ${HIREDIS_LIB}
        ${Boost_LIBRARIES}
        ${Protobuf_LIBRARIES}
        ${LIBEVENT_LIB}
        ${FORMAT_LIB}
        pthread rt
        -Xlinker "-)"
        )

INSTALL(
        TARGETS bbts-tracker
        RUNTIME
        DESTINATION bin)
