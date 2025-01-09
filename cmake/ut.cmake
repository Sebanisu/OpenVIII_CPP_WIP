enable_testing()
# if (DEFINED CMAKE_TOOLCHAIN_FILE)
#     message("CMAKE_TOOLCHAIN_FILE is set to ${CMAKE_TOOLCHAIN_FILE}")
#     #set VCPKG_DEFAULT_TRIPLET=x64-windows
#     #vcpkg install bext-ut
# else ()
#     message("CMAKE_TOOLCHAIN_FILE is not set")
#     include(FetchContent)
#     #options for FetchContent are at https://cmake.org/cmake/help/latest/module/ExternalProject.html
#     FetchContent_Declare(
#             ut_fetch
#             GIT_REPOSITORY https://github.com/boost-ext/ut.git
#             #GIT_REPOSITORY https://github.com/Sebanisu/ut.git
#             GIT_TAG origin/master
#             GIT_REMOTE_UPDATE_STRATEGY CHECKOUT
#     )
#     #FetchContent_MakeAvailable(ut)
#     FetchContent_GetProperties(ut_fetch)
#     if (NOT ut_fetch_POPULATED)
#         FetchContent_Populate(ut_fetch)
#         set(BOOST_UT_DISABLE_MODULE TRUE)
#         add_subdirectory(${ut_fetch_SOURCE_DIR} ${ut_fetch_BINARY_DIR} EXCLUDE_FROM_ALL)
#     endif ()
# endif ()

set(BOOST_UT_DISABLE_MODULE TRUE)
find_package(ut CONFIG REQUIRED)