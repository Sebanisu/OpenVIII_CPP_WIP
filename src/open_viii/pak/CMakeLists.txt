add_library(${PROJECT_NAME}_VIIIPak INTERFACE)
target_precompile_headers(${PROJECT_NAME}_VIIIPak INTERFACE
        <algorithm>
        <array>
        <cctype>
        <cinttypes>
        <cstdint>
        <filesystem>
        <iostream>
        <istream>
        <ostream>
        <ranges>
        <string>
        <string_view>
        <type_traits>
        <utility>
        <vector>
        )