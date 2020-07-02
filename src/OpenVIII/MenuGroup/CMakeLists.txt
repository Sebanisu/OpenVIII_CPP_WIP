add_library(VIIIMenuGroup
        mngrphd.cpp
        mngrphd.h
        tkmnmes.cpp
        tkmnmes.h
        )
set_target_properties(VIIIMenuGroup PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(VIIIMenuGroup)