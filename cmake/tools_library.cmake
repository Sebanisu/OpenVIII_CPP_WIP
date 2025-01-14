include(FetchContent)
FetchContent_Declare(
        tools_library
        GIT_REPOSITORY https://github.com/Sebanisu/ToolsLibrary.git
        GIT_TAG origin/main
        GIT_REMOTE_UPDATE_STRATEGY CHECKOUT
)
FetchContent_MakeAvailable(tools_library)