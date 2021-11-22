add_library(${PROJECT_NAME}_VIIIGraphics INTERFACE)
add_subdirectory(color)
add_subdirectory(tim)
add_subdirectory(sp1)
add_subdirectory(tex)
add_subdirectory(sp2)
target_link_libraries(${PROJECT_NAME}_VIIIGraphics
        INTERFACE ${PNG_LIBRARIES}
        INTERFACE ToolsLibrary_tl
        )
target_include_directories(${PROJECT_NAME}_VIIIGraphics INTERFACE ${PNG_INCLUDE_DIRS})