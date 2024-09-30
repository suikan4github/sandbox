#include "i2cmaster.hpp"

::rpp_driver::I2cMaster::I2cMaster(SdkWrapper& sdk, i2c_inst_t& i2c,
                                   uint clock_freq, uint scl_pin, uint sda_pin)
    : i2c_(i2c), sdk_(sdk) {
  sdk_.i2c_init(&i2c_, clock_freq);  // I2C bitrate

  sdk_.gpio_set_function(sda_pin, GPIO_FUNC_I2C);
  sdk_.gpio_set_function(scl_pin, GPIO_FUNC_I2C);
  sdk_.gpio_pull_up(sda_pin);
  sdk_.gpio_pull_up(scl_pin);
}

::rpp_driver::I2cMaster::~I2cMaster() { sdk_.i2c_deinit(&i2c_); }

int ::rpp_driver::I2cMaster::ReadBlocking(uint8_t addr, uint8_t* dst,
                                          size_t len, bool nostop) {
  return sdk_.i2c_read_blocking(&i2c_, addr, dst, len, nostop);
}

int ::rpp_driver::I2cMaster::WriteBlocking(uint8_t addr, const uint8_t* src,
                                           size_t len, bool nostop) {
  return sdk_.i2c_write_blocking(&i2c_, addr, src, len, nostop);
}

bool rpp_driver::I2cMaster::IsDeviceExisting(uint8_t addr) {
  uint8_t dummy_buf[1];

  // try to read one byte from specific address I2C device.
  // If no error, that mean there is a device.
  return (PICO_ERROR_GENERIC !=
          ReadBlocking(addr, dummy_buf, sizeof(dummy_buf), false));
}
