aux_source_directory(. URI_AGENT_SRC)


include(${PROJECT_SOURCE_DIR}/config_agent.cmake)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fPIC -ggdb -Wall -pipe -fpermissive -Wno-unused-function -Wno-unused-parameter -Wno-invalid-offsetof -Winline -Wpointer-arith -Wwrite-strings -Woverloaded-virtual -ftemplate-depth-128 -Wreorder -Wswitch -Wformat")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")

add_library(uri_agent STATIC ${URI_AGENT_SRC})

INSTALL(TARGETS uri_agent ARCHIVE DESTINATION lib)

