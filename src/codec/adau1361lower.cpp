#include "adau1361lower.hpp"

#include <assert.h>

#include <algorithm>

rpp_driver::Adau1361Lower::Adau1361Lower(
    ::rpp_driver::I2cMasterInterface& controller, unsigned int i2c_device_addr)
    : i2c_(controller), device_addr_(i2c_device_addr) {
  assert((0x3C > device_addr_) &&
         "ADAU1361 I2C Address must be lower than 0x3C.");
  assert((device_addr_ > 0x37) &&
         "ADAU1361 I2C Address must be higher than 0x37.");
}

/*
 *  Send single command
 *  table : command table :
 *  size : size of table.
 */
void ::rpp_driver::Adau1361Lower::SendCommand(const uint8_t command[],
                                              int size) {
  /*
   * Send the given table to the I2C slave device at device_addr
   */
  i2c_.WriteBlocking(device_addr_, command, size, false);
}

/*
 * Send entire command table
 */

void ::rpp_driver::Adau1361Lower::SendCommandTable(const uint8_t table[][3],
                                                   int rows) {
  /*
   * Send all rows of command table.
   */
  for (int i = 0; i < rows; i++) SendCommand(table[i], 3);
}

bool ::rpp_driver::Adau1361Lower::IsI2CDeviceExisting() {
  return (i2c_.IsDeviceExisting(device_addr_));
}

void ::rpp_driver::Adau1361Lower::InitializeCore() {
  // Core clock setting
  const uint8_t init_core[] = {
      0x40, 0x00, 0x00};  // R0:Clock control. Core clock disabled. PLL off.

  SendCommand(init_core, sizeof(init_core));
}

void ::rpp_driver::Adau1361Lower::DisablePLL() {
  // PLL Disable.
  // R1 : Must write 6 byte at once.
  const uint8_t disable_pll[] = {0x40, 0x02, 0x00, 0xFD,
                                 0x00, 0x0C, 0x10, 0x00};

  SendCommand(disable_pll, sizeof(disable_pll));
}

// loop while the PLL is not locked.
void ::rpp_driver::Adau1361Lower::WaitPllLock(void) {
  const uint8_t lock_status_address[] = {0x40, 0x02};  // R1 : 6 byte register.

  uint8_t status[6];

  int count = 0;

  while (true) {
    // Obtain PLL status
    // Write the lock status address and then, read the status by
    // repeated-stard.
    i2c_.WriteBlocking(device_addr_, lock_status_address,
                       sizeof(lock_status_address),
                       true);  // true for repeated start.
    i2c_.ReadBlocking(device_addr_, status, sizeof(status), false);

    // Check byte 5 of the control registers.
    // If bit 1 is 1, locked. If it is 0, unlocked.
    // if locked, terminate the loop.
    if (status[5] & (1 << 1)) break;
  }
}

