#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <random>

#include "i2sslaveduplex.hpp"
#include "sdkwrapper.hpp"

class I2sSlaveDuplexTest : public ::testing::Test {
 protected:
  std::random_device rng;
  ::rpp_driver::MockSdkWrapper sdk_;
  PIO pio_ = rng();
  uint32_t sm_ = rng();
  uint pin_base_ = rng();
  ::rpp_driver::I2sSlaveDuplex *i2s_;
};

// Test 4 parameters constructor.
TEST_F(I2sSlaveDuplexTest, Constructor4) {
  using ::testing::_;
  using ::testing::Return;

  EXPECT_CALL(sdk_, pio_sm_claim(pio_, sm_));
  EXPECT_CALL(sdk_, pio_claim_unused_sm(_, _)).Times(0);

  i2s_ = new ::rpp_driver::I2sSlaveDuplex(sdk_, pio_, sm_, pin_base_);

  // must be matched with Externally given SM.
  EXPECT_EQ(sm_, i2s_->GetStateMachine());

  // We can ignore these call inside destructor
  EXPECT_CALL(sdk_, pio_sm_unclaim(_, _));

  delete (i2s_);

}  // TEST_F(I2sSlaveDuplexTest, Constructor4)

// Test 3 parameters constructor.
TEST_F(I2sSlaveDuplexTest, Constructor3) {
  std::random_device rng;
  using ::testing::_;
  using ::testing::Return;
  const uint ret_val = rng();

  EXPECT_CALL(sdk_, pio_claim_unused_sm(pio_, true)).WillOnce(Return(ret_val));

  i2s_ = new ::rpp_driver::I2sSlaveDuplex(sdk_, pio_, pin_base_);

  // must be matched with Internally Registered SM.
  EXPECT_EQ(ret_val, i2s_->GetStateMachine());

  EXPECT_CALL(sdk_, pio_sm_unclaim(pio_, ret_val));

  delete (i2s_);

}  // TEST_F(I2sSlaveDuplexTest, Constructor3)

// Test Destructor
TEST_F(I2sSlaveDuplexTest, Destructor) {
  std::random_device rng;
  using ::testing::_;
  using ::testing::Return;

  // We can ignore these call in constructor.
  EXPECT_CALL(sdk_, pio_sm_claim(pio_, sm_));
  EXPECT_CALL(sdk_, pio_claim_unused_sm(_, _)).Times(0);

  i2s_ = new ::rpp_driver::I2sSlaveDuplex(sdk_, pio_, sm_, pin_base_);

  EXPECT_CALL(sdk_, pio_sm_unclaim(pio_, sm_));

  delete (i2s_);

}  // TEST_F(I2sSlaveDuplexTest, Destructor)

// Test Destructor with stopped state machine
TEST_F(I2sSlaveDuplexTest, Destructor_stopped) {
  std::random_device rng;
  using ::testing::_;
  using ::testing::InSequence;
  using ::testing::Return;

  // We can ignore these call in constructor.
  EXPECT_CALL(sdk_, pio_sm_claim(pio_, sm_));
  EXPECT_CALL(sdk_, pio_claim_unused_sm(_, _)).Times(0);

  i2s_ = new ::rpp_driver::I2sSlaveDuplex(sdk_, pio_, sm_, pin_base_);

  {
    InSequence dummy;

    EXPECT_CALL(sdk_, pio_sm_set_enabled(pio_, sm_, false));
    EXPECT_CALL(sdk_, pio_sm_clear_fifos(pio_, sm_));

    // Stop state machine.
    i2s_->Stop();

    EXPECT_CALL(sdk_, pio_sm_unclaim(_, _));
  }
  delete (i2s_);

}  // TEST_F(I2sSlaveDuplexTest, Destructor_stopped)

TEST_F(I2sSlaveDuplexTest, Stop) {
  std::random_device rng;
  using ::testing::_;
  using ::testing::Return;

  // We can ignore these call in constructor.
  EXPECT_CALL(sdk_, pio_sm_claim(pio_, sm_));
  EXPECT_CALL(sdk_, pio_claim_unused_sm(_, _)).Times(0);

  i2s_ = new ::rpp_driver::I2sSlaveDuplex(sdk_, pio_, sm_, pin_base_);

  EXPECT_CALL(sdk_, pio_sm_set_enabled(pio_, sm_, false));
  EXPECT_CALL(sdk_, pio_sm_clear_fifos(pio_, sm_));

  // Stop state machine.
  i2s_->Stop();

  // We can ignore these call inside destructor
  EXPECT_CALL(sdk_, pio_sm_unclaim(_, _));

  delete (i2s_);

}  // TEST_F(I2sSlaveDuplexTest, Stop)

TEST_F(I2sSlaveDuplexTest, GetStateMachine) {
  std::random_device rng;
  using ::testing::_;
  using ::testing::Return;

  // We can ignore these call in constructor.
  EXPECT_CALL(sdk_, pio_sm_claim(pio_, sm_));

  i2s_ = new ::rpp_driver::I2sSlaveDuplex(sdk_, pio_, sm_, pin_base_);

  // Must match the given state machine from constructor explicitly.
  EXPECT_EQ(i2s_->GetStateMachine(), sm_);

  // We can ignore these call inside destructor
  EXPECT_CALL(sdk_, pio_sm_unclaim(_, _));

  delete (i2s_);

}  // TEST_F(I2sSlaveDuplexTest, GetStateMachine)

