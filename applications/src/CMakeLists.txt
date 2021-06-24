find_package(Threads REQUIRED)
##test app for trying to extract a single file from the FIFLFS archives.
add_executable(${PROJECT_NAME}_TestExtractALLSearch
        TestExtractALLSearch.hpp
        TestExtractALLSearch.cpp)
set_target_properties(${PROJECT_NAME}_TestExtractALLSearch PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(${PROJECT_NAME}_TestExtractALLSearch
        PRIVATE ${PROJECT_NAME}_VIIIArchive
        PRIVATE ${PROJECT_NAME}_VIIIPaths
        PRIVATE Threads::Threads
        PRIVATE ${PROJECT_NAME}_project_warnings
        PRIVATE ToolsLibrary_tl
        )

#
##Test Archives loading struct
add_executable(${PROJECT_NAME}_TestArchives
        TestArchives.hpp TestArchives.cpp)
set_target_properties(${PROJECT_NAME}_TestArchives PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(${PROJECT_NAME}_TestArchives
        PRIVATE ${PROJECT_NAME}_VIIIArchive
        PRIVATE ${PROJECT_NAME}_VIIIPaths
        PRIVATE ${PROJECT_NAME}_project_warnings
        PRIVATE ToolsLibrary_tl
        )
#
##test searching threw zzz contents and nested fi fl fs files.
add_executable(${PROJECT_NAME}_TestSearchZZZ
        TestSearchZZZ.hpp TestSearchZZZ.cpp)
set_target_properties(${PROJECT_NAME}_TestSearchZZZ PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(${PROJECT_NAME}_TestSearchZZZ
        PRIVATE ${PROJECT_NAME}_VIIIArchive
        PRIVATE ${PROJECT_NAME}_VIIIPaths
        PRIVATE ${PROJECT_NAME}_project_warnings
        PRIVATE ToolsLibrary_tl)
if (NOT MSVC)
    ##test kernel.bin
    add_executable(${PROJECT_NAME}_TestKernel
            TestKernel.cpp TestKernel.hpp)
    set_target_properties(${PROJECT_NAME}_TestKernel PROPERTIES LINKER_LANGUAGE CXX)
    target_link_libraries(${PROJECT_NAME}_TestKernel
            PRIVATE ${PROJECT_NAME}_VIIIStrings
            PRIVATE ${PROJECT_NAME}_VIIIKernel
            PRIVATE ${PROJECT_NAME}_VIIIPaths
            PRIVATE ${PROJECT_NAME}_VIIIArchive
            PRIVATE ${PROJECT_NAME}_project_warnings
            PRIVATE ToolsLibrary_tl
            )
endif ()

##test mngrphd.bin and mngrp.bin
add_executable(${PROJECT_NAME}_TestMenuGroup
        TestMenuGroup.cpp TestMenuGroup.hpp)
set_target_properties(${PROJECT_NAME}_TestMenuGroup PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(${PROJECT_NAME}_TestMenuGroup
        PRIVATE ${PROJECT_NAME}_VIIIStrings
        PRIVATE ${PROJECT_NAME}_VIIIMenuGroup
        PRIVATE ${PROJECT_NAME}_VIIIPaths
        PRIVATE ${PROJECT_NAME}_VIIIArchive
        PRIVATE ${PROJECT_NAME}_VIIIGraphics
        PRIVATE ${PROJECT_NAME}_project_warnings
        PRIVATE ToolsLibrary_tl
        )
##test image texture libraries
add_executable(${PROJECT_NAME}_TestExtractImages
        TestExtractImages.cpp TestExtractImages.hpp)
set_target_properties(${PROJECT_NAME}_TestExtractImages PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(${PROJECT_NAME}_TestExtractImages
        PRIVATE ${PROJECT_NAME}_VIIIPaths
        PRIVATE ${PROJECT_NAME}_VIIIArchive
        PRIVATE ${PROJECT_NAME}_VIIIGraphics
        PRIVATE Threads::Threads
        PRIVATE ${PROJECT_NAME}_project_warnings
        PRIVATE ToolsLibrary_tl
        )

##test image Deswizzle of fields like pupu
add_executable(${PROJECT_NAME}_TestDeswizzleFields
        TestDeswizzleFields.cpp TestDeswizzleFields.hpp)
set_target_properties(${PROJECT_NAME}_TestDeswizzleFields PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(${PROJECT_NAME}_TestDeswizzleFields
        PRIVATE ${PROJECT_NAME}_VIIIPaths
        PRIVATE ${PROJECT_NAME}_VIIIArchive
        PRIVATE ${PROJECT_NAME}_VIIIGraphics
        PRIVATE Threads::Threads
        PRIVATE ${PROJECT_NAME}_project_warnings
        PRIVATE ToolsLibrary_tl
        )

##test image Reswizzle of fields like pupu
add_executable(${PROJECT_NAME}_TestReswizzleFields
        TestReswizzleFields.cpp TestReswizzleFields.hpp)
set_target_properties(${PROJECT_NAME}_TestReswizzleFields PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(${PROJECT_NAME}_TestReswizzleFields
        PRIVATE ${PROJECT_NAME}_VIIIPaths
        PRIVATE ${PROJECT_NAME}_VIIIArchive
        PRIVATE ${PROJECT_NAME}_VIIIGraphics
        PRIVATE Threads::Threads
        PRIVATE ${PROJECT_NAME}_project_warnings
        PRIVATE ToolsLibrary_tl
        )
##test image ReDeswizzle of fields like pupu
add_executable(${PROJECT_NAME}_TestReDeswizzleFields
        TestReDeswizzleFields.cpp TestReDeswizzleFields.hpp)
set_target_properties(${PROJECT_NAME}_TestReDeswizzleFields PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(${PROJECT_NAME}_TestReDeswizzleFields
        PRIVATE ${PROJECT_NAME}_VIIIPaths
        PRIVATE ${PROJECT_NAME}_VIIIArchive
        PRIVATE ${PROJECT_NAME}_VIIIGraphics
        PRIVATE Threads::Threads
        PRIVATE ${PROJECT_NAME}_project_warnings
        PRIVATE ToolsLibrary_tl
        )


##test extract all, uses new code to extract files instead of old versions.
add_executable(${PROJECT_NAME}_TestExtractALL
        TestExtractALL.cpp TestExtractALL.hpp)
set_target_properties(${PROJECT_NAME}_TestExtractALL PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(${PROJECT_NAME}_TestExtractALL
        PRIVATE ${PROJECT_NAME}_VIIIPaths
        PRIVATE ${PROJECT_NAME}_VIIIArchive
        PRIVATE Threads::Threads
        PRIVATE ${PROJECT_NAME}_project_warnings
        PRIVATE ToolsLibrary_tl
        )


##test extract all, uses new code to extract files instead of old versions.
add_executable(${PROJECT_NAME}_TestExtractPAK
        TestExtractPAK.cpp TestExtractPAK.hpp)
set_target_properties(${PROJECT_NAME}_TestExtractPAK PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(${PROJECT_NAME}_TestExtractPAK
        PRIVATE ${PROJECT_NAME}_VIIIPaths
        PRIVATE ${PROJECT_NAME}_project_warnings
        PRIVATE ToolsLibrary_tl
        )


##test battle, uses new code to extract files instead of old versions.
add_executable(${PROJECT_NAME}_TestBattle
        TestBattle.cpp TestBattle.hpp)
set_target_properties(${PROJECT_NAME}_TestBattle PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(${PROJECT_NAME}_TestBattle
        PRIVATE ${PROJECT_NAME}_VIIIPaths
        PRIVATE ${PROJECT_NAME}_VIIIBattle
        PRIVATE ${PROJECT_NAME}_VIIIArchive
        PRIVATE Threads::Threads
        PRIVATE ${PROJECT_NAME}_project_warnings
        PRIVATE ToolsLibrary_tl
        )

##test battle, uses new code to extract files instead of old versions.
add_executable(${PROJECT_NAME}_TestJSM
        TestJSM.cpp TestJSM.hpp)
set_target_properties(${PROJECT_NAME}_TestJSM PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(${PROJECT_NAME}_TestJSM
        PRIVATE ${PROJECT_NAME}_VIIIPaths
        PRIVATE ${PROJECT_NAME}_VIIIField
        PRIVATE ${PROJECT_NAME}_VIIIArchive
        PRIVATE Threads::Threads
        PRIVATE ${PROJECT_NAME}_project_warnings
        PRIVATE ToolsLibrary_tl
        )

#compress file with lz4 like ff8 would. or uncompress
add_executable(${PROJECT_NAME}_lz4_ff8
        lz4_ff8.cpp
        lz4_lzss_common_ff8.hpp)
set_target_properties(${PROJECT_NAME}_lz4_ff8 PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${PROJECT_NAME}_lz4_ff8 PROPERTIES OUTPUT_NAME "lz4_ff8")
target_link_libraries(${PROJECT_NAME}_lz4_ff8
        PRIVATE ${PROJECT_NAME}_VIIICompression
        PRIVATE ${PROJECT_NAME}_project_warnings
        PRIVATE Threads::Threads
        PRIVATE ToolsLibrary_tl
        )
#compress file with lzss like ff8 would. or uncompress
add_executable(${PROJECT_NAME}_lzss_ff8
        lzss_ff8.cpp
        lz4_lzss_common_ff8.hpp)
set_target_properties(${PROJECT_NAME}_lzss_ff8 PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${PROJECT_NAME}_lzss_ff8 PROPERTIES OUTPUT_NAME "lzss_ff8")
target_link_libraries(${PROJECT_NAME}_lzss_ff8
        PRIVATE ${PROJECT_NAME}_VIIICompression
        PRIVATE ${PROJECT_NAME}_project_warnings
        PRIVATE Threads::Threads
        PRIVATE ToolsLibrary_tl
        )

#fiflfs files extract not nested
add_executable(${PROJECT_NAME}_fiflfs_extract
        fiflfs_extract.cpp
        fiflfs_extract.hpp)
set_target_properties(${PROJECT_NAME}_fiflfs_extract PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${PROJECT_NAME}_fiflfs_extract PROPERTIES OUTPUT_NAME "fiflfs_extract")
target_link_libraries(${PROJECT_NAME}_fiflfs_extract
        PRIVATE ${PROJECT_NAME}_VIIICompression
        PRIVATE ${PROJECT_NAME}_VIIIArchive
        PRIVATE ${PROJECT_NAME}_project_warnings
        PRIVATE Threads::Threads
        PRIVATE ToolsLibrary_tl
        )

#fiflfs files create not nested
add_executable(${PROJECT_NAME}_fiflfs_create
        fiflfs_create.cpp
        fiflfs_create.hpp)
set_target_properties(${PROJECT_NAME}_fiflfs_create PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${PROJECT_NAME}_fiflfs_create PROPERTIES OUTPUT_NAME "fiflfs_create")
target_link_libraries(${PROJECT_NAME}_fiflfs_create
        PRIVATE ${PROJECT_NAME}_VIIICompression
        PRIVATE ${PROJECT_NAME}_VIIIArchive
        PRIVATE ${PROJECT_NAME}_project_warnings
        PRIVATE Threads::Threads
        PRIVATE ToolsLibrary_tl
        )

#convert tile map to csv
add_executable(${PROJECT_NAME}_tile_map_to_csv
        tile_map_to_csv.cpp
        tile_map_to_csv.hpp)
set_target_properties(${PROJECT_NAME}_tile_map_to_csv PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${PROJECT_NAME}_tile_map_to_csv PROPERTIES OUTPUT_NAME "tile_map_to_csv")
target_link_libraries(${PROJECT_NAME}_tile_map_to_csv
        PRIVATE ${PROJECT_NAME}_VIIIGraphics
        PRIVATE ${PROJECT_NAME}_VIIITools
        PRIVATE ${PROJECT_NAME}_project_warnings
        PRIVATE Threads::Threads
        PRIVATE ToolsLibrary_tl
        )

#convert back from csv to tile map
add_executable(${PROJECT_NAME}_tile_csv_to_map
        tile_csv_to_map.cpp
        tile_csv_to_map.hpp)
set_target_properties(${PROJECT_NAME}_tile_csv_to_map PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${PROJECT_NAME}_tile_csv_to_map PROPERTIES OUTPUT_NAME "tile_csv_to_map")
target_link_libraries(${PROJECT_NAME}_tile_csv_to_map
        PRIVATE ${PROJECT_NAME}_VIIIGraphics
        PRIVATE ${PROJECT_NAME}_VIIITools
        PRIVATE ${PROJECT_NAME}_project_warnings
        PRIVATE Threads::Threads
        PRIVATE ToolsLibrary_tl
        PRIVATE ctre
        )
if (CMAKE_BUILD_TYPE EQUAL "Debug" OR CMAKE_BUILD_TYPE EQUAL "RelWithDebInfo")
    if (NOT MSVC)
        target_compile_options(${PROJECT_NAME}_tile_csv_to_map
                PRIVATE -fno-inline)
    endif ()
endif ()
if (MSVC)
    target_compile_options(${PROJECT_NAME}_tile_csv_to_map
            PRIVATE /Ob0) #https://docs.microsoft.com/en-us/cpp/build/reference/ob-inline-function-expansion?view=msvc-160
endif ()