// Configure PLL and start. Then, initiate the core and set the CODEC Fs.
void ::rpp_driver::Adau1361Lower::ConfigurePll(unsigned int fs,
                                               unsigned int master_clock) {
  assert((fs == 24000 || fs == 32000 || fs == 48000 || fs == 96000 ||
          fs == 22050 || fs == 44100 || fs == 88200) &&
         "Bad Fs");

  if (fs == 24000 || fs == 32000 || fs == 48000 || fs == 96000) {
    // Configure the PLL. Target PLL out is 49.152MHz = 1024xfs
    // Regarding X, R, M, N, check ADAU1361 Datasheet register R1.
    // In the following code, the config_pll[0] and config_pll[1] contains
    // regsiter address of R1.

    switch (master_clock) {
      case 8000000: {
        /**
         * X : 1
         * R : 6
         * M : 125
         * N : 18
         * PLL = ( MCLK / X ) * ( R + N/M ) = 49.152MHz
         */
        uint8_t config_pll[] = {0x40, 0x02, 0x00, 0x7D, 0x00, 0x12, 0x31, 0x01};

        SendCommand(config_pll, sizeof(config_pll));
        break;
      }

      case 12000000: {
        /**
         * X : 1
         * R : 4
         * M : 125
         * N : 12
         * PLL = ( MCLK / X ) * ( R + N/M ) = 49.152MHz
         */
        uint8_t config_pll[] = {0x40, 0x02, 0x00, 0x7D, 0x00, 0x0C, 0x21, 0x01};

        SendCommand(config_pll, sizeof(config_pll));
        break;
      }

      case 13000000: {
        /**
         * X : 1
         * R : 3
         * M : 1625
         * N : 1269
         * PLL = ( MCLK / X ) * ( R + N/M ) = 49.152MHz
         */
        uint8_t config_pll[] = {0x40, 0x02, 0x06, 0x59, 0x04, 0xF5, 0x19, 0x01};

        SendCommand(config_pll, sizeof(config_pll));
        break;
      }

      case 14400000: {
        /**
         * X : 2
         * R : 6
         * M : 75
         * N : 62
         * PLL = ( MCLK / X ) * ( R + N/M ) = 49.152MHz
         */
        uint8_t config_pll[] = {0x40, 0x02, 0x00, 0x4B, 0x00, 0x3E, 0x33, 0x01};

        SendCommand(config_pll, sizeof(config_pll));
        break;
      }

      case 19200000: {
        /**
         * X : 2
         * R : 4
         * M : 25
         * N : 3
         * PLL = ( MCLK / X ) * ( R + N/M ) = 49.152MHz
         */
        uint8_t config_pll[] = {0x40, 0x02, 0x00, 0x19, 0x00, 0x03, 0x2B, 0x01};

        SendCommand(config_pll, sizeof(config_pll));
        break;
      }

      case 19680000: {
        /**
         * X : 2
         * R : 4
         * M : 205
         * N : 204
         * PLL = ( MCLK / X ) * ( R + N/M ) = 49.152MHz
         */
        uint8_t config_pll[] = {0x40, 0x02, 0x00, 0xCD, 0x00, 0xCC, 0x23, 0x01};

        SendCommand(config_pll, sizeof(config_pll));
        break;
      }

      case 19800000: {
        /**
         * X : 2
         * R : 4
         * M : 825
         * N : 796
         * PLL = ( MCLK / X ) * ( R + N/M ) = 49.152MHz
         */
        uint8_t config_pll[] = {0x40, 0x02, 0x03, 0x39, 0x03, 0x1C, 0x23, 0x01};

        SendCommand(config_pll, sizeof(config_pll));
        break;
      }

      case 24000000: {
        /**
         * X : 2
         * R : 4
         * M : 125
         * N : 12
         * PLL = ( MCLK / X ) * ( R + N/M ) = 49.152MHz
         */
        uint8_t config_pll[] = {0x40, 0x02, 0x00, 0x7D, 0x00, 0x0C, 0x23, 0x01};

        SendCommand(config_pll, sizeof(config_pll));
        break;
      }

      case 26000000: {
        /**
         * X : 2
         * R : 3
         * M : 1625
         * N : 1269
         * PLL = ( MCLK / X ) * ( R + N/M ) = 49.152MHz
         */
        uint8_t config_pll[] = {0x40, 0x02, 0x06, 0x59, 0x04, 0xF5, 0x1B, 0x01};

        SendCommand(config_pll, sizeof(config_pll));
        break;
      }

      case 27000000: {
        /**
         * X : 2
         * R : 3
         * M : 1125
         * N : 721
         * PLL = ( MCLK / X ) * ( R + N/M ) = 49.152MHz
         */
        uint8_t config_pll[] = {0x40, 0x02, 0x04, 0x65, 0x02, 0xD1, 0x1B, 0x01};

        SendCommand(config_pll, sizeof(config_pll));
        break;
      }

      case 12288000: {
        /**
         * X : 1
         * R : 4
         * M : Don't care
         * N : Don't care
         * PLL = ( MCLK / X ) * ( R + N/M ) = 49.152MHz
         */
        uint8_t config_pll[] = {0x40, 0x02, 0x04, 0x65, 0x02, 0xD1, 0x20, 0x01};

        SendCommand(config_pll, sizeof(config_pll));
        break;
      }

      case 24576000: {
        /**
         * X : 1
         * R : 2
         * M : Don't care
         * N : Don't care
         * PLL = ( MCLK / X ) * ( R + N/M ) = 49.152MHz
         */
        uint8_t config_pll[] = {0x40, 0x02, 0x04, 0x65, 0x02, 0xD1, 0x10, 0x01};

        SendCommand(config_pll, sizeof(config_pll));
        break;
      }

      default:
        assert(false && "Wrong Master Clock with Fs 48kHz Series");
    }

  } else if (fs == 22050 || fs == 44100 || fs == 88200) {
    // Configure the PLL. Target PLL out is 45.1584MHz = 1024xfs

    switch (master_clock) {
      case 8000000: {
        /**
         * X : 1
         * R : 5
         * M : 625
         * N : 403
         * PLL = ( MCLK / X ) * ( R + N/M ) = 45.1584MHz
         */
        uint8_t config_pll[] = {0x40, 0x02, 0x02, 0x71, 0x01, 0x93, 0x29, 0x01};

        SendCommand(config_pll, sizeof(config_pll));
        break;
      }

      case 12000000: {
        /**
         * X : 1
         * R : 3
         * M : 625
         * N : 477
         * PLL = ( MCLK / X ) * ( R + N/M ) = 45.1584MHz
         */
        uint8_t config_pll[] = {0x40, 0x02, 0x02, 0x71, 0x01, 0xDD, 0x19, 0x01};

        SendCommand(config_pll, sizeof(config_pll));
        break;
      }

      case 13000000: {
        /**
         * X : 1
         * R : 3
         * M : 8125
         * N : 3849
         * PLL = ( MCLK / X ) * ( R + N/M ) = 45.1584MHz
         */
        uint8_t config_pll[] = {0x40, 0x02, 0x1F, 0xBD, 0x0F, 0x09, 0x19, 0x01};

        SendCommand(config_pll, sizeof(config_pll));
        break;
      }

      case 14400000: {
        /**
         * X : 2
         * R : 6
         * M : 125
         * N : 34
         * PLL = ( MCLK / X ) * ( R + N/M ) = 45.1584MHz
         */
        uint8_t config_pll[] = {0x40, 0x02, 0x00, 0x7D, 0x00, 0x22, 0x33, 0x01};

        SendCommand(config_pll, sizeof(config_pll));
        break;
      }

      case 19200000: {
        /**
         * X : 2
         * R : 4
         * M : 125
         * N : 88
         * PLL = ( MCLK / X ) * ( R + N/M ) = 45.1584MHz
         */
        uint8_t config_pll[] = {0x40, 0x02, 0x00, 0x7D, 0x00, 0x58, 0x23, 0x01};

        SendCommand(config_pll, sizeof(config_pll));
        break;
      }

      case 19680000: {
        /**
         * X : 2
         * R : 4
         * M : 1025
         * N : 604
         * PLL = ( MCLK / X ) * ( R + N/M ) = 45.1584MHz
         */
        uint8_t config_pll[] = {0x40, 0x02, 0x04, 0x01, 0x02, 0x5C, 0x23, 0x01};

        SendCommand(config_pll, sizeof(config_pll));
        break;
      }

      case 19800000: {
        /**
         * X : 2
         * R : 4
         * M : 1375
         * N : 772
         * PLL = ( MCLK / X ) * ( R + N/M ) = 45.1584MHz
         */
        uint8_t config_pll[] = {0x40, 0x02, 0x05, 0x5F, 0x03, 0x04, 0x23, 0x01};

        SendCommand(config_pll, sizeof(config_pll));
        break;
      }

      case 24000000: {
        /**
         * X : 2
         * R : 3
         * M : 625
         * N : 477
         * PLL = ( MCLK / X ) * ( R + N/M ) = 45.1584MHz
         */
        uint8_t config_pll[] = {0x40, 0x02, 0x02, 0x71, 0x01, 0xDD, 0x1B, 0x01};

        SendCommand(config_pll, sizeof(config_pll));
        break;
      }

      case 26000000: {
        /**
         * X : 1
         * R : 3
         * M : 8125
         * N : 3849
         * PLL = ( MCLK / X ) * ( R + N/M ) = 45.1584MHz
         */
        uint8_t config_pll[] = {0x40, 0x02, 0x1F, 0xBD, 0x0F, 0x09, 0x1B, 0x01};

        SendCommand(config_pll, sizeof(config_pll));
        break;
      }

      case 27000000: {
        /**
         * X : 2
         * R : 3
         * M : 1875
         * N : 647
         * PLL = ( MCLK / X ) * ( R + N/M ) = 45.1584MHz
         */
        uint8_t config_pll[] = {0x40, 0x02, 0x07, 0x53, 0x02, 0x87, 0x1B, 0x01};

        SendCommand(config_pll, sizeof(config_pll));
        break;
      }

      case 12288000: {
        /**
         * X : 1
         * R : 3
         * M : 1000
         * N : 675
         * PLL = ( MCLK / X ) * ( R + N/M ) = 45.1584MHz
         */
        uint8_t config_pll[] = {0x40, 0x02, 0x03, 0xE8, 0x02, 0xA3, 0x19, 0x01};

        SendCommand(config_pll, sizeof(config_pll));
        break;
      }

      case 24576000: {
        /**
         * X : 2
         * R : 3
         * M : 1000
         * N : 675
         * PLL = ( MCLK / X ) * ( R + N/M ) = 45.1584MHz
         */
        uint8_t config_pll[] = {0x40, 0x02, 0x03, 0xE8, 0x02, 0xA3, 0x1B, 0x01};

        SendCommand(config_pll, sizeof(config_pll));
        break;
      }

      default:
        assert(false && "Wrong Master Clock with Fs 44.1kHz Series");
    }

  } else {  // Fs must be checked at the top of this routine.
    assert(false && "Wrong parameter validation");
  }

}  // ConfigurePlll

