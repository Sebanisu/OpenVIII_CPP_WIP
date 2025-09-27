add_library(${PROJECT_NAME}_VIIIGraphics INTERFACE)
add_subdirectory(color)
add_subdirectory(tim)
add_subdirectory(sp1)
add_subdirectory(tex)
add_subdirectory(sp2)
find_package(PNG REQUIRED)
target_link_libraries(${PROJECT_NAME}_VIIIGraphics
        INTERFACE ${PNG_LIBRARIES}
        INTERFACE ToolsLibrary_tl
        INTERFACE ${PROJECT_NAME}_VIIITools
        INTERFACE ${PROJECT_NAME}_VIIICommon
        INTERFACE PNG::PNG
        )
target_include_directories(${PROJECT_NAME}_VIIIGraphics INTERFACE ${PNG_INCLUDE_DIRS})
#target_precompile_headers(
#        ${PROJECT_NAME}_VIIIGraphics INTERFACE
#        <algorithm>
#        <array>
#        <bit>
#        <bitset>
#        <cassert>
#        <cctype>
#        <compare>
#        <concepts>
#        <cstdint>
#        <cstdio>
#        <cstring>
#        <execution>
#        <filesystem>
#        <fstream>
#        <iomanip>
#        <iostream>
#        <limits>
#        <memory>
#        <numeric>
#        <ostream>
#        <ranges>
#        <span>
#        <sstream>
#        <string>
#        <string_view>
#        <type_traits>
#        <utility>
#        <variant>
#        <vector>
#)