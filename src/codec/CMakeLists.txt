cmake_minimum_required(VERSION 3.13)

# These declaration works for both UNIX/Win and bare metal. 
target_sources(rpp_driver INTERFACE 
                adau1361.cpp
                adau1361lower.cpp
                umbadau1361lower.cpp)
target_include_directories(rpp_driver INTERFACE .)
