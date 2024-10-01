#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "adau1361lower.hpp"
#include "i2cmasterinterface.hpp"

// Class under test. Provide a dummy member function for test.
class CutAdau1361Lower : public ::rpp_driver::Adau1361Lower {
 public:
  CutAdau1361Lower(::rpp_driver::I2cMasterInterface& controller,
                   unsigned int i2c_device_addr)
      : ::rpp_driver::Adau1361Lower(controller, i2c_device_addr) {};
  virtual void ConfigureSignalPath() {};
};

class Adau1361LowerTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    device_address_ = 0x38;  // 7bit I2C address
    codec_lower_ = new ::CutAdau1361Lower(i2c_, device_address_);
  }

  virtual void TearDown() { delete codec_lower_; }

  unsigned int device_address_;  // 7bit I2C address
  ::rpp_driver::MockI2cMasterInterface i2c_;
  ::rpp_driver::Adau1361Lower* codec_lower_;
};

typedef Adau1361LowerTest Adau1361LowerDeathTest;

// -----------------------------------------------------------------
//
//                          Constructor Death Test
//
// -----------------------------------------------------------------

TEST(Adau1361LowerConstructorDeathTest, lower_address) {
  unsigned int device_address;  // 7bit I2C address
  ::rpp_driver::MockI2cMasterInterface i2c;
  ::rpp_driver::Adau1361Lower* codec_lower;

  device_address = 0x37;  // 7bit I2C address
  // check the assertion for bad I2C address for Analog Device ADAU1361.
  // See data sheet for details.
#ifndef NDEBUG
  ASSERT_DEATH(codec_lower = new ::CutAdau1361Lower(i2c, device_address);
               , "ADAU1361 I2C Address must be higher than 0x37.");
#endif
}

TEST(Adau1361LowerConstructorDeathTest, higher_address) {
  unsigned int device_address;  // 7bit I2C address
  ::rpp_driver::MockI2cMasterInterface i2c;
  ::rpp_driver::Adau1361Lower* codec_lower;

  device_address = 0x3C;  // 7bit I2C address
  // check the assertion for bad I2C address for Analog Device ADAU1361.
  // See data sheet for details.
#ifndef NDEBUG
  ASSERT_DEATH(codec_lower = new ::CutAdau1361Lower(i2c, device_address);
               , "ADAU1361 I2C Address must be lower than 0x3C.");
#endif
}

// -----------------------------------------------------------------
//
//                          SendCommand()
//
// -----------------------------------------------------------------

TEST_F(Adau1361LowerTest, SendCommand) {
  uint8_t cmd[7];

  // nostop parameter must be false. That mean, stop condition of I2C.
  EXPECT_CALL(i2c_, WriteBlocking(device_address_, cmd, sizeof(cmd), false));
  codec_lower_->SendCommand(cmd, sizeof(cmd));
}

// -----------------------------------------------------------------
//
//                          SendCommandTable()
//
// -----------------------------------------------------------------

TEST_F(Adau1361LowerTest, SendCommandTable) {
  const uint8_t cmd[][3] = {
      // R0,1, are set by init_freq_xxx table
      {0x40, 0x08, 0x00},  // R2: Digital Mic
      {0x40, 0x09, 0x00},  // R3: Rec Power Management
      {0x40, 0x0a, 0x00}   // R4: Rec Mixer Left 0
  };
  // nostop parameter must be false. That mean, stop condition of I2C.
  using ::testing::InSequence;
  {
    InSequence dummy;

    EXPECT_CALL(i2c_, WriteBlocking(device_address_, cmd[0], 3, false));
    EXPECT_CALL(i2c_, WriteBlocking(device_address_, cmd[1], 3, false));
    EXPECT_CALL(i2c_, WriteBlocking(device_address_, cmd[2], 3, false));
  }
  codec_lower_->SendCommandTable(cmd, sizeof(cmd) / 3);
}

// -----------------------------------------------------------------
//
//                          IsI2CDeviceExist()
//
// -----------------------------------------------------------------

TEST_F(Adau1361LowerTest, IsI2CDeviceExist) {
  uint8_t cmd[7];

  using ::testing::_;
  using ::testing::Return;

  using ::testing::InSequence;
  {
    InSequence dummy;

    // nostop parameter must be false. That mean, stop condition of I2C.
    EXPECT_CALL(i2c_, IsDeviceExisting(device_address_))
        .WillOnce(Return(false));
    EXPECT_CALL(i2c_, IsDeviceExisting(device_address_)).WillOnce(Return(true));
  }
  EXPECT_FALSE(codec_lower_->IsI2CDeviceExisting());
  EXPECT_TRUE(codec_lower_->IsI2CDeviceExisting());
}

TEST_F(Adau1361LowerTest, WaitPllLock) {
  const uint8_t lock_status_address[] = {0x40, 0x02};  // R1 : 6 byte register.
  uint8_t status_notlocked[6] = {0, 0, 0, 0, 0, 0};
  uint8_t status_locked[6] = {0, 0, 0, 0, 0, 2};

  using ::testing::Args;
  using ::testing::DoAll;
  using ::testing::ElementsAreArray;
  using ::testing::InSequence;
  using ::testing::NotNull;
  using ::testing::Return;
  using ::testing::SetArrayArgument;

  {
    InSequence dummy;

    //  we test unlocked status. Function must not return.

    // nostop parameter must be true. That mean repeated start of I2C.
    // The write command give only 2 byte length register address.
    EXPECT_CALL(i2c_,
                WriteBlocking(device_address_,  // Arg 0 : I2C Address.
                              NotNull(),        // Arg 1 : Data buffer address.
                              2,      // Arg 2 : Data buffer length to send.
                              true))  // Arg 3 : true to repeated start.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(lock_status_address)))
        .WillOnce(Return(2));  // 2 is the transferred data length

    // nostop parameter must be false. That mean, stop condition of I2C.
    // This will check the status register of ADAU1361A. These status
    // registers must be read 6 byte at once.
    EXPECT_CALL(i2c_,
                ReadBlocking(device_address_,  // Arg 0 : I2C Address
                             NotNull(),        // Arg 1 : Data buffer address
                             6,                // Arg 2 : Data buffer length
                             false))           // Arg 3 : false to stop I2C.
        .WillOnce(DoAll(
            SetArrayArgument<1>  // parameter position of the array : 0 origin.
            (status_notlocked,   // pointer to the beginning of the data.
             status_notlocked + 6),  // pointer to the end of the data + 1.
            Return(3)));             // 6 is the transferred data length.

    // From here, we test the PLL lock status.

    // nostop parameter must be true. That mean repeated start of I2C.
    // The write command give only 2 byte length register address.
    EXPECT_CALL(i2c_,
                WriteBlocking(device_address_,  // Arg 0 : I2C Address.
                              NotNull(),        // Arg 1 : Data buffer address.
                              2,      // Arg 2 : Data buffer length to send.
                              true))  // Arg 3 : true to repeated start.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(lock_status_address)))
        .WillOnce(Return(2));  // 2 is the transferred data length

    // nostop parameter must be false. That mean, stop condition of I2C.
    // This will check the status register of ADAU1361A. These status
    // registers must be read 6 byte at once.
    EXPECT_CALL(i2c_,
                ReadBlocking(device_address_,  // Arg 0 : I2C Address
                             NotNull(),        // Arg 1 : Data buffer address
                             6,                // Arg 2 : Data buffer length
                             false))           // Arg 3 : false to stop I2C.
        .WillOnce(DoAll(
            SetArrayArgument<1>   // parameter position of the array : 0 origin.
            (status_locked,       // pointer to the beginning of the data.
             status_locked + 6),  // pointer to the end of the data + 1.
            Return(3)));          // 6 is the transferred data length.
  }

  codec_lower_->WaitPllLock();
}

// -----------------------------------------------------------------
//
//                          InitializeCore()
//
// -----------------------------------------------------------------

TEST_F(Adau1361LowerTest, InitializeCore) {
  // Core clock setting
  const uint8_t init_core[] = {
      0x40, 0x00, 0x00};  // R0:Clock control. Core clock disabled. PLL off.

  using ::testing::Args;
  using ::testing::ElementsAreArray;
  using ::testing::NotNull;

  //  we test initialization of core.

  EXPECT_CALL(i2c_,
              WriteBlocking(device_address_,  // Arg 0 : I2C Address.
                            NotNull(),        // Arg 1 : Data buffer address.
                            3,       // Arg 2 : Data buffer length to send.
                            false))  // Arg 3 : false to stop.
      .With(Args<1,  // parameter position of the array : 0 origin.
                 2>  // parameter position of the size : 0 origin.
            (ElementsAreArray(init_core)));
  codec_lower_->InitializeCore();
}  // InitializeCore

// -----------------------------------------------------------------
//
//                          DisablePLL()
//
// -----------------------------------------------------------------

TEST_F(Adau1361LowerTest, DisablePLL) {
  // PLL Disable.
  // R1 : Must write 6 byte at once.
  const uint8_t disable_pll[] = {0x40, 0x02, 0x00, 0xFD,
                                 0x00, 0x0C, 0x10, 0x00};

  using ::testing::Args;
  using ::testing::ElementsAreArray;
  using ::testing::NotNull;

  //  we test initialization of core.

  EXPECT_CALL(i2c_,
              WriteBlocking(device_address_,  // Arg 0 : I2C Address.
                            NotNull(),        // Arg 1 : Data buffer address.
                            8,       // Arg 2 : Data buffer length to send.
                            false))  // Arg 3 : false to stop.
      .With(Args<1,  // parameter position of the array : 0 origin.
                 2>  // parameter position of the size : 0 origin.
            (ElementsAreArray(disable_pll)));
  codec_lower_->DisablePLL();
}  // DisablePLL

// #include <iostream>
static float pll_out(unsigned int mclock, const uint8_t config[]) {
  float x = ((config[6] >> 1) & 0x03) + 1;
  float r = (config[6] >> 3) & 0x0F;
  if (0 == r) r = 8;
  float n = config[4] * 256 + config[5];
  float m = config[2] * 256 + config[3];
  bool fractional = (config[6] & 1) != 0;

  // std::cout << x << "," << r << "," << n << "," << m << std::endl;
  if (fractional)
    return mclock * (1 / x) * (r + n / m);
  else
    return mclock * (1 / x) * r;
}

static bool is_pll_enabled(const uint8_t config[]) {
  // config[7] is the PLL configuration register 5.
  // It must be 1 ( enable PLL ) when setting PLL.
  return (1 == config[7]);
}

