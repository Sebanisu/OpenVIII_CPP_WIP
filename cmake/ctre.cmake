include(FetchContent)
#options for FetchContent are at https://cmake.org/cmake/help/latest/module/ExternalProject.html
FetchContent_Declare(
        ctre_fetch
        #GIT_REPOSITORY https://github.com/hanickadot/compile-time-regular-expressions.git
        GIT_REPOSITORY https://github.com/Sebanisu/compile-time-regular-expressions.git
        GIT_TAG origin/main
        GIT_REMOTE_UPDATE_STRATEGY CHECKOUT
)
#FetchContent_MakeAvailable(ctre_fetch)
FetchContent_GetProperties(ctre_fetch)
if (NOT ctre_fetch_POPULATED)
    FetchContent_Populate(ctre_fetch)
    add_subdirectory(${ctre_fetch_SOURCE_DIR} ${ctre_fetch_BINARY_DIR} EXCLUDE_FROM_ALL)
endif ()