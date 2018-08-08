find_path(GTEST_H NAMES gtest/gtest.h)
find_library(GTEST_LIB NAMES gtest)
find_library(GTEST_MAIN_LIB NAMES gtest_main)
if (APPLE)
    set(GTEST_LIB gtest)
    set(GTEST_MAIN_LIB gtest_main)
endif()
if(NOT GTEST_H OR NOT GTEST_LIB OR NOT GTEST_MAIN_LIB)
    message(${GTEST_H})
    message(${GTEST_LIB})
    message(${GTEST_MAIN_LIB})
    message(FATAL_ERROR "gtest not found.")
endif()

find_path(BOOST_H NAMES boost/system/error_code.hpp)
find_library(BOOST_SYSTEM_LIB NAMES boost_system)
if (APPLE)
    find_library(BOOST_THREAD_LIB NAMES boost_thread-mt)
elseif (UNIX)
    find_library(BOOST_THREAD_LIB NAMES boost_thread)
endif()
find_library(BOOST_REGEX_LIB NAMES boost_regex)
if(NOT BOOST_H OR NOT_TR1_H OR NOT BOOST_SYSTEM_LIB OR NOT BOOST_THREAD_LIB OR NOT BOOST_REGEX_LIB)
    message(FATAL_ERROR "boost library(boost_system/boost_thread/boost_regex) not found.")
endif()


find_path(GLOG_H NAMES glog/logging.h)
find_library(GLOG_LIB NAMES glog)
if(NOT GLOG_H OR NOT GLOG_LIB)
    message(FATAL_ERROR "glog not found.")
endif()


include_directories(
        ${GTEST_H}
        ${BOOST_H}
        ${GLOG_H}
)




