include(../config_test.cmake)



aux_source_directory(tracker TEST_TRACKER)
add_executable(test_tracker ${TEST_TRACKER})


target_link_libraries(
        test_tracker
        ${BOOST_TEST_LIB}
        ${BOOST_OPTIONS_LIB}
)





