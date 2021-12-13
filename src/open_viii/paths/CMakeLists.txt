add_library(${PROJECT_NAME}_VIIIPaths INTERFACE)
target_link_libraries(${PROJECT_NAME}_VIIIPaths INTERFACE ${PROJECT_NAME}_VIIICommon)
#target_precompile_headers(${PROJECT_NAME}_VIIIPaths INTERFACE
#        <algorithm>
#        <filesystem>
#        <ranges>
#        <string>
#        <vector>
#        )