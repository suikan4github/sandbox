/**
 * @file i2cmasterinterface.hpp
 *
 * @details Interface type to allow the GoogleTest without depending to RasPi
 * code.
 * @date 2024/Aug/16
 * @author Seiichi  Horie
 * @brief I2C master Interface.
 * @copyright COPYRIGHT 2024 Seiichi Horie
 */

#ifndef PICO_DRIVER_SRC_I2C_I2CMASATERINTERFACE_HPP_
#define PICO_DRIVER_SRC_I2C_I2CMASATERINTERFACE_HPP_

#include <stdint.h>
#include <stdlib.h>

#if !__has_include(<hardware/i2c.h>)
/// @brief  Alternate definition of the uint for Google Test build.
typedef unsigned uint;
#endif

#if __has_include(<gmock/gmock.h>)
#include <gmock/gmock.h>
#endif

namespace rpp_driver {
/**
 * @brief Interface type to allow the GoogleTest without depending to RasPi
 *
 */
class I2cMasterInterface {
 public:
  virtual ~I2cMasterInterface() {}
  /**
   * @brief Attempt to read specified number of bytes from address, blocking.
   * @param addr 7-bit address of device to read from
   * @param dst Pointer to buffer to receive data
   * @param len Length of data in bytes to receive
   * @param nostop If true, master retains control of the bus at the end of
   * the transfer (no Stop is issued), and the next transfer will begin with
   * a Restart rather than a Start.
   * @returns Number of bytes read, or PICO_ERROR_GENERIC if address
   * not acknowledged or no device present.
   */

  virtual int ReadBlocking(uint8_t addr, uint8_t *dst, size_t len,
                           bool nostop) = 0;
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
  virtual int WriteBlocking(uint8_t addr, const uint8_t *src, size_t len,
                            bool nostop) = 0;
  /**
   * @brief Check wether device at specified I2C address exists or not.
   * @param addr 7-bit address of device to read from
   * @returns true if exist, false if not exist.
   */
  virtual bool IsDeviceExisting(uint8_t addr) = 0;
};

#if __has_include(<gmock/gmock.h>)

class MockI2cMasterInterface : public I2cMasterInterface {
 public:
  MOCK_METHOD4(ReadBlocking,
               int(uint8_t addr, uint8_t *dst, size_t len, bool nostop));
  MOCK_METHOD4(WriteBlocking,
               int(uint8_t addr, const uint8_t *src, size_t len, bool nostop));
  MOCK_METHOD1(IsDeviceExisting, bool(uint8_t addr));
};

#endif  // __has_include(<gmock/gmock.h>)
}  // namespace rpp_driver

#endif /* PICO_DRIVER_SRC_I2C_I2CMASATERINTERFACE_HPP_ */