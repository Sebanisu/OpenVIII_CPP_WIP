add_library(${PROJECT_NAME}_VIIICommon INTERFACE)
#target_precompile_headers(${PROJECT_NAME}_VIIICommon INTERFACE
#        <variant>
#        <concepts>
#        )
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    target_compile_options(${PROJECT_NAME}_VIIICommon
            INTERFACE "-stdlib=libc++"
            INTERFACE "-march=native"
            INTERFACE "-fexperimental-library" ##new in clang15 for things like std::ranges and std::format
            )
    target_link_options(${PROJECT_NAME}_VIIICommon
            INTERFACE "-stdlib=libc++"
            INTERFACE "-fuse-ld=lld"
            INTERFACE "-Wl") #,--gdb-index
    target_compile_features(${PROJECT_NAME}_VIIICommon
            INTERFACE cxx_std_20)
endif ()
add_subdirectory(archive)
add_subdirectory(graphics)
add_subdirectory(compression)
add_subdirectory(kernel)
add_subdirectory(menu_group)
add_subdirectory(paths)
add_subdirectory(strings)
add_subdirectory(tools)
add_subdirectory(pak)
add_subdirectory(battle)
add_subdirectory(field)