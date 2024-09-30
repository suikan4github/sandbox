/**
 * @file umbadau1361lower.hpp
 *
 * @brief Lower class for the UMB-ADAU1361A.
 * @date 2024/Sep/16
 * @author Seiichi Horie
 * @copyright Copyright 2024 Seiichi Horie
 */

#ifndef PICO_DRIVER_SRC_CODEC_UMBADAU1361LOWER_HPP_
#define PICO_DRIVER_SRC_CODEC_UMBADAU1361LOWER_HPP_

#include "adau1361lower.hpp"
#include "i2cmaster.hpp"

namespace rpp_driver {

/**
 * @brief lower part of the Adau1361 CODEC controller class.
 * @details
 * This is dedicated class for
 * [UMB-ADAU1361-A](http://dsps.shop-pro.jp/?pid=82798273) board. All signal
 * routings are configured for this board.
 *
 */
class UmbAdau1361Lower : public Adau1361Lower {
 public:
  /**
   * @brief Construct a new object
   *
   * @param controller I2C master controller.
   * @param i2c_device_addr ADAU1361A 7bits I2C device address. Refer device
   * deta sheet for details.
   */
  UmbAdau1361Lower(::rpp_driver::I2cMasterInterface& controller,
                   unsigned int i2c_device_addr)
      : Adau1361Lower(controller, i2c_device_addr) {}
  UmbAdau1361Lower() = delete;
  virtual ~UmbAdau1361Lower() {}

  /**
   * @brief Initialize registers for the signal routing.
   * @details
   * This is baord dependent initialization for UMB-ADAU1361A.
   *
   * Need to call after InitializeRegisters().
   */
  void ConfigureSignalPath() override;
};

}  // namespace rpp_driver

#endif /* PICO_DRIVER_SRC_CODEC_UMBADAU1361LOWER_HPP_ */