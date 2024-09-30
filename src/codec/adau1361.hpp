/**
 * @file adau1361.hpp
 *
 * @brief ADAU1361A CODEC Control.
 * @date 2024/Sep/08
 * @author Seiichi Horie
 * @copyright Copyright 2024 Seiichi Horie
 */

#ifndef PICO_DRIVER_SRC_CODEC_ADAU1361_HPP_
#define PICO_DRIVER_SRC_CODEC_ADAU1361_HPP_

#include <i2cmasterinterface.hpp>

#include "adau1361lower.hpp"
namespace rpp_driver {

/**
 * \brief Analog Device ADAU1361A audio codec control class.
 * \details
 * Control the ADAU1361A through the lower level controller class
 * for example UmbAdau1361Lower.
 * Such the lower level controller variable is passed through the
 * adau1361_lower parameter of the constructor.
 *
 * The following is an example code to create an Adau1361 variable.
 * ```
  ::rpp_driver::I2cMaster i2c(sdk,          // Inject SDK Dependency.
                              *i2c1,        // I2C controller to use.
                              kI2cClock,    // I2C Clock [Hz]
                              kI2cScl_pin,  // GPIO pin for SCL
                              kI2cSdaPin);  // GPIO pin # for SDA

  ::rpp_driver::UmbAdau1361Lower codec_lower(
      i2c,                   // Inject I2C controller dependency.
      kAdau1361I2cAddress);  // I2C address of UMB-ADAU1361-A

  ::rpp_driver::Adau1361 codec(
      kFs,           // Sampling frequency[Hz].
      kMClock,       // Master clock of UMB-ADAU1361-A[Hz].
      codec_lower);  // Inject Codec lower part dependency.
 * ```
 */
class Adau1361 {
 public:
  /**
   * @brief Signal path definition.
   * @details
   * This type specify the CODEC input / output channel to change the gain and
   * mute.
   * @sa SetGain() and Mute().
   */
  enum CodecChannel {
    LineInput,       ///< Physical input channel as line in audio.
    AuxInput,        ///< Physical input channel as aux in audio.
    LineOutput,      ///< Physical output channel as line out audio.
    HeadphoneOutput  ///< Physical output channel as head phone out audio.
  };

  /**
   * \brief constructor.
   * \param fs Sampling frequency[Hz]
   * @param master_clock Input master clock frequency to the MCLK pin[Hz]
   * @param adau1361_lower Helper object.
   * \details
   *   initialize the internal variables.
   *   This constructor assumes the codec receive a master clock from outside.
   * And output the I2C clocks as clock master.
   *
   *   The fs parameter is the sampling frequency of the CODEC in Hz. This
   * parameter is limited as one of the following :
   *   @li 24000
   *   @li 32000
   *   @li 48000
   *   @li 96000
   *   @li 22050
   *   @li 44100
   *   @li 88200
   *
   *   The master_clock parameter is the MCLK input to the ADAU1361 in Hz.
   *   This parameter must be one of followings :
   *   @li  8000000
   *   @li 12000000
   *   @li 13000000
   *   @li 14400000
   *   @li 19200000
   *   @li 19680000
   *   @li 19800000
   *   @li 24000000
   *   @li 26000000
   *   @li 27000000
   *   @li 12288000
   *   @li 24576000
   *
   *   Note : Due to the limitation of the MCLK of hardware and PLL,
   *   only 8, 12, 13, 14.4, 12.288, 19.2MHz are tested
   *
   *   The analog signals are routed to the physical pins as following :
   *   @li Line In  : LINN/RINN single ended.
   *   @li Aux In : LAUX/RAUX input
   *   @li LINE out : LOUTP/ROUTP single ended
   *   @li HP out   : LHP/RHP
   */
  explicit Adau1361(unsigned int fs, unsigned int master_clock,
                    Adau1361Lower& adau1361_lower);
  Adau1361() = delete;
  /**
   * \brief Set up the ADAU1361 codec,  and then, start the codec.
   * \details
   *   This method starts the ADAU1361 AD/DA conversion and I2S communication.
   *
   *   The line in is configured to use the Single-End negative input. This is
   * funny but ADAU1361 data sheet specifies to do it. The positive in and diff
   * in are killed. All biases are set as "normal".
   *
   *   The CODEC is configured as master mode. That mean, BCLK and WS are
   * given from ADAU1361 to the micro processor.
   *
   *   At initial state, ADAU1361 is set as :
   *
   * @li All input and output channels are set as 0.0dB.
   * @li All input and output channels are muted.
   */
  virtual void Start(void);
  /**
   * @brief Set channel gain
   * @param channel CODEC input output channels like line-in, line-out,
   * aux-in, headphone-out
   * @param left_gain Gain by dB. [6 .. -12],  The gain value outside of the
   * acceptable range will be saturated.
   * @param right_gain Gain by dB. [6 .. -12],  The gain value outside of the
   * acceptable range will be saturated.
   */
  virtual void SetGain(CodecChannel channel, float left_gain, float right_gain);

  /**
   * @brief Mute the specific channel.
   * @param channel Channel to mute on / off
   * @param mute Mute-on if true. Mute-off if false.
   */
  virtual void Mute(CodecChannel channel, bool mute = true);

 private:
  const unsigned int fs_;
  const unsigned int master_clock_;
  Adau1361Lower& adau1361_lower_;

  float line_input_left_gain_;
  float line_input_right_gain_;
  float aux_input_left_gain_;
  float aux_input_right_gain_;
  float line_output_left_gain_;
  float line_output_right_gain_;
  float hp_output_left_gain_;  // headphone
  float hp_output_right_gain_;

  bool line_input_mute_;
  bool aux_input_mute_;
  bool line_output_mute_;
  bool hp_output_mute_;  // headphone
};
}  // namespace rpp_driver

#endif /* PICO_DRIVER_SRC_CODEC_ADAU1361_HPP_ */