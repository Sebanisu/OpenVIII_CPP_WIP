add_library(${PROJECT_NAME}_VIIIField INTERFACE)
target_link_libraries(${PROJECT_NAME}_VIIIField INTERFACE ${PROJECT_NAME}_VIIICommon)
#target_precompile_headers(
#        ${PROJECT_NAME}_VIIIField INTERFACE
#        <cstdint>
#        <iostream>
#        <span>
#        <type_traits>
#        <utility>
#        <vector>
#)