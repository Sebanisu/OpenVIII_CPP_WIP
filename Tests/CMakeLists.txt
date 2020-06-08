##define array with path info.
add_library(paths
        paths.cpp
        paths.h)
set_target_properties(paths PROPERTIES LINKER_LANGUAGE CXX)
#
##test app for trying to extract a all files from the FIFLFS archives.
add_executable(TestExtractFIFLFS
        TestExtractFIFLFS.h TestExtractFIFLFS.cpp)
set_target_properties(TestExtractFIFLFS PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(TestExtractFIFLFS Archive paths)
#
##test app for trying to extract a single file from the FIFLFS archives.
add_executable(TestExtractSingleFIFLFS
        TestExtractSingleFIFLFS.h
        TestExtractSingleFIFLFS.cpp)
set_target_properties(TestExtractSingleFIFLFS PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(TestExtractSingleFIFLFS Archive paths)

#
##test app for trying to extract a all files from the ZZZ and FIFLFS archives.
add_executable(TestExtractZZZ
        TestExtractZZZ.h
        TestExtractZZZ.cpp)
set_target_properties(TestExtractZZZ PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(TestExtractZZZ Archive paths)

#
##Test Archives loading struct
add_executable(TestArchives
        TestArchives.h TestArchives.cpp)
set_target_properties(TestArchives PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(TestArchives Archive paths)


#
##test searching threw zzz contents and nested fi fl fs files.
add_executable(TestExtractSingleZZZ
        TestExtractSingleZZZ.h TestExtractSingleZZZ.cpp)
set_target_properties(TestExtractSingleZZZ PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(TestExtractSingleZZZ Archive paths)

##test kernel.bin
add_executable(TestKernel
        TestKernel.cpp TestKernel.h ../Kernel.cpp ../Kernel.h)
set_target_properties(TestKernel PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(TestKernel Archive paths)