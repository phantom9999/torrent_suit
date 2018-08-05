include(${PROJECT_SOURCE_DIR}/config_agent.cmake)

find_path(HDFS_H NAMES hdfs.h)
if(NOT HDFS_H)
    # todo:
    set(HDFS_H hdfs.h)
    message(FATAL_ERROR "hdfs.h not found.")
endif()
include_directories(${HDFS_H})

find_library(HDFS_LIB NAMES hdfs)
if(NOT HDFS_LIB)
    # todo:
        set(HDFS_LIB hdfs)
    message(FATAL_ERROR "hdfs not found.")
endif()

find_library(JSIG_LIB NAMES jsig)
if(NOT JSIG_LIB)
    message(FATAL_ERROR "jsig not found.")
endif()

find_library(JVM_LIB NAMES jvm)
if(NOT JVM_LIB)
    message(FATAL_ERROR "jvm not found.")
endif()

add_library(hdfstool SHARED hdfstool)
target_link_libraries(hdfstool ${JSIG_LIB} ${JVM_LIB} ${HDFS_LIB})
install(TARGETS hdfstool LIBRARY DESTINATION lib)

