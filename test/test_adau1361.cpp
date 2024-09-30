#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "adau1361.hpp"
#include "i2cmasterinterface.hpp"
#include "umbadau1361lower.hpp"

class Adau1361Test : public ::testing::Test {
 protected:
  virtual void SetUp() {
    codec_lower_ = new ::rpp_driver::MockAdau1361Lower(i2c_);
    codec_ = new ::rpp_driver::Adau1361(fs_, master_clk_, *codec_lower_);
  }

  virtual void TearDown() {
    delete codec_;
    delete codec_lower_;
  }

  uint fs_ = 48000;
  uint master_clk_ = 12000000;
  ::rpp_driver::MockI2cMasterInterface i2c_;
  ::rpp_driver::MockAdau1361Lower *codec_lower_;
  ::rpp_driver::Adau1361 *codec_;
};

// -----------------------------------------------------------------
//
//                          SetGain()
//
// -----------------------------------------------------------------

TEST_F(Adau1361Test, SetGain_LineInput) {
  using ::testing::_;
  using ::testing::InSequence;
  // Check required function call order.
  {
    InSequence dummy;

    EXPECT_CALL(*codec_lower_, SetLineInputGain(2.0, 3.0, true));
    EXPECT_CALL(*codec_lower_, SetLineInputGain(2.0, 3.0, false));
    EXPECT_CALL(*codec_lower_, SetLineInputGain(3.0, 2.0, false));
  }
  // Check illegal func call is not existing.
  EXPECT_CALL(*codec_lower_, SetLineOutputGain(_, _, _)).Times(0);
  EXPECT_CALL(*codec_lower_, SetAuxInputGain(_, _, _)).Times(0);
  EXPECT_CALL(*codec_lower_, SetHpOutputGain(_, _, _)).Times(0);

  // Test run.
  codec_->SetGain(::rpp_driver::Adau1361::LineInput, 2.0, 3.0);
  codec_->Mute(::rpp_driver::Adau1361::LineInput, false);
  codec_->SetGain(::rpp_driver::Adau1361::LineInput, 3.0, 2.0);

}  // SetGain_LineInput

TEST_F(Adau1361Test, SetGain_LineOutput) {
  using ::testing::_;
  using ::testing::InSequence;
  {
    InSequence dummy;

    EXPECT_CALL(*codec_lower_, SetLineOutputGain(2.0, 3.0, true));
    EXPECT_CALL(*codec_lower_, SetLineOutputGain(2.0, 3.0, false));
    EXPECT_CALL(*codec_lower_, SetLineOutputGain(3.0, 2.0, false));
  }
  // Check illegal func call is not existing.
  EXPECT_CALL(*codec_lower_, SetLineInputGain(_, _, _)).Times(0);
  EXPECT_CALL(*codec_lower_, SetAuxInputGain(_, _, _)).Times(0);
  EXPECT_CALL(*codec_lower_, SetHpOutputGain(_, _, _)).Times(0);

  // Test run
  codec_->SetGain(::rpp_driver::Adau1361::LineOutput, 2.0, 3.0);
  codec_->Mute(::rpp_driver::Adau1361::LineOutput, false);
  codec_->SetGain(::rpp_driver::Adau1361::LineOutput, 3.0, 2.0);
}  // SetGain_LineOutput

TEST_F(Adau1361Test, SetGain_AuxInput) {
  using ::testing::_;
  using ::testing::InSequence;
  {
    InSequence dummy;

    EXPECT_CALL(*codec_lower_, SetAuxInputGain(2.0, 3.0, true));
    EXPECT_CALL(*codec_lower_, SetAuxInputGain(2.0, 3.0, false));
    EXPECT_CALL(*codec_lower_, SetAuxInputGain(3.0, 2.0, false));
  }
  // Check illegal func call is not existing.
  EXPECT_CALL(*codec_lower_, SetLineInputGain(_, _, _)).Times(0);
  EXPECT_CALL(*codec_lower_, SetLineOutputGain(_, _, _)).Times(0);
  EXPECT_CALL(*codec_lower_, SetHpOutputGain(_, _, _)).Times(0);

  // Test run
  codec_->SetGain(::rpp_driver::Adau1361::AuxInput, 2.0, 3.0);
  codec_->Mute(::rpp_driver::Adau1361::AuxInput, false);
  codec_->SetGain(::rpp_driver::Adau1361::AuxInput, 3.0, 2.0);
}  // SetGain_AuxInput