TEST_F(I2sSlaveDuplexTest, Start) {
  using ::testing::_;
  using ::testing::InSequence;
  using ::testing::Return;

  std::random_device rng;
  unsigned instruction_offset = rng();

  // We can ignore these call in constructor.
  EXPECT_CALL(sdk_, pio_sm_claim(pio_, sm_));

  i2s_ = new ::rpp_driver::I2sSlaveDuplex(sdk_, pio_, sm_, pin_base_);

  {
    InSequence dummy;
    // Assign GPIO pins to PIO.
    EXPECT_CALL(sdk_, pio_gpio_init(pio_, pin_base_));
    EXPECT_CALL(sdk_, pio_gpio_init(pio_, pin_base_ + 1));
    EXPECT_CALL(sdk_, pio_gpio_init(pio_, pin_base_ + 2));
    EXPECT_CALL(sdk_, pio_gpio_init(pio_, pin_base_ + 3));

    // PIO pin direction
    EXPECT_CALL(sdk_,
                pio_sm_set_consecutive_pindirs(pio_, sm_, pin_base_, 1, true));
    EXPECT_CALL(sdk_, pio_sm_set_consecutive_pindirs(pio_, sm_, pin_base_ + 1,
                                                     3, false));
    // Load program to memory.
    EXPECT_CALL(sdk_, pio_add_program(pio_, _))
        .WillOnce(Return(instruction_offset));

    // State machine pin mappings.
    EXPECT_CALL(sdk_, sm_config_set_out_pins(_, pin_base_, 1));
    EXPECT_CALL(sdk_, sm_config_set_in_pin_base(_, pin_base_ + 1));

    // To refuse the bug of pico sdk v2.0.0, comment out next line.
    //    EXPECT_CALL(sdk_, sm_config_set_in_pin_count(_, pin_base_, 1));
    EXPECT_CALL(sdk_, sm_config_set_jmp_pin(_, pin_base_ + 3));

    // Configure the clock to state machine.
    EXPECT_CALL(sdk_, clock_get_hz(clk_sys))
        .WillOnce(Return(125'000'000));  // 125MHz
    EXPECT_CALL(sdk_, sm_config_set_clkdiv(
                          _, 2));  // 125MHz/(96kHz*2*32bit*10instrucitons)

    // Shift register setting.
    EXPECT_CALL(sdk_, sm_config_set_in_shift(_, false, false, 32));
    EXPECT_CALL(sdk_, sm_config_set_out_shift(_, false, false, 32));

    // Shift register setting.
    EXPECT_CALL(sdk_, pio_sm_init(pio_, sm_, instruction_offset, _));

    // Fill FIFO
    EXPECT_CALL(sdk_, pio_sm_put(pio_, sm_, 0)).Times(4);

    // Fill FIFO
    EXPECT_CALL(sdk_, pio_sm_set_enabled(pio_, sm_, true));
  }
  i2s_->Start();

  // We can ignore these call inside destructor
  EXPECT_CALL(sdk_, pio_sm_unclaim(_, _));

  delete (i2s_);

}  // TEST_F(I2sSlaveDuplexTest, Start)

TEST_F(I2sSlaveDuplexTest, GetFifoBlocking) {
  std::random_device rng;
  int32_t ret_val = rng();
  using ::testing::_;
  using ::testing::Return;

  // We can ignore these call in constructor.
  EXPECT_CALL(sdk_, pio_sm_claim(pio_, sm_));

  i2s_ = new ::rpp_driver::I2sSlaveDuplex(sdk_, pio_, sm_, pin_base_);

  EXPECT_CALL(sdk_, pio_sm_get_blocking(pio_, sm_)).WillOnce(Return(ret_val));

  // Must match the given state machine from constructor explicitly.
  EXPECT_EQ(i2s_->GetFifoBlocking(), ret_val);

  // We can ignore these call inside destructor
  EXPECT_CALL(sdk_, pio_sm_unclaim(_, _));

  delete (i2s_);

}  // TEST_F(I2sSlaveDuplexTest, GetFifoBlocking)

TEST_F(I2sSlaveDuplexTest, PutFifoBlocking) {
  std::random_device rng;
  int32_t value = rng();
  using ::testing::_;
  using ::testing::Return;

  // We can ignore these call in constructor.
  EXPECT_CALL(sdk_, pio_sm_claim(pio_, sm_));

  i2s_ = new ::rpp_driver::I2sSlaveDuplex(sdk_, pio_, sm_, pin_base_);

  EXPECT_CALL(sdk_, pio_sm_put_blocking(pio_, sm_, value));

  // Must match the given state machine from constructor explicitly.
  i2s_->PutFifoBlocking(value);

  // We can ignore these call inside destructor
  EXPECT_CALL(sdk_, pio_sm_unclaim(_, _));

  delete (i2s_);

}  // TEST_F(I2sSlaveDuplexTest, PutFifoBlocking)
