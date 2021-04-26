cmake_minimum_required(VERSION 3.18)
project(VIIIArchive C CXX)

set(CMAKE_CXX_STANDARD 20) ## Supported values are 98, 11, 14, 17, and 20.
set(CMAKE_CXX_STANDARD_REQUIRED ON) ## the value of the CXX_STANDARD target property is treated as a requirement
set(CMAKE_CXX_EXTENSIONS OFF) ## on g++ this ensures: -std=c++11 and not -std=gnu++11
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON) ## automatically create a module definition (.def) file with all global
                                         ## symbols found in the input .obj files for a SHARED library on Windows.

# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(${PROJECT_NAME}_project_warnings INTERFACE)

# standard compiler warnings
include(cmake/CompilerWarnings.cmake)
set_project_warnings(${PROJECT_NAME}_project_warnings ON)

# get external dependencies.
include(cmake/lz4.cmake)
#include(cmake/googletest.cmake)
include(cmake/ut.cmake)
include(cmake/tools_library.cmake)

set(DOCS "NO" CACHE STRING "Build documentation")
# include docs folders
if(${DOCS} STREQUAL "YES")
    add_subdirectory(raw_docs)
endif()

# include source folders
add_subdirectory(src)
add_subdirectory(Tests)
add_subdirectory(ut)
#add_subdirectory(g_tests)