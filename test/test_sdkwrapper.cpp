
//    "Copyright 2024 Seiichi Horie
/**
 * @file
 */
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <random>

#include "fff/fff.h"

// Include for test
#include <stdint.h>

// Preparation for fff
DEFINE_FFF_GLOBALS;

// The header file of the library to test.
#include "../src/sdk/sdkwrapper.hpp"

// Create Test Spies
FAKE_VALUE_FUNC(bool, stdio_init_all);
FAKE_VOID_FUNC(sleep_ms, uint32_t);
FAKE_VALUE_FUNC(uint32_t, clock_get_hz, clock_handle_t);

FAKE_VOID_FUNC(gpio_init, uint);
FAKE_VOID_FUNC(gpio_deinit, uint);
FAKE_VOID_FUNC(gpio_set_function, uint, gpio_function_t);
FAKE_VOID_FUNC(gpio_set_dir, uint, bool);
FAKE_VOID_FUNC(gpio_set_input_enabled, uint, bool);
FAKE_VOID_FUNC(gpio_put, uint, bool);
FAKE_VALUE_FUNC(bool, gpio_get, uint);
FAKE_VOID_FUNC(gpio_pull_up, uint);
FAKE_VOID_FUNC(gpio_pull_down, uint);
FAKE_VOID_FUNC(gpio_disable_pulls, uint);
FAKE_VALUE_FUNC(uint, i2c_init, i2c_inst_t *, uint);

FAKE_VOID_FUNC(i2c_deinit, i2c_inst_t *);
FAKE_VALUE_FUNC(int, i2c_read_blocking, i2c_inst_t *, uint8_t, uint8_t *,
                size_t, bool);
FAKE_VALUE_FUNC(int, i2c_write_blocking, i2c_inst_t *, uint8_t, const uint8_t *,
                size_t, bool);

FAKE_VALUE_FUNC(int, pio_sm_set_consecutive_pindirs, PIO, uint, uint, uint,
                bool);
FAKE_VOID_FUNC(pio_gpio_init, PIO, uint);
FAKE_VOID_FUNC(sm_config_set_out_pins, pio_sm_config *, uint, uint);
FAKE_VOID_FUNC(sm_config_set_in_pin_base, pio_sm_config *, uint);
FAKE_VOID_FUNC(sm_config_set_in_pin_count, pio_sm_config *, uint);
FAKE_VOID_FUNC(sm_config_set_clkdiv, pio_sm_config *, float);
FAKE_VOID_FUNC(sm_config_set_in_shift, pio_sm_config *, bool, bool, uint);
FAKE_VOID_FUNC(sm_config_set_out_shift, pio_sm_config *, bool, bool, uint);
FAKE_VALUE_FUNC(int, pio_sm_init, PIO, uint, uint, const pio_sm_config *);
FAKE_VOID_FUNC(pio_sm_put, PIO, uint, uint32_t);
FAKE_VOID_FUNC(pio_sm_put_blocking, PIO, uint, uint32_t);
FAKE_VALUE_FUNC(uint32_t, pio_sm_get, PIO, uint);
FAKE_VALUE_FUNC(uint32_t, pio_sm_get_blocking, PIO, uint);
FAKE_VOID_FUNC(pio_sm_set_enabled, PIO, uint, bool);
FAKE_VALUE_FUNC(int, pio_add_program, PIO, const pio_program_t *);
FAKE_VOID_FUNC(sm_config_set_jmp_pin, pio_sm_config *, uint);
FAKE_VOID_FUNC(pio_sm_claim, PIO, uint);
FAKE_VOID_FUNC(pio_sm_unclaim, PIO, uint);
FAKE_VALUE_FUNC(int, pio_claim_unused_sm, PIO, bool);
FAKE_VALUE_FUNC(bool, pio_sm_is_claimed, PIO, uint);
FAKE_VOID_FUNC(pio_sm_clear_fifos, PIO, uint);

// The cpp file of the library to test.
#include "../src/sdk/sdkwrapper.cpp"

TEST(SdkWrapper, stdio_init_all) {
  ::rpp_driver::SdkWrapper pico;

  FFF_RESET_HISTORY();
  RESET_FAKE(stdio_init_all);

  bool return_vals[] = {true, false};
  SET_RETURN_SEQ(stdio_init_all, return_vals,
                 sizeof(return_vals) / sizeof(bool));

  ASSERT_EQ(pico.stdio_init_all(), return_vals[0]);

  // Check the data from test spy. How many time called?
  ASSERT_EQ(stdio_init_all_fake.call_count, 1);

  // Check the data from test spy. Call order.
  ASSERT_EQ(fff.call_history[0], (void *)stdio_init_all);

  // Check the data from test spy. : Parameters.
  ASSERT_EQ(pico.stdio_init_all(), return_vals[1]);

  RESET_FAKE(stdio_init_all);

}  // stdio_init_all

TEST(SdkWrapper, sleep_ms) {
  std::random_device rng;
  ::rpp_driver::SdkWrapper pico;
  uint ms_array[] = {rng(), rng()};

  FFF_RESET_HISTORY();
  RESET_FAKE(sleep_ms);

  for (auto &&ms : ms_array) {
    pico.sleep_ms(ms);
  }

  // Check the data from test spy. How many time called?
  ASSERT_EQ(sleep_ms_fake.call_count, std::size(ms_array));

  int index = 0;
  for (auto &&ms : ms_array) {
    // Check the data from test spy. Call order.
    ASSERT_EQ(fff.call_history[index], (void *)sleep_ms);
    // Check the data from test spy. : Parameters.
    ASSERT_EQ(sleep_ms_fake.arg0_history[index], ms);
    index++;
  }

  RESET_FAKE(sleep_ms);
}

TEST(SdkWrapper, gpio_init) {
  std::random_device rng;
  ::rpp_driver::SdkWrapper pico;
  uint gpioarray[] = {rng(), rng()};

  FFF_RESET_HISTORY();
  RESET_FAKE(gpio_init);

  for (auto &&gpio : gpioarray) {
    pico.gpio_init(gpio);
  }

  // Check the data from test spy. How many time called?
  ASSERT_EQ(gpio_init_fake.call_count, std::size(gpioarray));

  int index = 0;
  for (auto &&gpio : gpioarray) {
    // Check the data from test spy. Call order.
    ASSERT_EQ(fff.call_history[index], (void *)gpio_init);
    // Check the data from test spy. : Parameters.
    ASSERT_EQ(gpio_init_fake.arg0_history[index], gpio);
    index++;
  }

  RESET_FAKE(gpio_init);
}  // TEST(SdkWrapper, gpio_init)