// Enable core.
void ::rpp_driver::Adau1361Lower::EnableCore() {
  // Set core source to PLL
  const uint8_t config_core[] = {
      0x40, 0x00, 0x0f};  // R0:Source is PLL, 1024fs, core clock enable.

  SendCommand(config_core, sizeof(config_core));
}

void ::rpp_driver::Adau1361Lower::InitializeRegisters() {
  // Set all registers.
  // Set non clock registers as default
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
      // R17 is set by ConfigSrc(). Do not touch.
      {0x40, 0x18, 0x00},  // R18: Converter 1
      {0x40, 0x19, 0x10},  // R19:ADC Control.
      {0x40, 0x1a, 0x00},  // R20: Left digital volume
      {0x40, 0x1b, 0x00},  // R21: Rignt digital volume
      {0x40, 0x1c, 0x00},  // R22: Play Mixer Left 0
      {0x40, 0x1d, 0x00},  // R23: Play Mixer Left 1
      {0x40, 0x1e, 0x00},  // R24: Play Mixer Right 0
      {0x40, 0x1f, 0x00},  // R25: Play Mixer Right 1
      {0x40, 0x20, 0x00},  // R26: Play L/R mixer left
      {0x40, 0x21, 0x00},  // R27: Play L/R mixer right
      {0x40, 0x22, 0x00},  // R28: Play L/R mixer monot
      {0x40, 0x23, 0x02},  // R29: Play HP Left vol : Mute, Line out mode
      {0x40, 0x24, 0x02},  // R30: Play HP Right vol : Mute, Line out mode
      {0x40, 0x25, 0x02},  // R31: Line output Left vol : Mute, Line out mode
      {0x40, 0x26, 0x02},  // R32: Line output Right vol : Mute, Line out mode
      {0x40, 0x27, 0x02},  // R33: Play Mono output
      {0x40, 0x28, 0x00},  // R34: Pop surpress
      {0x40, 0x29, 0x00},  // R35: Play Power Management
      {0x40, 0x2a, 0x00},  // R36: DAC Control 0
      {0x40, 0x2b, 0x00},  // R37: DAC Control 1
      {0x40, 0x2c, 0x00},  // R38: DAC control 2
      {0x40, 0x2d, 0xaa},  // R39: Seial port Pad
      {0x40, 0x2f, 0xaa},  // R40: Control Pad 1
      {0x40, 0x30, 0x00},  // R41: Control Pad 2
      {0x40, 0x31, 0x08},  // R42: Jack detect
      {0x40, 0x36, 0x03}   // R67: Dejitter control
  };

  SendCommandTable(
      config_Adau1361,
      sizeof(config_Adau1361) / 3);  // init Adau1361 as default state.
}

