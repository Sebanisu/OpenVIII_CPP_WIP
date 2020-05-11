cmake_minimum_required (VERSION 3.16)
project(VIIIcppTest C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
# Set default compile flags for GCC
if(CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "GCC detected, adding compile flags")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -Wextra -Wshadow -Wpedantic -Wconversion -Wpadded")
    # change to 2a for cpp20 pre release
elseif(MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17 /W4")
    # change to latest for cpp20 pre release
endif(CMAKE_COMPILER_IS_GNUCXX)
include_directories(Libs)
include_directories(Tests)

add_library(lz4
        External/lz4/lib/lz4.h
        External/lz4/lib/LZ4_.c
        )
set_target_properties(lz4 PROPERTIES LINKER_LANGUAGE CXX)

add_library(Compression STATIC
        Libs/Compression/LZSS/LZSS.h
        Libs/Compression/Compression.cpp
        Libs/Compression/L4Z/L4Z.h Libs/Compression/L4Z/L4Z.cpp)
set_target_properties(Compression PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(Compression lz4)

add_library(Tools STATIC
        Libs/Tools/Tools.h
        Libs/Tools/Tools.cpp
        Libs/color.h)
set_target_properties(Tools PROPERTIES LINKER_LANGUAGE CXX)

add_library(Archive STATIC
        Libs/Archive/FI.hpp
        Libs/Archive/FIFLFS.h
        Libs/Archive/FL.h
        Libs/Archive/FS.h
        Libs/Archive/ZZZ.h
        Libs/Archive/ZZZ/FileData.h
        Libs/Archive/Archive.h
        Libs/Archive/Archive.cpp
        )
set_target_properties(Archive PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(Archive Tools Compression)

add_executable(TestExtractFIFLFS
        Tests/TestExtractFIFLFS.h Tests/TestExtractFIFLFS.cpp
        )
set_target_properties(TestExtractFIFLFS PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(TestExtractFIFLFS Archive)

add_executable(TestExtractZZZ
        Tests/TestExtractZZZ.h Tests/TestExtractZZZ.cpp
)
target_link_libraries(TestExtractZZZ Archive)
set_target_properties(TestExtractZZZ PROPERTIES LINKER_LANGUAGE CXX)

add_executable(TestLZSS
        Tests/TestLZSS.h Tests/TestLZSS.cpp
)
target_link_libraries(TestLZSS Compression)
set_target_properties(TestLZSS PROPERTIES LINKER_LANGUAGE CXX)
