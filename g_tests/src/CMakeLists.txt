################################
# Tests
################################
# Add test cpp file
add_executable(${PROJECT_NAME}_gtestexample gtestexample.cpp)
# Link test executable against gtest & gtest_main
target_link_libraries(${PROJECT_NAME}_gtestexample gtest gtest_main)

