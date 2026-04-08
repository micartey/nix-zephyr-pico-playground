#include "blink.hpp"

#include <errno.h>

namespace {
const gpio_dt_spec *g_led;

void timer_expiry(struct k_timer *timer) {
  ARG_UNUSED(timer);
  gpio_pin_toggle_dt(g_led);
}
}  // namespace

int blink_setup(const gpio_dt_spec &led) {
  if (!gpio_is_ready_dt(&led)) {
    return -ENODEV;
  }

  return gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
}

int blink_once(const gpio_dt_spec &led, int interval_ms) {
  gpio_pin_toggle_dt(&led);
  k_msleep(interval_ms);
  gpio_pin_toggle_dt(&led);
  k_msleep(10);
  return 0;
}

void blink_timer_start(const gpio_dt_spec &led, struct k_timer *timer,
                       k_timeout_t delay, k_timeout_t period) {

  g_led = &led;
  k_timer_init(timer, timer_expiry, NULL);
  k_timer_start(timer, delay, period);
}
