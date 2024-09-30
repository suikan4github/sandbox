/**
 * @file gpiobasic.hpp
 *
 * @brief basic GPIO controller.
 * @date 2024/Sep/20
 * @author Seiichi  Horie
 * @brief Collection of the basic fuction of GPIO .
 * @copyright COPYRIGHT 2024 Seiichi Horie
 */

#ifndef PICO_DRIVER_SRC_GPIO_GPIOBASIC_HPP_
#define PICO_DRIVER_SRC_GPIO_GPIOBASIC_HPP_

#include <stdint.h>

#if __has_include(<hardware/gpio.h>)
#include "hardware/gpio.h"
#else
// Alternate definition for unit test.
#endif  // __has_include(<hardware/i2c.h>)

#include "sdkwrapper.hpp"

namespace rpp_driver {
/**
 * @brief Basic GPIO driver class.
 * @details
 * This class provides the handy way to control GPIO. To use
 * this class, pass a GPIO pin# through the constructor.
 * So, this class initialize and deinitialize that pin in the constructor and
 * destructor, respectively.
 */
class GpioBasic {
 public:
  /**
   * @brief Initialize the given GPIO pin and setup the pins.
   * @param sdk Sdk wrapper to inject the dependency.
   * @param pin GPIO pin number.
   * @details
   * Receive the GPIO pin.
   * And then, init it by SdkWrapper::gpio_init().
   */
  GpioBasic(SdkWrapper &sdk, uint pin);
  GpioBasic() = delete;
  /**
   * @brief deinit the GPIO by SdkWrapper::gpio_deinit();
   */
  virtual ~GpioBasic();
  /**
   * @brief Set a single GPIO direction.
   * @param out true for out, false for in.
   */
  virtual void SetDir(bool out);

  /**
   * @brief Enable GPIO input.
   * @param enabled true to enable input on this GPIO.
   */
  virtual void SetInputEnabled(bool enabled);

  /**
   * @brief Drive a single GPIO high/low.
   * @param value If false clear the GPIO, otherwise set it.
   */
  virtual void Put(bool value);

  /**
   * @brief Get state of a single GPIO.
   * @returns Current state of this GPIO. 0 for low, non-zero for high
   */
  virtual bool Get();

  /**
   * @brief Toggle the output level of this GPIO.
   * @details
   * If the current level is "H", set it to "L".
   * If the current level is "L", set it to "H".
   */
  virtual void Toggle();

  /**
   * @brief Set this GPIO to be pulled up.
   * @
   */
  virtual void PullUp();

  /**
   * @brief Set this GPIO to be pulled down.
   */
  virtual void PullDown();

  /**
   * @brief Unset pulls on this GPIO.
   *
   */
  virtual void DisablePulls();

 private:
  SdkWrapper &sdk_;
  const uint pin_;
};
#if __has_include(<gmock/gmock.h>)

class MockGpioBasic : public SdkWrapper {
 public:
  MOCK_METHOD1(SetDir, void(bool));
  MOCK_METHOD1(SetInputEnabled, void(bool));
  MOCK_METHOD1(Put, void(bool));
  MOCK_METHOD0(Get, bool(void));
  MOCK_METHOD0(Toggle, void(void));
  MOCK_METHOD0(PullUp, void(void));
  MOCK_METHOD0(PullDown, void(void));
  MOCK_METHOD0(DisablePulls, void(void));
};  // MockGpioBasic

#endif  //  __has_include(<gmock/gmock.h>)
};  // namespace rpp_driver

#endif /* PICO_DRIVER_SRC_GPIO_GPIOBASIC_HPP_ */