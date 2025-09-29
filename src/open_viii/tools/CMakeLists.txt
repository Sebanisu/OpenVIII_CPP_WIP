add_library(${PROJECT_NAME}_VIIITools INTERFACE)
target_link_libraries(${PROJECT_NAME}_VIIITools 
    INTERFACE ${PROJECT_NAME}_VIIICommon 
    INTERFACE ToolsLibrary_tl
)
#target_precompile_headers(${PROJECT_NAME}_VIIITools INTERFACE
#        <algorithm>
#        <array>
#        <atomic>
#        <cctype>
#        <concepts>
#        <cstdint>
#        <cstring>
#        <filesystem>
#        <fstream>
#        <iostream>
#        <iterator>
#        <limits>
#        <optional>
#        <random>
#        <ranges>
#        <span>
#        <string>
#        <string_view>
#        <thread>
#        <type_traits>
#        <vector>
#        )
