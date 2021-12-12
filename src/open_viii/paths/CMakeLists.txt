##define array with path info.
add_library(${PROJECT_NAME}_VIIIPaths INTERFACE)
target_precompile_headers(${PROJECT_NAME}_VIIIPaths INTERFACE
        <algorithm>
        <filesystem>
        <ranges>
        <string>
        <vector>
        )