#pragma once

#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>

int blink_setup(const gpio_dt_spec &led);

int blink_once(const gpio_dt_spec &led, int interval_ms);

void blink_timer_start(const gpio_dt_spec &led, struct k_timer *timer,
                       k_timeout_t delay, k_timeout_t period);
