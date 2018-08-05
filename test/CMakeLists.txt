include(../config_test.cmake)


aux_source_directory(. TEST_SRC)


add_executable(test_src ${TEST_SRC})

target_link_libraries(
        test_src
        ${GTEST_MAIN_LIB}
        ${GTEST_LIB}
        -pthread

)






