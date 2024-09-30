# Raspberry Pi Pico Driver class collection
A Collection of the Raspberry Pi [Pico](https://www.raspberrypi.com/products/raspberry-pi-pico/)/[Pico2](https://www.raspberrypi.com/products/raspberry-pi-pico-2/) classes. Also, a [SDK API](https://www.raspberrypi.com/documentation/pico-sdk/hardware.html#group_sm_config_1gaed7a6e7dc4f1979c7c62e4773df8c79b) wrapper class is provided. 

An online [HTML documentation](https://suikan4github.github.io/rpp_driver/) is available for the details.  

# Details
This class collection ( or class library ) encapsule the certain data inside class. And it also allows you to use the dependency-injection inside your project. So, you can test your code with [Google Test](https://github.com/google/googletest) before testing on the target hardware. 

These classes are provided in this version. 

| Class                           | Header file          | Description |
|---------------------------------|----                  |----------------------------- |
| ::rpp_driver::SdkWrapper        | sdkwrapper.hpp       | Wrapper class of Pico SDK    |
| ::rpp_driver::GpioBasic         | gpiobasic.hpp        | Basic GPIO controller        |
| ::rpp_driver::I2cMaster         | i2cmaster.hpp        | I2C Master controller        |
| ::rpp_driver::I2sSlaveDuplex    | i2sslaveduplex.hpp   | Polling based PIO I2S driver |
| ::rpp_driver::Adau1361          | adau1361.hpp         | Audio CODEC driver           |
| ::rpp_driver::UmbAdau1361Lower  | umbadau1361lower.hpp | CODEC lower driver dedicated to UMB-ADAU1361-A board |


## How to obtain this project

The newest copy of this project is found in the [GitHub repository](https://github.com/suikan4github/rpp_driver). 

Run the following command to obtain this project from CLI:
```sh
git clone --recursive https://github.com/suikan4github/rpp_driver.git
```

## How to obtain the sample programs

The newest copy of the sample program project is found in the [GitHub repository](https://github.com/suikan4github/rpp_driver-sample). 

Run the following command to obtain the sample from CLI:
```sh
git clone --recursive https://github.com/suikan4github/rpp_driver-sample.git
```

## Sample codes
You can obtain sample applications from [rpp-driver_sample](https://github.com/suikan4github/rpp_driver-sample) repository.


# Tools and building
## Installing tool
To build the samples or test, you need to install the build tools. 
The installation of Ubuntu is : 

```sh
apt-get -y update
apt-get -y install build-essential cmake ninja-build git doxygen
apt-get -y install gcc-arm-none-eabi libnewlib-arm-none-eabi
apt-get -y install doxygen graphviz
```

## Building the tests
The driver classes are tested by GoogleTest and fff. Follow the procedure to build the tests. 

From the repository root ( where this README.md stays), run the following commands. 
```sh
mkdir build
cmake -B build -S . -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ 
cmake --build build --config Debug --target all
```

## Building document
An API document is provided as HTML files. 
To obtain it, run doxygen at the project root ( where the README.md exists).

```sh
doxygen
```

The documentation will be generated under the docs/html/ subdirectory. 

# Integration to your RasPi Pico project
To use the rpp_driver, as a first step, you need to add following lines into your CMakeLists.txt. 

```
add_subdirectory(rpp_driver)
```

```
target_link_libraries(${PROJECT_NAME}  pico_stdlib 
                                        hardware_i2c
                                        hardware_pio
                                        rpp_driver)
```

Note : Whichever you use PIO/I2C or not, you need to link these libraries. 

By linking library, the include path for the include files are set automatically. 

# License
This project is provided under [MIT License](LICENSE). 

# Copyright
@author Seiichi Horie
