cmake_minimum_required(VERSION 3.16)
project(VIIIArchive C CXX)

set(CMAKE_CXX_STANDARD 20) ## Supported values are 98, 11, 14, 17, and 20.
set(CMAKE_CXX_STANDARD_REQUIRED ON) ## the value of the CXX_STANDARD target property is treated as a requirement
set(CMAKE_CXX_EXTENSIONS OFF) ## on g++ this ensures: -std=c++11 and not -std=gnu++11
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON) ## automatically create a module definition (.def) file with all global
                                         ## symbols found in the input .obj files for a SHARED library on Windows.
# Set default compile flags for GCC
if (CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "GCC detected, adding compile flags")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wshadow -Wpedantic -Wconversion -Wold-style-cast -Wunused -Wcast-align -Werror -Wuseless-cast -Wlogical-op -Wsign-conversion -Wduplicated-cond -Wduplicated-branches -Wnull-dereference -Wdouble-promotion -Wconversion -Woverloaded-virtual -Wnon-virtual-dtor -Wformat=2 -fsanitize=address,undefined")
    # -Wfatal-errors -Wpadded
    # change to 2a for cpp20 pre release
elseif (MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    # change to latest for cpp20 pre release
endif (CMAKE_COMPILER_IS_GNUCXX)
#FetchContent_Declare(
#        googletest
#        GIT_REPOSITORY https://github.com/google/googletest.git
#        GIT_TAG release-1.8.0
#)
#FetchContent_MakeAvailable(googletest)

include(lz4.cmake)
include(googletest.cmake)

add_subdirectory(src)
add_subdirectory(Tests)