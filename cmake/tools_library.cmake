include(FetchContent)
FetchContent_Declare(
        tools_library
        GIT_REPOSITORY https://github.com/Sebanisu/ToolsLibrary.git
        GIT_TAG origin/main
)
#FetchContent_MakeAvailable(tools_library)
FetchContent_GetProperties(tools_library)
if (NOT tools_library_POPULATED)
    FetchContent_Populate(tools_library)
    add_subdirectory(${tools_library_SOURCE_DIR} ${tools_library_BINARY_DIR} EXCLUDE_FROM_ALL)
endif ()