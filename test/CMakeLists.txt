include(../config_test.cmake)



aux_source_directory(tracker TEST_TRACKER)
add_executable(test_tracker ${TEST_TRACKER})


target_link_libraries(
        test_tracker
        -Xlinker "-("
        ${BOOST_TEST_LIB}
        ${BOOST_OPTIONS_LIB}
        ${BOOST_SYSTEM_LIB}
        ${BOOST_FILESYSTEM_LIB}
        ${BOOST_LOG_LIB}
        ${BOOST_LOG_SETUP_LIB}
        ${BOOST_THREAD_LIB}
        ${LOG4CPP_LIB}
        -pthread
        common_lib
        -Xlinker "-)"
)





