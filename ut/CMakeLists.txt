add_executable(unit_tests
        unit_tests.cpp)
target_link_libraries(${PROJECT_NAME}_unit_tests PRIVATE Boost::ut)
