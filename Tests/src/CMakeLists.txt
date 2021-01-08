find_package(Threads REQUIRED)
#
##test app for trying to extract a all files from the FIFLFS archives.
add_executable(TestExtractFIFLFS
        TestExtractFIFLFS.hpp TestExtractFIFLFS.cpp)
set_target_properties(TestExtractFIFLFS PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(TestExtractFIFLFS VIIIArchive VIIIPaths project_warnings)
#
##test app for trying to extract a single file from the FIFLFS archives.
add_executable(TestExtractALLSearch
        TestExtractALLSearch.hpp
        TestExtractALLSearch.cpp)
set_target_properties(TestExtractALLSearch PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(TestExtractALLSearch VIIIArchive VIIIPaths Threads::Threads project_warnings)

#
##test app for trying to extract a all files from the ZZZ and FIFLFS archives.
add_executable(TestExtractZZZ
        TestExtractZZZ.hpp
        TestExtractZZZ.cpp)
set_target_properties(TestExtractZZZ PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(TestExtractZZZ VIIIArchive VIIIPaths project_warnings)

#
##Test Archives loading struct
add_executable(TestArchives
        TestArchives.hpp TestArchives.cpp)
set_target_properties(TestArchives PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(TestArchives VIIIArchive VIIIPaths project_warnings)


#
##test searching threw zzz contents and nested fi fl fs files.
add_executable(TestSearchZZZ
        TestSearchZZZ.hpp TestSearchZZZ.cpp)
set_target_properties(TestSearchZZZ PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(TestSearchZZZ VIIIArchive VIIIPaths project_warnings)
#test app LZSS compression and decompression by passing random vector<char> to it with gradually increasing lengths.

add_executable(TestLZSS
        TestLZSS.hpp
        TestLZSS.cpp
        )
set_target_properties(TestLZSS PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(TestLZSS VIIICompression Threads::Threads project_warnings)

#test app L4Z compression and decompression by passing random vector<char> to it with gradually increasing lengths.

add_executable(TestL4Z
        TestL4Z.hpp
        TestL4Z.cpp
        )
set_target_properties(TestL4Z PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(TestL4Z VIIICompression Threads::Threads project_warnings)

##test kernel.bin
add_executable(TestKernel
        TestKernel.cpp TestKernel.hpp)
set_target_properties(TestKernel PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(TestKernel VIIIStrings VIIIKernel VIIIPaths VIIIArchive project_warnings)


##test mngrphd.bin and mngrp.bin
add_executable(TestMenuGroup
        TestMenuGroup.cpp TestMenuGroup.hpp)
set_target_properties(TestMenuGroup PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(TestMenuGroup VIIIStrings VIIIMenuGroup VIIIPaths VIIIArchive VIIIGraphics project_warnings)


##test image texture libraries
add_executable(TestExtractImages
        TestExtractImages.cpp TestExtractImages.hpp)
set_target_properties(TestExtractImages PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(TestExtractImages VIIIPaths VIIIArchive VIIIGraphics Threads::Threads project_warnings)


##test image Deswizzle of fields like pupu
add_executable(TestDeswizzleFields
        TestDeswizzleFields.cpp TestDeswizzleFields.hpp)
set_target_properties(TestDeswizzleFields PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(TestDeswizzleFields VIIIPaths VIIIArchive VIIIGraphics Threads::Threads project_warnings)


##test image Reswizzle of fields like pupu
add_executable(TestReswizzleFields
        TestReswizzleFields.cpp TestReswizzleFields.hpp)
set_target_properties(TestReswizzleFields PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(TestReswizzleFields VIIIPaths VIIIArchive VIIIGraphics Threads::Threads project_warnings)

##test image ReDeswizzle of fields like pupu
add_executable(TestReDeswizzleFields
        TestReDeswizzleFields.cpp TestReDeswizzleFields.hpp)
set_target_properties(TestReDeswizzleFields PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(TestReDeswizzleFields VIIIPaths VIIIArchive VIIIGraphics Threads::Threads project_warnings)


##test extract all, uses new code to extract files instead of old versions.
add_executable(TestExtractALL
        TestExtractALL.cpp TestExtractALL.hpp)
set_target_properties(TestExtractALL PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(TestExtractALL VIIIPaths VIIIArchive Threads::Threads project_warnings)


##test extract all, uses new code to extract files instead of old versions.
add_executable(TestExtractPAK
        TestExtractPAK.cpp TestExtractPAK.hpp)
set_target_properties(TestExtractPAK PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(TestExtractPAK VIIIPaths project_warnings)

##test battle, uses new code to extract files instead of old versions.
add_executable(TestBattle
        TestBattle.cpp TestBattle.hpp)
set_target_properties(TestBattle PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(TestBattle VIIIPaths VIIIBattle VIIIArchive Threads::Threads  project_warnings)