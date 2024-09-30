#test_i2cmaster TDD

## Adau1361Lower::Constructor
- [x] Add assertion to the I2C address. 
- [x] Create death test to fail. 
- [x] Make it success. 
- [x] Remove the duplication. 

## Old test

- [x] Create Adau1361LowerTest.ConfigureSingalPath()to fail.
- [x] Make it success.
- [x] Remove the duplication.

- [x] Create Adau1361LowerTest.InitializeRegisters()to fail.
- [x] Make it success.
- [x] Remove the duplication.

- [x] Create Adau1361LowerTest.EnableCore()to fail.
- [x] Make it success.
- [x] Remove the duplication.

- [x] Create Adau1361Lower.ConfigureSRC_96000()to fail.
- [x] Make it success.
- [x] Remove the duplication.

- [x] Create Adau1361Lower.ConfigureSRC_88200()to fail.
- [x] Make it success.-
- [x] Remove the duplication.

- [x] Create Adau1361Lower.ConfigureSRC_48000()to fail.
- [x] Make it success.-
- [x] Remove the duplication.

- [x] Create Adau1361Lower.ConfigureSRC_44100()to fail.
- [x] Make it success.-
- [x] Remove the duplication.

- [x] Create Adau1361Lower.ConfigureSRC_32000()to fail.
- [x] Make it success.-
- [x] Remove the duplication.

- [x] Create Adau1361Lower.ConfigureSRC_24000()to fail.
- [x] Make it success.-
- [x] Remove the duplication.

- [x] Create Adau1361Lower.ConfigureSRC_22050()to fail.
- [x] Make it success.-
- [x] Remove the duplication.

- [x] Create Adau1361LowerDeathTest.ConfigureSRC_wrong_fs()to fail.
- [x] Make it success.-
- [x] Remove the duplication.

- [x] Create Adau1361Lower.ConfigurePll()to fail.
- [x] Make it success.-
- [x] Remove the duplication.

- [x] Create Adau1361Lower.SetHPOutputGain_appropriate_gain()to fail.
- [x] Make it success.-
- [x] Remove the duplication.

- [x] Create Adau1361Lower.SetHPOutputGain_undergain()to fail.
- [x] Make it success.-
- [x] Remove the duplication.

- [x] Create Adau1361Lower.SetHPOutputGain_overgain()to fail.
- [x] Make it success.-
- [x] Remove the duplication.

- [x] Create Adau1361Lower.SetHPOutputGain_mute()to fail.
- [x] Make it success.-
- [x] Remove the duplication.

- [x] Create Adau1361Lower.SetAuxInputGain_mute()to fail.
- [x] Make it success.-
- [x] Remove the duplication.

- [x] Create Adau1361Lower.SetAuxInputGain_undergain()to fail.
- [x] Make it success.-
- [x] Remove the duplication.

- [x] Create Adau1361Lower.SetAuxInputGain_overgain()to fail.
- [x] Make it success.-
- [x] Remove the duplication.

- [x] Create Adau1361Lower.SetAuxInputGain_appropriate_gain()to fail.
- [x] Make it success.-
- [x] Remove the duplication.

- [x] Refactor Adau1361Lower::SetLineItputGain()

- [x] Create Adau1361Lower.SetLineOutputGain_appropriate_gain()to fail.
- [x] Make it success.-
- [x] Remove the duplication.

- [x] Create Adau1361Lower.SetLineOutputGain_undergain()to fail.
- [x] Make it success.-
- [x] Remove the duplication.

- [x] Create Adau1361Lower.SetLineOutputGain_overgain()to fail.
- [x] Make it success.-
- [x] Remove the duplication.

- [x] Create Adau1361Lower.SetLineOutputGain_mute()to fail.
- [x] Make it success.-
- [x] Remove the duplication.



- [x] Create Adau1361Lower.SetLineInputGain_appropriate_gain()to fail.
- [x] Make it success.-
- [x] Remove the duplication.

- [x] Create Adau1361Lower.SetLineInputGain_undergain()to fail.
- [x] Make it success.-
- [x] Remove the duplication.

- [x] Create Adau1361Lower.SetLineInputGain_overgain()to fail.
- [x] Make it success.-
- [x] Remove the duplication.

- [x] Create Adau1361Lower.SetLineInputGain_mute()to fail.
- [x] Make it success.-
- [x] Remove the duplication.


- [x] Create Adau1361Lower.DisablePLL()to fail.
- [x] Make it success.-
- [x] Remove the duplication.

- [x] Create Adau1361Lower.InitializeCore()to fail.
- [x] Make it success.-
- [x] Remove the duplication.

- [x] Create Adau1361Lower.WaitPll()to fail.
- [x] Make it success.-
- [s] Remove the duplication.

- [x] Create Adau1361Lower.IsI2CDeviceExisting()to fail.
- [x] Make it success.-
- [x] Remove the duplication.

- [x] Create Adau1361Lower.SendCommandTable()to fail.
- [x] Make it success.-
- [x] Remove the duplication.

- [x] Create Adau1361Lower.SendCommand()to fail.
- [x] Make it success.-
- [x] Remove the duplication.
