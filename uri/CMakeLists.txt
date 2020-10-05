aux_source_directory(. URI_AGENT_SRC)


include(${PROJECT_SOURCE_DIR}/config_agent.cmake)

#add_compile_options(
#        -W -pedantic-errors -Wall -Wextra -pedantic -Wold-style-cast -Wundef
#        -Wredundant-decls -Wwrite-strings -Wpointer-arith
#        -Wcast-qual -Wformat=2 -Wmissing-include-dirs -Wcast-align
#        -Wctor-dtor-privacy -Wdisabled-optimization -Winvalid-pch -Woverloaded-virtual
#        -Wconversion -Wswitch-enum -Wundef -Winline -Wreorder -Wswitch
#)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -pipe -fpermissive")

add_library(uri_agent STATIC ${URI_AGENT_SRC})

INSTALL(TARGETS uri_agent ARCHIVE DESTINATION lib)

