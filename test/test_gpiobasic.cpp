#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <random>

#include "gpiobasic.hpp"
#include "sdkwrapper.hpp"

class GpioBasicTest : public ::testing::Test {
 protected:
  ::rpp_driver::MockSdkWrapper sdk_;
  unsigned int gpio_pin_;  // GPIO pin number.
  ::rpp_driver::GpioBasic* gpio_under_test_;

  virtual void SetUp() {
    std::random_device rng;
    gpio_pin_ = rng();
    EXPECT_CALL(sdk_, gpio_init(gpio_pin_)).Times(1);
    gpio_under_test_ = new ::rpp_driver::GpioBasic(sdk_, gpio_pin_);
  }

  virtual void TearDown() {
    EXPECT_CALL(sdk_, gpio_deinit(gpio_pin_)).Times(1);
    delete (gpio_under_test_);
  }
};

// -----------------------------------------------------------------
//
//                          Constructor and Deconstructor ()
//
// -----------------------------------------------------------------

TEST(GpioBasic, ConstructorDeconstructor) {
  ::rpp_driver::MockSdkWrapper sdk;
  std::random_device rng;

  const unsigned int pin = rng();  // GPIO pin number.

  using ::testing::InSequence;
  {
    InSequence dummy;
    EXPECT_CALL(sdk, gpio_init(pin));
    EXPECT_CALL(sdk, gpio_deinit(pin));
  }

  ::rpp_driver::GpioBasic* gpio = new ::rpp_driver::GpioBasic(sdk, pin);
  delete (gpio);
}  // TEST(GpioBasic, ConstructorDeconstructor)

// -----------------------------------------------------------------
//
//                          Member functions.
//
// -----------------------------------------------------------------

TEST_F(GpioBasicTest, SetDir) {
  using ::testing::InSequence;
  {
    InSequence dummy;
    EXPECT_CALL(sdk_, gpio_set_dir(gpio_pin_, true));
    EXPECT_CALL(sdk_, gpio_set_dir(gpio_pin_, false));
  }
  gpio_under_test_->SetDir(true);
  gpio_under_test_->SetDir(false);
}  // TEST_F(GpioBasicTest, SetDir)

TEST_F(GpioBasicTest, SetInputEnabled) {
  using ::testing::InSequence;
  {
    InSequence dummy;
    EXPECT_CALL(sdk_, gpio_set_input_enabled(gpio_pin_, true));
    EXPECT_CALL(sdk_, gpio_set_input_enabled(gpio_pin_, false));
  }
  gpio_under_test_->SetInputEnabled(true);
  gpio_under_test_->SetInputEnabled(false);
}  // TEST_F(GpioBasicTest, SetInputEnabled)

TEST_F(GpioBasicTest, Put) {
  using ::testing::InSequence;
  {
    InSequence dummy;
    EXPECT_CALL(sdk_, gpio_put(gpio_pin_, true));
    EXPECT_CALL(sdk_, gpio_put(gpio_pin_, false));
  }
  gpio_under_test_->Put(true);
  gpio_under_test_->Put(false);
}  // TEST_F(GpioBasicTest, Put)

TEST_F(GpioBasicTest, Toggle) {
  using ::testing::InSequence;
  using ::testing::Return;
  {
    InSequence dummy;
    EXPECT_CALL(sdk_, gpio_get(gpio_pin_)).WillOnce(Return(true));
    EXPECT_CALL(sdk_, gpio_put(gpio_pin_, false));
    EXPECT_CALL(sdk_, gpio_get(gpio_pin_)).WillOnce(Return(false));
    EXPECT_CALL(sdk_, gpio_put(gpio_pin_, true));
  }
  gpio_under_test_->Toggle();
  gpio_under_test_->Toggle();
}  // TEST_F(GpioBasicTest, Toggle)

TEST_F(GpioBasicTest, Get) {
  using ::testing::InSequence;
  using ::testing::Return;
  {
    InSequence dummy;
    EXPECT_CALL(sdk_, gpio_get(gpio_pin_)).WillOnce(Return(true));
    EXPECT_CALL(sdk_, gpio_get(gpio_pin_)).WillOnce(Return(false));
  }

  EXPECT_TRUE(gpio_under_test_->Get());
  EXPECT_FALSE(gpio_under_test_->Get());
}  // TEST_F(GpioBasicTest, Get)

TEST_F(GpioBasicTest, PullUp) {
  EXPECT_CALL(sdk_, gpio_pull_up(gpio_pin_));
  gpio_under_test_->PullUp();
}  // TEST_F(GpioBasicTest, PullUp)

TEST_F(GpioBasicTest, PullDown) {
  EXPECT_CALL(sdk_, gpio_pull_down(gpio_pin_));
  gpio_under_test_->PullDown();
}  // TEST_F(GpioBasicTest, PullDown)

TEST_F(GpioBasicTest, DisablePulls) {
  EXPECT_CALL(sdk_, gpio_disable_pulls(gpio_pin_));
  gpio_under_test_->DisablePulls();
}  // TEST_F(GpioBasicTest, DisablePulls)
