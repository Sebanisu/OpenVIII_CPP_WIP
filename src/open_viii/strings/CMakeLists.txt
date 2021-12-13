add_library(${PROJECT_NAME}_VIIIStrings INTERFACE)
target_link_libraries(${PROJECT_NAME}_VIIIStrings INTERFACE ${PROJECT_NAME}_VIIICommon)
#target_precompile_headers(
#        ${PROJECT_NAME}_VIIIStrings INTERFACE
#        <array>
#        <compare>
#        <cstdint>
#        <ranges>
#        <sstream>
#        <string_view>
#)