TEST(SdkWrapper, gpio_deinit) {
  std::random_device rng;
  ::rpp_driver::SdkWrapper pico;
  uint gpioarray[] = {rng(), rng()};

  FFF_RESET_HISTORY();
  RESET_FAKE(gpio_deinit);

  for (auto &&gpio : gpioarray) {
    pico.gpio_deinit(gpio);
  }

  // Check the data from test spy. How many time called?
  ASSERT_EQ(gpio_deinit_fake.call_count, std::size(gpioarray));

  int index = 0;
  for (auto &&gpio : gpioarray) {
    // Check the data from test spy. Call order.
    ASSERT_EQ(fff.call_history[index], (void *)gpio_deinit);
    // Check the data from test spy. : Parameters.
    ASSERT_EQ(gpio_deinit_fake.arg0_history[index], gpio);
    index++;
  }

  RESET_FAKE(gpio_deinit);
}  // TEST(SdkWrapper, gpio_deinit)

TEST(SdkWrapper, gpio_set_function) {
  std::random_device rng;
  ::rpp_driver::SdkWrapper pico;
  uint gpioarray[] = {rng(), rng()};
  int functionarray[] = {static_cast<int>(rng()), static_cast<int>(rng())};

  FFF_RESET_HISTORY();
  RESET_FAKE(gpio_set_function);

  for (auto &&gpio : gpioarray) {
    for (auto &&fn : functionarray) {
      pico.gpio_set_function(gpio, fn);
    }
  }

  // Check the data from test spy. How many time called?
  ASSERT_EQ(gpio_set_function_fake.call_count,
            std::size(gpioarray) * std::size(functionarray));

  uint index = 0;
  for (auto &&gpio : gpioarray) {
    for (auto &&fn : functionarray) {
      // Check the data from test spy. Call order.
      ASSERT_EQ(fff.call_history[index], (void *)gpio_set_function);
      // Check the data from test spy.  Parameters.
      ASSERT_EQ(gpio_set_function_fake.arg0_history[index], gpio);
      ASSERT_EQ(gpio_set_function_fake.arg1_history[index], fn);
      index++;
    }
  }

  RESET_FAKE(gpio_set_function);

}  // gpio_set_function

TEST(SdkWrapper, gpio_set_dir) {
  std::random_device rng;
  ::rpp_driver::SdkWrapper pico;
  uint gpioarray[] = {rng(), rng()};
  bool dirarray[] = {true, false};

  FFF_RESET_HISTORY();
  RESET_FAKE(gpio_set_dir);

  for (auto &&gpio : gpioarray) {
    for (auto &&dir : dirarray) {
      pico.gpio_set_dir(gpio, dir);
    }
  }

  // Check the data from test spy. How many time called?
  ASSERT_EQ(gpio_set_dir_fake.call_count,
            std::size(gpioarray) * std::size(dirarray));

  uint index = 0;
  for (auto &&gpio : gpioarray) {
    for (auto &&dir : dirarray) {
      // Check the data from test spy. Call order.
      ASSERT_EQ(fff.call_history[index], (void *)gpio_set_dir);
      // Check the data from test spy. : Parameters.
      ASSERT_EQ(gpio_set_dir_fake.arg0_history[index], gpio);
      ASSERT_EQ(gpio_set_dir_fake.arg1_history[index], dir);
      index++;
    }
  }

  RESET_FAKE(gpio_set_dir);
}  // TEST(SdkWrapper, gpio_set_dir)

TEST(SdkWrapper, gpio_set_input_enabled) {
  std::random_device rng;
  ::rpp_driver::SdkWrapper pico;
  uint gpioarray[] = {rng(), rng()};
  bool dirarray[] = {true, false};

  FFF_RESET_HISTORY();
  RESET_FAKE(gpio_set_input_enabled);

  for (auto &&gpio : gpioarray) {
    for (auto &&dir : dirarray) {
      pico.gpio_set_input_enabled(gpio, dir);
    }
  }

  // Check the data from test spy. How many time called?
  ASSERT_EQ(gpio_set_input_enabled_fake.call_count,
            std::size(gpioarray) * std::size(dirarray));

  uint index = 0;
  for (auto &&gpio : gpioarray) {
    for (auto &&dir : dirarray) {
      // Check the data from test spy. Call order.
      ASSERT_EQ(fff.call_history[index], (void *)gpio_set_input_enabled);
      // Check the data from test spy. : Parameters.
      ASSERT_EQ(gpio_set_input_enabled_fake.arg0_history[index], gpio);
      ASSERT_EQ(gpio_set_input_enabled_fake.arg1_history[index], dir);
      index++;
    }
  }

  RESET_FAKE(gpio_set_input_enabled);
}  // TEST(SdkWrapper, gpio_set_input_enabled)

TEST(SdkWrapper, gpio_put) {
  std::random_device rng;
  ::rpp_driver::SdkWrapper pico;
  uint gpioarray[] = {rng(), rng()};
  bool valuearray[] = {true, false};

  FFF_RESET_HISTORY();
  RESET_FAKE(gpio_put);

  for (auto &&gpio : gpioarray) {
    for (auto &&value : valuearray) {
      pico.gpio_put(gpio, value);
    }
  }

  // Check the data from test spy. How many time called?
  ASSERT_EQ(gpio_put_fake.call_count,
            std::size(gpioarray) * std::size(valuearray));

  // Test all combination of the parameter.
  uint index = 0;
  for (auto &&gpio : gpioarray) {
    for (auto &&value : valuearray) {
      // Check the data from test spy. Call order.
      ASSERT_EQ(fff.call_history[index], (void *)gpio_put);
      // Check the data from test spy. : Parameters.
      ASSERT_EQ(gpio_put_fake.arg0_history[index], gpio);
      ASSERT_EQ(gpio_put_fake.arg1_history[index], value);
      index++;
    }
  }

  RESET_FAKE(gpio_put);
}

TEST(SdkWrapper, gpio_get) {
  ::rpp_driver::SdkWrapper pico;
  uint gpio = 17;

  FFF_RESET_HISTORY();
  RESET_FAKE(gpio_get);

  bool return_vals[] = {true, false};
  SET_RETURN_SEQ(gpio_get, return_vals, sizeof(return_vals) / sizeof(bool));

  ASSERT_EQ(pico.gpio_get(gpio), return_vals[0]);

  // Check the data from test spy. How many time called?
  ASSERT_EQ(gpio_get_fake.call_count, 1);

  // Check the data from test spy. Call order.
  ASSERT_EQ(fff.call_history[0], (void *)gpio_get);

  gpio = 11;

  // Check the data from test spy. : Parameters.
  ASSERT_EQ(pico.gpio_get(gpio), return_vals[1]);

  RESET_FAKE(gpio_get);
}

TEST(SdkWrapper, gpio_pull_up) {
  ::rpp_driver::SdkWrapper pico;
  uint gpio = 17;

  FFF_RESET_HISTORY();
  RESET_FAKE(gpio_pull_up);

  pico.gpio_pull_up(gpio);

  // Check the data from test spy. How many time called?
  ASSERT_EQ(gpio_pull_up_fake.call_count, 1);

  // Check the data from test spy. Call order.
  ASSERT_EQ(fff.call_history[0], (void *)gpio_pull_up);

  // Check the data from test spy. : Parameters.
  ASSERT_EQ(gpio_pull_up_fake.arg0_history[0], gpio);

  // Try another parameter
  FFF_RESET_HISTORY();
  RESET_FAKE(gpio_pull_up);

  gpio = 11;
  pico.gpio_pull_up(gpio);

  // Check the data from test spy. : Parameters.
  ASSERT_EQ(gpio_pull_up_fake.arg0_history[0], gpio);

  RESET_FAKE(gpio_pull_up);
}  // TEST(SdkWrapper, gpio_pull_up)

