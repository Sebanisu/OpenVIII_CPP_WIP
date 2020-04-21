cmake_minimum_required(VERSION 3.16)
project(VIIIcppTest)

set(CMAKE_CXX_STANDARD 14)

include_directories(Archive)
include_directories(Compression)
include_directories(Tools)

add_executable(VIIIcppTest
        Archive/Archive.cpp
        Archive/Archive.hpp
        Archive/FI.hpp
        Archive/FIFLFS.hpp
        Archive/FL.hpp
        Archive/FS.hpp
        Compression/LZSS.hpp
        Tools/Tools.hpp)
