/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

// The original of this file is found at :
// https://github.com/raspberrypi/pico-examples/blob/master/blink/blink.c

// Un-comment the desired include line to use the peripheral API.

// #include <hardware/adc.h>
// #include <hardware/claim.h>
// #include <hardware/clocks.h>
// #include <hardware/divider.h>
// #include <hardware/dma.h>
// #include <hardware/flash.h>
// #include <hardware/gpio.h>
// #include <hardware/i2c.h>
// #include <hardware/interp.h>
// #include <hardware/irq.h>
// #include <hardware/pio.h>
// #include <hardware/pll.h>
// #include <hardware/pwm.h>
// #include <hardware/resets.h>
// #include <hardware/rtc.h>
// #include <hardware/spi.h>
// #include <hardware/sync.h>
// #include <hardware/timer.h>
// #include <hardware/uart.h>
// #include <hardware/vreg.h>
// #include <hardware/watchdog.h>
// #include <hardware/xosc.h>

#include "pico/stdlib.h"

int main() {
#ifndef PICO_DEFAULT_LED_PIN
#warning blink example requires a board with a regular LED
#else
  const uint LED_PIN = PICO_DEFAULT_LED_PIN;
  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);
  while (true) {
    gpio_put(LED_PIN, 1);
    sleep_ms(250);
    gpio_put(LED_PIN, 0);
    sleep_ms(250);
  }
#endif
}
