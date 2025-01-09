# include(FetchContent)
# FetchContent_Declare(
#         lz4_lib
#         GIT_REPOSITORY https://github.com/lz4/lz4.git
#         GIT_TAG v1.9.2
# )
# FetchContent_GetProperties(lz4_lib)
# if (NOT lz4_lib_POPULATED)
#     message(NOTICE "--lz4_lib_POPULATED")
#     FetchContent_Populate(lz4_lib)
#     add_library(${PROJECT_NAME}_External_lz4
#             ${lz4_lib_SOURCE_DIR}/lib/lz4.h
#             ${lz4_lib_SOURCE_DIR}/lib/lz4.c
#             )
#     set_target_properties(${PROJECT_NAME}_External_lz4 PROPERTIES LINKER_LANGUAGE CXX)
#     target_include_directories(${PROJECT_NAME}_External_lz4 INTERFACE ${lz4_lib_SOURCE_DIR})
# endif ()