// Set the converter clock.
void ::rpp_driver::Adau1361Lower::ConfigureSRC(unsigned int fs) {
  assert((fs == 24000 || fs == 32000 || fs == 48000 || fs == 96000 ||
          fs == 22050 || fs == 44100 || fs == 88200) &&
         "Bad Fs");

  switch (fs) {
    case 22050:
    case 24000: {
      // R17: Converter 0, SRC = 1/2 * core clock
      const uint8_t config_src[] = {0x40, 0x17, 0x04};

      SendCommand(config_src, sizeof(config_src));
      break;
    }
    case 32000: {
      // R17: Converter 0, SRC = 2/3 * core clock
      const uint8_t config_src[] = {0x40, 0x17, 0x05};

      SendCommand(config_src, sizeof(config_src));
      break;
    }
    case 44100:
    case 48000: {
      // R17: Converter 0, SRC = 1 * core clock
      const uint8_t config_src[] = {0x40, 0x17, 0x00};

      SendCommand(config_src, sizeof(config_src));
      break;
    }
    case 88200:
    case 96000: {
      // R17: Converter 0, SRC = 2 * core clock
      const uint8_t config_src[] = {0x40, 0x17, 0x06};

      SendCommand(config_src, sizeof(config_src));
      break;
    }
    default:
      assert(false);
  }
}

