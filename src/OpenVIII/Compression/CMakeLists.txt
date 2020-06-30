add_library(VIIICompression
        L4Z.cpp
        L4Z.h
        LZSS.h
        LZSS.cpp)
set_target_properties(VIIICompression PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(VIIICompression External_lz4)