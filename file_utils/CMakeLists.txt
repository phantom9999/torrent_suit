



add_executable(file_is_locked file_is_locked.cpp)

INSTALL(
        TARGETS file_is_locked
        RUNTIME
        DESTINATION bin
)
