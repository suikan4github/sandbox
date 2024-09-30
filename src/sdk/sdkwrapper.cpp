#include "sdkwrapper.hpp"

void ::rpp_driver::SdkWrapper::sleep_ms(uint32_t ms) { ::sleep_ms(ms); }

uint32_t rpp_driver::SdkWrapper::clock_get_hz(clock_handle_t clock) {
  return ::clock_get_hz(clock);
}

bool rpp_driver::SdkWrapper::stdio_init_all(void) { return ::stdio_init_all(); }

void rpp_driver::SdkWrapper::sm_config_set_clkdiv(pio_sm_config* c, float div) {
  ::sm_config_set_clkdiv(c, div);
}

void rpp_driver::SdkWrapper::sm_config_set_in_shift(pio_sm_config* c,
                                                    bool shift_right,
                                                    bool autopush,
                                                    uint push_threshold) {
  ::sm_config_set_in_shift(c, shift_right, autopush, push_threshold);
}

void rpp_driver::SdkWrapper::sm_config_set_out_shift(pio_sm_config* c,
                                                     bool shift_right,
                                                     bool autopull,
                                                     uint push_threshold) {
  ::sm_config_set_out_shift(c, shift_right, autopull, push_threshold);
}

int rpp_driver::SdkWrapper::pio_sm_init(PIO pio, uint sm, uint initial_pc,
                                        const pio_sm_config* config) {
  return ::pio_sm_init(pio, sm, initial_pc, config);
}

void rpp_driver::SdkWrapper::pio_sm_put(PIO pio, uint sm, uint32_t data) {
  ::pio_sm_put(pio, sm, data);
}

void rpp_driver::SdkWrapper::pio_sm_put_blocking(PIO pio, uint sm,
                                                 uint32_t data) {
  ::pio_sm_put_blocking(pio, sm, data);
}

void rpp_driver::SdkWrapper::pio_sm_set_enabled(PIO pio, uint sm,
                                                bool enabled) {
  ::pio_sm_set_enabled(pio, sm, enabled);
}

int rpp_driver::SdkWrapper::pio_add_program(PIO pio,
                                            const pio_program_t* program) {
  return ::pio_add_program(pio, program);
}

void ::rpp_driver::SdkWrapper::gpio_init(uint gpio) { ::gpio_init(gpio); }

void rpp_driver::SdkWrapper::gpio_deinit(uint gpio) { ::gpio_deinit(gpio); }

void rpp_driver::SdkWrapper::gpio_set_function(uint gpio, gpio_function_t fn) {
  ::gpio_set_function(gpio, fn);
}

void ::rpp_driver::SdkWrapper::gpio_set_dir(uint gpio, bool out) {
  ::gpio_set_dir(gpio, out);
}

void rpp_driver::SdkWrapper::gpio_set_input_enabled(uint gpio, bool enabled) {
  ::gpio_set_input_enabled(gpio, enabled);
}

void ::rpp_driver::SdkWrapper::gpio_put(uint gpio, bool value) {
  ::gpio_put(gpio, value);
}

bool ::rpp_driver::SdkWrapper::gpio_get(uint gpio) { return ::gpio_get(gpio); }

void ::rpp_driver::SdkWrapper::gpio_pull_up(uint gpio) { ::gpio_pull_up(gpio); }

void rpp_driver::SdkWrapper::gpio_pull_down(uint gpio) {
  ::gpio_pull_down(gpio);
}

void rpp_driver::SdkWrapper::gpio_disable_pulls(uint gpio) {
  ::gpio_disable_pulls(gpio);
}

// To avoid the ill formed program, the global :: scope resolution operator
// is removed in the following line.
// See also https://cplusplus.github.io/CWG/issues/1828.html
uint rpp_driver::SdkWrapper::i2c_init(i2c_inst_t* i2c, uint baudrate) {
  return ::i2c_init(i2c, baudrate);
}

void ::rpp_driver::SdkWrapper::i2c_deinit(i2c_inst_t* i2c) {
  ::i2c_deinit(i2c);
}

int ::rpp_driver::SdkWrapper::i2c_read_blocking(i2c_inst_t* i2c, uint8_t addr,
                                                uint8_t* dst, size_t len,
                                                bool nostop) {
  return ::i2c_read_blocking(i2c, addr, dst, len, nostop);
}

int ::rpp_driver::SdkWrapper::i2c_write_blocking(i2c_inst_t* i2c, uint8_t addr,
                                                 const uint8_t* src, size_t len,
                                                 bool nostop) {
  return ::i2c_write_blocking(i2c, addr, src, len, nostop);
}

void rpp_driver::SdkWrapper::sm_config_set_jmp_pin(pio_sm_config* c, uint pin) {
  ::sm_config_set_jmp_pin(c, pin);
}

uint32_t rpp_driver::SdkWrapper::pio_sm_get(PIO pio, uint sm) {
  return ::pio_sm_get(pio, sm);
}

uint32_t rpp_driver::SdkWrapper::pio_sm_get_blocking(PIO pio, uint sm) {
  return ::pio_sm_get_blocking(pio, sm);
}

void rpp_driver::SdkWrapper::pio_sm_claim(PIO pio, uint sm) {
  ::pio_sm_claim(pio, sm);
}

void rpp_driver::SdkWrapper::pio_sm_unclaim(PIO pio, uint sm) {
  ::pio_sm_unclaim(pio, sm);
}

int rpp_driver::SdkWrapper::pio_claim_unused_sm(PIO pio, bool required) {
  return ::pio_claim_unused_sm(pio, required);
}

bool rpp_driver::SdkWrapper::pio_sm_is_claimed(PIO pio, uint sm) {
  return ::pio_sm_is_claimed(pio, sm);
}

void rpp_driver::SdkWrapper::pio_sm_clear_fifos(PIO pio, uint sm) {
  ::pio_sm_clear_fifos(pio, sm);
}

int rpp_driver::SdkWrapper::pio_sm_set_consecutive_pindirs(PIO pio, uint sm,
                                                           uint pins_base,
                                                           uint pin_count,
                                                           bool is_out) {
  return ::pio_sm_set_consecutive_pindirs(pio, sm, pins_base, pin_count,
                                          is_out);
}

void rpp_driver::SdkWrapper::pio_gpio_init(PIO pio, uint pin) {
  ::pio_gpio_init(pio, pin);
}

void rpp_driver::SdkWrapper::sm_config_set_out_pins(pio_sm_config* c,
                                                    uint out_base,
                                                    uint out_count) {
  ::sm_config_set_out_pins(c, out_base, out_count);
}

void rpp_driver::SdkWrapper::sm_config_set_in_pin_base(pio_sm_config* c,
                                                       uint in_base) {
  ::sm_config_set_in_pin_base(c, in_base);
}

void rpp_driver::SdkWrapper::sm_config_set_in_pin_count(pio_sm_config* c,
                                                        uint in_count) {
  ::sm_config_set_in_pin_count(c, in_count);
}
