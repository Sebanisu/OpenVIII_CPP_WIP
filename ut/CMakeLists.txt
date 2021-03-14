find_package(Threads REQUIRED)
include(../cmake/add_test_common.cmake)
add_test_common(FS)
target_link_libraries(${PROJECT_NAME}_FS
        PRIVATE ${PROJECT_NAME}_VIIIArchive
        )
add_test_common(compression)
target_link_libraries(${PROJECT_NAME}_compression
        PRIVATE ${PROJECT_NAME}_VIIICompression
        PRIVATE ToolsLibrary_tl
        )
add_test_common(FI)
target_link_libraries(${PROJECT_NAME}_FI
        PRIVATE ${PROJECT_NAME}_VIIIArchive
        )
add_test_common(FL)
target_link_libraries(${PROJECT_NAME}_FL
        PRIVATE ${PROJECT_NAME}_VIIIArchive
        )

#PRIVATE ToolsLibrary_tl
#PRIVATE Threads::Threads