include(../config_test.cmake)



aux_source_directory(tracker TEST_TRACKER)
add_executable(test_tracker ${TEST_TRACKER})


target_link_libraries(
        test_tracker
        -Xlinker "-("
        ${Boost_LIBRARIES}
        ${LOG4CPP_LIB}
        -pthread
        common_lib
        -Xlinker "-)"
)





