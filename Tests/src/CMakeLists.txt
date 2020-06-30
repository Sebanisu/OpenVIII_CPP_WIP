
#test app LZSS compression and decompression by passing random vector<char> to it with gradually increasing lengths.

add_executable(TestLZSS
        TestLZSS.h
        TestLZSS.cpp
        )
set_target_properties(TestLZSS PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(TestLZSS VIIICompression)