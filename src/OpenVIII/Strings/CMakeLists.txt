add_library(Strings
        EncodedStringOffset.cpp
        EncodedStringOffset.h
        FF8String.h
        FF8String.cpp
        )
set_target_properties(Strings PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(Strings)