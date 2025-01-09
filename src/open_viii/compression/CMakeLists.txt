add_library(${PROJECT_NAME}_VIIICompression INTERFACE)
# target_link_libraries(${PROJECT_NAME}_VIIICompression
#         INTERFACE lz4::lz4
#         INTERFACE ${PROJECT_NAME}_VIIICommon
#         )
# add_library(${PROJECT_NAME}_VIIICompression INTERFACE)

# Find LZ4 package and set appropriate target name
if(CONAN_PACKAGE_FOUND)
    find_package(LZ4 REQUIRED)
    target_link_libraries(${PROJECT_NAME}_VIIICompression
    INTERFACE LZ4::lz4_static
    INTERFACE ${PROJECT_NAME}_VIIICommon
    )
elseif(VCPKG_TOOLCHAIN)
    find_package(lz4 CONFIG REQUIRED)
    target_link_libraries(${PROJECT_NAME}_VIIICompression
    INTERFACE lz4::lz4
    INTERFACE ${PROJECT_NAME}_VIIICommon)
else()
    message(FATAL_ERROR "No package manager found. Please use Conan or Vcpkg.")
endif()