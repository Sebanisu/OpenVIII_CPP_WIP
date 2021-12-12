add_library(${PROJECT_NAME}_VIIIStrings INTERFACE)
target_precompile_headers(
        ${PROJECT_NAME}_VIIIStrings INTERFACE
        <array>
        <compare>
        <cstdint>
        <ranges>
        <sstream>
        <string_view>
)