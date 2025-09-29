cmake_minimum_required(VERSION 3.18)
project(OpenVIII_CPP_WIP
        DESCRIPTION "Port of OpenVIII core library to C++20"
        LANGUAGES C CXX)
function(SET_TARGET_PATHS target)
    set_target_properties(${target}
            PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
            )
endfunction()
message(STATUS ${CMAKE_VERSION})
message(STATUS ${CMAKE_PATCH_VERSION})

set(CMAKE_CXX_STANDARD_REQUIRED ON) ## the value of the CXX_STANDARD target property is treated as a requirement
set(CMAKE_CXX_EXTENSIONS OFF) ## on g++ this ensures: -std=c++11 and not -std=gnu++11
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON) ## automatically create a module definition (.def) file with all global
## symbols found in the input .obj files for a SHARED library on Windows.

# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(${PROJECT_NAME}_project_warnings INTERFACE)

# standard compiler warnings
include(cmake/CompilerWarnings.cmake)
set_project_warnings(${PROJECT_NAME}_project_warnings ON)

# get external dependencies.
include(cmake/tools_library.cmake) # general use library

set(DOCS "NO" CACHE STRING "Build documentation")
# include docs folders
if ((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME) AND ${DOCS} STREQUAL "YES")
    add_subdirectory(raw_docs)
endif ()

find_package(Threads REQUIRED)
##test app for trying to extract a single file from the FIFLFS archives.
function(apply_common_operations target_name)
    # Find required Threads package
    find_package(Threads REQUIRED)

    # Set target properties
    set_target_properties(${PROJECT_NAME}_${target_name} PROPERTIES
        LINKER_LANGUAGE CXX
        OUTPUT_NAME ${target_name}
    )

     # Only link libraries if this is not VIIIPaths
    if(NOT target_name STREQUAL "VIIIPaths")
        target_link_libraries(${PROJECT_NAME}_${target_name}
            PUBLIC ${PROJECT_NAME}_VIIIArchive
            PUBLIC ${PROJECT_NAME}_VIIIPaths
            PUBLIC ${PROJECT_NAME}_project_warnings
            PUBLIC ToolsLibrary_tl
            PUBLIC Threads::Threads
            PRIVATE ${PROJECT_NAME}_VIIIGraphics
        )
    endif()

    target_compile_features(${PROJECT_NAME}_${target_name}
        PUBLIC cxx_std_23
        PUBLIC cxx_std_20
    )

    if(MSVC)
        target_compile_definitions(${PROJECT_NAME}_${target_name}
            PUBLIC _CRT_SECURE_NO_WARNINGS
        )
        target_compile_options(${PROJECT_NAME}_${target_name} 
            PUBLIC "/bigobj"
            PUBLIC "/Qpar"
            PUBLIC "/MP"
        )
        target_link_options(${PROJECT_NAME}_${target_name} PUBLIC "/PROFILE")

        set_property(TARGET ${PROJECT_NAME}_${target_name}
            PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
        )
    endif()

    SET_TARGET_PATHS(${PROJECT_NAME}_${target_name})
endfunction()

# include source folders
add_subdirectory(src)
if ((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME))
    add_subdirectory(applications)
endif ()
if ((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME) AND BUILD_TESTING)
    include(CTest)
    #include(cmake/googletest.cmake)
    include(cmake/ut.cmake)
    add_subdirectory(tests)
endif ()
#add_subdirectory(g_tests)