// ------------------------------------------------------------
//
//                               ConfigurePll()
//
// ------------------------------------------------------------
// ------------------------------------------------------------
//
//                               48kHz
//
// ------------------------------------------------------------
TEST_F(Adau1361LowerTest, ConfigurePll_48_08000) {
  using ::testing::Args;
  using ::testing::ElementsAreArray;
  using ::testing::NotNull;
  const unsigned int target_pll_freq_48 = 49152000;  // Hz. See data sheet.
  const float acceptable_error = 5;  // Hz. Error of the PLL out.

  // Test 8MHz master clock for Fs 48kHz series.
  {
    const unsigned int mclock = 8000000;
    const unsigned int fs = 24000;
    uint8_t config_pll[] = {0x40, 0x02, 0x00, 0x7D, 0x00, 0x12, 0x31, 0x01};
    EXPECT_CALL(i2c_,
                WriteBlocking(
                    device_address_,     // Arg 0 : I2C Address.
                    NotNull(),           // Arg 1 : Data buffer address.
                    sizeof(config_pll),  // Arg 2 : Data buffer length to send.
                    false))              // Arg 3 : false to stop.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(config_pll)));
    // check the frequency error of the PLL
    EXPECT_NEAR(pll_out(mclock, config_pll), target_pll_freq_48,
                acceptable_error);
    // check whether PLL is enabled or not.
    EXPECT_TRUE(is_pll_enabled(config_pll));
    codec_lower_->ConfigurePll(fs, mclock);
  }
}  // ConfigurePll_48_08000

TEST_F(Adau1361LowerTest, ConfigurePll_48_12000) {
  using ::testing::Args;
  using ::testing::ElementsAreArray;
  using ::testing::NotNull;
  const unsigned int target_pll_freq_48 = 49152000;  // Hz. See data sheet.
  const float acceptable_error = 5;  // Hz. Error of the PLL out.

  // Test 12MHz master clock for Fs 48kHz series.
  {
    const unsigned int mclock = 12000000;
    const unsigned int fs = 48000;
    uint8_t config_pll[] = {0x40, 0x02, 0x00, 0x7D, 0x00, 0x0C, 0x21, 0x01};
    EXPECT_CALL(i2c_,
                WriteBlocking(
                    device_address_,     // Arg 0 : I2C Address.
                    NotNull(),           // Arg 1 : Data buffer address.
                    sizeof(config_pll),  // Arg 2 : Data buffer length to send.
                    false))              // Arg 3 : false to stop.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(config_pll)));
    // check the frequency error of the PLL
    EXPECT_NEAR(pll_out(mclock, config_pll), target_pll_freq_48,
                acceptable_error);
    // check whether PLL is enabled or not.
    EXPECT_TRUE(is_pll_enabled(config_pll));
    codec_lower_->ConfigurePll(fs, mclock);
  }
}  // ConfigurePll_48_12000

TEST_F(Adau1361LowerTest, ConfigurePll_48_13000) {
  using ::testing::Args;
  using ::testing::ElementsAreArray;
  using ::testing::NotNull;
  const unsigned int target_pll_freq_48 = 49152000;  // Hz. See data sheet.
  const float acceptable_error = 5;  // Hz. Error of the PLL out.

  // Test 13MHz master clock for Fs 48kHz series.
  {
    const unsigned int mclock = 13000000;
    const unsigned int fs = 32000;
    uint8_t config_pll[] = {0x40, 0x02, 0x06, 0x59, 0x04, 0xF5, 0x19, 0x01};
    EXPECT_CALL(i2c_,
                WriteBlocking(
                    device_address_,     // Arg 0 : I2C Address.
                    NotNull(),           // Arg 1 : Data buffer address.
                    sizeof(config_pll),  // Arg 2 : Data buffer length to send.
                    false))              // Arg 3 : false to stop.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(config_pll)));
    // check the frequency error of the PLL
    EXPECT_NEAR(pll_out(mclock, config_pll), target_pll_freq_48,
                acceptable_error);
    // check whether PLL is enabled or not.
    EXPECT_TRUE(is_pll_enabled(config_pll));
    codec_lower_->ConfigurePll(fs, mclock);
  }
}  // ConfigurePll_48_13000

TEST_F(Adau1361LowerTest, ConfigurePll_48_14400) {
  using ::testing::Args;
  using ::testing::ElementsAreArray;
  using ::testing::NotNull;
  const unsigned int target_pll_freq_48 = 49152000;  // Hz. See data sheet.
  const float acceptable_error = 5;  // Hz. Error of the PLL out.

  // Test 14.4MHz master clock for Fs 48kHz series.
  {
    const unsigned int mclock = 14400000;
    const unsigned int fs = 48000;
    uint8_t config_pll[] = {0x40, 0x02, 0x00, 0x4B, 0x00, 0x3E, 0x33, 0x01};
    EXPECT_CALL(i2c_,
                WriteBlocking(
                    device_address_,     // Arg 0 : I2C Address.
                    NotNull(),           // Arg 1 : Data buffer address.
                    sizeof(config_pll),  // Arg 2 : Data buffer length to send.
                    false))              // Arg 3 : false to stop.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(config_pll)));
    // check the frequency error of the PLL
    EXPECT_NEAR(pll_out(mclock, config_pll), target_pll_freq_48,
                acceptable_error);
    // check whether PLL is enabled or not.
    EXPECT_TRUE(is_pll_enabled(config_pll));
    codec_lower_->ConfigurePll(fs, mclock);
  }
}  // ConfigurePll_48_14400

TEST_F(Adau1361LowerTest, ConfigurePll_48_19200) {
  using ::testing::Args;
  using ::testing::ElementsAreArray;
  using ::testing::NotNull;
  const unsigned int target_pll_freq_48 = 49152000;  // Hz. See data sheet.
  const float acceptable_error = 5;  // Hz. Error of the PLL out.

  // Test 19.2MHz master clock for Fs 48kHz series.
  {
    const unsigned int mclock = 19200000;
    const unsigned int fs = 96000;
    uint8_t config_pll[] = {0x40, 0x02, 0x00, 0x19, 0x00, 0x03, 0x2B, 0x01};
    EXPECT_CALL(i2c_,
                WriteBlocking(
                    device_address_,     // Arg 0 : I2C Address.
                    NotNull(),           // Arg 1 : Data buffer address.
                    sizeof(config_pll),  // Arg 2 : Data buffer length to send.
                    false))              // Arg 3 : false to stop.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(config_pll)));
    // check the frequency error of the PLL
    EXPECT_NEAR(pll_out(mclock, config_pll), target_pll_freq_48,
                acceptable_error);
    // check whether PLL is enabled or not.
    EXPECT_TRUE(is_pll_enabled(config_pll));
    codec_lower_->ConfigurePll(fs, mclock);
  }

}  // ConfigurePll_48_19200

TEST_F(Adau1361LowerTest, ConfigurePll_48_19680) {
  using ::testing::Args;
  using ::testing::ElementsAreArray;
  using ::testing::NotNull;
  const unsigned int target_pll_freq_48 = 49152000;  // Hz. See data sheet.
  const float acceptable_error = 5;  // Hz. Error of the PLL out.

  // Test 19.6MHz master clock for Fs 48kHz series.
  {
    const unsigned int mclock = 19680000;
    const unsigned int fs = 48000;
    uint8_t config_pll[] = {0x40, 0x02, 0x00, 0xCD, 0x00, 0xCC, 0x23, 0x01};
    EXPECT_CALL(i2c_,
                WriteBlocking(
                    device_address_,     // Arg 0 : I2C Address.
                    NotNull(),           // Arg 1 : Data buffer address.
                    sizeof(config_pll),  // Arg 2 : Data buffer length to send.
                    false))              // Arg 3 : false to stop.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(config_pll)));
    // check the frequency error of the PLL
    EXPECT_NEAR(pll_out(mclock, config_pll), target_pll_freq_48,
                acceptable_error);
    // check whether PLL is enabled or not.
    EXPECT_TRUE(is_pll_enabled(config_pll));
    codec_lower_->ConfigurePll(fs, mclock);
  }
}  // ConfigurePll_48_19680

TEST_F(Adau1361LowerTest, ConfigurePll_48_19800) {
  using ::testing::Args;
  using ::testing::ElementsAreArray;
  using ::testing::NotNull;
  const unsigned int target_pll_freq_48 = 49152000;  // Hz. See data sheet.
  const float acceptable_error = 5;  // Hz. Error of the PLL out.

  // Test 19.8MHz master clock for Fs 48kHz series.
  {
    const unsigned int mclock = 19800000;
    const unsigned int fs = 48000;
    uint8_t config_pll[] = {0x40, 0x02, 0x03, 0x39, 0x03, 0x1C, 0x23, 0x01};
    EXPECT_CALL(i2c_,
                WriteBlocking(
                    device_address_,     // Arg 0 : I2C Address.
                    NotNull(),           // Arg 1 : Data buffer address.
                    sizeof(config_pll),  // Arg 2 : Data buffer length to send.
                    false))              // Arg 3 : false to stop.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(config_pll)));
    // check the frequency error of the PLL
    EXPECT_NEAR(pll_out(mclock, config_pll), target_pll_freq_48,
                acceptable_error);
    // check whether PLL is enabled or not.
    EXPECT_TRUE(is_pll_enabled(config_pll));
    codec_lower_->ConfigurePll(fs, mclock);
  }
}  // ConfigurePll_48_19800

TEST_F(Adau1361LowerTest, ConfigurePll_48_24000) {
  using ::testing::Args;
  using ::testing::ElementsAreArray;
  using ::testing::NotNull;
  const unsigned int target_pll_freq_48 = 49152000;  // Hz. See data sheet.
  const float acceptable_error = 5;  // Hz. Error of the PLL out.

  // Test 24MHz master clock for Fs 48kHz series.
  {
    const unsigned int mclock = 24000000;
    const unsigned int fs = 48000;
    uint8_t config_pll[] = {0x40, 0x02, 0x00, 0x7D, 0x00, 0x0C, 0x23, 0x01};
    EXPECT_CALL(i2c_,
                WriteBlocking(
                    device_address_,     // Arg 0 : I2C Address.
                    NotNull(),           // Arg 1 : Data buffer address.
                    sizeof(config_pll),  // Arg 2 : Data buffer length to send.
                    false))              // Arg 3 : false to stop.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(config_pll)));
    // check the frequency error of the PLL
    EXPECT_NEAR(pll_out(mclock, config_pll), target_pll_freq_48,
                acceptable_error);
    // check whether PLL is enabled or not.
    // check whether PLL is enabled or not.
    EXPECT_TRUE(is_pll_enabled(config_pll));
    codec_lower_->ConfigurePll(fs, mclock);
  }

}  // ConfigurePll_48_24000

