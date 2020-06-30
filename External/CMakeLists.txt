add_library(External_lz4
        lz4/lib/lz4.h
        lz4/lib/lz4.c
        )
set_target_properties(External_lz4 PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(External_lz4 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})