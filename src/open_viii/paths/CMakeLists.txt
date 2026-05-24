find_package(ctre CONFIG REQUIRED)

add_library(${PROJECT_NAME}_VIIIPaths STATIC
    Paths.cpp
)

target_link_libraries(${PROJECT_NAME}_VIIIPaths
    PUBLIC ${PROJECT_NAME}_VIIICommon
    PUBLIC ${PROJECT_NAME}_VIIITools
    PUBLIC ${PROJECT_NAME}_project_warnings
    PUBLIC ctre::ctre
    PUBLIC spdlog::spdlog
    PUBLIC Threads::Threads
)

apply_common_operations(VIIIPaths)

#target_precompile_headers(${PROJECT_NAME}_VIIIPaths INTERFACE
#        <algorithm>
#        <filesystem>
#        <ranges>
#        <string>
#        <vector>
#        )