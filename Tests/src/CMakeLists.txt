#
##test app for trying to extract a all files from the FIFLFS archives.
add_executable(TestExtractFIFLFS
        TestExtractFIFLFS.h TestExtractFIFLFS.cpp)
set_target_properties(TestExtractFIFLFS PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(TestExtractFIFLFS VIIIArchive VIIIPaths)
#
##test app for trying to extract a single file from the FIFLFS archives.
add_executable(TestExtractSingleFIFLFS
        TestExtractSingleFIFLFS.h
        TestExtractSingleFIFLFS.cpp)
set_target_properties(TestExtractSingleFIFLFS PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(TestExtractSingleFIFLFS VIIIArchive VIIIPaths)

#
##test app for trying to extract a all files from the ZZZ and FIFLFS archives.
add_executable(TestExtractZZZ
        TestExtractZZZ.h
        TestExtractZZZ.cpp)
set_target_properties(TestExtractZZZ PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(TestExtractZZZ VIIIArchive VIIIPaths)

#
##Test Archives loading struct
add_executable(TestArchives
        TestArchives.h TestArchives.cpp)
set_target_properties(TestArchives PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(TestArchives VIIIArchive VIIIPaths)


#
##test searching threw zzz contents and nested fi fl fs files.
add_executable(TestExtractSingleZZZ
        TestExtractSingleZZZ.h TestExtractSingleZZZ.cpp)
set_target_properties(TestExtractSingleZZZ PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(TestExtractSingleZZZ VIIIArchive VIIIPaths)
#test app LZSS compression and decompression by passing random vector<char> to it with gradually increasing lengths.

add_executable(TestLZSS
        TestLZSS.h
        TestLZSS.cpp
        )
set_target_properties(TestLZSS PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(TestLZSS VIIICompression)

##test kernel.bin
add_executable(TestKernel
        TestKernel.cpp TestKernel.h)
set_target_properties(TestKernel PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(TestKernel VIIIStrings VIIIKernel VIIIPaths VIIIArchive)


##test mngrphd.bin and mngrp.bin
add_executable(TestMenuGroup
        TestMenuGroup.cpp TestMenuGroup.h)
set_target_properties(TestMenuGroup PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(TestMenuGroup VIIIStrings VIIIMenuGroup VIIIPaths VIIIArchive)