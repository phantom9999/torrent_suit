

aux_source_directory(. SRC_COMMON)

add_library(
        common_lib
        ${SRC_COMMON}
)



INSTALL(TARGETS common_lib ARCHIVE DESTINATION lib)

