find_path(GTEST_H NAMES gtest/gtest.h)
find_library(GTEST_LIB NAMES libgtest.a)
find_library(GTEST_MAIN_LIB NAMES libgtest_main.a)
if (APPLE)
    set(GTEST_LIB libgtest.a)
    set(GTEST_MAIN_LIB libgtest_main.a)
endif()
if(NOT GTEST_H OR NOT GTEST_LIB OR NOT GTEST_MAIN_LIB)
    message(${GTEST_H})
    message(${GTEST_LIB})
    message(${GTEST_MAIN_LIB})
    message(FATAL_ERROR "gtest not found.")
endif()

find_path(BOOST_H NAMES boost/system/error_code.hpp)
find_library(BOOST_SYSTEM_LIB NAMES libboost_system.a)
if (APPLE)
    find_library(BOOST_THREAD_LIB NAMES libboost_thread-mt.a)
elseif (UNIX)
    find_library(BOOST_THREAD_LIB NAMES libboost_thread.a)
endif()
find_library(BOOST_REGEX_LIB NAMES libboost_regex.a)
if(NOT BOOST_H OR NOT_TR1_H OR NOT BOOST_SYSTEM_LIB OR NOT BOOST_THREAD_LIB OR NOT BOOST_REGEX_LIB)
    message(FATAL_ERROR "boost library(boost_system/boost_thread/boost_regex) not found.")
endif()


include_directories(
        ${GTEST_H}
        ${BOOST_H}
)




