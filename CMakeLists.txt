#[[
Copyright 2020 (c) 2020 Raspberry Pi (Trading) Ltd.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following
   disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
   disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
]]

# The first part of the CMakeLists.txt which download SDK automatically. 

# The original of this file can be found at : 
# https://github.com/raspberrypi/pico-sdk/blob/master/README.md

cmake_minimum_required(VERSION 3.13)

# initialize pico-sdk from GIT
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_FETCH_FROM_GIT on)

# pico_sdk_import.cmake is a single file copied from this SDK
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(my_project)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project

# The original of this file can be found at : 
# https://github.com/raspberrypi/pico-examples/blob/master/blink/CMakeLists.txt

add_executable(blink
        blink.cpp
        )

# pull in common dependencies
# Regarding the independe peripheral libraries, see : 
# - Understanding the C/C++ SDK architecture for the Raspberry Pi Pico
#   ( https://vanhunteradams.com/Pico/Setup/SDKArchitecture.html#Hardware-support-libraries )
# and : 
# - The section 4.1 of the Raspberry Pi Pico C/C++ SDK
#   ( https://vanhunteradams.com/Pico/Setup/PicoCSDK.pdf )
# Each hardware_FOO library needs the :
# #include <hardware/FOO.h>
# in the source code. 
target_link_libraries(blink pico_stdlib
# Un-comment the desired line to use the peripheral library.

# hardware_adc      # Analog to Digital Converter (ADC) API.
# hardware_claim    # Lightweight hardware resource management.
# hardware_clocks   # Clock Management API.
# hardware_divider  # Low-level hardware-divider access.
# hardware_dma      # DMA  Controller API.
# hardware_flash    # Low level flash programming and erase API.
# hardware_gpio     # General Purpose Input/Output (GPIO) API.
# hardware_i2c      # I2C Controller API.
# hardware_interp   # Hardware Interpolator API.
# hardware_irq      # Hardware interrupt handling.
# hardware_pio      # Programmable I/O (PIO) API.
# hardware_pll      # Phase Locked Loop control APIs.
# hardware_pwm      # Hardware Pulse Width Modulation (PWM) API.
# hardware_resets   # Hardware Reset API.
# hardware_rtc      # Hardware Real Time Clock API.
# hardware_spi      # Hardware SPI API.
# hardware_sync     # Low level hardware spin-lock, barrier and processor event API.
# hardware_timer    # Low-level hardware timer API.
# hardware_uart     # Hardware UART API.
# hardware_vreg     # Voltage Regulation API.
# hardware_watchdog # Hardware Watchdog Timer API.
# hardware_xosc     # Crystal Oscillator (XOSC) API.
)

# create map/bin/hex file etc.
pico_add_extra_outputs(blink)

# The Following command makes error outside the pico-examples framework. 
# So removed. 
# example_auto_set_url(blink)
