cmake_minimum_required(VERSION 3.13)

# These declaration works for both UNIX/Win and bare metal. 
target_sources(rpp_driver INTERFACE 
                i2cmaster.cpp)
target_include_directories(rpp_driver INTERFACE .)
