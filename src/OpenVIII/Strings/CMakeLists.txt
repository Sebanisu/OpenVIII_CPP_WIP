add_library(VIIIStrings
        EncodedStringOffset.cpp
        EncodedStringOffset.h
        FF8String.h
        FF8String.cpp
        )
set_target_properties(VIIIStrings PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(VIIIStrings)