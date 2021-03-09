add_executable(unit_tests
        unit_tests.cpp)
target_link_libraries(unit_tests PRIVATE boost::ut)