#define DATA 2 /* data payload of register */
#define ADDL 1 /* lower address of register */
#define ADDH 0 /* upper address of register */

#define SET_INPUT_GAIN(x, mute) ((mute) ? 0x00 : (x << 1))
/*
 * This function assumes the single-end input. The gain control is LINNG.
 * See Figure 31 "Record Signal Path" in the ADAU1361 data sheet
 *
 */
void ::rpp_driver::Adau1361Lower::SetLineInputGain(float left_gain,
                                                   float right_gain,
                                                   bool mute) {
  uint8_t txbuf[3];
  uint8_t rxbuf[1];
  int left, right;

  // set left gain
  left = std::max(static_cast<int>(left_gain), -12);
  left = std::min(left, 6);
  left = (left + 15) / 3;  // See table 31 LINNG

  // set right gain
  right = std::max(static_cast<int>(right_gain), -12);
  right = std::min(right, 6);
  right = (right + 15) / 3;  // See table 31 LINNG

  /*
   *  *************** Read Modify light the Left Channel Gain
   * *********************
   */
  // R4 : Record Mixer Left
  txbuf[ADDH] = 0x40;  // Upper address of register
  txbuf[ADDL] = 0x0a;

  txbuf[DATA] =
      0x01 | SET_INPUT_GAIN(left, mute);  // mixer enable and set mute and gain.

  // Set the R4.
  SendCommand(txbuf, 3);

  /*
   *  *************** Read Modify light the Right Channel Gain
   * *********************
   */
  // R6 : Record Mixer 2
  txbuf[ADDH] = 0x40;  // Upper address of register
  txbuf[ADDL] = 0x0c;

  // Create a register value
  txbuf[DATA] = 0x01 | SET_INPUT_GAIN(
                           right, mute);  // mixer enable and set mute and gain.

  // Set the R4.
  SendCommand(txbuf, 3);
}

#define SET_AUX_GAIN(x, mute) ((mute) ? 0x00 : x)

/*
 * This function assumes the input is the single end. Then,
 */
void ::rpp_driver::Adau1361Lower::SetAuxInputGain(float left_gain,
                                                  float right_gain, bool mute) {
  uint8_t txbuf[3];
  uint8_t rxbuf[1];
  int left, right;

  // set left gain LDBOOST is muted.
  left = std::max(static_cast<int>(left_gain), -12);
  left = std::min(left, 6);
  left = (left + 15) / 3;  // See table 32 MX1AUGXG
  // set right gain. LDBOOST is muted.
  right = std::max(static_cast<int>(right_gain), -12);
  right = std::min(right, 6);
  right = (right + 15) / 3;  // See table 34 MX1AUGXG

  /*
   *  *************** Read Modify light the Left Channel Gain
   * *********************
   */
  // R5 : Record Mixer Left Control 1
  txbuf[ADDH] = 0x40;  // Upper address of register
  txbuf[ADDL] = 0x0b;

  // Create a register value
  txbuf[DATA] = SET_AUX_GAIN(left, mute);  // Negative input only
  // Set the R5.
  SendCommand(txbuf, 3);

  /*
   *  *************** Read Modify light the Right Channel Gain
   * *********************
   */
  // R7 : Record Mixer Right Control 1
  txbuf[ADDH] = 0x40;  // Upper address of register
  txbuf[ADDL] = 0x0d;

  // Create a register value
  txbuf[DATA] = SET_AUX_GAIN(right, mute);  // Negative input only
  // Set the R7.
  SendCommand(txbuf, 3);
}

