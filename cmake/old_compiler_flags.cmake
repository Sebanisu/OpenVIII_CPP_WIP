
#if (MSVC)
#    # warning level 4 and all warnings as errors
#    add_compile_options($<$<CONFIG:DEBUG>:/MDd> $<$<CONFIG:RELEASE>:/O2> /W4 /WX)
#else()
#    # lots of warnings and all warnings as errors
#    add_compile_options($<$<CONFIG:DEBUG>:-g> $<$<CONFIG:RELEASE>:-O3> -Wall -Wextra -Wshadow -Wpedantic -Wconversion -Wold-style-cast -Wunused -Wcast-align -Werror -Wuseless-cast -Wlogical-op -Wsign-conversion -Wduplicated-cond -Wduplicated-branches -Wnull-dereference -Wdouble-promotion -Wconversion -Woverloaded-virtual -Wnon-virtual-dtor -Wformat=2 -fsanitize=address,undefined -pthread)
#endif()

# Set default compile flags for GCC
#if (CMAKE_COMPILER_IS_GNUCXX)
#    message(STATUS "GCC detected, adding compile flags")
#    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
#    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wshadow -Wpedantic -Wconversion -Wold-style-cast -Wunused -Wcast-align -Werror -Wuseless-cast -Wlogical-op -Wsign-conversion -Wduplicated-cond -Wduplicated-branches -Wnull-dereference -Wdouble-promotion -Wconversion -Woverloaded-virtual -Wnon-virtual-dtor -Wformat=2 -fsanitize=address,undefined")
#    # -Wfatal-errors -Wpadded
#    # change to 2a for cpp20 pre release
#elseif (MSVC)
#    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
#    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
#    # change to latest for cpp20 pre release
#endif ()