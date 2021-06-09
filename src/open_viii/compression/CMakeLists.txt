add_library(${PROJECT_NAME}_VIIICompression INTERFACE)
target_link_libraries(${PROJECT_NAME}_VIIICompression
        INTERFACE ${PROJECT_NAME}_External_lz4
        )