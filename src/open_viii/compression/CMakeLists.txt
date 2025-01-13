add_library(${PROJECT_NAME}_VIIICompression INTERFACE)

# Check for vcpkg first, then fall back to Conan
if(VCPKG_TOOLCHAIN)
    # Assume vcpkg is being used
    find_package(lz4 CONFIG REQUIRED)
    target_link_libraries(${PROJECT_NAME}_VIIICompression
        INTERFACE lz4::lz4
        INTERFACE ${PROJECT_NAME}_VIIICommon
    )
else()
    # Assume Conan if vcpkg is not found
    find_package(lz4 REQUIRED)
    target_link_libraries(${PROJECT_NAME}_VIIICompression
        INTERFACE LZ4::lz4_static
        INTERFACE ${PROJECT_NAME}_VIIICommon
    )
endif()