TEST_F(Adau1361LowerTest, ConfigurePll_48_26000) {
  using ::testing::Args;
  using ::testing::ElementsAreArray;
  using ::testing::NotNull;
  const unsigned int target_pll_freq_48 = 49152000;  // Hz. See data sheet.
  const float acceptable_error = 5;  // Hz. Error of the PLL out.

  // Test 26MHz master clock for Fs 48kHz series.
  {
    const unsigned int mclock = 26000000;
    const unsigned int fs = 48000;
    uint8_t config_pll[] = {0x40, 0x02, 0x06, 0x59, 0x04, 0xF5, 0x1B, 0x01};
    EXPECT_CALL(i2c_,
                WriteBlocking(
                    device_address_,     // Arg 0 : I2C Address.
                    NotNull(),           // Arg 1 : Data buffer address.
                    sizeof(config_pll),  // Arg 2 : Data buffer length to send.
                    false))              // Arg 3 : false to stop.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(config_pll)));
    // check the frequency error of the PLL
    EXPECT_NEAR(pll_out(mclock, config_pll), target_pll_freq_48,
                acceptable_error);
    // check whether PLL is enabled or not.
    EXPECT_TRUE(is_pll_enabled(config_pll));
    codec_lower_->ConfigurePll(fs, mclock);
  }
}  // ConfigurePll_48_26000

TEST_F(Adau1361LowerTest, ConfigurePll_48_27000) {
  using ::testing::Args;
  using ::testing::ElementsAreArray;
  using ::testing::NotNull;
  const unsigned int target_pll_freq_48 = 49152000;  // Hz. See data sheet.
  const float acceptable_error = 5;  // Hz. Error of the PLL out.

  // Test 27MHz master clock for Fs 48kHz series.
  {
    const unsigned int mclock = 27000000;
    const unsigned int fs = 48000;
    uint8_t config_pll[] = {0x40, 0x02, 0x04, 0x65, 0x02, 0xD1, 0x1B, 0x01};
    EXPECT_CALL(i2c_,
                WriteBlocking(
                    device_address_,     // Arg 0 : I2C Address.
                    NotNull(),           // Arg 1 : Data buffer address.
                    sizeof(config_pll),  // Arg 2 : Data buffer length to send.
                    false))              // Arg 3 : false to stop.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(config_pll)));
    // check the frequency error of the PLL
    EXPECT_NEAR(pll_out(mclock, config_pll), target_pll_freq_48,
                acceptable_error);
    // check whether PLL is enabled or not.
    EXPECT_TRUE(is_pll_enabled(config_pll));
    codec_lower_->ConfigurePll(fs, mclock);
  }
}  // ConfigurePll_48_27000

TEST_F(Adau1361LowerTest, ConfigurePll_48_12288) {
  using ::testing::Args;
  using ::testing::ElementsAreArray;
  using ::testing::NotNull;
  const unsigned int target_pll_freq_48 = 49152000;  // Hz. See data sheet.
  const float acceptable_error = 5;  // Hz. Error of the PLL out.

  // Test 27MHz master clock for Fs 48kHz series.
  {
    const unsigned int mclock = 12288000;
    const unsigned int fs = 48000;
    uint8_t config_pll[] = {0x40, 0x02, 0x04, 0x65, 0x02, 0xD1, 0x20, 0x01};
    EXPECT_CALL(i2c_,
                WriteBlocking(
                    device_address_,     // Arg 0 : I2C Address.
                    NotNull(),           // Arg 1 : Data buffer address.
                    sizeof(config_pll),  // Arg 2 : Data buffer length to send.
                    false))              // Arg 3 : false to stop.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(config_pll)));
    // check the frequency error of the PLL
    EXPECT_NEAR(pll_out(mclock, config_pll), target_pll_freq_48,
                acceptable_error);
    // check whether PLL is enabled or not.
    EXPECT_TRUE(is_pll_enabled(config_pll));
    codec_lower_->ConfigurePll(fs, mclock);
  }
}  // ConfigurePll_48_12288

TEST_F(Adau1361LowerTest, ConfigurePll_48_24576) {
  using ::testing::Args;
  using ::testing::ElementsAreArray;
  using ::testing::NotNull;
  const unsigned int target_pll_freq_48 = 49152000;  // Hz. See data sheet.
  const float acceptable_error = 5;  // Hz. Error of the PLL out.

  // Test 27MHz master clock for Fs 48kHz series.
  {
    const unsigned int mclock = 24576000;
    const unsigned int fs = 48000;
    uint8_t config_pll[] = {0x40, 0x02, 0x04, 0x65, 0x02, 0xD1, 0x10, 0x01};
    EXPECT_CALL(i2c_,
                WriteBlocking(
                    device_address_,     // Arg 0 : I2C Address.
                    NotNull(),           // Arg 1 : Data buffer address.
                    sizeof(config_pll),  // Arg 2 : Data buffer length to send.
                    false))              // Arg 3 : false to stop.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(config_pll)));
    // check the frequency error of the PLL
    EXPECT_NEAR(pll_out(mclock, config_pll), target_pll_freq_48,
                acceptable_error);
    // check whether PLL is enabled or not.
    EXPECT_TRUE(is_pll_enabled(config_pll));
    codec_lower_->ConfigurePll(fs, mclock);
  }
}  // ConfigurePll_48_24576

// ------------------------------------------------------------
//
//                               41.1kHz
//
// ------------------------------------------------------------

TEST_F(Adau1361LowerTest, ConfigurePll_441_08000) {
  using ::testing::Args;
  using ::testing::ElementsAreArray;
  using ::testing::NotNull;
  const unsigned int target_pll_freq_441 = 45158400;  // Hz. See data sheet.
  const float acceptable_error = 5;  // Hz. Error of the PLL out.

  // Test 8MHz master clock for Fs 48kHz series.
  {
    const unsigned int mclock = 8000000;
    const unsigned int fs = 22050;
    uint8_t config_pll[] = {0x40, 0x02, 0x02, 0x71, 0x01, 0x93, 0x29, 0x01};
    EXPECT_CALL(i2c_,
                WriteBlocking(
                    device_address_,     // Arg 0 : I2C Address.
                    NotNull(),           // Arg 1 : Data buffer address.
                    sizeof(config_pll),  // Arg 2 : Data buffer length to send.
                    false))              // Arg 3 : false to stop.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(config_pll)));
    // check the frequency error of the PLL
    EXPECT_NEAR(pll_out(mclock, config_pll), target_pll_freq_441,
                acceptable_error);
    // check whether PLL is enabled or not.
    EXPECT_TRUE(is_pll_enabled(config_pll));
    codec_lower_->ConfigurePll(fs, mclock);
  }
}  // ConfigurePll_441_08000

TEST_F(Adau1361LowerTest, ConfigurePll_441_12000) {
  using ::testing::Args;
  using ::testing::ElementsAreArray;
  using ::testing::NotNull;
  const unsigned int target_pll_freq_441 = 45158400;  // Hz. See data sheet.
  const float acceptable_error = 5;  // Hz. Error of the PLL out.

  // Test 12MHz master clock for Fs 48kHz series.
  {
    const unsigned int mclock = 12000000;
    const unsigned int fs = 44100;
    uint8_t config_pll[] = {0x40, 0x02, 0x02, 0x71, 0x01, 0xDD, 0x19, 0x01};
    EXPECT_CALL(i2c_,
                WriteBlocking(
                    device_address_,     // Arg 0 : I2C Address.
                    NotNull(),           // Arg 1 : Data buffer address.
                    sizeof(config_pll),  // Arg 2 : Data buffer length to send.
                    false))              // Arg 3 : false to stop.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(config_pll)));
    // check the frequency error of the PLL
    EXPECT_NEAR(pll_out(mclock, config_pll), target_pll_freq_441,
                acceptable_error);
    // check whether PLL is enabled or not.
    EXPECT_TRUE(is_pll_enabled(config_pll));
    codec_lower_->ConfigurePll(fs, mclock);
  }
}  // ConfigurePll_441_12000

TEST_F(Adau1361LowerTest, ConfigurePll_441_13000) {
  using ::testing::Args;
  using ::testing::ElementsAreArray;
  using ::testing::NotNull;
  const unsigned int target_pll_freq_441 = 45158400;  // Hz. See data sheet.
  const float acceptable_error = 5;  // Hz. Error of the PLL out.

  // Test 13MHz master clock for Fs 48kHz series.
  {
    const unsigned int mclock = 13000000;
    const unsigned int fs = 88200;
    uint8_t config_pll[] = {0x40, 0x02, 0x1F, 0xBD, 0x0F, 0x09, 0x19, 0x01};
    EXPECT_CALL(i2c_,
                WriteBlocking(
                    device_address_,     // Arg 0 : I2C Address.
                    NotNull(),           // Arg 1 : Data buffer address.
                    sizeof(config_pll),  // Arg 2 : Data buffer length to send.
                    false))              // Arg 3 : false to stop.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(config_pll)));
    // check the frequency error of the PLL
    EXPECT_NEAR(pll_out(mclock, config_pll), target_pll_freq_441,
                acceptable_error);
    // check whether PLL is enabled or not.
    EXPECT_TRUE(is_pll_enabled(config_pll));
    codec_lower_->ConfigurePll(fs, mclock);
  }
}  // ConfigurePll_441_13000

TEST_F(Adau1361LowerTest, ConfigurePll_441_14400) {
  using ::testing::Args;
  using ::testing::ElementsAreArray;
  using ::testing::NotNull;
  const unsigned int target_pll_freq_441 = 45158400;  // Hz. See data sheet.
  const float acceptable_error = 5;  // Hz. Error of the PLL out.

  // Test 14.4MHz master clock for Fs 48kHz series.
  {
    const unsigned int mclock = 14400000;
    const unsigned int fs = 44100;
    uint8_t config_pll[] = {0x40, 0x02, 0x00, 0x7D, 0x00, 0x22, 0x33, 0x01};
    EXPECT_CALL(i2c_,
                WriteBlocking(
                    device_address_,     // Arg 0 : I2C Address.
                    NotNull(),           // Arg 1 : Data buffer address.
                    sizeof(config_pll),  // Arg 2 : Data buffer length to send.
                    false))              // Arg 3 : false to stop.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(config_pll)));
    // check the frequency error of the PLL
    EXPECT_NEAR(pll_out(mclock, config_pll), target_pll_freq_441,
                acceptable_error);
    // check whether PLL is enabled or not.
    EXPECT_TRUE(is_pll_enabled(config_pll));
    codec_lower_->ConfigurePll(fs, mclock);
  }
}  // ConfigurePll_441_14400