TEST(SdkWrapper, gpio_pull_down) {
  ::rpp_driver::SdkWrapper pico;
  uint gpio = 17;

  FFF_RESET_HISTORY();
  RESET_FAKE(gpio_pull_down);

  pico.gpio_pull_down(gpio);

  // Check the data from test spy. How many time called?
  ASSERT_EQ(gpio_pull_down_fake.call_count, 1);

  // Check the data from test spy. Call order.
  ASSERT_EQ(fff.call_history[0], (void *)gpio_pull_down);

  // Check the data from test spy. : Parameters.
  ASSERT_EQ(gpio_pull_down_fake.arg0_history[0], gpio);

  // Try another parameter
  FFF_RESET_HISTORY();
  RESET_FAKE(gpio_pull_down);

  gpio = 11;
  pico.gpio_pull_down(gpio);

  // Check the data from test spy. : Parameters.
  ASSERT_EQ(gpio_pull_down_fake.arg0_history[0], gpio);

  RESET_FAKE(gpio_pull_down);
}  // TEST(SdkWrapper, gpio_pull_down)

TEST(SdkWrapper, gpio_disable_pulls) {
  ::rpp_driver::SdkWrapper pico;
  uint gpio = 17;

  FFF_RESET_HISTORY();
  RESET_FAKE(gpio_disable_pulls);

  pico.gpio_disable_pulls(gpio);

  // Check the data from test spy. How many time called?
  ASSERT_EQ(gpio_disable_pulls_fake.call_count, 1);

  // Check the data from test spy. Call order.
  ASSERT_EQ(fff.call_history[0], (void *)gpio_disable_pulls);

  // Check the data from test spy. : Parameters.
  ASSERT_EQ(gpio_disable_pulls_fake.arg0_history[0], gpio);

  // Try another parameter
  FFF_RESET_HISTORY();
  RESET_FAKE(gpio_disable_pulls);

  gpio = 11;
  pico.gpio_disable_pulls(gpio);

  // Check the data from test spy. : Parameters.
  ASSERT_EQ(gpio_disable_pulls_fake.arg0_history[0], gpio);

  RESET_FAKE(gpio_disable_pulls);
}  // TEST(SdkWrapper, gpio_disable_pulls)

// FAKE_VALUE_FUNC(uint, i2c_init, i2c_inst_t *, uint);
TEST(SdkWrapper, i2c_init) {
  std::random_device rng;
  ::rpp_driver::SdkWrapper pico;
  i2c_inst_t i2c = 17;
  const uint baud[] = {rng(), rng()};

  FFF_RESET_HISTORY();
  RESET_FAKE(i2c_init);

  uint return_vals[] = {rng(), rng()};
  SET_RETURN_SEQ(i2c_init, return_vals,
                 sizeof(return_vals) / sizeof(return_vals[0]));

  ASSERT_EQ(pico.i2c_init(&i2c, baud[0]), return_vals[0]);
  ASSERT_EQ(pico.i2c_init(&i2c, baud[1]), return_vals[1]);

  // Check the data from test spy. How many time called?
  ASSERT_EQ(i2c_init_fake.call_count, 2);

  // Check the data from test spy. Call order.
  ASSERT_EQ(fff.call_history[0], (void *)i2c_init);
  ASSERT_EQ(fff.call_history[1], (void *)i2c_init);

  // Check the data from test spy. : Parameters.
  ASSERT_EQ(i2c_init_fake.arg0_history[0], &i2c);
  ASSERT_EQ(i2c_init_fake.arg1_history[0], baud[0]);
  ASSERT_EQ(i2c_init_fake.arg0_history[1], &i2c);
  ASSERT_EQ(i2c_init_fake.arg1_history[1], baud[1]);

  RESET_FAKE(i2c_init);
}

// FAKE_VOID_FUNC(i2c_deinit, i2c_inst_t *);
TEST(SdkWrapper, i2c_deinit) {
  std::random_device rng;
  ::rpp_driver::SdkWrapper pico;
  i2c_inst_t i2c = 17;
  const uint baud[] = {rng(), rng()};

  FFF_RESET_HISTORY();
  RESET_FAKE(i2c_deinit);

  pico.i2c_deinit(&i2c);

  // Check the data from test spy. How many time called?
  ASSERT_EQ(i2c_deinit_fake.call_count, 1);

  // Check the data from test spy. Call order.
  ASSERT_EQ(fff.call_history[0], (void *)i2c_deinit);

  // Check the data from test spy. : Parameters.
  ASSERT_EQ(i2c_deinit_fake.arg0_history[0], &i2c);

  RESET_FAKE(i2c_deinit);
}

// FAKE_VALUE_FUNC(int, i2c_read_blocking, i2c_inst_t *, uint8_t, uint8_t *,
//                  size_t, bool);
TEST(SdkWrapper, i2c_read_blocking) {
  std::random_device rng;
  ::rpp_driver::SdkWrapper pico;
  i2c_inst_t i2c = 17;
  uint8_t buf[10];
  uint8_t addrs_array[] = {static_cast<uint8_t>(rng()),
                           static_cast<uint8_t>(rng())};
  size_t bufsize_array[2] = {rng(), rng()};
  bool nostop_array[2] = {true, false};
  int return_vals_array[8] = {1, 2, 3, 4, 5, 6, 7, 8};

  FFF_RESET_HISTORY();
  RESET_FAKE(i2c_read_blocking);

  SET_RETURN_SEQ(i2c_read_blocking, return_vals_array,
                 std::size(return_vals_array));

  // Check wether return value is correct.
  int index = 0;
  for (auto &&addrs : addrs_array) {
    for (auto &&bufsize : bufsize_array) {
      for (auto &&nostop : nostop_array) {
        ASSERT_EQ(pico.i2c_read_blocking(&i2c, addrs, buf, bufsize, nostop),
                  return_vals_array[index]);
        index++;
      }
    }
  }

  // Check the data from test spy. How many time called?
  ASSERT_EQ(i2c_read_blocking_fake.call_count, 8);

  // Check wether parameters are passed collectly.
  index = 0;
  for (auto &&addrs : addrs_array) {
    for (auto &&bufsize : bufsize_array) {
      for (auto &&nostop : nostop_array) {
        // Check the data from test spy. Call order.
        ASSERT_EQ(fff.call_history[index], (void *)i2c_read_blocking);
        // Check the data from test spy. : Parameters.
        ASSERT_EQ(i2c_read_blocking_fake.arg0_history[index], &i2c);
        ASSERT_EQ(i2c_read_blocking_fake.arg1_history[index], addrs);
        ASSERT_EQ(i2c_read_blocking_fake.arg2_history[index], buf);
        ASSERT_EQ(i2c_read_blocking_fake.arg3_history[index], bufsize);
        ASSERT_EQ(i2c_read_blocking_fake.arg4_history[index], nostop);
        index++;
      }
    }
  }

  RESET_FAKE(i2c_read_blocking);
}

