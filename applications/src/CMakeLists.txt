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

add_subdirectory(TestBattle)

##test JSM, uses new code to extract files instead of old versions.
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


#compress file with lzss like ff8 would. or uncompress
add_executable(${PROJECT_NAME}_test_lzss_for_rust
        test_lzss_for_rust.cpp)
apply_common_operations(test_lzss_for_rust)

add_subdirectory(fiflfs_create)

add_subdirectory(fiflfs_extract)

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
find_package(ctre CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME}_tile_csv_to_map
        PRIVATE ctre::ctre
        )
if (MSVC)
    target_compile_options(${PROJECT_NAME}_tile_csv_to_map
            PRIVATE /Ob0) #https://docs.microsoft.com/en-us/cpp/build/reference/ob-inline-function-expansion?view=msvc-160
endif ()

##test extract all, uses new code to extract files instead of old versions.
add_executable(${PROJECT_NAME}_double_cam
        double_cam.cpp double_cam.hpp)
apply_common_operations(double_cam)

#compress file with lzss like ff8 would. or uncompress
add_executable(${PROJECT_NAME}_tiz
        tiz.cpp
        lz4_lzss_common_ff8.hpp)
apply_common_operations(tiz)


add_executable(${PROJECT_NAME}_detected_ff8_paths
        detected_ff8_paths.cpp)
apply_common_operations(detected_ff8_paths)