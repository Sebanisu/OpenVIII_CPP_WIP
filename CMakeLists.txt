cmake_minimum_required (VERSION 3.16)
project(VIIIcppTest)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
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
include_directories(Archive)
include_directories(Compression)
include_directories(Tools)

add_executable(Archive
        Archive/Archive.cpp
        Archive/Archive.hpp
        Archive/FI.hpp
        Archive/FIFLFS.hpp
        Archive/FL.hpp
        Archive/FS.hpp
        Compression/LZSS.hpp
        Tools/Tools.hpp)