// FAKE_VALUE_FUNC(int, i2c_write_blocking, i2c_inst_t *, uint8_t,
// const uint8_t *,  size_t, bool);
TEST(SdkWrapper, i2c_write_blocking) {
  std::random_device rng;
  ::rpp_driver::SdkWrapper pico;
  i2c_inst_t i2c = 17;
  uint8_t buf[10];
  uint8_t addrs_array[] = {static_cast<uint8_t>(rng()),
                           static_cast<uint8_t>(rng())};
  size_t bufsize_array[2] = {rng(), rng()};
  bool nostop_array[2] = {true, false};
  int return_vals_array[8] = {1, 2, 3, 4, 5, 6, 7, 8};

  FFF_RESET_HISTORY();
  RESET_FAKE(i2c_write_blocking);

  SET_RETURN_SEQ(i2c_write_blocking, return_vals_array,
                 std::size(return_vals_array));

  // Check wether return value is correct.
  int index = 0;
  for (auto &&addrs : addrs_array) {
    for (auto &&bufsize : bufsize_array) {
      for (auto &&nostop : nostop_array) {
        ASSERT_EQ(pico.i2c_write_blocking(&i2c, addrs, buf, bufsize, nostop),
                  return_vals_array[index]);
        index++;
      }
    }
  }

  // Check the data from test spy. How many time called?
  ASSERT_EQ(i2c_write_blocking_fake.call_count, 8);

  // Check wether parameters are passed collectly.
  index = 0;
  for (auto &&addrs : addrs_array) {
    for (auto &&bufsize : bufsize_array) {
      for (auto &&nostop : nostop_array) {
        // Check the data from test spy. Call order.
        ASSERT_EQ(fff.call_history[index], (void *)i2c_write_blocking);
        // Check the data from test spy. : Parameters.
        ASSERT_EQ(i2c_write_blocking_fake.arg0_history[index], &i2c);
        ASSERT_EQ(i2c_write_blocking_fake.arg1_history[index], addrs);
        ASSERT_EQ(i2c_write_blocking_fake.arg2_history[index], buf);
        ASSERT_EQ(i2c_write_blocking_fake.arg3_history[index], bufsize);
        ASSERT_EQ(i2c_write_blocking_fake.arg4_history[index], nostop);
        index++;
      }
    }
  }

  RESET_FAKE(i2c_write_blocking);
}

TEST(SdkWrapper, pio_sm_set_consecutive_pindirs) {
  std::random_device rng;
  ::rpp_driver::SdkWrapper pico;
  PIO pio_array[] = {rng(), rng()};
  uint sm_array[] = {rng(), rng()};
  uint pins_base_array[] = {rng(), rng()};
  uint pin_count_array[] = {rng(), rng()};
  bool is_out_array[2] = {true, false};
  int return_vals_array[32] = {1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8,
                               1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8};

  FFF_RESET_HISTORY();
  RESET_FAKE(pio_sm_set_consecutive_pindirs);

  SET_RETURN_SEQ(pio_sm_set_consecutive_pindirs, return_vals_array,
                 std::size(return_vals_array));

  // Check wether return value is correct.
  int index = 0;
  for (auto &&pio : pio_array)
    for (auto &&sm : sm_array)
      for (auto &&pins : pins_base_array)
        for (auto &&pin_count : pin_count_array)
          for (auto &&is_out : is_out_array) {
            ASSERT_EQ(pico.pio_sm_set_consecutive_pindirs(pio, sm, pins,
                                                          pin_count, is_out),
                      return_vals_array[index]);
            index++;
          }

  // Check the data from test spy. How many time called?
  ASSERT_EQ(pio_sm_set_consecutive_pindirs_fake.call_count, 32);

  // Check wether parameters are passed collectly.
  index = 0;
  for (auto &&pio : pio_array)
    for (auto &&sm : sm_array)
      for (auto &&pins : pins_base_array)
        for (auto &&pin_count : pin_count_array)
          for (auto &&is_out : is_out_array) {
            // Check the data from test spy. Call order.
            ASSERT_EQ(fff.call_history[index],
                      (void *)pio_sm_set_consecutive_pindirs);
            // Check the data from test spy. : Parameters.
            ASSERT_EQ(pio_sm_set_consecutive_pindirs_fake.arg0_history[index],
                      pio);
            ASSERT_EQ(pio_sm_set_consecutive_pindirs_fake.arg1_history[index],
                      sm);
            ASSERT_EQ(pio_sm_set_consecutive_pindirs_fake.arg2_history[index],
                      pins);
            ASSERT_EQ(pio_sm_set_consecutive_pindirs_fake.arg3_history[index],
                      pin_count);
            ASSERT_EQ(pio_sm_set_consecutive_pindirs_fake.arg4_history[index],
                      is_out);
            index++;
          }

  RESET_FAKE(pio_sm_set_consecutive_pindirs);
}  // TEST(SdkWrapper, pio_sm_set_consecutive_pindirs)

TEST(SdkWrapper, pio_gpio_init) {
  std::random_device rng;
  ::rpp_driver::SdkWrapper pico;
  PIO pio_array[] = {rng(), rng()};
  uint pin_array[] = {rng(), rng()};

  FFF_RESET_HISTORY();
  RESET_FAKE(pio_gpio_init);

  // Trial call.
  for (auto &&pio : pio_array)
    for (auto &&pin : pin_array) pico.pio_gpio_init(pio, pin);

  // Check the data from test spy. How many time called?
  ASSERT_EQ(pio_gpio_init_fake.call_count, 4);

  // Check wether parameters are passed collectly.
  int index = 0;
  for (auto &&pio : pio_array)
    for (auto &&pin : pin_array) {
      // Check the data from test spy. Call order.
      ASSERT_EQ(fff.call_history[index], (void *)pio_gpio_init);
      // Check the data from test spy. : Parameters.
      ASSERT_EQ(pio_gpio_init_fake.arg0_history[index], pio);
      ASSERT_EQ(pio_gpio_init_fake.arg1_history[index], pin);
      index++;
    }

  RESET_FAKE(pio_gpio_init);
}  // TEST(SdkWrapper, pio_gpio_init)

