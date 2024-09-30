# To use FetchContent, we need CMake 3.14 or newer. 
cmake_minimum_required(VERSION 3.14)

  # GoogleTest requires at least C++14
  # To use std::size() and __has_include, we need c++17
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)


# If the configuratopm target is Windows or Unix, this project
# will be build as a test program of the rpp_driver. 

# If the configuration target is not above, this porject 
# will be build as a rpp_driver library. 

if((${WIN32}) OR (${UNIX})) # Is Target Windows or Unix? 

  # Redefine project for testing.
  project(test_rpp_driver)

  # Enable test to let the VS code find the test.
  include(CTest)
  enable_testing()

  include(FetchContent)
  FetchContent_Declare(
    googletest
  	GIT_REPOSITORY "https://github.com/google/googletest.git"
  	GIT_TAG "v1.15.2"
    )
  # For Windows: Prevent overriding the parent project's compiler/linker settings
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)

  add_subdirectory(src)
  add_subdirectory(test)

else()  # Is target RasPi Pico? 
  add_subdirectory(src)

endif()
