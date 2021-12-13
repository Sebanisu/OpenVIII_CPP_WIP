add_library(${PROJECT_NAME}_VIIIMenuGroup INTERFACE)
target_link_libraries(${PROJECT_NAME}_VIIIMenuGroup INTERFACE ${PROJECT_NAME}_VIIICommon)
#target_precompile_headers(${PROJECT_NAME}_VIIIMenuGroup INTERFACE
#        <array>
#        <cstdint>
#        <cstring>
#        <iostream>
#        <ostream>
#        <sstream>
#        <string_view>
#        <vector>
#        )