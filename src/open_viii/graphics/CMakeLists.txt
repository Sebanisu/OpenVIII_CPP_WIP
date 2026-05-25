add_library(${PROJECT_NAME}_VIIIGraphics STATIC
    background/PupuID.cpp
    background/UniquifyPupu.cpp
)
add_subdirectory(color)
add_subdirectory(tim)
add_subdirectory(sp1)
add_subdirectory(tex)
add_subdirectory(sp2)
find_package(PNG REQUIRED)
target_link_libraries(${PROJECT_NAME}_VIIIGraphics
    PUBLIC ToolsLibrary_tl
    PUBLIC ${PROJECT_NAME}_VIIITools
    PUBLIC ${PROJECT_NAME}_VIIICommon
    PUBLIC ${PROJECT_NAME}_project_warnings
    PUBLIC PNG::PNG
    PUBLIC spdlog::spdlog
    PUBLIC Threads::Threads
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
if (MSVC)
    target_compile_options(${PROJECT_NAME}_VIIIGraphics
        PUBLIC /utf-8
    )

    target_compile_options(${PROJECT_NAME}_VIIIGraphics  PUBLIC
            $<$<CONFIG:Debug>:/MTd>
            $<$<NOT:$<CONFIG:Debug>>:/MT>
    )
endif()