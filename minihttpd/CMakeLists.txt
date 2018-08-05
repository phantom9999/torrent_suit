aux_source_directory(. MINIHTTPD_SRC)

SET(MINIHTTPD_HEADER
    closure.h
    common.h
    current_thread.h
    eventloop.h
    minihttpd.h
        path.h
    process_info.h
    process_inspector.h
    scoped_ptr.h
    spinlock.h
    strutil.h
    timestamp.h
    types.h)

ADD_LIBRARY(minihttpd STATIC ${MINIHTTPD_SRC})
INCLUDE(${PROJECT_SOURCE_DIR}/config_tracker.cmake)
