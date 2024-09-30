#include "gpiobasic.hpp"

rpp_driver::GpioBasic::GpioBasic(SdkWrapper& sdk, uint pin)
    : sdk_(sdk), pin_(pin) {
  sdk_.gpio_init(pin_);
}

rpp_driver::GpioBasic::~GpioBasic() { sdk_.gpio_deinit(pin_); }

void rpp_driver::GpioBasic::SetDir(bool out) { sdk_.gpio_set_dir(pin_, out); }

void rpp_driver::GpioBasic::SetInputEnabled(bool enabled) {
  sdk_.gpio_set_input_enabled(pin_, enabled);
}

void rpp_driver::GpioBasic::Put(bool value) { sdk_.gpio_put(pin_, value); }

bool rpp_driver::GpioBasic::Get() { return sdk_.gpio_get(pin_); }

void rpp_driver::GpioBasic::Toggle() { Put(!Get()); }

void rpp_driver::GpioBasic::PullUp() { sdk_.gpio_pull_up(pin_); }

void rpp_driver::GpioBasic::PullDown() { sdk_.gpio_pull_down(pin_); }

void rpp_driver::GpioBasic::DisablePulls() { sdk_.gpio_disable_pulls(pin_); }
