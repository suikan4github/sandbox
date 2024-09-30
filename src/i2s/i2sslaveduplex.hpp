#ifndef PICO_DRIVER_SRC_I2S_DUPLEXSLAVEI2S_HPP_
#define PICO_DRIVER_SRC_I2S_DUPLEXSLAVEI2S_HPP_

#if __has_include(<hardware/pio.h>)
#include "hardware/pio.h"
#else
// Alternate definition for unit test.
#define i2s_slave_duplex_program_get_default_config(config) 1
#define clk_sys 51
#endif  //__has_include(<hardware/i2c.h>)

#if __has_include(<gmock/gmock.h>)
#include <gmock/gmock.h>
#endif

#include "sdkwrapper.hpp"

namespace rpp_driver {
/**
 * @brief Duplex Slave I2S controller class.
 * @details
 * This class supports the duplex communication of the I2S on the PIO port of
 * the RP2040/2350 SoC MCU.
 *
 * The timing signal (BCLK and WS) of the I2S must be provided from the external
 * device. This class supports up to 192kHz Fs if the MCU system clock is higher
 * than 120MHz.
 *
 * The I2S pins can be mapped on the GPIO. This mapping is based on the
 * pin_base parameter of the constructors.
 *
 * The pin_base parameter of the constructors is the first pin of 4 I2S signals.
 * The signals must be consecutive on the GPIO pins as like :
 * -# SDOUT
 * -# SDIN
 * -# BCLK
 * -# WS
 *
 * For example, let's assume SDTOUT is GPIO 15. Then, SDIN, BCLK and WS are
 * GPIO 16, 17 and 18 respectively.
 *
 * To start and stop the I2S transfer, call the Start() and the Stop() member
 * functions respectively.
 *
 * The audio sample in and out are through the GetFifoBlocking() and the
 * PutFifoBlocking() member functions, respectively. These are blocking
 * function. That mean, program will wait until the data is ready, or FIFO has
 * room for data.
 *
 * This class assumes polling based data transfer instead of interrupt / DMA
 * based data transfer.
 *
 * Typically, program process the signal obtained from GetFifoBLock() and
 * output to PutFifoBLock(). To provide the high quality processing,
 * The first GetFifoBlocking() call should be as soon as possible, after
 * calling start() function.
 */
class I2sSlaveDuplex {
 private:
  ::rpp_driver::SdkWrapper &sdk_;
  PIO pio_;
  const uint32_t sm_;    // State machine ( 0..3 )
  const uint pin_base_;  // first GPIO pin number of the I2S signal.

 public:
  I2sSlaveDuplex(/* args */) = delete;
  /**
   * @brief Construct a new Duplex Slave I2S object
   *
   * @param sdk SDK wrapper class injection.
   * @param pio PIO to use.
   * @param pin_base The GPIO pin number of SDOUT signal.
   * @details
   * The state machine number is not specified in this constructor.
   * Internally, the state machine will be allocate from the unused one.
   *
   * This constructor just registers the given parameters and allocated state
   * machine to the internal parameters. And then, claim the state machine. No
   * other processing will be done.
   */
  I2sSlaveDuplex(::rpp_driver::SdkWrapper &sdk, PIO pio, uint pin_base);
  /**
   * @brief Construct a new Duplex Slave I2S object
   *
   * @param sdk SDK wrapper class injection.
   * @param pio PIO to use.
   * @param sm State machine to use.
   * @param pin_base The GPIO pin number of SDOUT signal.
   * @details
   * This constructor just registers the given parameters to the internal
   * parameters. And then, claim the state machine. No other processing will be
   * done.
   */
  I2sSlaveDuplex(::rpp_driver::SdkWrapper &sdk, PIO pio, uint32_t sm,
                 uint pin_base);

  /**
   * @brief Unclaim the state machine. No actual processing.
   */
  ~I2sSlaveDuplex();

  /**
   * @brief Initialize the I2S port, and run.
   * @details
   * Assign the GPIO, configure them, load the PIO program, configure the state
   * machine and run.
   */
  virtual void Start();
  /**
   * @brief Stop the I2S port and disable the PIO state machine in use.
   * @details
   * Make FIFOs empty.
   *
   */
  virtual void Stop();
  /**
   * @brief Get the State Machine object
   *
   * @return uint32_t The number of the state machine which is claimed.
   * @details
   * This is convenient if the state machine is assigned internally.
   */
  virtual uint32_t GetStateMachine();
  /**
   * @brief Get one audio data from RX FIFO.
   *
   * @return int32_t An audio data.
   * @details
   * To get one stere sample, call this member function twice. The left data
   * will be given and then, right data.
   *
   * This function is blocking. That mean, program will wait until the data has
   * been received.
   */
  virtual int32_t GetFifoBlocking();
  /**
   * @brief Put one audio data to TX FIFO.
   *
   * @param value An audio data to send.
   * To put one stere sample, call this member function twice. The left data
   * must be put and then, right data.
   *
   */
  virtual void PutFifoBlocking(int32_t value);
};

#if __has_include(<gmock/gmock.h>)

class MockI2sSlaveDuplex : public I2sSlaveDuplex {
 public:
  MOCK_METHOD0(GetStateMachine, uint32_t(void));
  MOCK_METHOD0(Start, void(void));
  MOCK_METHOD0(Stop, void(void));
  MOCK_METHOD1(PutFifoBlocking, void(int32_t value));
  MOCK_METHOD0(GetFifoBlocking, int32_t());
};
#endif  // __has_include(<gmock/gmock.h>)

}  // namespace rpp_driver

#endif  // PICO_DRIVER_SRC_I2S_DUPLEXSLAVEI2S_HPP_