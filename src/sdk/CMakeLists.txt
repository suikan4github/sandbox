cmake_minimum_required(VERSION 3.13)

# Common declaration for both UNIX/Win and bare metal.
target_include_directories(rpp_driver INTERFACE .)

# Bare metal specific declaration. 
if(NOT (${WIN32}) AND NOT (${UNIX})) # Is target bare metal?   
        # sdkwrapper.cpp is unable to build on Unix/Win. 
        # Then, test program include this cpp file to test. 
        # We need to build it only for bare metal target. 
        target_sources(rpp_driver INTERFACE 
                        sdkwrapper.cpp
        )
endif()