TEST(SdkWrapper, sm_config_set_out_pins) {
  std::random_device rng;
  ::rpp_driver::SdkWrapper pico;
  PIO pio_array[] = {rng(), rng()};
  uint length_array[] = {rng(), rng()};
  pio_sm_config config;

  FFF_RESET_HISTORY();
  RESET_FAKE(sm_config_set_out_pins);

  // Trial call.
  for (auto &&pio : pio_array)
    for (auto &&length : length_array)
      pico.sm_config_set_out_pins(&config, pio, length);

  // Check the data from test spy. How many time called?
  ASSERT_EQ(sm_config_set_out_pins_fake.call_count, 4);

  // Check wether parameters are passed collectly.
  int index = 0;
  for (auto &&pio : pio_array)
    for (auto &&length : length_array) {
      // Check the data from test spy. Call order.
      ASSERT_EQ(fff.call_history[index], (void *)sm_config_set_out_pins);
      // Check the data from test spy. : Parameters.
      ASSERT_EQ(sm_config_set_out_pins_fake.arg0_history[index], &config);
      ASSERT_EQ(sm_config_set_out_pins_fake.arg1_history[index], pio);
      ASSERT_EQ(sm_config_set_out_pins_fake.arg2_history[index], length);
      index++;
    }

  RESET_FAKE(sm_config_set_out_pins);
}  // TEST(SdkWrapper, sm_config_set_out_pins)

TEST(SdkWrapper, sm_config_set_in_pin_base) {
  std::random_device rng;
  ::rpp_driver::SdkWrapper pico;
  uint base_array[] = {rng(), rng()};
  pio_sm_config config;

  FFF_RESET_HISTORY();
  RESET_FAKE(sm_config_set_in_pin_base);

  // Trial call.
  for (auto &&base : base_array) pico.sm_config_set_in_pin_base(&config, base);

  // Check the data from test spy. How many time called?
  ASSERT_EQ(sm_config_set_in_pin_base_fake.call_count, 2);

  // Check wether parameters are passed collectly.
  int index = 0;
  for (auto &&base : base_array) {
    // Check the data from test spy. Call order.
    ASSERT_EQ(fff.call_history[index], (void *)sm_config_set_in_pin_base);
    // Check the data from test spy. : Parameters.
    ASSERT_EQ(sm_config_set_in_pin_base_fake.arg0_history[index], &config);
    ASSERT_EQ(sm_config_set_in_pin_base_fake.arg1_history[index], base);
    index++;
  }

  RESET_FAKE(sm_config_set_in_pin_base);
}  // TEST(SdkWrapper, sm_config_set_in_pin_base)

TEST(SdkWrapper, sm_config_set_in_pin_count) {
  std::random_device rng;
  ::rpp_driver::SdkWrapper pico;
  PIO count_array[] = {rng(), rng()};
  pio_sm_config config;

  FFF_RESET_HISTORY();
  RESET_FAKE(sm_config_set_in_pin_base);

  // Trial call.
  for (auto &&count : count_array)
    pico.sm_config_set_in_pin_count(&config, count);

  // Check the data from test spy. How many time called?
  ASSERT_EQ(sm_config_set_in_pin_count_fake.call_count, 2);

  // Check wether parameters are passed collectly.
  int index = 0;
  for (auto &&count : count_array) {
    // Check the data from test spy. Call order.
    ASSERT_EQ(fff.call_history[index], (void *)sm_config_set_in_pin_count);
    // Check the data from test spy. : Parameters.
    ASSERT_EQ(sm_config_set_in_pin_count_fake.arg0_history[index], &config);
    ASSERT_EQ(sm_config_set_in_pin_count_fake.arg1_history[index], count);
    index++;
  }

  RESET_FAKE(sm_config_set_in_pin_base);
}  // TEST(SdkWrapper, sm_config_set_in_pin_count)

TEST(SdkWrapper, clock_get_hz) {
  std::random_device rng;
  ::rpp_driver::SdkWrapper pico;
  clock_handle_t clock_handle_array[] = {rng(), rng()};
  uint32_t return_vals_array[] = {rng(), rng()};

  FFF_RESET_HISTORY();
  RESET_FAKE(clock_get_hz);

  SET_RETURN_SEQ(clock_get_hz, return_vals_array, std::size(return_vals_array));

  // Check whether call matches.
  uint index = 0;

  for (auto &&clock_handle : clock_handle_array) {
    ASSERT_EQ(pico.clock_get_hz(clock_handle), return_vals_array[index]);
    index++;
  }

  // Check the data from test spy. How many time called?
  ASSERT_EQ(clock_get_hz_fake.call_count, 2);

  // Check wether parameters are passed collectly.
  index = 0;
  for (auto &&clock_handle : clock_handle_array) {
    // Check the data from test spy. Call order.
    ASSERT_EQ(fff.call_history[index], (void *)clock_get_hz);
    // Check the data from test spy. : Parameters.
    ASSERT_EQ(clock_get_hz_fake.arg0_history[index], clock_handle);
    index++;
  }

  RESET_FAKE(clock_get_hz);
}  // TEST(SdkWrapper, clock_get_hz)

TEST(SdkWrapper, sm_config_set_clkdiv) {
  std::random_device rng;
  ::rpp_driver::SdkWrapper pico;
  float div_array[] = {static_cast<float>(rng()), static_cast<float>(rng())};
  pio_sm_config config;

  FFF_RESET_HISTORY();
  RESET_FAKE(sm_config_set_clkdiv);

  // Trial call.
  for (auto &&div : div_array) pico.sm_config_set_clkdiv(&config, div);

  // Check the data from test spy. How many time called?
  ASSERT_EQ(sm_config_set_clkdiv_fake.call_count, 2);

  // Check wether parameters are passed collectly.
  int index = 0;
  for (auto &&div : div_array) {
    // Check the data from test spy. Call order.
    ASSERT_EQ(fff.call_history[index], (void *)sm_config_set_clkdiv);
    // Check the data from test spy. : Parameters.
    ASSERT_EQ(sm_config_set_clkdiv_fake.arg0_history[index], &config);
    ASSERT_EQ(sm_config_set_clkdiv_fake.arg1_history[index], div);
    index++;
  }

  RESET_FAKE(sm_config_set_clkdiv);
}  // TEST(SdkWrapper, sm_config_set_clkdiv)

TEST(SdkWrapper, sm_config_set_in_shift) {
  std::random_device rng;
  ::rpp_driver::SdkWrapper pico;
  pio_sm_config config;

  bool shift_right_array[] = {true, false};
  bool autopush_array[] = {true, false};
  uint push_threshold_array[] = {rng(), rng()};

  FFF_RESET_HISTORY();
  RESET_FAKE(sm_config_set_in_shift);

  // Trial call.
  for (auto &&shift_right : shift_right_array)
    for (auto &&autopush : autopush_array)
      for (auto &&push_threshold : push_threshold_array)
        pico.sm_config_set_in_shift(&config, shift_right, autopush,
                                    push_threshold);

  // Check the data from test spy. How many time called?
  ASSERT_EQ(sm_config_set_in_shift_fake.call_count, 8);

  // Check wether parameters are passed collectly.
  int index = 0;
  for (auto &&shift_right : shift_right_array)
    for (auto &&autopush : autopush_array)
      for (auto &&push_threshold : push_threshold_array) {
        // Check the data from test spy. Call order.
        ASSERT_EQ(fff.call_history[index], (void *)sm_config_set_in_shift);
        // Check the data from test spy. : Parameters.
        ASSERT_EQ(sm_config_set_in_shift_fake.arg0_history[index], &config);
        ASSERT_EQ(sm_config_set_in_shift_fake.arg1_history[index], shift_right);
        ASSERT_EQ(sm_config_set_in_shift_fake.arg2_history[index], autopush);
        ASSERT_EQ(sm_config_set_in_shift_fake.arg3_history[index],
                  push_threshold);
        index++;
      }

  RESET_FAKE(sm_config_set_in_shift);
}  // TEST(SdkWrapper, sm_config_set_in_shift)

