add_library(${PROJECT_NAME}_VIIIGraphics
    background/PupuID.cpp
)
add_subdirectory(color)
add_subdirectory(tim)
add_subdirectory(sp1)
add_subdirectory(tex)
add_subdirectory(sp2)
find_package(PNG REQUIRED)
target_link_libraries(${PROJECT_NAME}_VIIIGraphics
    PUBLIC
        ToolsLibrary_tl
        ${PROJECT_NAME}_VIIITools
        ${PROJECT_NAME}_VIIICommon
        PNG::PNG
)

target_include_directories(${PROJECT_NAME}_VIIIGraphics
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${PNG_INCLUDE_DIRS}
)
target_compile_features(${PROJECT_NAME}_VIIIGraphics
        PUBLIC cxx_std_23
        PUBLIC cxx_std_20
)