add_subdirectory(OpenVIII)
target_include_directories(VIIIKernel PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(VIIIArchive PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(VIIIStrings PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})