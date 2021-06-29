# https://sourceforge.net/p/libpng/code/ci/libpng16/tree/
# git://git.code.sf.net/p/libpng/code
#
#include(FetchContent)
##options for FetchContent are at https://cmake.org/cmake/help/latest/module/ExternalProject.html
#FetchContent_Declare(
#        libpng_fetch
#        GIT_REPOSITORY git://git.code.sf.net/p/libpng/code
#        GIT_TAG origin/master
#        GIT_REMOTE_UPDATE_STRATEGY CHECKOUT
#)
##FetchContent_MakeAvailable(libpng_fetch)
#FetchContent_GetProperties(libpng_fetch)
#if (NOT libpng_fetch_POPULATED)
#    FetchContent_Populate(libpng_fetch)
#    add_subdirectory(${libpng_fetch_SOURCE_DIR} ${libpng_fetch_BINARY_DIR} EXCLUDE_FROM_ALL)
#endif ()

find_package(PNG REQUIRED)