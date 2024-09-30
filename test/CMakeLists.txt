# To use FetchContent, we need CMake 3.14 or newer. 
cmake_minimum_required(VERSION 3.14)



file(GLOB SOURCES CMAKE_CONFIGURE_DEPENDS "./*.cpp")

# Add the executable for the testcase which is using googletest
add_executable(${PROJECT_NAME} ${SOURCES})

    # Add the library under test.
    target_link_libraries(${PROJECT_NAME}  
                            GTest::gmock_main
                            rpp_driver  
                            )
    target_link_options(${PROJECT_NAME}
                            PRIVATE --coverage)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})

