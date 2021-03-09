add_executable(${PROJECT_NAME}_unit_tests
        unit_tests.cpp)
target_link_libraries(${PROJECT_NAME}_unit_tests PRIVATE Boost::ut)