TEST_F(Adau1361LowerTest, ConfigurePll_441_19200) {
  using ::testing::Args;
  using ::testing::ElementsAreArray;
  using ::testing::NotNull;
  const unsigned int target_pll_freq_441 = 45158400;  // Hz. See data sheet.
  const float acceptable_error = 5;  // Hz. Error of the PLL out.

  // Test 19.2MHz master clock for Fs 48kHz series.
  {
    const unsigned int mclock = 19200000;
    const unsigned int fs = 88200;
    uint8_t config_pll[] = {0x40, 0x02, 0x00, 0x7D, 0x00, 0x58, 0x23, 0x01};
    EXPECT_CALL(i2c_,
                WriteBlocking(
                    device_address_,     // Arg 0 : I2C Address.
                    NotNull(),           // Arg 1 : Data buffer address.
                    sizeof(config_pll),  // Arg 2 : Data buffer length to send.
                    false))              // Arg 3 : false to stop.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(config_pll)));
    // check the frequency error of the PLL
    EXPECT_NEAR(pll_out(mclock, config_pll), target_pll_freq_441,
                acceptable_error);
    // check whether PLL is enabled or not.
    EXPECT_TRUE(is_pll_enabled(config_pll));
    codec_lower_->ConfigurePll(fs, mclock);
  }

}  // ConfigurePll_441_19200

TEST_F(Adau1361LowerTest, ConfigurePll_441_19680) {
  using ::testing::Args;
  using ::testing::ElementsAreArray;
  using ::testing::NotNull;
  const unsigned int target_pll_freq_441 = 45158400;  // Hz. See data sheet.
  const float acceptable_error = 5;  // Hz. Error of the PLL out.

  // Test 19.6MHz master clock for Fs 48kHz series.
  {
    const unsigned int mclock = 19680000;
    const unsigned int fs = 44100;
    uint8_t config_pll[] = {0x40, 0x02, 0x04, 0x01, 0x02, 0x5C, 0x23, 0x01};
    EXPECT_CALL(i2c_,
                WriteBlocking(
                    device_address_,     // Arg 0 : I2C Address.
                    NotNull(),           // Arg 1 : Data buffer address.
                    sizeof(config_pll),  // Arg 2 : Data buffer length to send.
                    false))              // Arg 3 : false to stop.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(config_pll)));
    // check the frequency error of the PLL
    EXPECT_NEAR(pll_out(mclock, config_pll), target_pll_freq_441,
                acceptable_error);
    // check whether PLL is enabled or not.
    EXPECT_TRUE(is_pll_enabled(config_pll));
    codec_lower_->ConfigurePll(fs, mclock);
  }
}  // ConfigurePll_441_19680

TEST_F(Adau1361LowerTest, ConfigurePll_441_19800) {
  using ::testing::Args;
  using ::testing::ElementsAreArray;
  using ::testing::NotNull;
  const unsigned int target_pll_freq_441 = 45158400;  // Hz. See data sheet.
  const float acceptable_error = 5;  // Hz. Error of the PLL out.

  // Test 19.8MHz master clock for Fs 48kHz series.
  {
    const unsigned int mclock = 19800000;
    const unsigned int fs = 44100;
    uint8_t config_pll[] = {0x40, 0x02, 0x05, 0x5F, 0x03, 0x04, 0x23, 0x01};
    EXPECT_CALL(i2c_,
                WriteBlocking(
                    device_address_,     // Arg 0 : I2C Address.
                    NotNull(),           // Arg 1 : Data buffer address.
                    sizeof(config_pll),  // Arg 2 : Data buffer length to send.
                    false))              // Arg 3 : false to stop.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(config_pll)));
    // check the frequency error of the PLL
    EXPECT_NEAR(pll_out(mclock, config_pll), target_pll_freq_441,
                acceptable_error);
    // check whether PLL is enabled or not.
    EXPECT_TRUE(is_pll_enabled(config_pll));
    codec_lower_->ConfigurePll(fs, mclock);
  }
}  // ConfigurePll_441_19800

TEST_F(Adau1361LowerTest, ConfigurePll_441_24000) {
  using ::testing::Args;
  using ::testing::ElementsAreArray;
  using ::testing::NotNull;
  const unsigned int target_pll_freq_441 = 45158400;  // Hz. See data sheet.
  const float acceptable_error = 5;  // Hz. Error of the PLL out.

  // Test 24MHz master clock for Fs 48kHz series.
  {
    const unsigned int mclock = 24000000;
    const unsigned int fs = 44100;
    uint8_t config_pll[] = {0x40, 0x02, 0x02, 0x71, 0x01, 0xDD, 0x1B, 0x01};
    EXPECT_CALL(i2c_,
                WriteBlocking(
                    device_address_,     // Arg 0 : I2C Address.
                    NotNull(),           // Arg 1 : Data buffer address.
                    sizeof(config_pll),  // Arg 2 : Data buffer length to send.
                    false))              // Arg 3 : false to stop.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(config_pll)));
    // check the frequency error of the PLL
    EXPECT_NEAR(pll_out(mclock, config_pll), target_pll_freq_441,
                acceptable_error);
    // check whether PLL is enabled or not.
    EXPECT_TRUE(is_pll_enabled(config_pll));
    codec_lower_->ConfigurePll(fs, mclock);
  }

}  // ConfigurePll_441_24000

TEST_F(Adau1361LowerTest, ConfigurePll_441_26000) {
  using ::testing::Args;
  using ::testing::ElementsAreArray;
  using ::testing::NotNull;
  const unsigned int target_pll_freq_441 = 45158400;  // Hz. See data sheet.
  const float acceptable_error = 5;  // Hz. Error of the PLL out.

  // Test 26MHz master clock for Fs 48kHz series.
  {
    const unsigned int mclock = 26000000;
    const unsigned int fs = 44100;
    uint8_t config_pll[] = {0x40, 0x02, 0x1F, 0xBD, 0x0F, 0x09, 0x1B, 0x01};
    EXPECT_CALL(i2c_,
                WriteBlocking(
                    device_address_,     // Arg 0 : I2C Address.
                    NotNull(),           // Arg 1 : Data buffer address.
                    sizeof(config_pll),  // Arg 2 : Data buffer length to send.
                    false))              // Arg 3 : false to stop.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(config_pll)));
    // check the frequency error of the PLL
    EXPECT_NEAR(pll_out(mclock, config_pll), target_pll_freq_441,
                acceptable_error);
    // check whether PLL is enabled or not.
    EXPECT_TRUE(is_pll_enabled(config_pll));
    codec_lower_->ConfigurePll(fs, mclock);
  }
}  // ConfigurePll_441_26000

TEST_F(Adau1361LowerTest, ConfigurePll_441_27000) {
  using ::testing::Args;
  using ::testing::ElementsAreArray;
  using ::testing::NotNull;
  const unsigned int target_pll_freq_441 = 45158400;  // Hz. See data sheet.
  const float acceptable_error = 5;  // Hz. Error of the PLL out.

  // Test 27MHz master clock for Fs 48kHz series.
  {
    const unsigned int mclock = 27000000;
    const unsigned int fs = 44100;
    uint8_t config_pll[] = {0x40, 0x02, 0x07, 0x53, 0x02, 0x87, 0x1B, 0x01};
    EXPECT_CALL(i2c_,
                WriteBlocking(
                    device_address_,     // Arg 0 : I2C Address.
                    NotNull(),           // Arg 1 : Data buffer address.
                    sizeof(config_pll),  // Arg 2 : Data buffer length to send.
                    false))              // Arg 3 : false to stop.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(config_pll)));
    // check the frequency error of the PLL
    EXPECT_NEAR(pll_out(mclock, config_pll), target_pll_freq_441,
                acceptable_error);
    // check whether PLL is enabled or not.
    EXPECT_TRUE(is_pll_enabled(config_pll));
    codec_lower_->ConfigurePll(fs, mclock);
  }
}  // ConfigurePll_441_27000

TEST_F(Adau1361LowerTest, ConfigurePll_441_12288) {
  using ::testing::Args;
  using ::testing::ElementsAreArray;
  using ::testing::NotNull;
  const unsigned int target_pll_freq_441 = 45158400;  // Hz. See data sheet.
  const float acceptable_error = 5;  // Hz. Error of the PLL out.

  // Test 27MHz master clock for Fs 48kHz series.
  {
    const unsigned int mclock = 12288000;
    const unsigned int fs = 44100;
    uint8_t config_pll[] = {0x40, 0x02, 0x03, 0xE8, 0x02, 0xA3, 0x19, 0x01};
    EXPECT_CALL(i2c_,
                WriteBlocking(
                    device_address_,     // Arg 0 : I2C Address.
                    NotNull(),           // Arg 1 : Data buffer address.
                    sizeof(config_pll),  // Arg 2 : Data buffer length to send.
                    false))              // Arg 3 : false to stop.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(config_pll)));
    // check the frequency error of the PLL
    EXPECT_NEAR(pll_out(mclock, config_pll), target_pll_freq_441,
                acceptable_error);
    // check whether PLL is enabled or not.
    EXPECT_TRUE(is_pll_enabled(config_pll));
    codec_lower_->ConfigurePll(fs, mclock);
  }
}  // ConfigurePll_441_12288

TEST_F(Adau1361LowerTest, ConfigurePll_441_24576) {
  using ::testing::Args;
  using ::testing::ElementsAreArray;
  using ::testing::NotNull;
  const unsigned int target_pll_freq_441 = 45158400;  // Hz. See data sheet.
  const float acceptable_error = 5;  // Hz. Error of the PLL out.

  // Test 27MHz master clock for Fs 48kHz series.
  {
    const unsigned int mclock = 24576000;
    const unsigned int fs = 44100;
    uint8_t config_pll[] = {0x40, 0x02, 0x03, 0xE8, 0x02, 0xA3, 0x1B, 0x01};
    EXPECT_CALL(i2c_,
                WriteBlocking(
                    device_address_,     // Arg 0 : I2C Address.
                    NotNull(),           // Arg 1 : Data buffer address.
                    sizeof(config_pll),  // Arg 2 : Data buffer length to send.
                    false))              // Arg 3 : false to stop.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(config_pll)));
    // check the frequency error of the PLL
    EXPECT_NEAR(pll_out(mclock, config_pll), target_pll_freq_441,
                acceptable_error);
    // check whether PLL is enabled or not.
    EXPECT_TRUE(is_pll_enabled(config_pll));
    codec_lower_->ConfigurePll(fs, mclock);
  }
}  // ConfigurePll_441_24576

