add_library(${PROJECT_NAME}_VIIIKernel INTERFACE)
target_precompile_headers(${PROJECT_NAME}_VIIIKernel INTERFACE
        <algorithm>
        <array>
        <compare>
        <concepts>
        <cstdint>
        <cstring>
        <iostream>
        <sstream>
        <string>
        <string_view>
        <utility>
        <vector>
        )