TEST(SdkWrapper, sm_config_set_out_shift) {
  std::random_device rng;
  ::rpp_driver::SdkWrapper pico;
  pio_sm_config config;

  bool shift_right_array[] = {false, true};
  bool autopull_array[] = {false, true};
  uint push_threshold_array[] = {rng(), rng()};

  FFF_RESET_HISTORY();
  RESET_FAKE(sm_config_set_out_shift);

  // Trial call.
  for (auto &&shift_right : shift_right_array)
    for (auto &&autopull : autopull_array)
      for (auto &&push_threshold : push_threshold_array)
        pico.sm_config_set_out_shift(&config, shift_right, autopull,
                                     push_threshold);

  // Check the data from test spy. How many time called?
  ASSERT_EQ(sm_config_set_out_shift_fake.call_count, 8);

  // Check wether parameters are passed collectly.
  int index = 0;
  for (auto &&shift_right : shift_right_array)
    for (auto &&autopull : autopull_array)
      for (auto &&push_threshold : push_threshold_array) {
        // Check the data from test spy. Call order.
        ASSERT_EQ(fff.call_history[index], (void *)sm_config_set_out_shift);
        // Check the data from test spy. : Parameters.
        ASSERT_EQ(sm_config_set_out_shift_fake.arg0_history[index], &config);
        ASSERT_EQ(sm_config_set_out_shift_fake.arg1_history[index],
                  shift_right);
        ASSERT_EQ(sm_config_set_out_shift_fake.arg2_history[index], autopull);
        ASSERT_EQ(sm_config_set_out_shift_fake.arg3_history[index],
                  push_threshold);
        index++;
      }

  RESET_FAKE(sm_config_set_out_shift);
}  // TEST(SdkWrapper, sm_config_set_out_shift)

TEST(SdkWrapper, pio_sm_init) {
  std::random_device rng;
  ::rpp_driver::SdkWrapper pico;
  pio_sm_config config;

  PIO pio_array[] = {rng(), rng()};
  uint sm_array[] = {rng(), rng()};
  uint initial_pc_array[] = {rng(), rng()};
  int return_vals_array[32] = {1, 2, 3, 4, 5, 6, 7, 8};

  FFF_RESET_HISTORY();
  RESET_FAKE(pio_sm_init);

  SET_RETURN_SEQ(pio_sm_init, return_vals_array, std::size(return_vals_array));

  // Check wether return value is correct.
  int index = 0;
  for (auto &&pio : pio_array)
    for (auto &&sm : sm_array)
      for (auto &&initial_pc : initial_pc_array) {
        ASSERT_EQ(pico.pio_sm_init(pio, sm, initial_pc, &config),
                  return_vals_array[index]);
        index++;
      }

  // Check the data from test spy. How many time called?
  ASSERT_EQ(pio_sm_init_fake.call_count, 8);

  // Check wether parameters are passed collectly.
  index = 0;
  for (auto &&pio : pio_array)
    for (auto &&sm : sm_array)
      for (auto &&initial_pc : initial_pc_array) {
        // Check the data from test spy. Call order.
        ASSERT_EQ(fff.call_history[index], (void *)pio_sm_init);
        // Check the data from test spy. : Parameters.
        ASSERT_EQ(pio_sm_init_fake.arg0_history[index], pio);
        ASSERT_EQ(pio_sm_init_fake.arg1_history[index], sm);
        ASSERT_EQ(pio_sm_init_fake.arg2_history[index], initial_pc);
        ASSERT_EQ(pio_sm_init_fake.arg3_history[index], &config);
        index++;
      }

  RESET_FAKE(pio_sm_init);
}  // TEST(SdkWrapper, pio_sm_init)

TEST(SdkWrapper, pio_sm_put) {
  std::random_device rng;
  ::rpp_driver::SdkWrapper pico;
  pio_sm_config config;

  PIO pio_array[] = {rng(), rng()};
  uint sm_array[] = {rng(), rng()};
  uint32_t data_array[] = {11, 13};

  FFF_RESET_HISTORY();
  RESET_FAKE(pio_sm_put);

  // Trial call.
  for (auto &&pio : pio_array)
    for (auto &&sm : sm_array)
      for (auto &&data : data_array) pico.pio_sm_put(pio, sm, data);

  // Check the data from test spy. How many time called?
  ASSERT_EQ(pio_sm_put_fake.call_count, 8);

  // Check wether parameters are passed collectly.
  int index = 0;
  for (auto &&pio : pio_array)
    for (auto &&sm : sm_array)
      for (auto &&data : data_array) {
        // Check the data from test spy. Call order.
        ASSERT_EQ(fff.call_history[index], (void *)pio_sm_put);
        // Check the data from test spy. : Parameters.
        ASSERT_EQ(pio_sm_put_fake.arg0_history[index], pio);
        ASSERT_EQ(pio_sm_put_fake.arg1_history[index], sm);
        ASSERT_EQ(pio_sm_put_fake.arg2_history[index], data);
        index++;
      }

  RESET_FAKE(pio_sm_put);
}  // TEST(SdkWrapper, pio_sm_put)

TEST(SdkWrapper, pio_sm_put_blocking) {
  std::random_device rng;
  ::rpp_driver::SdkWrapper pico;
  pio_sm_config config;

  PIO pio_array[] = {rng(), rng()};
  uint sm_array[] = {rng(), rng()};
  uint32_t data_array[] = {11, 13};

  FFF_RESET_HISTORY();
  RESET_FAKE(pio_sm_put_blocking);

  // Trial call.
  for (auto &&pio : pio_array)
    for (auto &&sm : sm_array)
      for (auto &&data : data_array) pico.pio_sm_put_blocking(pio, sm, data);

  // Check the data from test spy. How many time called?
  ASSERT_EQ(pio_sm_put_blocking_fake.call_count, 8);

  // Check wether parameters are passed collectly.
  int index = 0;
  for (auto &&pio : pio_array)
    for (auto &&sm : sm_array)
      for (auto &&data : data_array) {
        // Check the data from test spy. Call order.
        ASSERT_EQ(fff.call_history[index], (void *)pio_sm_put_blocking);
        // Check the data from test spy. : Parameters.
        ASSERT_EQ(pio_sm_put_blocking_fake.arg0_history[index], pio);
        ASSERT_EQ(pio_sm_put_blocking_fake.arg1_history[index], sm);
        ASSERT_EQ(pio_sm_put_blocking_fake.arg2_history[index], data);
        index++;
      }

  RESET_FAKE(pio_sm_put_blocking);
}  // TEST(SdkWrapper, pio_sm_put_blocking)

