include(../cmake/add_test_common.cmake)
add_test_common(FS)
target_link_libraries(${PROJECT_NAME}_FS
        PRIVATE ${PROJECT_NAME}_VIIIArchive
        )