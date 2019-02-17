INCLUDE(${PROJECT_SOURCE_DIR}/config_tracker.cmake)
if (APPLE)
SET(MOCK_LINK_LIBS gko3-protocol ${THRIFTNB_LIB} ${THRIFT_LIB} ${Boost_LIBRARIES} pthread)
    else(APPLE)
    SET(MOCK_LINK_LIBS gko3-protocol ${THRIFTNB_LIB} ${THRIFT_LIB} ${Boost_LIBRARIES} pthread rt)
endif(APPLE)
ADD_EXECUTABLE(client_mock_infohash client_mock_infohash.cpp)
TARGET_LINK_LIBRARIES(client_mock_infohash ${MOCK_LINK_LIBS})

ADD_EXECUTABLE(client_mock client_mock.cpp)
TARGET_LINK_LIBRARIES(client_mock ${MOCK_LINK_LIBS})

ADD_EXECUTABLE(client_mock_s client_mock_s.cpp)
TARGET_LINK_LIBRARIES(client_mock_s ${MOCK_LINK_LIBS})

ADD_EXECUTABLE(ControlByInfohash ControlByInfohash.cpp)
TARGET_LINK_LIBRARIES(ControlByInfohash ${MOCK_LINK_LIBS})

INSTALL(
        TARGETS client_mock_infohash client_mock client_mock_s ControlByInfohash
        RUNTIME
        DESTINATION clientmock
)