##define array with path info.
add_library(VIIIPaths
        Paths.cpp
        Paths.h)
set_target_properties(VIIIPaths PROPERTIES LINKER_LANGUAGE CXX)