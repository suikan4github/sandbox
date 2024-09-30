/**
 * @file adau1361lower.hpp
 *
 * @brief Lower class for the ADAU1361.
 * @date 2024/Sep/08
 * @author Seiichi Horie
 * @copyright Copyright 2024 Seiichi Horie
 */

#ifndef PICO_DRIVER_SRC_CODEC_ADAU1361LOWER_HPP_
#define PICO_DRIVER_SRC_CODEC_ADAU1361LOWER_HPP_

#include "i2cmaster.hpp"

#if __has_include(<gmock/gmock.h>)
#include <gmock/gmock.h>
#endif

namespace rpp_driver {

/**
 * @brief lower part of the Adau1361 CODEC controller class.
 * @details
 * This class is helper class for the Adau1361 class.
 *
 * The ConfigureSignalPath() member function is a pure virtual
 * method. This function is depend on the signal routing of
 * the CODEC and it is depend on the circuit board.
 * User must derive a subclass of this class to implement
 * that function for his/her board. As an example of sub-class, UmbAdau1361Lower
 * class is provided.
 *
 * To initialize codec, follow the sequence. This is done by the Adau1361 class.
 * @li InitializeCore()
 * @li DisablePLL()
 * @li ConfigurePll()
 * @li WaitPllLock()
 * @li EnableCore()
 * @li ConfigureSRC()
 * @li InitializeRegisters()
 * @li ConfigureSignalPath()
 *
 * After these initialization, the ADC, DAC, Mixer
 * and signals by the I2S protocols are alive.
 * All volumes are muted.
 *
 */
class Adau1361Lower {
 public:
  /**
   * @brief Construct a new object
   *
   * @param controller I2C master controller.
   * @param i2c_device_addr ADAU1361A 7bits I2C device address. Refer device
   * deta sheet for details.
   */
  Adau1361Lower(::rpp_driver::I2cMasterInterface& controller,
                unsigned int i2c_device_addr);
  Adau1361Lower() = delete;
  virtual ~Adau1361Lower() {}
  /**
   *  Service function for the ADAu1361 board implementer.
   *
   * \brief send one command to ADAU1361.
   * \param command Command data array. It have to have register address of
   * ADAU1361 in first two bytes.
   * \param size Number of bytes in the command,
   * including the register address.
   * \details Send one complete command to
   * ADAU3161 by I2C. In the typical case, the command length is 3.
   * \li command[0] : USB of the register address. 0x40.
   * \li command[1] : LSB of the register address.
   * \li command[2] : Value to right the register.
   */
  virtual void SendCommand(const uint8_t command[], int size);
  /**
   * \brief Send multiple commands to ADAU1361.
   * \param table Command table. All commands are stored in one row. Each row
   * has only 1 byte data after reg address. In other words, each row has 3
   * bytes length.
   * \param rows Number of the rows in the table.
   * \details Service
   * function for the ADAu1361 board implementer.
   *
   * Send a list of command to ADAU1361. All commands has 3 bytes length.
   * That mean, after two byte register address, only 1 byte data pay load is
   * allowed. Commands are sent by I2C
   */
  virtual void SendCommandTable(const uint8_t table[][3], int rows);

  /**
   * \brief Check whether I2C device exist or not.
   * \return true if device exist. false if not exist.
   */
  virtual bool IsI2CDeviceExisting();

  /**
   * @brief Reset the core for fresh procedure.
   * @details
   * Must call at first when initialize codec.
   */
  virtual void InitializeCore();

  /**
   * @brief stop PLL to re-program.
   * @details
   * Must call after InitializeCore().
   */
  virtual void DisablePLL();

  /**
   * \brief wait until PLL locks.
   * \details
   *   Service function for the ADAu1361 board implementer.
   *
   *   Read the PLL status and repeat it until the PLL locks.
   */

  virtual void WaitPllLock(void);

  /**
   * @brief Initialize the PLL with given fs and master clock.
   * @param fs Sampling Frequency [Hz].
   * @param master_clock Master Clock input to the CODEC [Hz].
   * @details
   * At first, initialize the PLL based on the given fst and master clock.
   * Then, setup the Converter sampling rate.
   *
   * Must call after DisablePLL().
   */
  virtual void ConfigurePll(unsigned int fs, unsigned int master_clock);

  /**
   * @brief Initialize the SRC with given fs clock.
   * @param fs Sampling frequency in Hz.
   * @details
   * Must call after EnableCore().
   */
  virtual void ConfigureSRC(unsigned int fs);

  /**
   * @brief Initialize the core part of the ADAU1361A.
   * @details
   * Must call after ConfigurePLL(), followings WailtPLLLock().
   */
  virtual void EnableCore();

