function(add_test_common file_name)
    add_executable(${PROJECT_NAME}_UT_${file_name}
            ${file_name}.cpp)
    target_link_libraries(${PROJECT_NAME}_UT_${file_name}
            PRIVATE Boost::ut
            PRIVATE ${PROJECT_NAME}_project_warnings
            )
    target_include_directories(${PROJECT_NAME}_UT_${file_name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    set_target_properties(${PROJECT_NAME}_UT_${file_name} PROPERTIES OUTPUT_NAME "${file_name}")
    target_compile_definitions(${PROJECT_NAME}_UT_${file_name} PRIVATE BOOST_UT_DISABLE_MODULE=1)
    add_test(
            NAME ${PROJECT_NAME}_UT_${file_name} COMMAND ${PROJECT_NAME}_UT_${file_name}
    )
    target_compile_features(${PROJECT_NAME}_UT_${file_name}
            PUBLIC cxx_std_23
            PUBLIC cxx_std_20)
    if (MSVC)
        target_compile_definitions(${PROJECT_NAME}_UT_${file_name}
                PUBLIC _CRT_SECURE_NO_WARNINGS) # required by libpng
        target_compile_options(${PROJECT_NAME}_UT_${file_name} PUBLIC "/bigobj")
        target_link_options(${PROJECT_NAME}_UT_${file_name} PUBLIC "/PROFILE")
    endif ()
    #    target_compile_definitions(${PROJECT_NAME}_UT_${file_name}
    #            PRIVATE BOOST_UT_DISABLE_MODULE
    #            )
endfunction()