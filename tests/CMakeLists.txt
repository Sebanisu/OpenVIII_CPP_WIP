find_package(Threads REQUIRED)
include(../cmake/add_test_common.cmake)
add_test_common(FS)
target_link_libraries(${PROJECT_NAME}_UT_FS
        PRIVATE ${PROJECT_NAME}_VIIIArchive
        )

add_test_common(BattleStage)
target_link_libraries(${PROJECT_NAME}_UT_BattleStage
        PRIVATE ${PROJECT_NAME}_VIIIBattle
        PRIVATE ${PROJECT_NAME}_VIIIArchive
        PRIVATE ToolsLibrary_tl
        )
add_test_common(compression)
target_link_libraries(${PROJECT_NAME}_UT_compression
        PRIVATE ${PROJECT_NAME}_VIIICompression
        PRIVATE ToolsLibrary_tl
        )
add_test_common(FI)
target_link_libraries(${PROJECT_NAME}_UT_FI
        PRIVATE ${PROJECT_NAME}_VIIIArchive
        )
add_test_common(FL)
target_link_libraries(${PROJECT_NAME}_UT_FL
        PRIVATE ${PROJECT_NAME}_VIIIArchive
        )
add_test_common(FileData)
target_link_libraries(${PROJECT_NAME}_UT_FileData
        PRIVATE ${PROJECT_NAME}_VIIIArchive
        )

add_test_common(ArchiveRange)
target_link_libraries(${PROJECT_NAME}_UT_ArchiveRange
        PRIVATE ${PROJECT_NAME}_VIIIArchive
        )

add_test_common(BackgroundTiles)
target_link_libraries(${PROJECT_NAME}_UT_BackgroundTiles
        PRIVATE ${PROJECT_NAME}_VIIIGraphics
        PRIVATE ToolsLibrary_tl
        PRIVATE Threads::Threads
        )

#PRIVATE ToolsLibrary_tl
#PRIVATE Threads::Threads