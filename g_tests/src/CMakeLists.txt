################################
# Tests
################################
# Add test cpp file
add_executable(gtestexample gtestexample.cpp)
# Link test executable against gtest & gtest_main
target_link_libraries(gtestexample gtest gtest_main)