TEST(SdkWrapper, pio_sm_set_enabled) {
  std::random_device rng;
  ::rpp_driver::SdkWrapper pico;
  pio_sm_config config;

  PIO pio_array[] = {rng(), rng()};
  uint sm_array[] = {rng(), rng()};
  bool enabled_array[] = {true, false};

  FFF_RESET_HISTORY();
  RESET_FAKE(pio_sm_set_enabled);

  // Trial call.
  for (auto &&pio : pio_array)
    for (auto &&sm : sm_array)
      for (auto &&enabled : enabled_array)
        pico.pio_sm_set_enabled(pio, sm, enabled);

  // Check the data from test spy. How many time called?
  ASSERT_EQ(pio_sm_set_enabled_fake.call_count, 8);

  // Check wether parameters are passed collectly.
  int index = 0;
  for (auto &&pio : pio_array)
    for (auto &&sm : sm_array)
      for (auto &&enabled : enabled_array) {
        // Check the data from test spy. Call order.
        ASSERT_EQ(fff.call_history[index], (void *)pio_sm_set_enabled);
        // Check the data from test spy. : Parameters.
        ASSERT_EQ(pio_sm_set_enabled_fake.arg0_history[index], pio);
        ASSERT_EQ(pio_sm_set_enabled_fake.arg1_history[index], sm);
        ASSERT_EQ(pio_sm_set_enabled_fake.arg2_history[index], enabled);
        index++;
      }

  RESET_FAKE(pio_sm_set_enabled);
}  // TEST(SdkWrapper, pio_sm_set_enabled)

TEST(SdkWrapper, pio_add_program) {
  std::random_device rng;
  ::rpp_driver::SdkWrapper pico;

  PIO pio_array[] = {rng(), rng()};
  pio_program_t dummy_program;
  int return_vals_array[32] = {1, 2};

  FFF_RESET_HISTORY();
  RESET_FAKE(pio_add_program);

  SET_RETURN_SEQ(pio_add_program, return_vals_array,
                 std::size(return_vals_array));

  // Check wether return value is correct.
  int index = 0;
  for (auto &&pio : pio_array) {
    ASSERT_EQ(pico.pio_add_program(pio, &dummy_program),
              return_vals_array[index]);
    index++;
  }

  // Check the data from test spy. How many time called?
  ASSERT_EQ(pio_add_program_fake.call_count, 2);

  // Check wether parameters are passed collectly.
  index = 0;
  for (auto &&pio : pio_array) {
    // Check the data from test spy. Call order.
    ASSERT_EQ(fff.call_history[index], (void *)pio_add_program);
    // Check the data from test spy. : Parameters.
    ASSERT_EQ(pio_add_program_fake.arg0_history[index], pio);
    ASSERT_EQ(pio_add_program_fake.arg1_history[index], &dummy_program);
    index++;
  }

  RESET_FAKE(pio_add_program);
}  // TEST(SdkWrapper, pio_add_program)

TEST(SdkWrapper, sm_config_set_jmp_pin) {
  std::random_device rng;
  ::rpp_driver::SdkWrapper pico;
  PIO pin_array[] = {rng(), rng()};
  pio_sm_config config;

  FFF_RESET_HISTORY();
  RESET_FAKE(sm_config_set_jmp_pin);

  // Trial call.
  for (auto &&count : pin_array) pico.sm_config_set_jmp_pin(&config, count);

  // Check the data from test spy. How many time called?
  ASSERT_EQ(sm_config_set_jmp_pin_fake.call_count, 2);

  // Check wether parameters are passed collectly.
  int index = 0;
  for (auto &&pin : pin_array) {
    // Check the data from test spy. Call order.
    ASSERT_EQ(fff.call_history[index], (void *)sm_config_set_jmp_pin);
    // Check the data from test spy. : Parameters.
    ASSERT_EQ(sm_config_set_jmp_pin_fake.arg0_history[index], &config);
    ASSERT_EQ(sm_config_set_jmp_pin_fake.arg1_history[index], pin);
    index++;
  }

  RESET_FAKE(sm_config_set_jmp_pin);
}  // TEST(SdkWrapper, sm_config_set_jmp_pin)

TEST(SdkWrapper, pio_sm_get) {
  std::random_device rng;
  ::rpp_driver::SdkWrapper pico;
  pio_sm_config config;

  PIO pio_array[] = {rng(), rng()};
  uint sm_array[] = {rng(), rng()};
  uint32_t return_vals_array[] = {1, 2, 3, 4};

  FFF_RESET_HISTORY();
  RESET_FAKE(pio_sm_get);

  SET_RETURN_SEQ(pio_sm_get, return_vals_array, std::size(return_vals_array));

  // Check wether return value is correct.
  int index = 0;
  for (auto &&pio : pio_array)
    for (auto &&sm : sm_array) {
      ASSERT_EQ(pico.pio_sm_get(pio, sm), return_vals_array[index]);
      index++;
    }

  // Check the data from test spy. How many time called?
  ASSERT_EQ(pio_sm_get_fake.call_count, 4);

  // Check wether parameters are passed collectly.
  index = 0;
  for (auto &&pio : pio_array)
    for (auto &&sm : sm_array) {
      // Check the data from test spy. Call order.
      ASSERT_EQ(fff.call_history[index], (void *)pio_sm_get);
      // Check the data from test spy. : Parameters.
      ASSERT_EQ(pio_sm_get_fake.arg0_history[index], pio);
      ASSERT_EQ(pio_sm_get_fake.arg1_history[index], sm);
      index++;
    }

  RESET_FAKE(pio_sm_get);
}  // TEST(SdkWrapper, pio_sm_get)

TEST(SdkWrapper, pio_sm_get_blocking) {
  std::random_device rng;
  ::rpp_driver::SdkWrapper pico;
  pio_sm_config config;

  PIO pio_array[] = {rng(), rng()};
  uint sm_array[] = {rng(), rng()};
  uint32_t return_vals_array[] = {1, 2, 3, 4};

  FFF_RESET_HISTORY();
  RESET_FAKE(pio_sm_get_blocking);

  SET_RETURN_SEQ(pio_sm_get_blocking, return_vals_array,
                 std::size(return_vals_array));

  // Check wether return value is correct.
  int index = 0;
  for (auto &&pio : pio_array)
    for (auto &&sm : sm_array) {
      ASSERT_EQ(pico.pio_sm_get_blocking(pio, sm), return_vals_array[index]);
      index++;
    }

  // Check the data from test spy. How many time called?
  ASSERT_EQ(pio_sm_get_blocking_fake.call_count, 4);

  // Check wether parameters are passed collectly.
  index = 0;
  for (auto &&pio : pio_array)
    for (auto &&sm : sm_array) {
      // Check the data from test spy. Call order.
      ASSERT_EQ(fff.call_history[index], (void *)pio_sm_get_blocking);
      // Check the data from test spy. : Parameters.
      ASSERT_EQ(pio_sm_get_blocking_fake.arg0_history[index], pio);
      ASSERT_EQ(pio_sm_get_blocking_fake.arg1_history[index], sm);
      index++;
    }

  RESET_FAKE(pio_sm_get_blocking);
}  // TEST(SdkWrapper, pio_sm_get_blocking)
TEST(SdkWrapper, pio_sm_claim) {
  std::random_device rng;
  ::rpp_driver::SdkWrapper pico;
  pio_sm_config config;

  PIO pio_array[] = {rng(), rng()};
  uint sm_array[] = {rng(), rng()};

  FFF_RESET_HISTORY();
  RESET_FAKE(pio_sm_claim);

  // Trial call.
  for (auto &&pio : pio_array)
    for (auto &&sm : sm_array) pico.pio_sm_claim(pio, sm);

  // Check the data from test spy. How many time called?
  ASSERT_EQ(pio_sm_claim_fake.call_count, 4);

  // Check wether parameters are passed collectly.
  int index = 0;
  for (auto &&pio : pio_array)
    for (auto &&sm : sm_array) {
      // Check the data from test spy. Call order.
      ASSERT_EQ(fff.call_history[index], (void *)pio_sm_claim);
      // Check the data from test spy. : Parameters.
      ASSERT_EQ(pio_sm_claim_fake.arg0_history[index], pio);
      ASSERT_EQ(pio_sm_claim_fake.arg1_history[index], sm);
      index++;
    }

  RESET_FAKE(pio_sm_claim);

}  // TEST(SdkWrapper, pio_sm_claim)