TEST_F(Adau1361Test, SetGain_HeadhphoneOutput) {
  using ::testing::_;
  using ::testing::InSequence;
  {
    InSequence dummy;

    EXPECT_CALL(*codec_lower_, SetHpOutputGain(2.0, 3.0, true));
    EXPECT_CALL(*codec_lower_, SetHpOutputGain(2.0, 3.0, false));
    EXPECT_CALL(*codec_lower_, SetHpOutputGain(3.0, 2.0, false));
  }
  // Check illegal func call is not existing.
  EXPECT_CALL(*codec_lower_, SetLineInputGain(_, _, _)).Times(0);
  EXPECT_CALL(*codec_lower_, SetLineOutputGain(_, _, _)).Times(0);
  EXPECT_CALL(*codec_lower_, SetAuxInputGain(_, _, _)).Times(0);

  // Test run
  codec_->SetGain(::rpp_driver::Adau1361::HeadphoneOutput, 2.0, 3.0);
  codec_->Mute(::rpp_driver::Adau1361::HeadphoneOutput, false);
  codec_->SetGain(::rpp_driver::Adau1361::HeadphoneOutput, 3.0, 2.0);
}  // SetGain_HeadhphoneOutput

// -----------------------------------------------------------------
//
//                          Mute()
//
// -----------------------------------------------------------------

TEST_F(Adau1361Test, Mute_LineInput) {
  using ::testing::_;
  using ::testing::InSequence;
  // Check required function call order.
  {
    InSequence dummy;

    EXPECT_CALL(*codec_lower_, SetLineInputGain(0.0, 0.0, false));
    EXPECT_CALL(*codec_lower_, SetLineInputGain(2.0, 3.0, false));
    EXPECT_CALL(*codec_lower_, SetLineInputGain(2.0, 3.0, true));
  }
  // Check illegal func call is not existing.
  EXPECT_CALL(*codec_lower_, SetLineOutputGain(_, _, _)).Times(0);
  EXPECT_CALL(*codec_lower_, SetAuxInputGain(_, _, _)).Times(0);
  EXPECT_CALL(*codec_lower_, SetHpOutputGain(_, _, _)).Times(0);

  // Test run.
  codec_->Mute(::rpp_driver::Adau1361::LineInput, false);
  codec_->SetGain(::rpp_driver::Adau1361::LineInput, 2.0, 3.0);
  codec_->Mute(::rpp_driver::Adau1361::LineInput, true);

}  // Mute_LineInput

TEST_F(Adau1361Test, Mute_LineOutput) {
  using ::testing::_;
  using ::testing::InSequence;
  {
    InSequence dummy;

    EXPECT_CALL(*codec_lower_, SetLineOutputGain(0.0, 0.0, true));
    EXPECT_CALL(*codec_lower_, SetLineOutputGain(2.0, 3.0, true));
    EXPECT_CALL(*codec_lower_, SetLineOutputGain(2.0, 3.0, false));
  }
  // Check illegal func call is not existing.
  EXPECT_CALL(*codec_lower_, SetLineInputGain(_, _, _)).Times(0);
  EXPECT_CALL(*codec_lower_, SetAuxInputGain(_, _, _)).Times(0);
  EXPECT_CALL(*codec_lower_, SetHpOutputGain(_, _, _)).Times(0);

  // Test run
  codec_->Mute(::rpp_driver::Adau1361::LineOutput, true);
  codec_->SetGain(::rpp_driver::Adau1361::LineOutput, 2.0, 3.0);
  codec_->Mute(::rpp_driver::Adau1361::LineOutput, false);
}  // Mute_LineOutput

