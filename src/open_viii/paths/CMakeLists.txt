find_package(ctre CONFIG REQUIRED)

add_library(${PROJECT_NAME}_VIIIPaths STATIC
    Paths.cpp
)

target_link_libraries(${PROJECT_NAME}_VIIIPaths
    PUBLIC ${PROJECT_NAME}_VIIICommon
    PUBLIC ${PROJECT_NAME}_VIIITools
)

# Only link ctre on Windows
target_link_libraries(${PROJECT_NAME}_VIIIPaths PRIVATE ctre::ctre)

apply_common_operations(VIIIPaths)

#target_precompile_headers(${PROJECT_NAME}_VIIIPaths INTERFACE
#        <algorithm>
#        <filesystem>
#        <ranges>
#        <string>
#        <vector>
#        )