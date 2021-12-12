add_library(${PROJECT_NAME}_VIIIBattle INTERFACE)
target_precompile_headers(${PROJECT_NAME}_VIIIBattle INTERFACE
        <algorithm>
        <array>
        <bit>
        <cstdint>
        <iterator>
        <ostream>
        <ranges>
        <span>
        <string>
        <string_view>
        <type_traits>
        <vector>
        )