// ------------------------------------------------------------
//
//             ConfigurePll_48_wrong_master_clock
//
// ------------------------------------------------------------
// Validation test for wrong master clock at fs 48kHz.
TEST_F(Adau1361LowerDeathTest, ConfigurePll_48_wrong_master_clock) {
  {
    const unsigned int mclock = 123;
    const unsigned int fs = 48000;
    // check the assertion for bad mclock.
#ifndef NDEBUG
    EXPECT_DEATH(codec_lower_->ConfigurePll(fs, mclock),
                 "Wrong Master Clock with Fs 48kHz Series");
#endif
  }
}  // ConfigurePll_48_wrong_master_clock

// ------------------------------------------------------------
//
//             ConfigurePll_441_wrong_master_clock
//
// ------------------------------------------------------------
// Validation test for wrong master clock at fs 44.1kHz.
TEST_F(Adau1361LowerDeathTest, ConfigurePll_441_wrong_master_clock) {
  {
    const unsigned int mclock = 123;
    const unsigned int fs = 44100;
#ifndef NDEBUG
    ASSERT_DEATH(codec_lower_->ConfigurePll(fs, mclock),
                 "Wrong Master Clock with Fs 44.1kHz Series");
#endif
  }
}  // ConfigurePll_441_wrong_master_clock

// ------------------------------------------------------------
//
//             ConfigurePll_wrong_fs
//
// ------------------------------------------------------------
// Validation test for wrong fs .
TEST_F(Adau1361LowerDeathTest, ConfigurePll_wrong_fs) {
  // Test 27MHz master clock for Fs 48kHz series.
  {
    const unsigned int mclock = 8000000;
    const unsigned int fs = 192000;
    // check the assertion for bad mclock.
#ifndef NDEBUG
    ASSERT_DEATH(codec_lower_->ConfigurePll(fs, mclock), "Bad Fs");
#endif
  }
}  // ConfigurePll_wrong_fs

// -----------------------------------------------------------------
//
//                          SetLineInputGain()
//
// -----------------------------------------------------------------
// Mute test
TEST_F(Adau1361LowerTest, SetLineInputGain_mute) {
  using ::testing::Args;
  using ::testing::DoAll;
  using ::testing::ElementsAreArray;
  using ::testing::InSequence;
  using ::testing::NotNull;
  using ::testing::Return;
  using ::testing::SetArrayArgument;

  // Left check
  {
    InSequence dummy;
    // First two bytes are register number.
    // in the case of mute, bit3:1 are zero.
    uint8_t ltxbuf1[3] = {0x40, 0x0a, 0x01};

    // Expectation of mute
    EXPECT_CALL(
        i2c_,
        WriteBlocking(device_address_,  // Arg 0 : I2C Address.
                      NotNull(),        // Arg 1 : Data buffer address.
                      sizeof(ltxbuf1),  // Arg 2 : Data buffer length to send.
                      false))           // Arg 3 : false to stop.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(ltxbuf1)))
        .WillOnce(Return(sizeof(ltxbuf1)));

    // First two bytes are register number.
    // in the case of mute, bit3:1 are zero.
    uint8_t rtxbuf1[3] = {0x40, 0x0c, 0x01};

    // Expectation of mute.
    EXPECT_CALL(
        i2c_,
        WriteBlocking(device_address_,  // Arg 0 : I2C Address.
                      NotNull(),        // Arg 1 : Data buffer address.
                      sizeof(rtxbuf1),  // Arg 2 : Data buffer length to send.
                      false))           // Arg 3 : false to stop.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(rtxbuf1)))
        .WillOnce(Return(sizeof(rtxbuf1)));
  }

  // must be set to muted.
  codec_lower_->SetLineInputGain(0.0, 0.0, true);
}  // SetLineInputGain_mute

// The gain over 6dB must be truncated to 6dB.
TEST_F(Adau1361LowerTest, SetLineInputGain_overgain) {
  using ::testing::Args;
  using ::testing::DoAll;
  using ::testing::ElementsAreArray;
  using ::testing::InSequence;
  using ::testing::NotNull;
  using ::testing::Return;
  using ::testing::SetArrayArgument;

  // Left check
  {
    InSequence dummy;

    uint8_t ltxbuf1[3] = {0x40, 0x0a,
                          0x0f};  // in the case of gain=6dB, bit 3:1 are 111.

    //  expectation of gain setting.
    EXPECT_CALL(
        i2c_,
        WriteBlocking(device_address_,  // Arg 0 : I2C Address.
                      NotNull(),        // Arg 1 : Data buffer address.
                      sizeof(ltxbuf1),  // Arg 2 : Data buffer length to send.
                      false))           // Arg 3 : false to stop.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(ltxbuf1)))
        .WillOnce(Return(sizeof(ltxbuf1)));

    // right check
    uint8_t rtxbuf1[3] = {0x40, 0x0c,
                          0x0f};  // in the case of gain=6dB, bit 3:1 are 111

    //  expectation of gain setting.
    EXPECT_CALL(
        i2c_,
        WriteBlocking(device_address_,  // Arg 0 : I2C Address.
                      NotNull(),        // Arg 1 : Data buffer address.
                      sizeof(rtxbuf1),  // Arg 2 : Data buffer length to send.
                      false))           // Arg 3 : false to stop.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(rtxbuf1)))
        .WillOnce(Return(sizeof(rtxbuf1)));
  }

  // must be truncated to 6dB
  codec_lower_->SetLineInputGain(20.0, 30.0, false);
}  // SetLineInputGain_overgain

// The gain under -12dB must be truncated to -12dB
TEST_F(Adau1361LowerTest, SetLineInputGain_undergain) {
  using ::testing::Args;
  using ::testing::DoAll;
  using ::testing::ElementsAreArray;
  using ::testing::InSequence;
  using ::testing::NotNull;
  using ::testing::Return;
  using ::testing::SetArrayArgument;

  // Left check
  {
    InSequence dummy;

    uint8_t ltxbuf1[3] = {0x40, 0x0a,
                          0x03};  // in the case of gain=-12dB, bit 3:1 are 001
    // Set the address to read.

    //  expectation of -12dB.
    EXPECT_CALL(
        i2c_,
        WriteBlocking(device_address_,  // Arg 0 : I2C Address.
                      NotNull(),        // Arg 1 : Data buffer address.
                      sizeof(ltxbuf1),  // Arg 2 : Data buffer length to send.
                      false))           // Arg 3 : false to stop.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(ltxbuf1)))
        .WillOnce(Return(sizeof(ltxbuf1)));

    // right check
    uint8_t rtxbuf1[3] = {0x40, 0x0c,
                          0x03};  // in the case of gain=-12dB, bit 3:1 are 001

    //  expectation of -12dB.
    EXPECT_CALL(
        i2c_,
        WriteBlocking(device_address_,  // Arg 0 : I2C Address.
                      NotNull(),        // Arg 1 : Data buffer address.
                      sizeof(rtxbuf1),  // Arg 2 : Data buffer length to send.
                      false))           // Arg 3 : false to stop.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(rtxbuf1)))
        .WillOnce(Return(sizeof(rtxbuf1)));
  }

  // must be truncated to -12dB
  codec_lower_->SetLineInputGain(-20.0, -30.0, false);
}  // SetLineInputGain_undergain

// The gain between [-12dB, 6dB] must be truncated to 3dB steps.
TEST_F(Adau1361LowerTest, SetLineInputGain_appropriate_gain) {
  using ::testing::Args;
  using ::testing::DoAll;
  using ::testing::ElementsAreArray;
  using ::testing::InSequence;
  using ::testing::NotNull;
  using ::testing::Return;
  using ::testing::SetArrayArgument;

  // Left check
  {
    InSequence dummy;

    uint8_t ltxbuf1[3] = {0x40, 0x0a,
                          0x0B};  // in the case of gain=0dB, bit 3:1 are 101

    // expectation of appropriate gain.
    EXPECT_CALL(
        i2c_,
        WriteBlocking(device_address_,  // Arg 0 : I2C Address.
                      NotNull(),        // Arg 1 : Data buffer address.
                      sizeof(ltxbuf1),  // Arg 2 : Data buffer length to send.
                      false))           // Arg 3 : false to stop.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(ltxbuf1)))
        .WillOnce(Return(sizeof(ltxbuf1)));

    // right check
    uint8_t rtxbuf1[3] = {0x40, 0x0c,
                          0x09};  // in the case of gain=-3dB, bit 3:1 are 100
    // Set the address to read.

    // expectation of appropriate gain.
    EXPECT_CALL(
        i2c_,
        WriteBlocking(device_address_,  // Arg 0 : I2C Address.
                      NotNull(),        // Arg 1 : Data buffer address.
                      sizeof(rtxbuf1),  // Arg 2 : Data buffer length to send.
                      false))           // Arg 3 : false to stop.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(rtxbuf1)))
        .WillOnce(Return(sizeof(rtxbuf1)));
  }

  // must be truncated to 0dB and -3dB.
  codec_lower_->SetLineInputGain(1, -1, false);
}  // SetLineInputGain_appropriate_gain

// -----------------------------------------------------------------
//
//                          SetLineOutputGain()
//
// -----------------------------------------------------------------
// Mute test
TEST_F(Adau1361LowerTest, SetLineOutputGain_mute) {
  using ::testing::Args;
  using ::testing::ElementsAreArray;
  using ::testing::InSequence;
  using ::testing::NotNull;
  using ::testing::Return;

  {
    InSequence dummy;

    // Left check
    uint8_t ltxbuf1[3] = {0x40, 0x25, 0xff};  // R31

    //  expectation of mute.
    ltxbuf1[2] = (0x39 << 2) | 0x00;  // given 0dB, mute, line
    EXPECT_CALL(
        i2c_,
        WriteBlocking(device_address_,  // Arg 0 : I2C Address.
                      NotNull(),        // Arg 1 : Data buffer address.
                      sizeof(ltxbuf1),  // Arg 2 : Data buffer length to send.
                      false))           // Arg 3 : false to stop.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(ltxbuf1)))
        .WillOnce(Return(sizeof(ltxbuf1)));

    // right check
    uint8_t rtxbuf1[3] = {0x40, 0x26, 0xff};  // L31

    //  expectation of mute.
    rtxbuf1[2] = (0x39 << 2) | 0x00;  // given 0dB, mute, line
    EXPECT_CALL(
        i2c_,
        WriteBlocking(device_address_,  // Arg 0 : I2C Address.
                      NotNull(),        // Arg 1 : Data buffer address.
                      sizeof(rtxbuf1),  // Arg 2 : Data buffer length to send.
                      false))           // Arg 3 : false to stop.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(rtxbuf1)))
        .WillOnce(Return(sizeof(rtxbuf1)));
  }

  // must be set to muted.
  codec_lower_->SetLineOutputGain(0.0, 0.0, true);
}  // SetLineOutputGain_mute

