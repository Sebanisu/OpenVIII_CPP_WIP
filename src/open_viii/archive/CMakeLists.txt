add_library(${PROJECT_NAME}_VIIIArchive INTERFACE FIFLFS_operations.hpp)
target_link_libraries(${PROJECT_NAME}_VIIIArchive
        INTERFACE ${PROJECT_NAME}_VIIICompression
        INTERFACE ${PROJECT_NAME}_VIIITools
        INTERFACE ToolsLibrary_tl
        INTERFACE ${PROJECT_NAME}_VIIICommon
        )
#target_precompile_headers(${PROJECT_NAME}_VIIIArchive
#        INTERFACE
#        <algorithm>
#        <array>
#        <bit>
#        <cassert>
#        <compare>
#        <concepts>
#        <cstdint>
#        <cstring>
#        <execution>
#        <filesystem>
#        <fstream>
#        <functional>
#        <future>
#        <initializer_list>
#        <iostream>
#        <iterator>
#        <map>
#        <memory>
#        <optional>
#        <ranges>
#        <set>
#        <span>
#        <sstream>
#        <string>
#        <string_view>
#        <thread>
#        <type_traits>
#        <utility>
#        <variant>
#        <vector>
#        )