TEST(SdkWrapper, pio_sm_unclaim) {
  std::random_device rng;
  ::rpp_driver::SdkWrapper pico;
  pio_sm_config config;

  PIO pio_array[] = {rng(), rng()};
  uint sm_array[] = {rng(), rng()};

  FFF_RESET_HISTORY();
  RESET_FAKE(pio_sm_unclaim);

  // Trial call.
  for (auto &&pio : pio_array)
    for (auto &&sm : sm_array) pico.pio_sm_unclaim(pio, sm);

  // Check the data from test spy. How many time called?
  ASSERT_EQ(pio_sm_unclaim_fake.call_count, 4);

  // Check wether parameters are passed collectly.
  int index = 0;
  for (auto &&pio : pio_array)
    for (auto &&sm : sm_array) {
      // Check the data from test spy. Call order.
      ASSERT_EQ(fff.call_history[index], (void *)pio_sm_unclaim);
      // Check the data from test spy. : Parameters.
      ASSERT_EQ(pio_sm_unclaim_fake.arg0_history[index], pio);
      ASSERT_EQ(pio_sm_unclaim_fake.arg1_history[index], sm);
      index++;
    }

  RESET_FAKE(pio_sm_unclaim);

}  // TEST(SdkWrapper, pio_sm_unclaim)

TEST(SdkWrapper, pio_claim_unused_sm) {
  std::random_device rng;
  ::rpp_driver::SdkWrapper pico;
  PIO pio_array[] = {rng(), rng()};
  bool required_array[2] = {true, false};
  int return_vals_array[] = {1, 2, 3, 4};

  FFF_RESET_HISTORY();
  RESET_FAKE(pio_claim_unused_sm);

  SET_RETURN_SEQ(pio_claim_unused_sm, return_vals_array,
                 std::size(return_vals_array));

  // Check wether return value is correct.
  int index = 0;
  for (auto &&pio : pio_array)
    for (auto &&required : required_array) {
      ASSERT_EQ(pico.pio_claim_unused_sm(pio, required),
                return_vals_array[index]);
      index++;
    }

  // Check the data from test spy. How many time called?
  ASSERT_EQ(pio_claim_unused_sm_fake.call_count, 4);

  // Check wether parameters are passed collectly.
  index = 0;
  for (auto &&pio : pio_array)
    for (auto &&required : required_array) {
      // Check the data from test spy. Call order.
      ASSERT_EQ(fff.call_history[index], (void *)pio_claim_unused_sm);
      // Check the data from test spy. : Parameters.
      ASSERT_EQ(pio_claim_unused_sm_fake.arg0_history[index], pio);
      ASSERT_EQ(pio_claim_unused_sm_fake.arg1_history[index], required);
      index++;
    }

  RESET_FAKE(pio_claim_unused_sm);
}  // TEST(SdkWrapper, pio_claim_unused_sm)

TEST(SdkWrapper, pio_sm_is_claimed) {
  std::random_device rng;
  ::rpp_driver::SdkWrapper pico;
  PIO pio_array[] = {rng(), rng()};
  uint sm_array[2] = {rng(), rng()};
  bool return_vals_array[] = {true, false, false, true};

  FFF_RESET_HISTORY();
  RESET_FAKE(pio_sm_is_claimed);

  SET_RETURN_SEQ(pio_sm_is_claimed, return_vals_array,
                 std::size(return_vals_array));

  // Check wether return value is correct.
  int index = 0;
  for (auto &&pio : pio_array)
    for (auto &&sm : sm_array) {
      ASSERT_EQ(pico.pio_sm_is_claimed(pio, sm), return_vals_array[index]);
      index++;
    }

  // Check the data from test spy. How many time called?
  ASSERT_EQ(pio_sm_is_claimed_fake.call_count, 4);

  // Check wether parameters are passed collectly.
  index = 0;
  for (auto &&pio : pio_array)
    for (auto &&sm : sm_array) {
      // Check the data from test spy. Call order.
      ASSERT_EQ(fff.call_history[index], (void *)pio_sm_is_claimed);
      // Check the data from test spy. : Parameters.
      ASSERT_EQ(pio_sm_is_claimed_fake.arg0_history[index], pio);
      ASSERT_EQ(pio_sm_is_claimed_fake.arg1_history[index], sm);
      index++;
    }

  RESET_FAKE(pio_sm_is_claimed);
}  // TEST(SdkWrapper, pio_sm_is_claimed)

TEST(SdkWrapper, pio_sm_clear_fifos) {
  std::random_device rng;
  ::rpp_driver::SdkWrapper pico;
  pio_sm_config config;

  PIO pio_array[] = {rng(), rng()};
  uint sm_array[] = {rng(), rng()};

  FFF_RESET_HISTORY();
  RESET_FAKE(pio_sm_clear_fifos);

  // Trial call.
  for (auto &&pio : pio_array)
    for (auto &&sm : sm_array) pico.pio_sm_clear_fifos(pio, sm);

  // Check the data from test spy. How many time called?
  ASSERT_EQ(pio_sm_clear_fifos_fake.call_count, 4);

  // Check wether parameters are passed collectly.
  int index = 0;
  for (auto &&pio : pio_array)
    for (auto &&sm : sm_array) {
      // Check the data from test spy. Call order.
      ASSERT_EQ(fff.call_history[index], (void *)pio_sm_clear_fifos);
      // Check the data from test spy. : Parameters.
      ASSERT_EQ(pio_sm_clear_fifos_fake.arg0_history[index], pio);
      ASSERT_EQ(pio_sm_clear_fifos_fake.arg1_history[index], sm);
      index++;
    }

  RESET_FAKE(pio_sm_clear_fifos);

}  // TEST(SdkWrapper, pio_sm_clear_fifos)
