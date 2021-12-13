add_library(${PROJECT_NAME}_VIIIBattle INTERFACE)
target_link_libraries(${PROJECT_NAME}_VIIIBattle INTERFACE ${PROJECT_NAME}_VIIICommon)
#target_precompile_headers(${PROJECT_NAME}_VIIIBattle INTERFACE
#        <algorithm>
#        <array>
#        <bit>
#        <cstdint>
#        <iterator>
#        <ostream>
#        <ranges>
#        <span>
#        <string>
#        <string_view>
#        <type_traits>
#        <vector>
#        )