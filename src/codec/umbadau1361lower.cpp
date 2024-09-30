#include "umbadau1361lower.hpp"

#include <assert.h>

#include <algorithm>

void ::rpp_driver::UmbAdau1361Lower::ConfigureSignalPath() {
  // Set certain signal pass. If it
  // doen't fit to your system,
  // override it by
  // SendCommand()

  // Set UMB_ADAU1361A. No mono output, No cross channel Mix, No analog path
  // through.
  const uint8_t config_UMB_ADAU1361A[][3] = {
      // Configuration for UMB-ADAU1361-A http://dsps.shop-pro.jp/?pid=82798273
      {0x40, 0x0a, 0x0B},  // R4: Rec Mixer Left 0,  Mixer enable, LINNG 0dB
      {0x40, 0x0c, 0x0B},  // R6: Rec Mixer Right 0, Mixer enable, RINNG 0dB
      {0x40, 0x15,
       0x01},  // R15: I2S Port control, Set code as Master mode I2S.
      {0x40, 0x19, 0x63},  // R19: ADC Ctrl. Inverted, Enable L/R, HPF on
      {0x40, 0x29, 0x03},  // R35: DAC Ctrl. Stereo, Enable L/R.
      {0x40, 0x2a, 0x03},  // R36: DAC Control 0. Enable DAC. Both channels on.
      {0x40, 0x1c, 0x21},  // R22: MIXER 3, L DAC Mixer (set L DAC to L Mixer )
      {0x40, 0x1e, 0x41},  // R24: MIXER 4, R DAC Mixer (set R DAC to R Mixer )
      {0x40, 0x20, 0x03},  // R26: MIXER 5, L out mixer. L out MIX5G3 and enable
      {0x40, 0x21,
       0x09},  // R27: MIXER 6, R out mixer. R out MIX6G4 and enable.
      {0x40, 0x23, 0x03},  // R29: Play HP Left vol : Mute, Enable
      {0x40, 0x24, 0x03},  // R30: Play HP Right vol : Mute, HP Mode
  };

  SendCommandTable(
      config_UMB_ADAU1361A,
      sizeof(config_UMB_ADAU1361A) / 3);  // init UMB-ADAU1361 as default state.
}