// Overgain test : must be truncated to 6dB
TEST_F(Adau1361LowerTest, SetLineOutputGain_overgain) {
  using ::testing::Args;
  using ::testing::ElementsAreArray;
  using ::testing::InSequence;
  using ::testing::NotNull;
  using ::testing::Return;

  {
    InSequence dummy;

    // Left check
    uint8_t ltxbuf1[3] = {0x40, 0x25, 0xff};  // R31

    //  expectation of mute.
    ltxbuf1[2] = (0x3F << 2) | 0x02;  // 6dB is 111111, unmute, line
    EXPECT_CALL(
        i2c_,
        WriteBlocking(device_address_,  // Arg 0 : I2C Address.
                      NotNull(),        // Arg 1 : Data buffer address.
                      sizeof(ltxbuf1),  // Arg 2 : Data buffer length to send.
                      false))           // Arg 3 : false to stop.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(ltxbuf1)))
        .WillOnce(Return(sizeof(ltxbuf1)));

    // right check
    uint8_t rtxbuf1[3] = {0x40, 0x26, 0xff};  // L31

    //  expectation of mute.
    rtxbuf1[2] = (0x3F << 2) | 0x02;  // 6dB is 1111, unmute, line
    EXPECT_CALL(
        i2c_,
        WriteBlocking(device_address_,  // Arg 0 : I2C Address.
                      NotNull(),        // Arg 1 : Data buffer address.
                      sizeof(rtxbuf1),  // Arg 2 : Data buffer length to send.
                      false))           // Arg 3 : false to stop.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(rtxbuf1)))
        .WillOnce(Return(sizeof(rtxbuf1)));
  }

  // must be truncated to 6dB, not muted.
  codec_lower_->SetLineOutputGain(12.0, 18.0, false);
}  // SetLineOutputGain_overgain

// Overgain test : The gain under -57dB must be truncated to -57dB
TEST_F(Adau1361LowerTest, SetLineOutputGain_undergain) {
  using ::testing::Args;
  using ::testing::ElementsAreArray;
  using ::testing::InSequence;
  using ::testing::NotNull;
  using ::testing::Return;

  {
    InSequence dummy;

    // Left check
    uint8_t ltxbuf1[3] = {0x40, 0x25, 0xff};  // R31

    //  expectation of mute.
    ltxbuf1[2] = (0x00 << 2) | 0x02;  // -57dB is 0000, unmute, line
    EXPECT_CALL(
        i2c_,
        WriteBlocking(device_address_,  // Arg 0 : I2C Address.
                      NotNull(),        // Arg 1 : Data buffer address.
                      sizeof(ltxbuf1),  // Arg 2 : Data buffer length to send.
                      false))           // Arg 3 : false to stop.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(ltxbuf1)))
        .WillOnce(Return(sizeof(ltxbuf1)));

    // right check
    uint8_t rtxbuf1[3] = {0x40, 0x26, 0xff};  // L31

    //  expectation of mute.
    rtxbuf1[2] = (0x00 << 2) | 0x02;  // -57dB is 0000, unmute, line
    EXPECT_CALL(
        i2c_,
        WriteBlocking(device_address_,  // Arg 0 : I2C Address.
                      NotNull(),        // Arg 1 : Data buffer address.
                      sizeof(rtxbuf1),  // Arg 2 : Data buffer length to send.
                      false))           // Arg 3 : false to stop.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(rtxbuf1)))
        .WillOnce(Return(sizeof(rtxbuf1)));
  }

  // must be truncated to 57dB, not muted.
  codec_lower_->SetLineOutputGain(-60.0, -70.0, false);
}  // SetLineOutputGain_undergain

// The gain between [-57dB, 6dB] must be truncated to 1dB steps.
// Overgain test : The gain under -57dB must be truncated to -57dB
TEST_F(Adau1361LowerTest, SetLineOutputGain_appropriate_gain) {
  using ::testing::Args;
  using ::testing::ElementsAreArray;
  using ::testing::InSequence;
  using ::testing::NotNull;
  using ::testing::Return;

  {
    InSequence dummy;

    // Left check
    uint8_t ltxbuf1[3] = {0x40, 0x25, 0xff};  // R31

    //  expectation of mute.
    ltxbuf1[2] = (0x3b << 2) | 0x02;  // 2dB is 110011, unmute, line
    EXPECT_CALL(
        i2c_,
        WriteBlocking(device_address_,  // Arg 0 : I2C Address.
                      NotNull(),        // Arg 1 : Data buffer address.
                      sizeof(ltxbuf1),  // Arg 2 : Data buffer length to send.
                      false))           // Arg 3 : false to stop.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(ltxbuf1)))
        .WillOnce(Return(sizeof(ltxbuf1)));

    // right check
    uint8_t rtxbuf1[3] = {0x40, 0x26, 0xff};  // L31

    //  expectation of mute.
    rtxbuf1[2] = (0x34 << 2) | 0x02;  // -57dB is 000000, unmute, line
    EXPECT_CALL(
        i2c_,
        WriteBlocking(device_address_,  // Arg 0 : I2C Address.
                      NotNull(),        // Arg 1 : Data buffer address.
                      sizeof(rtxbuf1),  // Arg 2 : Data buffer length to send.
                      false))           // Arg 3 : false to stop.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(rtxbuf1)))
        .WillOnce(Return(sizeof(rtxbuf1)));
  }

  // must be set to 2.0 and -5.0dB, not muted.
  codec_lower_->SetLineOutputGain(2.0, -5.0, false);
}  // SetLineOutputGain_appropriate_gain

// -----------------------------------------------------------------
//
//                          SetAuxInputGain()
//
// -----------------------------------------------------------------

// Mute test
TEST_F(Adau1361LowerTest, SetAuxInputGain_mute) {
  using ::testing::Args;
  using ::testing::ElementsAreArray;
  using ::testing::InSequence;
  using ::testing::NotNull;
  using ::testing::Return;

  {
    InSequence dummy;

    // Left check
    uint8_t ltxbuf1[3] = {0x40, 0x0b, 0x00};  // R5 : 0 if muted.

    //  expectation of mute.
    EXPECT_CALL(
        i2c_,
        WriteBlocking(device_address_,  // Arg 0 : I2C Address.
                      NotNull(),        // Arg 1 : Data buffer address.
                      sizeof(ltxbuf1),  // Arg 2 : Data buffer length to send.
                      false))           // Arg 3 : false to stop.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(ltxbuf1)))
        .WillOnce(Return(sizeof(ltxbuf1)));

    // right check
    uint8_t rtxbuf1[3] = {0x40, 0x0d, 0x00};  // R7 : 0 if muted.

    //  expectation of mute.
    EXPECT_CALL(
        i2c_,
        WriteBlocking(device_address_,  // Arg 0 : I2C Address.
                      NotNull(),        // Arg 1 : Data buffer address.
                      sizeof(rtxbuf1),  // Arg 2 : Data buffer length to send.
                      false))           // Arg 3 : false to stop.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(rtxbuf1)))
        .WillOnce(Return(sizeof(rtxbuf1)));
  }

  // must be set to muted.
  codec_lower_->SetAuxInputGain(0.0, 0.0, true);
}  // SetAuxInputGain_mute

// Overgain test : must be truncated to 6dB
TEST_F(Adau1361LowerTest, SetAuxInputGain_overgain) {
  using ::testing::Args;
  using ::testing::ElementsAreArray;
  using ::testing::InSequence;
  using ::testing::NotNull;
  using ::testing::Return;

  {
    InSequence dummy;

    // Left check
    uint8_t ltxbuf1[3] = {0x40, 0x0b, 0x07};  // R5 : 111 if 6dB.

    //  expectation of 6db.
    EXPECT_CALL(
        i2c_,
        WriteBlocking(device_address_,  // Arg 0 : I2C Address.
                      NotNull(),        // Arg 1 : Data buffer address.
                      sizeof(ltxbuf1),  // Arg 2 : Data buffer length to send.
                      false))           // Arg 3 : false to stop.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(ltxbuf1)))
        .WillOnce(Return(sizeof(ltxbuf1)));

    // right check
    uint8_t rtxbuf1[3] = {0x40, 0x0d, 0x07};  // R7 : 111 if 6dB.

    //  expectation of 6db.
    EXPECT_CALL(
        i2c_,
        WriteBlocking(device_address_,  // Arg 0 : I2C Address.
                      NotNull(),        // Arg 1 : Data buffer address.
                      sizeof(rtxbuf1),  // Arg 2 : Data buffer length to send.
                      false))           // Arg 3 : false to stop.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(rtxbuf1)))
        .WillOnce(Return(sizeof(rtxbuf1)));
  }

  // must be truncated to 6dB, not muted.
  codec_lower_->SetAuxInputGain(12.0, 18.0, false);
}  // SetAuxInputGain_overgain

// Overgain test : The gain under -12dB must be truncated to -12dB
TEST_F(Adau1361LowerTest, SetAuxInputGain_undergain) {
  using ::testing::Args;
  using ::testing::ElementsAreArray;
  using ::testing::InSequence;
  using ::testing::NotNull;
  using ::testing::Return;

  {
    InSequence dummy;

    // Left check
    uint8_t ltxbuf1[3] = {0x40, 0x0b, 0x01};  // R5 : 001 if -12dB.

    //  expectation of -12dB.
    EXPECT_CALL(
        i2c_,
        WriteBlocking(device_address_,  // Arg 0 : I2C Address.
                      NotNull(),        // Arg 1 : Data buffer address.
                      sizeof(ltxbuf1),  // Arg 2 : Data buffer length to send.
                      false))           // Arg 3 : false to stop.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(ltxbuf1)))
        .WillOnce(Return(sizeof(ltxbuf1)));

    // right check
    uint8_t rtxbuf1[3] = {0x40, 0x0d, 0x01};  // R7 : 001 if -12dB.

    //  expectation of -12dB.
    EXPECT_CALL(
        i2c_,
        WriteBlocking(device_address_,  // Arg 0 : I2C Address.
                      NotNull(),        // Arg 1 : Data buffer address.
                      sizeof(rtxbuf1),  // Arg 2 : Data buffer length to send.
                      false))           // Arg 3 : false to stop.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(rtxbuf1)))
        .WillOnce(Return(sizeof(rtxbuf1)));
  }

  // must be truncated to 57dB, not muted.
  codec_lower_->SetAuxInputGain(-60.0, -70.0, false);
}  // SetAuxInputGain_undergain

