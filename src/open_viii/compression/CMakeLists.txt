add_library(${PROJECT_NAME}_VIIICompression INTERFACE)
target_link_libraries(${PROJECT_NAME}_VIIICompression
        INTERFACE lz4::lz4
        INTERFACE ${PROJECT_NAME}_VIIICommon
        )
#target_precompile_headers(${PROJECT_NAME}_VIIICompression INTERFACE
#        <algorithm>
#        <array>
#        <ranges>
#        <span>
#        <vector>
#        <lib/lz4.h>
#        <variant> #concepts.hpp uses this.
#        )