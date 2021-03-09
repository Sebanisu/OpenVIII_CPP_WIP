include(FetchContent)
FetchContent_Declare(
        ToolsLibrary
        GIT_REPOSITORY https://github.com/Sebanisu/ToolsLibrary.git
        GIT_TAG origin/main
)
#FetchContent_MakeAvailable(ToolsLibrary)
FetchContent_GetProperties(ToolsLibrary)
if(NOT ToolsLibrary_POPULATED)
    FetchContent_Populate(ToolsLibrary)
    add_subdirectory(${ToolsLibrary_SOURCE_DIR} ${ToolsLibrary_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()