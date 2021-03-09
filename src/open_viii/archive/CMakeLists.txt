add_library(${PROJECT_NAME}_VIIIArchive INTERFACE)
target_link_libraries(${PROJECT_NAME}_VIIIArchive
        INTERFACE ${PROJECT_NAME}_VIIICompression
        )
add_subdirectory(test)
