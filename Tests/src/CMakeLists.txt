find_package(Threads REQUIRED)
#
##test app for trying to extract a single file from the FIFLFS archives.
add_executable(TestExtractALLSearch
        TestExtractALLSearch.hpp
        TestExtractALLSearch.cpp)
set_target_properties(TestExtractALLSearch PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(TestExtractALLSearch
        PRIVATE VIIIArchive
        PRIVATE VIIIPaths
        PRIVATE Threads::Threads project_warnings
        PRIVATE ToolsLibrary_tl
        )

#
##Test Archives loading struct
add_executable(TestArchives
        TestArchives.hpp TestArchives.cpp)
set_target_properties(TestArchives PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(TestArchives
        PRIVATE VIIIArchive
        PRIVATE VIIIPaths
        PRIVATE project_warnings
        PRIVATE ToolsLibrary_tl
        )


#
##test searching threw zzz contents and nested fi fl fs files.
add_executable(TestSearchZZZ
        TestSearchZZZ.hpp TestSearchZZZ.cpp)
set_target_properties(TestSearchZZZ PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(TestSearchZZZ
        PRIVATE VIIIArchive
        PRIVATE VIIIPaths
        PRIVATE project_warnings
        PRIVATE ToolsLibrary_tl)
#test app LZSS compression and decompression by passing random vector<char> to it with gradually increasing lengths.

add_executable(TestLZSS
        TestLZSS.hpp
        TestLZSS.cpp
        )
set_target_properties(TestLZSS PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(TestLZSS
        PRIVATE VIIICompression
        PRIVATE Threads::Threads
        PRIVATE project_warnings
        PRIVATE ToolsLibrary_tl
        )

#test app L4Z compression and decompression by passing random vector<char> to it with gradually increasing lengths.

add_executable(TestL4Z
        TestL4Z.hpp
        TestL4Z.cpp
        )
set_target_properties(TestL4Z PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(TestL4Z
        PRIVATE VIIICompression
        PRIVATE Threads::Threads
        PRIVATE project_warnings
        PRIVATE ToolsLibrary_tl

        )

##test kernel.bin
add_executable(TestKernel
        TestKernel.cpp TestKernel.hpp)
set_target_properties(TestKernel PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(TestKernel
        PRIVATE VIIIStrings
        PRIVATE VIIIKernel
        PRIVATE VIIIPaths
        PRIVATE VIIIArchive
        PRIVATE project_warnings
        PRIVATE ToolsLibrary_tl
        )


##test mngrphd.bin and mngrp.bin
add_executable(TestMenuGroup
        TestMenuGroup.cpp TestMenuGroup.hpp)
set_target_properties(TestMenuGroup PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(TestMenuGroup
        PRIVATE VIIIStrings
        PRIVATE VIIIMenuGroup
        PRIVATE VIIIPaths
        PRIVATE VIIIArchive
        PRIVATE VIIIGraphics
        PRIVATE project_warnings
        PRIVATE ToolsLibrary_tl
        )


##test image texture libraries
add_executable(TestExtractImages
        TestExtractImages.cpp TestExtractImages.hpp)
set_target_properties(TestExtractImages PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(TestExtractImages
        PRIVATE VIIIPaths
        PRIVATE VIIIArchive
        PRIVATE VIIIGraphics
        PRIVATE Threads::Threads
        PRIVATE project_warnings
        PRIVATE ToolsLibrary_tl
        )


##test image Deswizzle of fields like pupu
add_executable(TestDeswizzleFields
        TestDeswizzleFields.cpp TestDeswizzleFields.hpp)
set_target_properties(TestDeswizzleFields PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(TestDeswizzleFields
        PRIVATE VIIIPaths
        PRIVATE VIIIArchive
        PRIVATE VIIIGraphics
        PRIVATE Threads::Threads
        PRIVATE project_warnings
        PRIVATE ToolsLibrary_tl
        )


##test image Reswizzle of fields like pupu
add_executable(TestReswizzleFields
        TestReswizzleFields.cpp TestReswizzleFields.hpp)
set_target_properties(TestReswizzleFields PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(TestReswizzleFields
        PRIVATE VIIIPaths
        PRIVATE VIIIArchive
        PRIVATE VIIIGraphics
        PRIVATE Threads::Threads
        PRIVATE project_warnings
        PRIVATE ToolsLibrary_tl
        )

##test image ReDeswizzle of fields like pupu
add_executable(TestReDeswizzleFields
        TestReDeswizzleFields.cpp TestReDeswizzleFields.hpp)
set_target_properties(TestReDeswizzleFields PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(TestReDeswizzleFields
        PRIVATE VIIIPaths
        PRIVATE VIIIArchive
        PRIVATE VIIIGraphics
        PRIVATE Threads::Threads
        PRIVATE project_warnings
        PRIVATE ToolsLibrary_tl
        )


##test extract all, uses new code to extract files instead of old versions.
add_executable(TestExtractALL
        TestExtractALL.cpp TestExtractALL.hpp)
set_target_properties(TestExtractALL PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(TestExtractALL
        PRIVATE VIIIPaths
        PRIVATE VIIIArchive
        PRIVATE Threads::Threads
        PRIVATE project_warnings
        PRIVATE ToolsLibrary_tl
        )


##test extract all, uses new code to extract files instead of old versions.
add_executable(TestExtractPAK
        TestExtractPAK.cpp TestExtractPAK.hpp)
set_target_properties(TestExtractPAK PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(TestExtractPAK
        PRIVATE VIIIPaths
        PRIVATE project_warnings
        PRIVATE ToolsLibrary_tl
        )

##test battle, uses new code to extract files instead of old versions.
add_executable(TestBattle
        TestBattle.cpp TestBattle.hpp)
set_target_properties(TestBattle PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(TestBattle
        PRIVATE VIIIPaths
        PRIVATE VIIIBattle
        PRIVATE VIIIArchive
        PRIVATE Threads::Threads
        PRIVATE project_warnings
        PRIVATE ToolsLibrary_tl
        )


##test battle, uses new code to extract files instead of old versions.
add_executable(TestJSM
        TestJSM.cpp TestJSM.hpp)
set_target_properties(TestJSM PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(TestJSM
        PRIVATE VIIIPaths
        PRIVATE VIIIField
        PRIVATE VIIIArchive
        PRIVATE Threads::Threads
        PRIVATE project_warnings
        PRIVATE ToolsLibrary_tl
        )