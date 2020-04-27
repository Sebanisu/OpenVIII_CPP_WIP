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
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -Wextra -Wshadow -Wpedantic -Wconversion")
    # change to 2a for cpp20 pre release
elseif(MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17 /W4")
    # change to latest for cpp20 pre release
endif(CMAKE_COMPILER_IS_GNUCXX)
include_directories(Libs)
include_directories(Tests)

add_library(Compression STATIC
        Libs/Compression/LZSS/LZSS.hpp
        Libs/Compression/Compression.cpp)
set_target_properties(Compression PROPERTIES LINKER_LANGUAGE CXX)

add_library(Tools STATIC
        Libs/Tools/Tools.hpp
        Libs/Tools/Tools.cpp
)
set_target_properties(Tools PROPERTIES LINKER_LANGUAGE CXX)

add_library(Archive STATIC
        Libs/Archive/FI.hpp
        Libs/Archive/FIFLFS.hpp
        Libs/Archive/FL.hpp
        Libs/Archive/FS.hpp
        Libs/Archive/ZZZ.hpp
        Libs/Archive/ZZZ/FileData.hpp
        Libs/Archive/Archive.hpp
        Libs/Archive/Archive.cpp
        )
set_target_properties(Archive PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(Archive Tools Compression)

add_executable(TestExtractFIFLFS
        Tests/TestExtractFIFLFS.hpp Tests/TestExtractFIFLFS.cpp
        )
set_target_properties(TestExtractFIFLFS PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(TestExtractFIFLFS Archive)

add_executable(TestExtractZZZ
        Tests/TestExtractZZZ.hpp Tests/TestExtractZZZ.cpp
)
target_link_libraries(TestExtractZZZ Archive)
set_target_properties(TestExtractZZZ PROPERTIES LINKER_LANGUAGE CXX)

add_executable(TestLZSS
        Tests/TestLZSS.hpp Tests/TestLZSS.cpp
)
target_link_libraries(TestLZSS Compression)
set_target_properties(TestLZSS PROPERTIES LINKER_LANGUAGE CXX)
