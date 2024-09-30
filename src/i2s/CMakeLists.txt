cmake_minimum_required(VERSION 3.13)

# Common declaration for both UNIX/Win and bare metal.
target_sources(rpp_driver INTERFACE 
                i2sslaveduplex.cpp)
target_include_directories(rpp_driver INTERFACE .)

# Bare metal specific declaration. 
if(NOT (${WIN32}) AND NOT(${UNIX})) # Is Target bare metal? 
        # Generate PIO program. 
        pico_generate_pio_header(rpp_driver ${CMAKE_CURRENT_LIST_DIR}/i2s_slave_duplex.pio)
endif()
