cmake_minimum_required(VERSION 3.16)
project(VIIIArchive C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
# Set default compile flags for GCC
if (CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "GCC detected, adding compile flags")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -Wextra -Wshadow -Wpedantic -Wconversion -Wfatal-errors -Wold-style-cast -Wunused -Wcast-align -Werror -Wuseless-cast -Wlogical-op -Wsign-conversion -Wduplicated-cond -Wduplicated-branches -Wnull-dereference -Wdouble-promotion -Wconversion -Woverloaded-virtual -Wnon-virtual-dtor -Wformat=2 -fsanitize=address")
    #-Wpadded
    # change to 2a for cpp20 pre release
elseif (MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17 /W4")
    # change to latest for cpp20 pre release
endif (CMAKE_COMPILER_IS_GNUCXX)
add_subdirectory(External/VIIICompression)
add_subdirectory(External/VIIITools)
find_package(Git QUIET)
if (GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    # Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if (GIT_SUBMODULE)
        message(STATUS "Submodule foreach reset")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule foreach git reset --hard
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                RESULT_VARIABLE GIT_SUBMOD_RESULT)
        message(STATUS "Submodule foreach git pull")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule foreach git pull origin master
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if (NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif ()
        #        message(STATUS "Submodule update")
        #        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
        #                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        #                RESULT_VARIABLE GIT_SUBMOD_RESULT)
        #        if (NOT GIT_SUBMOD_RESULT EQUAL "0")
        #            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        #        endif ()
    endif ()
endif ()

#include_directories(External/VIIICompression)
#include_directories(External/VIIITools)
#add_library(lz4
#        External/lz4/lib/lz4.h
#        External/lz4/lib/lz4.c
#        )
#set_target_properties(lz4 PROPERTIES LINKER_LANGUAGE CXX)
#
#add_library(Compression STATIC
#        Libs/Compression/LZSS/LZSS.h
#        Libs/Compression/Compression.cpp
#        Libs/Compression/L4Z/L4Z.h Libs/Compression/L4Z/L4Z.cpp)
#set_target_properties(Compression PROPERTIES LINKER_LANGUAGE CXX)
#target_link_libraries(Compression lz4)
#
#add_library(Tools STATIC
#        Libs/Tools/Tools.h
#        Libs/Tools/Tools.cpp
#        Libs/color.h)
#set_target_properties(Tools PROPERTIES LINKER_LANGUAGE CXX)
#


add_subdirectory(src)
add_subdirectory(Tests)