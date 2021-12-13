add_library(${PROJECT_NAME}_VIIICommon INTERFACE)
#target_precompile_headers(${PROJECT_NAME}_VIIICommon INTERFACE
#        <variant>
#        <concepts>
#        )
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