#define SET_LO_GAIN(x, unmute, headphone) \
  ((x << 2) | (unmute << 1) | (headphone))

// Read modify right the R31 and R32
void ::rpp_driver::Adau1361Lower::SetLineOutputGain(float left_gain,
                                                    float right_gain,
                                                    bool mute) {
  uint8_t txbuf[3];
  int left, right;

  // set 0 if mute, set 1 if unmute;
  int unmute_flag = mute ? 0 : 1;

  // Calc left gain setting.
  // LOUTVOL[5:0] = 0  ; -57dB
  // LOUTVOL[5:0] = 63 ; 6dB
  left = left_gain + 57;
  left = std::max(left, 0);
  left = std::min(left, 63);

  // Calc left gain setting.
  // ROUTVOL[5:0] = 0  ; -57dB
  // ROUTVOL[5:0] = 63 ; 6dB
  right = right_gain + 57;
  right = std::max(right, 0);
  right = std::min(right, 63);

  /*
   *  *************** Read Modify light the Left Channel Gain
   * *********************
   */
  txbuf[ADDH] = 0x40;  // Upper address of register
  txbuf[ADDL] = 0x25;  // R31: LOUTVOL

  // Set line out of Left
  txbuf[DATA] = SET_LO_GAIN(left,        /* GAIN */
                            unmute_flag, /* UNMUTE */
                            0);          /* LOMODE of R31 : 0 for Line out*/
  // Set LOUTVOL : R31
  SendCommand(txbuf, 3);

  /*
   *  *************** Read Modify light the Right Channel Gain
   * *********************
   */
  txbuf[ADDH] = 0x40;  // Upper address of register
  txbuf[ADDL] = 0x26;  // R32: ROUTVOL

  txbuf[DATA] = SET_LO_GAIN(right,       /* GAIN */
                            unmute_flag, /* UNMUTE */
                            0);          /* ROMODE of R32 : 0 for Line out*/
  // Set ROUTVOL : R32
  SendCommand(txbuf, 3);
}

#define SET_HP_GAIN(x, unmute, headphone) \
  ((x << 2) | (unmute << 1) | (headphone))

// Read modify right the R29 and R30

void ::rpp_driver::Adau1361Lower::SetHpOutputGain(float left_gain,
                                                  float right_gain, bool mute) {
  uint8_t txbuf[3];
  int left, right;

  // set 0 if mute, set 1 if unmute;
  int unmute_flag = mute ? 0 : 1;

  // Calc left gain setting.
  // LHPVOL[5:0] = 0  ; -57dB
  // LHPVOL[5:0] = 63 ; 6dB
  left = left_gain + 57;
  left = std::max(left, 0);
  left = std::min(left, 63);

  // Calc left gain setting.
  // RHPVOL[5:0] = 0  ; -57dB
  // RHPVOL[5:0] = 63 ; 6dB
  right = right_gain + 57;
  right = std::max(right, 0);
  right = std::min(right, 63);

  /*
   *  *************** Read Modify light the Left Channel Gain
   * *********************
   */
  // R29: LHPVOL
  txbuf[ADDH] = 0x40;  // Upper address of register
  txbuf[ADDL] = 0x23;

  // HP left out control data
  txbuf[DATA] = SET_HP_GAIN(left,        /* GAIN */
                            unmute_flag, /* UNMUTE */
                            1);          /* HPEN of R29*/
  // SET LHPVOL : R29
  SendCommand(txbuf, 3);

  /*
   *  *************** Read Modify light the Left Channel Gain
   * *********************
   */
  // R30 : RHPVOL address
  txbuf[ADDH] = 0x40;  // Upper address of register
  txbuf[ADDL] = 0x24;

  // HP right out control data
  txbuf[DATA] = SET_HP_GAIN(right,       /* GAIN */
                            unmute_flag, /* UNMUTE */
                            1);          /* HPMODE of R30*/
  // SET RHPVOL : R30
  SendCommand(txbuf, 3);
}
