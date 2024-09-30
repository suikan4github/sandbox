#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "i2cmaster.hpp"

using testing::Return;

// Constructor test
TEST(I2cMaster, Constructor) {
  ::rpp_driver::MockSdkWrapper sdk;
  i2c_inst_t i2c_inst;

  uint freq = 100 * 1000;
  uint sda_pin = 17;
  uint scl_pin = 18;

  using ::testing::_;

  EXPECT_CALL(sdk, i2c_init(&i2c_inst, freq));
  EXPECT_CALL(sdk, gpio_set_function(scl_pin, GPIO_FUNC_I2C));
  EXPECT_CALL(sdk, gpio_set_function(sda_pin, GPIO_FUNC_I2C));
  EXPECT_CALL(sdk, gpio_pull_up(scl_pin));
  EXPECT_CALL(sdk, gpio_pull_up(sda_pin));

  ::rpp_driver::I2cMaster i2c(sdk, i2c_inst, freq, scl_pin, sda_pin);

  // We can ignore these call inside destructor
  EXPECT_CALL(sdk, i2c_deinit(_));

}  // Constructor

// Destructor test
TEST(I2cMaster, Destructor) {
  ::rpp_driver::MockSdkWrapper sdk;
  i2c_inst_t i2c_inst;

  uint freq = 100 * 1000;
  uint sda_pin = 17;
  uint scl_pin = 18;

  using ::testing::_;

  EXPECT_CALL(sdk, i2c_init(&i2c_inst, freq));
  EXPECT_CALL(sdk, gpio_set_function(scl_pin, GPIO_FUNC_I2C));
  EXPECT_CALL(sdk, gpio_set_function(sda_pin, GPIO_FUNC_I2C));
  EXPECT_CALL(sdk, gpio_pull_up(scl_pin));
  EXPECT_CALL(sdk, gpio_pull_up(sda_pin));

  ::rpp_driver::I2cMaster i2c(sdk, i2c_inst, freq, scl_pin, sda_pin);

  EXPECT_CALL(sdk, i2c_deinit(&i2c_inst));

}  // Destructor

TEST(I2cMaster, ReadBlocking) {
  ::rpp_driver::MockSdkWrapper sdk;
  i2c_inst_t i2c_inst;

  uint8_t addr = 3;
  uint8_t buf[5];
  bool nostop = true;
  int return_value = 11;

  using ::testing::_;

  // We can ignore these call inside constructor
  EXPECT_CALL(sdk, i2c_init(&i2c_inst, _));
  EXPECT_CALL(sdk, gpio_set_function(_, GPIO_FUNC_I2C)).Times(2);
  EXPECT_CALL(sdk, gpio_pull_up(_)).Times(2);

  ::rpp_driver::I2cMaster i2c(sdk, i2c_inst, 100 * 1000, 17, 23);

  EXPECT_CALL(sdk, i2c_read_blocking(&i2c_inst, addr, buf, sizeof(buf), nostop))
      .WillOnce(Return(return_value));

  EXPECT_CALL(sdk,
              i2c_read_blocking(&i2c_inst, addr, buf, sizeof(buf), !nostop))
      .WillOnce(Return(return_value));

  EXPECT_EQ(i2c.ReadBlocking(addr, buf, sizeof(buf), nostop), return_value);
  EXPECT_EQ(i2c.ReadBlocking(addr, buf, sizeof(buf), !nostop), return_value);
  // We can ignore these call inside destructor
  EXPECT_CALL(sdk, i2c_deinit(_));
}  // ReadBlocking

TEST(I2cMaster, WriteBlocking) {
  ::rpp_driver::MockSdkWrapper sdk;
  i2c_inst_t i2c_inst;

  uint8_t addr = 3;
  uint8_t buf[5];
  bool nostop = true;
  int return_value = 11;

  using ::testing::_;

  // We can ignore these call inside constructor
  EXPECT_CALL(sdk, i2c_init(&i2c_inst, _));
  EXPECT_CALL(sdk, gpio_set_function(_, GPIO_FUNC_I2C)).Times(2);
  EXPECT_CALL(sdk, gpio_pull_up(_)).Times(2);

  ::rpp_driver::I2cMaster i2c(sdk, i2c_inst, 100 * 1000, 17, 23);

  EXPECT_CALL(sdk,
              i2c_write_blocking(&i2c_inst, addr, buf, sizeof(buf), nostop))
      .WillOnce(Return(return_value));
  EXPECT_CALL(sdk,
              i2c_write_blocking(&i2c_inst, addr, buf, sizeof(buf), !nostop))
      .WillOnce(Return(return_value));

  EXPECT_EQ(i2c.WriteBlocking(addr, buf, sizeof(buf), nostop), return_value);
  EXPECT_EQ(i2c.WriteBlocking(addr, buf, sizeof(buf), !nostop), return_value);
  // We can ignore these call inside destructor
  EXPECT_CALL(sdk, i2c_deinit(_));
}  // WriteBlocking

TEST(I2cMaster, IsDeviceExisting) {
  ::rpp_driver::MockSdkWrapper sdk;
  i2c_inst_t i2c_inst;

  uint8_t addr[] = {17, 23};
  bool nostop = false;
  int return_value[] = {-1, 1};

  using ::testing::_;

  // We can ignore these call inside constructor
  EXPECT_CALL(sdk, i2c_init(&i2c_inst, _));
  EXPECT_CALL(sdk, gpio_set_function(_, GPIO_FUNC_I2C)).Times(2);
  EXPECT_CALL(sdk, gpio_pull_up(_)).Times(2);

  ::rpp_driver::I2cMaster i2c(sdk, i2c_inst, 100 * 1000, 17, 23);

  EXPECT_CALL(sdk, i2c_read_blocking(_, addr[0], _, 1, nostop))
      .WillOnce(Return(return_value[0]));

  EXPECT_CALL(sdk, i2c_read_blocking(_, addr[1], _, 1, nostop))
      .WillOnce(Return(return_value[1]));

  EXPECT_FALSE(i2c.IsDeviceExisting(addr[0]));
  EXPECT_TRUE(i2c.IsDeviceExisting(addr[1]));

  // We can ignore these call inside destructor
  EXPECT_CALL(sdk, i2c_deinit(_));
}
