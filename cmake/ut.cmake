enable_testing()
include(FetchContent)
#options for FetchContent are at https://cmake.org/cmake/help/latest/module/ExternalProject.html
FetchContent_Declare(
        ut
        GIT_REPOSITORY https://github.com/boost-ext/ut.git
        GIT_TAG origin/master
        GIT_REMOTE_UPDATE_STRATEGY CHECKOUT
)
#FetchContent_MakeAvailable(ut)
FetchContent_GetProperties(ut)
if (NOT ut_POPULATED)
    FetchContent_Populate(ut)
    add_subdirectory(${ut_SOURCE_DIR} ${ut_BINARY_DIR} EXCLUDE_FROM_ALL)
endif ()