// The gain between [-12dB, 6dB] must be truncated to 3dB steps.
TEST_F(Adau1361LowerTest, SetAuxInputGain_appropriate_gain) {
  using ::testing::Args;
  using ::testing::ElementsAreArray;
  using ::testing::InSequence;
  using ::testing::NotNull;
  using ::testing::Return;

  {
    InSequence dummy;

    // Left check
    uint8_t ltxbuf1[3] = {0x40, 0x0b, 0x05};  // R5 : 101 if 0dB.

    //  expectation of mute.
    EXPECT_CALL(
        i2c_,
        WriteBlocking(device_address_,  // Arg 0 : I2C Address.
                      NotNull(),        // Arg 1 : Data buffer address.
                      sizeof(ltxbuf1),  // Arg 2 : Data buffer length to send.
                      false))           // Arg 3 : false to stop.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(ltxbuf1)))
        .WillOnce(Return(sizeof(ltxbuf1)));

    // right check
    uint8_t rtxbuf1[3] = {0x40, 0x0d, 0x03};  // R7 : 011 if -6dB.

    //  expectation of mute.
    EXPECT_CALL(
        i2c_,
        WriteBlocking(device_address_,  // Arg 0 : I2C Address.
                      NotNull(),        // Arg 1 : Data buffer address.
                      sizeof(rtxbuf1),  // Arg 2 : Data buffer length to send.
                      false))           // Arg 3 : false to stop.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(rtxbuf1)))
        .WillOnce(Return(sizeof(rtxbuf1)));
  }

  // must be set to 0.0 and -6.0dB, not muted.
  codec_lower_->SetAuxInputGain(2.0, -5.0, false);
}  // SetAuxInputGain_appropriate_gain

// -----------------------------------------------------------------
//
//                          SetHpOutputGain()
//
// -----------------------------------------------------------------

// Mute test
TEST_F(Adau1361LowerTest, SetHpOutputGain_mute) {
  using ::testing::Args;
  using ::testing::ElementsAreArray;
  using ::testing::InSequence;
  using ::testing::NotNull;
  using ::testing::Return;

  {
    InSequence dummy;

    // Left check
    uint8_t ltxbuf1[3] = {0x40, 0x23,
                          0xe5};  // R29 : 11001,0,1 ( muted, headphone)

    //  expectation of mute.
    EXPECT_CALL(
        i2c_,
        WriteBlocking(device_address_,  // Arg 0 : I2C Address.
                      NotNull(),        // Arg 1 : Data buffer address.
                      sizeof(ltxbuf1),  // Arg 2 : Data buffer length to send.
                      false))           // Arg 3 : false to stop.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(ltxbuf1)))
        .WillOnce(Return(sizeof(ltxbuf1)));

    // right check
    uint8_t rtxbuf1[3] = {0x40, 0x24,
                          0xe5};  // R30 : 11001,0,1 ( muted, headphone)

    //  expectation of mute.
    EXPECT_CALL(
        i2c_,
        WriteBlocking(device_address_,  // Arg 0 : I2C Address.
                      NotNull(),        // Arg 1 : Data buffer address.
                      sizeof(rtxbuf1),  // Arg 2 : Data buffer length to send.
                      false))           // Arg 3 : false to stop.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(rtxbuf1)))
        .WillOnce(Return(sizeof(rtxbuf1)));
  }

  // must be set to muted.
  codec_lower_->SetHpOutputGain(0.0, 0.0, true);
}  // SetHpOutputGain_mute

// Overgain test : must be truncated to 6dB
TEST_F(Adau1361LowerTest, SetHpOutputGain_overgain) {
  using ::testing::Args;
  using ::testing::ElementsAreArray;
  using ::testing::InSequence;
  using ::testing::NotNull;
  using ::testing::Return;

  {
    InSequence dummy;

    // Left check
    uint8_t ltxbuf1[3] = {0x40, 0x23,
                          0xff};  // R29 : 6dB is 111111, unmute, line

    //  expectation of 6dB.
    EXPECT_CALL(
        i2c_,
        WriteBlocking(device_address_,  // Arg 0 : I2C Address.
                      NotNull(),        // Arg 1 : Data buffer address.
                      sizeof(ltxbuf1),  // Arg 2 : Data buffer length to send.
                      false))           // Arg 3 : false to stop.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(ltxbuf1)))
        .WillOnce(Return(sizeof(ltxbuf1)));

    // right check
    uint8_t rtxbuf1[3] = {0x40, 0x24, 0xff};  // R30 : 6dB is 1111, unmute, line

    //  expectation of 6dB.
    EXPECT_CALL(
        i2c_,
        WriteBlocking(device_address_,  // Arg 0 : I2C Address.
                      NotNull(),        // Arg 1 : Data buffer address.
                      sizeof(rtxbuf1),  // Arg 2 : Data buffer length to send.
                      false))           // Arg 3 : false to stop.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(rtxbuf1)))
        .WillOnce(Return(sizeof(rtxbuf1)));
  }

  // must be truncated to 6dB, not muted.
  codec_lower_->SetHpOutputGain(12.0, 18.0, false);
}  // SetHpOutputGain_overgain

// Overgain test : The gain under -57dB must be truncated to -57dB
TEST_F(Adau1361LowerTest, SetHpOutputGain_undergain) {
  using ::testing::Args;
  using ::testing::ElementsAreArray;
  using ::testing::InSequence;
  using ::testing::NotNull;
  using ::testing::Return;

  {
    InSequence dummy;

    // Left check
    uint8_t ltxbuf1[3] = {0x40, 0x23,
                          0x03};  // R29 : -57dB is 0000, unmute, line

    //  expectation of -57.
    EXPECT_CALL(
        i2c_,
        WriteBlocking(device_address_,  // Arg 0 : I2C Address.
                      NotNull(),        // Arg 1 : Data buffer address.
                      sizeof(ltxbuf1),  // Arg 2 : Data buffer length to send.
                      false))           // Arg 3 : false to stop.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(ltxbuf1)))
        .WillOnce(Return(sizeof(ltxbuf1)));

    // right check
    uint8_t rtxbuf1[3] = {0x40, 0x24,
                          0x03};  // R30 :  -57dB is 0000, unmute, line

    //  expectation of -57dB.
    EXPECT_CALL(
        i2c_,
        WriteBlocking(device_address_,  // Arg 0 : I2C Address.
                      NotNull(),        // Arg 1 : Data buffer address.
                      sizeof(rtxbuf1),  // Arg 2 : Data buffer length to send.
                      false))           // Arg 3 : false to stop.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(rtxbuf1)))
        .WillOnce(Return(sizeof(rtxbuf1)));
  }

  // must be truncated to 57dB, not muted.
  codec_lower_->SetHpOutputGain(-60.0, -70.0, false);
}  // SetHpOutputGain_undergain

// The gain between [-57dB, 6dB] must be truncated to 1dB steps.
// Overgain test : The gain under -57dB must be truncated to -57dB
TEST_F(Adau1361LowerTest, SetHpOutputGain_appropriate_gain) {
  using ::testing::Args;
  using ::testing::ElementsAreArray;
  using ::testing::InSequence;
  using ::testing::NotNull;
  using ::testing::Return;

  {
    InSequence dummy;

    // Left check
    uint8_t ltxbuf1[3] = {0x40, 0x23,
                          0xef};  // R29 : 2dB is 111011, unmute, line

    //  expectation of mute.
    EXPECT_CALL(
        i2c_,
        WriteBlocking(device_address_,  // Arg 0 : I2C Address.
                      NotNull(),        // Arg 1 : Data buffer address.
                      sizeof(ltxbuf1),  // Arg 2 : Data buffer length to send.
                      false))           // Arg 3 : false to stop.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(ltxbuf1)))
        .WillOnce(Return(sizeof(ltxbuf1)));

    // right check
    uint8_t rtxbuf1[3] = {0x40, 0x24,
                          0xd3};  // R30 : -5dB is 110100, unmute, line

    //  expectation of mute.
    EXPECT_CALL(
        i2c_,
        WriteBlocking(device_address_,  // Arg 0 : I2C Address.
                      NotNull(),        // Arg 1 : Data buffer address.
                      sizeof(rtxbuf1),  // Arg 2 : Data buffer length to send.
                      false))           // Arg 3 : false to stop.
        .With(Args<1,  // parameter position of the array : 0 origin.
                   2>  // parameter position of the size : 0 origin.
              (ElementsAreArray(rtxbuf1)))
        .WillOnce(Return(sizeof(rtxbuf1)));
  }

  // must be set to 2.0 and -5.0dB, not muted.
  codec_lower_->SetHpOutputGain(2.0, -5.0, false);
}  // SetHpOutputGain_appropriate_gain

// -----------------------------------------------------------------
//
//                          ConfigureSRC()
//
// -----------------------------------------------------------------

// Validation test for wrong fs.
TEST_F(Adau1361LowerDeathTest, ConfigureSRC_wrong_fs) {
  const unsigned int fs = 192000;  // not supported by ADAU1361
  // check the assertion for bad mclock.
#ifndef NDEBUG
  ASSERT_DEATH(codec_lower_->ConfigureSRC(fs), "Bad Fs");
#endif
}  // ConfigureSRC_wrong_fs

// Validation test for fs 22050 Hz.
TEST_F(Adau1361LowerTest, ConfigureSRC_22050) {
  const unsigned int fs = 22050;  // Hz
  const uint8_t config_src[] = {0x40, 0x17, 0x04};

  using ::testing::Args;
  using ::testing::ElementsAreArray;
  using ::testing::NotNull;
  using ::testing::Return;

  EXPECT_CALL(
      i2c_,
      WriteBlocking(device_address_,     // Arg 0 : I2C Address.
                    NotNull(),           // Arg 1 : Data buffer address.
                    sizeof(config_src),  // Arg 2 : Data buffer length to send.
                    false))              // Arg 3 : false to stop.
      .With(Args<1,  // parameter position of the array : 0 origin.
                 2>  // parameter position of the size : 0 origin.
            (ElementsAreArray(config_src)))
      .WillOnce(Return(sizeof(config_src)));

  // right configuration of SRC.
  codec_lower_->ConfigureSRC(fs);
}  // ConfigureSRC_22050

// Validation test for fs 24000 Hz.
TEST_F(Adau1361LowerTest, ConfigureSRC_24000) {
  const unsigned int fs = 24000;  // Hz
  const uint8_t config_src[] = {0x40, 0x17, 0x04};

  using ::testing::Args;
  using ::testing::ElementsAreArray;
  using ::testing::NotNull;
  using ::testing::Return;

  EXPECT_CALL(
      i2c_,
      WriteBlocking(device_address_,     // Arg 0 : I2C Address.
                    NotNull(),           // Arg 1 : Data buffer address.
                    sizeof(config_src),  // Arg 2 : Data buffer length to send.
                    false))              // Arg 3 : false to stop.
      .With(Args<1,  // parameter position of the array : 0 origin.
                 2>  // parameter position of the size : 0 origin.
            (ElementsAreArray(config_src)))
      .WillOnce(Return(sizeof(config_src)));

  // right configuration of SRC.
  codec_lower_->ConfigureSRC(fs);
}  // ConfigureSRC_24000