  /**
   * @brief Initialize registers for the chip operation.
   * @details This is is board independent initialization.
   *
   *  The, gain, ADC, Mixer and I2S are set in this routine.
   * Clock must working well before calling this routine.
   *
   * This function clean-up.
   * Need to call after ConfigureSRC() and before InitializeSignalPath()
   */
  virtual void InitializeRegisters();

  /**
   * @brief Initialize registers for the signal routing.
   * @details
   * This is baord dependent initialization.
   *
   * Need to call after InitializeRegisters().
   */
  virtual void ConfigureSignalPath() = 0;

  /**
   * \brief Set the line input gain and enable the relevant mixer.
   * \param left_gain Gain by dB. [6 .. -12],  The gain value outside of the
   * acceptable range will be saturated.
   * \param right_gain Gain by dB. [6 ..
   * -12], The gain value outside of the acceptable range will be saturated.
   * \param mute True if mute-on, false if mute-off
   * \details
   *   This gain control function uses the single-end
   * negative input only. Other input signal of the line in like positive
   * signal or diff signal are killed.
   *
   *   Other input line like aux are not killed. To kill it, user have to mute
   * them explicitly.
   */
  virtual void SetLineInputGain(float left_gain, float right_gain, bool mute);
  /**
   * \brief Set the aux input gain and enable the relevant mixer.
   * \param left_gain Gain by dB. [6 .. -12], The gain value outside of the
   * acceptable range will be saturated.
   * \param right_gain Gain by dB. [6 ..
   * -12], The gain value outside of the acceptable range will be saturated.
   * \param mute True if mute-on, false if mute-off
   * \details
   *   Other input lines are not killed. To kill it, user have to mute them
   * explicitly.
   */
  virtual void SetAuxInputGain(float left_gain, float right_gain, bool mute);

  /**
   * \brief Set the line output gain and enable the relevant mixer.
   * \param left_gain Gain by dB. [6 .. -12], The gain value outside of the
   * acceptable range will be saturated.
   * \param right_gain Gain by dB. [6 ..
   * -12], The gain value outside of the acceptable range will be saturated.
   * \param mute True if mute-on, false if mute-off
   * \details
   *   Other output lines are not killed. To kill it, user have to mute them
   * explicitly.
   *
   */
  virtual void SetLineOutputGain(float left_gain, float right_gain, bool mute);

  /**
   * \brief Set the headphone output gain and enable the relevant mixer.
   * \param left_gain Gain by dB. [6 .. -12], The gain value outside of the
   * acceptable range will be saturated.
   * \param right_gain Gain by dB. [6 ..
   * -12], The gain value outside of the acceptable range will be saturated.
   * \param mute True if mute-on, false if mute-off
   * \details
   *   Other out line like line in are not killed. To kill it, user have to
   * mute them explicitly.
   */
  virtual void SetHpOutputGain(float left_gain, float right_gain, bool mute);

 protected:
  /// @brief Internal variable to hold the I2C controller variable.
  ::rpp_driver::I2cMasterInterface& i2c_;
  /// @brief Internal variable to hold the I2C device address.
  const unsigned int device_addr_;
};

// #ifdef MOCK_METHOD0
#if __has_include(<gmock/gmock.h>)

class MockAdau1361Lower : public Adau1361Lower {
 public:
  explicit MockAdau1361Lower(::rpp_driver::I2cMasterInterface& controller)
      : Adau1361Lower(controller, 0x3A) {};
  MOCK_METHOD2(SendCommand, void(const uint8_t command[], int size));
  MOCK_METHOD2(SendCommandTable, void(const uint8_t table[][3], int rows));
  MOCK_METHOD0(IsI2CDeviceExisting, bool());
  MOCK_METHOD0(InitializeCore, void());
  MOCK_METHOD0(DisablePLL, void());
  MOCK_METHOD0(WaitPllLock, void());
  MOCK_METHOD2(ConfigurePll, void(unsigned int fs, unsigned int master_clock));
  MOCK_METHOD1(ConfigureSRC, void(unsigned int fs));
  MOCK_METHOD0(EnableCore, void());
  MOCK_METHOD0(InitializeRegisters, void());
  MOCK_METHOD0(ConfigureSignalPath, void());
  MOCK_METHOD3(SetLineInputGain,
               void(float left_gain, float right_gain, bool mute));
  MOCK_METHOD3(SetAuxInputGain,
               void(float left_gain, float right_gain, bool mute));
  MOCK_METHOD3(SetLineOutputGain,
               void(float left_gain, float right_gain, bool mute));
  MOCK_METHOD3(SetHpOutputGain,
               void(float left_gain, float right_gain, bool mute));
};
#endif  //  __has_include(<gmock/gmock.h>)

}  // namespace rpp_driver

#endif /* PICO_DRIVER_SRC_CODEC_ADAU1361LOWER_HPP_ */