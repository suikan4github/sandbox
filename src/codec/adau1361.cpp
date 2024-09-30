/*
 * adau1361.cpp
 *
 *  Created on: 2018/05/11
 *      Author: Seiichi "Suikan" Horie
 */

#include "adau1361.hpp"

#include <assert.h>

#include "i2cmasterinterface.hpp"

// Macro for easy-to-read
#define CODEC_SYSLOG(fmt, ...) \
  //  MURASAKI_SYSLOG(this, kfaAudioCodec, kseDebug, fmt, ##__VA_ARGS__)

::rpp_driver::Adau1361::Adau1361(unsigned int fs, unsigned int master_clock,
                                 ::rpp_driver::Adau1361Lower& adau1361_lower)
    : fs_(fs),
      master_clock_(master_clock),
      adau1361_lower_(adau1361_lower),
      line_input_left_gain_(0.0),
      line_input_right_gain_(0.0),
      aux_input_left_gain_(0.0),
      aux_input_right_gain_(0.0),
      line_output_left_gain_(0.0),
      line_output_right_gain_(0.0),
      hp_output_left_gain_(0.0),
      hp_output_right_gain_(0.0),
      line_input_mute_(true),
      aux_input_mute_(true),
      line_output_mute_(true),
      hp_output_mute_(true) {}

void ::rpp_driver::Adau1361::Start(void) {
  CODEC_SYSLOG("Enter.")

  // Check if target device exist.
  // send no payload. Just enquire address.
  assert(adau1361_lower_.IsI2CDeviceExisting());

  // then, start of configuration as register address order
  adau1361_lower_.InitializeCore();
  // And then, stop PLL explicitly.
  adau1361_lower_.DisablePLL();
  // Now, start PLL again as desired frequency.
  adau1361_lower_.ConfigurePll(fs_, master_clock_);
  // Wait until PLL stable.
  adau1361_lower_.WaitPllLock();
  // Enable core to start operation.
  adau1361_lower_.EnableCore();
  // Set SRC for the right FS.
  adau1361_lower_.ConfigureSRC(fs_);
  // Board independent register initialization.
  adau1361_lower_.InitializeRegisters();
  // Board dependent register initialization.
  adau1361_lower_.ConfigureSignalPath();

  CODEC_SYSLOG("Leave.")
}

/*
 * Set gain of the specific input channel.
 * The mute status is not affected.
 */
void ::rpp_driver::Adau1361::SetGain(CodecChannel channel, float left_gain,
                                     float right_gain) {
  CODEC_SYSLOG("Enter. %d, %f, %f ", channel, left_gain, right_gain)

  switch (channel) {
    case LineInput:
      line_input_left_gain_ = left_gain;
      line_input_right_gain_ = right_gain;
      adau1361_lower_.SetLineInputGain(
          line_input_left_gain_, line_input_right_gain_, line_input_mute_);
      break;
    case AuxInput:
      aux_input_left_gain_ = left_gain;
      aux_input_right_gain_ = right_gain;
      adau1361_lower_.SetAuxInputGain(aux_input_left_gain_,
                                      aux_input_right_gain_, aux_input_mute_);
      break;
    case LineOutput:
      line_output_left_gain_ = left_gain;
      line_output_right_gain_ = right_gain;
      adau1361_lower_.SetLineOutputGain(
          line_output_left_gain_, line_output_right_gain_, line_output_mute_);
      break;

    case HeadphoneOutput:
      hp_output_left_gain_ = left_gain;
      hp_output_right_gain_ = right_gain;
      adau1361_lower_.SetHpOutputGain(hp_output_left_gain_,
                                      hp_output_right_gain_, hp_output_mute_);
      break;
    default:
      assert(false);
      break;
  }
  CODEC_SYSLOG("Leave.")
}

/*
 * Set mute status of specific channels.
 * Channel gains are not affected.
 */
void ::rpp_driver::Adau1361::Mute(CodecChannel channel, bool mute) {
  CODEC_SYSLOG("Enter. %d, %s ", channel, mute ? "true" : "false")

  switch (channel) {
    case LineInput:
      line_input_mute_ = mute;
      adau1361_lower_.SetLineInputGain(
          line_input_left_gain_, line_input_right_gain_, line_input_mute_);
      break;
    case AuxInput:
      aux_input_mute_ = mute;
      adau1361_lower_.SetAuxInputGain(aux_input_left_gain_,
                                      aux_input_right_gain_, aux_input_mute_);
      break;
    case LineOutput:
      line_output_mute_ = mute;
      adau1361_lower_.SetLineOutputGain(
          line_output_left_gain_, line_output_right_gain_, line_output_mute_);
      break;

    case HeadphoneOutput:
      hp_output_mute_ = mute;
      adau1361_lower_.SetHpOutputGain(hp_output_left_gain_,
                                      hp_output_right_gain_, hp_output_mute_);
      break;
    default:
      assert(false);
      break;
  }
  CODEC_SYSLOG("Leave.")
}