// Validation test for fs 32000 Hz.
TEST_F(Adau1361LowerTest, ConfigureSRC_32000) {
  const unsigned int fs = 32000;  // Hz
  const uint8_t config_src[] = {0x40, 0x17, 0x05};

  using ::testing::Args;
  using ::testing::ElementsAreArray;
  using ::testing::NotNull;
  using ::testing::Return;

  EXPECT_CALL(
      i2c_,
      WriteBlocking(device_address_,     // Arg 0 : I2C Address.
                    NotNull(),           // Arg 1 : Data buffer address.
                    sizeof(config_src),  // Arg 2 : Data buffer length to send.
                    false))              // Arg 3 : false to stop.
      .With(Args<1,  // parameter position of the array : 0 origin.
                 2>  // parameter position of the size : 0 origin.
            (ElementsAreArray(config_src)))
      .WillOnce(Return(sizeof(config_src)));

  // right configuration of SRC.
  codec_lower_->ConfigureSRC(fs);
}  // ConfigureSRC_32000

// Validation test for fs 44100 Hz.
TEST_F(Adau1361LowerTest, ConfigureSRC_44100) {
  const unsigned int fs = 44100;  // Hz
  const uint8_t config_src[] = {0x40, 0x17, 0x00};

  using ::testing::Args;
  using ::testing::ElementsAreArray;
  using ::testing::NotNull;
  using ::testing::Return;

  EXPECT_CALL(
      i2c_,
      WriteBlocking(device_address_,     // Arg 0 : I2C Address.
                    NotNull(),           // Arg 1 : Data buffer address.
                    sizeof(config_src),  // Arg 2 : Data buffer length to send.
                    false))              // Arg 3 : false to stop.
      .With(Args<1,  // parameter position of the array : 0 origin.
                 2>  // parameter position of the size : 0 origin.
            (ElementsAreArray(config_src)))
      .WillOnce(Return(sizeof(config_src)));

  // right configuration of SRC.
  codec_lower_->ConfigureSRC(fs);
}  // ConfigureSRC_44100

// Validation test for fs 48000 Hz.
TEST_F(Adau1361LowerTest, ConfigureSRC_48000) {
  const unsigned int fs = 48000;  // Hz
  const uint8_t config_src[] = {0x40, 0x17, 0x00};

  using ::testing::Args;
  using ::testing::ElementsAreArray;
  using ::testing::NotNull;
  using ::testing::Return;

  EXPECT_CALL(
      i2c_,
      WriteBlocking(device_address_,     // Arg 0 : I2C Address.
                    NotNull(),           // Arg 1 : Data buffer address.
                    sizeof(config_src),  // Arg 2 : Data buffer length to send.
                    false))              // Arg 3 : false to stop.
      .With(Args<1,  // parameter position of the array : 0 origin.
                 2>  // parameter position of the size : 0 origin.
            (ElementsAreArray(config_src)))
      .WillOnce(Return(sizeof(config_src)));

  // right configuration of SRC.
  codec_lower_->ConfigureSRC(fs);
}  // ConfigureSRC_48000

// Validation test for fs 88200 Hz.
TEST_F(Adau1361LowerTest, ConfigureSRC_88200) {
  const unsigned int fs = 88200;  // Hz
  const uint8_t config_src[] = {0x40, 0x17, 0x06};

  using ::testing::Args;
  using ::testing::ElementsAreArray;
  using ::testing::NotNull;
  using ::testing::Return;

  EXPECT_CALL(
      i2c_,
      WriteBlocking(device_address_,     // Arg 0 : I2C Address.
                    NotNull(),           // Arg 1 : Data buffer address.
                    sizeof(config_src),  // Arg 2 : Data buffer length to send.
                    false))              // Arg 3 : false to stop.
      .With(Args<1,  // parameter position of the array : 0 origin.
                 2>  // parameter position of the size : 0 origin.
            (ElementsAreArray(config_src)))
      .WillOnce(Return(sizeof(config_src)));

  // right configuration of SRC.
  codec_lower_->ConfigureSRC(fs);
}  // ConfigureSRC_88200

// Validation test for fs 96000 Hz.
TEST_F(Adau1361LowerTest, ConfigureSRC_96000) {
  const unsigned int fs = 96000;  // Hz
  const uint8_t config_src[] = {0x40, 0x17, 0x06};

  using ::testing::Args;
  using ::testing::ElementsAreArray;
  using ::testing::NotNull;
  using ::testing::Return;

  EXPECT_CALL(
      i2c_,
      WriteBlocking(device_address_,     // Arg 0 : I2C Address.
                    NotNull(),           // Arg 1 : Data buffer address.
                    sizeof(config_src),  // Arg 2 : Data buffer length to send.
                    false))              // Arg 3 : false to stop.
      .With(Args<1,  // parameter position of the array : 0 origin.
                 2>  // parameter position of the size : 0 origin.
            (ElementsAreArray(config_src)))
      .WillOnce(Return(sizeof(config_src)));

  // right configuration of SRC.
  codec_lower_->ConfigureSRC(fs);
}  // ConfigureSRC_96000

// -----------------------------------------------------------------
//
//                          ConfigureSRC()
//
// -----------------------------------------------------------------

TEST_F(Adau1361LowerTest, EnableCore) {
  // Set core source to PLL
  const uint8_t config_core[] = {
      0x40, 0x00, 0x0f};  // R0: R0:Source is PLL, 1024fs, core clock enable.

  using ::testing::Args;
  using ::testing::ElementsAreArray;
  using ::testing::NotNull;

  //  we test initialization of core.

  EXPECT_CALL(i2c_,
              WriteBlocking(device_address_,  // Arg 0 : I2C Address.
                            NotNull(),        // Arg 1 : Data buffer address.
                            3,       // Arg 2 : Data buffer length to send.
                            false))  // Arg 3 : false to stop.
      .With(Args<1,  // parameter position of the array : 0 origin.
                 2>  // parameter position of the size : 0 origin.
            (ElementsAreArray(config_core)));
  codec_lower_->EnableCore();
}  // EnableCore

// -----------------------------------------------------------------
//
//                          InitializeRegisters()
//
// -----------------------------------------------------------------

TEST_F(Adau1361LowerTest, InitializeRegisters) {
  // Set registers to initialize state
  const uint8_t config_Adau1361[][3] = {
      // R0,1, are set by init_freq_xxx table
      {0x40, 0x08, 0x00},  // R2: Digital Mic
      {0x40, 0x09, 0x00},  // R3: Rec Power Management
      {0x40, 0x0a, 0x00},  // R4: Rec Mixer Left 0
      {0x40, 0x0b, 0x00},  // R5: Rec Mixer Left 1
      {0x40, 0x0c, 0x00},  // R6: Rec Mixer Right 0
      {0x40, 0x0d, 0x00},  // R7: Rec Mixer Right 1
      {0x40, 0x0e, 0x00},  // R8: Left diff input vol
      {0x40, 0x0f, 0x00},  // R9: Right diff input vol
      {0x40, 0x10, 0x00},  // R10: Rec mic bias
      {0x40, 0x11, 0x00},  // R11: ALC0
      {0x40, 0x12, 0x00},  // R12: ALC1
      {0x40, 0x13, 0x00},  // R13: ALC2
      {0x40, 0x14, 0x00},  // R14: ALC3
      {0x40, 0x15, 0x00},  // R15: Serial Port 0
      {0x40, 0x16, 0x00},  // R16: Serial Port 1
      // R17 is set by config_src_xx table
      {0x40, 0x18, 0x00},  // R18: Converter 1
      {0x40, 0x19, 0x10},  // R19:ADC Control.
      {0x40, 0x1a, 0x00},  // R20: Left digital volume
      {0x40, 0x1b, 0x00},  // R21: Right digital volume
      {0x40, 0x1c, 0x00},  // R22: Play Mixer Left 0
      {0x40, 0x1d, 0x00},  // R23: Play Mixer Left 1
      {0x40, 0x1e, 0x00},  // R24: Play Mixer Right 0
      {0x40, 0x1f, 0x00},  // R25: Play Mixer Right 1
      {0x40, 0x20, 0x00},  // R26: Play L/R mixer left
      {0x40, 0x21, 0x00},  // R27: Play L/R mixer right
      {0x40, 0x22, 0x00},  // R28: Play L/R mixer mono
      {0x40, 0x23, 0x02},  // R29: Play HP Left vol : Mute, Line out mode
      {0x40, 0x24, 0x02},  // R30: Play HP Right vol : Mute, Line out Mode
      {0x40, 0x25, 0x02},  // R31: Line output Left vol : Mute, Line out mode
      {0x40, 0x26, 0x02},  // R32: Line output Right vol : Mute, Line out mode
      {0x40, 0x27, 0x02},  // R33: Play Mono output
      {0x40, 0x28, 0x00},  // R34: Pop surpress
      {0x40, 0x29, 0x00},  // R35: Play Power Management
      {0x40, 0x2a, 0x00},  // R36: DAC Control 0
      {0x40, 0x2b, 0x00},  // R37: DAC Control 1
      {0x40, 0x2c, 0x00},  // R38: DAC control 2
      {0x40, 0x2d, 0xaa},  // R39: Serial port Pad
      {0x40, 0x2f, 0xaa},  // R40: Control Pad 1
      {0x40, 0x30, 0x00},  // R41: Control Pad 2
      {0x40, 0x31, 0x08},  // R42: Jack detect
      {0x40, 0x36, 0x03}   // R67: Dejitter control
  };

  using ::testing::Args;
  using ::testing::ElementsAreArray;
  using ::testing::InSequence;
  using ::testing::NotNull;
  //  we test initialization of core.
  {
    InSequence dummy;

    for (int i = 0; i < sizeof(config_Adau1361) / 3; i++)
      EXPECT_CALL(i2c_,
                  WriteBlocking(device_address_,  // Arg 0 : I2C Address.
                                NotNull(),  // Arg 1 : Data buffer address.
                                3,       // Arg 2 : Data buffer length to send.
                                false))  // Arg 3 : false to stop.
          .With(Args<1,  // parameter position of the array : 0 origin.
                     2>  // parameter position of the size : 0 origin.
                (ElementsAreArray(config_Adau1361[i])));
  }
  codec_lower_->InitializeRegisters();
}  // InitializeRegisters

#if 0
// -----------------------------------------------------------------
//
//                          ConfigureSignalPath()
//
// -----------------------------------------------------------------

TEST_F(Adau1361LowerTest, ConfigureSignalPath) {
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
#endif