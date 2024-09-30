/**
 * @file i2cmaster.hpp
 *
 * @brief I2C Master Controller.
 * @date 2024/Aug/16
 * @author Seiichi  Horie
 * @brief I2C master Implementation.
 * @copyright COPYRIGHT 2024 Seiichi Horie
 */

#ifndef PICO_DRIVER_SRC_I2C_I2CMASATER_HPP_
#define PICO_DRIVER_SRC_I2C_I2CMASATER_HPP_

#include <stdint.h>

#if __has_include(<hardware/i2c.h>)
#include "hardware/i2c.h"
#else
/// Alternate definition for unit test.
typedef int i2c_inst_t;
/// Alternate definition of the return value on error for Google Test build.
#define PICO_ERROR_GENERIC -1
/// Alternate definition of the return value on error for I2C function number
/// for GPIO.
#define GPIO_FUNC_I2C 11
#endif  // __has_include(<hardware/i2c.h>)

#include "i2cmasterinterface.hpp"
#include "sdkwrapper.hpp"

namespace rpp_driver {
/**
 * @brief I2C Master controller class.
 * @details
 * This class provides a set of easy to use member function to control the I2C
 * master controller. Everything operation in this class is polling based and
 * blocking.
 *
 * The constructor and destructor initializes and finalize the given I2C
 * controller, respectively.
 *
 * The ReadBlocking() and WriteBlocking() functions has nostop parameter. To use
 * the restart condition, set this parameter to true.
 *
 */
class I2cMaster : public I2cMasterInterface {
 public:
  /**
   * @brief Initialize the given I2C port and setup the pins.
   * @param sdk Object of the SdkWrapper class.
   * @param i2c i2c_inst_t type &. *I2C0 or *I2C1
   * @param clock_freq Usually 100,000 or 400,000[Hz].
   * @param scl_pin GPIO pin #
   * @param sda_pin GPIO pin #
   * @details
   * Receive the uninitialized I2C hardware by parameter i2c, and initialize
   * it by SdkWrapper::i2c_init().
   * And then, set given pins to I2C function, and pull them up.
   */
  I2cMaster(SdkWrapper &sdk, i2c_inst_t &i2c, uint clock_freq, uint scl_pin,
            uint sda_pin);
  I2cMaster() = delete;
  /**
   * @brief deinit the I2C by SdkWrapper::i2c_deinit();
   */
  virtual ~I2cMaster();
  /**
   * @brief Attempt to read specified number of bytes from address,
   * blocking.
   * @param addr 7-bit address of device to read from
   * @param dst Pointer to buffer to receive data
   * @param len Length of data in bytes to receive
   * @param nostop If true, master retains control of the bus at the end of
   * the transfer (no Stop is issued), and the next transfer will begin with
   * a Restart rather than a Start.
   * @returns Number of bytes read, or PICO_ERROR_GENERIC if address
   * not acknowledged or no device present.
   */

  int ReadBlocking(uint8_t addr, uint8_t *dst, size_t len,
                   bool nostop) override;
  /**
   * @brief Attempt to write specified number of bytes to address, blocking.
   * @param addr 7-bit address of device to write to
   * @param src Pointer to data to send
   * @param len Length of data in bytes to send
   * @param nostop If true, master retains control of the bus at the end of
   * the transfer (no Stop is issued), and the next transfer will begin with
   * a Restart rather than a Start.
   * @returns Number of bytes written, or PICO_ERROR_GENERIC
   * if address not acknowledged, no device present.
   */
  int WriteBlocking(uint8_t addr, const uint8_t *src, size_t len,
                    bool nostop) override;

  /**
   * @brief Check wether device at specified I2C address exists or not.
   * @param addr 7-bit address of device to read from
   * @returns true if exist, false if not exist.
   */

  bool IsDeviceExisting(uint8_t addr) override;

 private:
  i2c_inst_t &i2c_;
  SdkWrapper &sdk_;
};
};  // namespace rpp_driver

#endif /* PICO_DRIVER_SRC_I2C_I2CMASATER_HPP_ */