TEST_F(Adau1361Test, Mute_AuxInput) {
  using ::testing::_;
  using ::testing::InSequence;
  {
    InSequence dummy;

    EXPECT_CALL(*codec_lower_, SetAuxInputGain(0.0, 0.0, false));
    EXPECT_CALL(*codec_lower_, SetAuxInputGain(2.0, 3.0, false));
    EXPECT_CALL(*codec_lower_, SetAuxInputGain(2.0, 3.0, true));
  }
  // Check illegal func call is not existing.
  EXPECT_CALL(*codec_lower_, SetLineInputGain(_, _, _)).Times(0);
  EXPECT_CALL(*codec_lower_, SetLineOutputGain(_, _, _)).Times(0);
  EXPECT_CALL(*codec_lower_, SetHpOutputGain(_, _, _)).Times(0);

  // Test run
  codec_->Mute(::rpp_driver::Adau1361::AuxInput, false);
  codec_->SetGain(::rpp_driver::Adau1361::AuxInput, 2.0, 3.0);
  codec_->Mute(::rpp_driver::Adau1361::AuxInput);
}  // Mute_AuxInput

TEST_F(Adau1361Test, Mute_HeadhphoneOutput) {
  using ::testing::_;
  using ::testing::InSequence;
  {
    InSequence dummy;

    EXPECT_CALL(*codec_lower_, SetHpOutputGain(0.0, 0.0, false));
    EXPECT_CALL(*codec_lower_, SetHpOutputGain(2.0, 3.0, false));
    EXPECT_CALL(*codec_lower_, SetHpOutputGain(2.0, 3.0, true));
  }
  // Check illegal func call is not existing.
  EXPECT_CALL(*codec_lower_, SetLineInputGain(_, _, _)).Times(0);
  EXPECT_CALL(*codec_lower_, SetLineOutputGain(_, _, _)).Times(0);
  EXPECT_CALL(*codec_lower_, SetAuxInputGain(_, _, _)).Times(0);

  // Test run
  codec_->Mute(::rpp_driver::Adau1361::HeadphoneOutput, false);
  codec_->SetGain(::rpp_driver::Adau1361::HeadphoneOutput, 2.0, 3.0);
  codec_->Mute(::rpp_driver::Adau1361::HeadphoneOutput, true);
}  // Mute_HeadhphoneOutput

// -----------------------------------------------------------------
//
//                          Start()
//
// -----------------------------------------------------------------

TEST_F(Adau1361Test, Start) {
  using ::testing::_;
  using ::testing::InSequence;
  using ::testing::Return;
  // Check required function call order.
  {
    InSequence dummy;
#ifndef NDEBUG
    // This call is inside assertion. So, will not called with Release build.
    EXPECT_CALL(*codec_lower_, IsI2CDeviceExisting()).WillOnce(Return(true));
#else
    EXPECT_CALL(*codec_lower_, IsI2CDeviceExisting()).Times(0);
#endif
    EXPECT_CALL(*codec_lower_, InitializeCore());
    EXPECT_CALL(*codec_lower_, DisablePLL());
    EXPECT_CALL(*codec_lower_, ConfigurePll(fs_, master_clk_));
    EXPECT_CALL(*codec_lower_, WaitPllLock());
    EXPECT_CALL(*codec_lower_, EnableCore());
    EXPECT_CALL(*codec_lower_, ConfigureSRC(fs_));
    EXPECT_CALL(*codec_lower_, InitializeRegisters());
    EXPECT_CALL(*codec_lower_, ConfigureSignalPath());
  }
  // Check illegal func call is not existing.
  EXPECT_CALL(*codec_lower_, SetLineOutputGain(_, _, _)).Times(0);
  EXPECT_CALL(*codec_lower_, SetAuxInputGain(_, _, _)).Times(0);
  EXPECT_CALL(*codec_lower_, SetHpOutputGain(_, _, _)).Times(0);
  EXPECT_CALL(*codec_lower_, SetLineInputGain(_, _, _)).Times(0);

  // Test run.
  codec_->Start();

}  // Start
