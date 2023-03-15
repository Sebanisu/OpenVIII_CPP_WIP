find_package(Threads REQUIRED)
##test app for trying to extract a single file from the FIFLFS archives.
function(apply_common_operations target_name)
    # Find required Threads package
    find_package(Threads REQUIRED)

    # Set target properties
    set_target_properties(${PROJECT_NAME}_${target_name} PROPERTIES LINKER_LANGUAGE CXX)
    set_target_properties(${PROJECT_NAME}_${target_name} PROPERTIES OUTPUT_NAME ${target_name})

    # Link required libraries
    target_link_libraries(${PROJECT_NAME}_${target_name}
            PUBLIC ${PROJECT_NAME}_VIIIArchive
            PUBLIC ${PROJECT_NAME}_VIIIPaths
            PUBLIC ${PROJECT_NAME}_project_warnings
            PUBLIC ToolsLibrary_tl
            PUBLIC Threads::Threads
            PRIVATE ${PROJECT_NAME}_VIIIGraphics
            )
    target_compile_features(${PROJECT_NAME}_${target_name}
            PUBLIC cxx_std_23
            PUBLIC cxx_std_20)
    # Apply MSVC-specific settings if applicable
    if (MSVC)
        target_compile_definitions(${PROJECT_NAME}_${target_name}
                PUBLIC _CRT_SECURE_NO_WARNINGS) # required by libpng
        target_compile_options(${PROJECT_NAME}_${target_name} PUBLIC "/bigobj")
        target_link_options(${PROJECT_NAME}_${target_name} PUBLIC "/PROFILE")
    endif ()

    # Set target paths
    SET_TARGET_PATHS(${PROJECT_NAME}_${target_name})
endfunction()

add_executable(${PROJECT_NAME}_TestExtractALLSearch
        TestExtractALLSearch.hpp
        TestExtractALLSearch.cpp)
apply_common_operations(TestExtractALLSearch)

add_executable(${PROJECT_NAME}_TestArchives
        TestArchives.hpp TestArchives.cpp)
apply_common_operations(TestArchives)

add_executable(${PROJECT_NAME}_TestSearchZZZ
        TestSearchZZZ.hpp TestSearchZZZ.cpp)
apply_common_operations(TestSearchZZZ)

add_executable(${PROJECT_NAME}_TestKernel
        TestKernel.cpp TestKernel.hpp)
apply_common_operations(TestKernel)

##test mngrphd.bin and mngrp.bin
add_executable(${PROJECT_NAME}_TestMenuGroup
        TestMenuGroup.cpp TestMenuGroup.hpp)
apply_common_operations(TestMenuGroup)

##test image texture libraries
add_executable(${PROJECT_NAME}_TestExtractImages
        TestExtractImages.cpp TestExtractImages.hpp)
apply_common_operations(TestExtractImages)

##test image Deswizzle of fields like pupu
add_executable(${PROJECT_NAME}_TestDeswizzleFields
        TestDeswizzleFields.cpp TestDeswizzleFields.hpp)
apply_common_operations(TestDeswizzleFields)

##test image Reswizzle of fields like pupu
add_executable(${PROJECT_NAME}_TestReswizzleFields
        TestReswizzleFields.cpp TestReswizzleFields.hpp)
apply_common_operations(TestReswizzleFields)

##test image ReDeswizzle of fields like pupu
add_executable(${PROJECT_NAME}_TestReDeswizzleFields
        TestReDeswizzleFields.cpp TestReDeswizzleFields.hpp)
apply_common_operations(TestReDeswizzleFields)

##test extract all, uses new code to extract files instead of old versions.
add_executable(${PROJECT_NAME}_TestExtractALL
        TestExtractALL.cpp TestExtractALL.hpp)
apply_common_operations(TestExtractALL)

##test extract all, uses new code to extract files instead of old versions.
add_executable(${PROJECT_NAME}_TestExtractPAK
        TestExtractPAK.cpp TestExtractPAK.hpp)
apply_common_operations(TestExtractPAK)

##test battle, uses new code to extract files instead of old versions.
add_executable(${PROJECT_NAME}_TestBattle
        TestBattle.cpp TestBattle.hpp)
apply_common_operations(TestBattle)

##test battle, uses new code to extract files instead of old versions.
add_executable(${PROJECT_NAME}_TestJSM
        TestJSM.cpp TestJSM.hpp)
apply_common_operations(TestJSM)

#compress file with lz4 like ff8 would. or uncompress
add_executable(${PROJECT_NAME}_lz4_ff8
        lz4_ff8.cpp
        lz4_lzss_common_ff8.hpp)
apply_common_operations(lz4_ff8)

#compress file with lzss like ff8 would. or uncompress
add_executable(${PROJECT_NAME}_lzss_ff8
        lzss_ff8.cpp
        lz4_lzss_common_ff8.hpp)
apply_common_operations(lzss_ff8)

#fiflfs files extract not nested
add_executable(${PROJECT_NAME}_fiflfs_extract
        fiflfs_extract.cpp
        fiflfs_extract.hpp)
apply_common_operations(fiflfs_extract)

#fiflfs files create not nested
add_executable(${PROJECT_NAME}_fiflfs_create
        fiflfs_create.cpp
        fiflfs_create.hpp)
apply_common_operations(fiflfs_create)

#convert tile map to csv
add_executable(${PROJECT_NAME}_tile_map_to_csv
        tile_map_to_csv.cpp
        tile_map_to_csv.hpp)
apply_common_operations(tile_map_to_csv)

#convert back from csv to tile map
add_executable(${PROJECT_NAME}_tile_csv_to_map
        tile_csv_to_map.cpp
        tile_csv_to_map.hpp)
apply_common_operations(tile_csv_to_map)
target_link_libraries(${PROJECT_NAME}_tile_csv_to_map
        PRIVATE ctre
        )
if (MSVC)
    target_compile_options(${PROJECT_NAME}_tile_csv_to_map
            PRIVATE /Ob0) #https://docs.microsoft.com/en-us/cpp/build/reference/ob-inline-function-expansion?view=msvc-160
endif ()

##test extract all, uses new code to extract files instead of old versions.
add_executable(${PROJECT_NAME}_double_cam
        double_cam.cpp double_cam.hpp)
apply_common_operations(double_cam)