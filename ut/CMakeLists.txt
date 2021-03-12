find_package(Threads REQUIRED)
include(../cmake/add_test_common.cmake)
add_test_common(FS)
target_link_libraries(${PROJECT_NAME}_FS
        PRIVATE ${PROJECT_NAME}_VIIIArchive
        )
add_test_common(compression)
target_link_libraries(${PROJECT_NAME}_compression
        PRIVATE ${PROJECT_NAME}_VIIICompression
        PRIVATE Threads::Threads
        )


#PRIVATE ToolsLibrary_tl