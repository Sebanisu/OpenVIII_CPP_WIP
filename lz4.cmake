include(FetchContent)
FetchContent_Declare(
        lz4_lib
        GIT_REPOSITORY https://github.com/lz4/lz4.git
        GIT_TAG v1.9.2
)
FetchContent_MakeAvailable(lz4_lib)
FetchContent_GetProperties(lz4_lib)
add_library(External_lz4
        ${lz4_lib_SOURCE_DIR}/lib/lz4.h
        ${lz4_lib_SOURCE_DIR}/lib/lz4.c
        )
set_target_properties(External_lz4 PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(External_lz4 PUBLIC ${lz4_lib_SOURCE_DIR})