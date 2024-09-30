#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "i2cmasterinterface.hpp"
#include "umbadau1361lower.hpp"

class UmbAdau1361LowerTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    device_address_ = 0x39;  // 7bit I2C address
    codec_lower_ = new ::rpp_driver::UmbAdau1361Lower(i2c_, device_address_);
  }

  virtual void TearDown() { delete codec_lower_; }

  unsigned int device_address_;  // 7bit I2C address
  ::rpp_driver::MockI2cMasterInterface i2c_;
  ::rpp_driver::Adau1361Lower *codec_lower_;
};

// -----------------------------------------------------------------
//
//                          ConfigureSignalPath()
//
// -----------------------------------------------------------------

TEST_F(UmbAdau1361LowerTest, ConfigureSignalPath) {
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

  using ::testing::Args;
  using ::testing::ElementsAreArray;
  using ::testing::InSequence;
  using ::testing::NotNull;
  //  we test initialization of core.
  {
    InSequence dummy;

    for (int i = 0; i < sizeof(config_UMB_ADAU1361A) / 3; i++)
      EXPECT_CALL(i2c_,
                  WriteBlocking(device_address_,  // Arg 0 : I2C Address.
                                NotNull(),  // Arg 1 : Data buffer address.
                                3,       // Arg 2 : Data buffer length to send.
                                false))  // Arg 3 : false to stop.
          .With(Args<1,  // parameter position of the array : 0 origin.
                     2>  // parameter position of the size : 0 origin.
                (ElementsAreArray(config_UMB_ADAU1361A[i])));
  }
  codec_lower_->ConfigureSignalPath();
}  // ConfigureSignalPath
