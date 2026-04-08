#include <stdio.h>
#include <zephyr/ztest.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>

#include "blink.hpp"

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);

static void *suite_setup(void) {
  blink_setup(led);
  return NULL;
}

ZTEST(blink_integration, test_timer_starts_and_stops) {
  static struct k_timer timer;
  blink_timer_start(led, &timer, K_NO_WAIT, K_MSEC(10));

  k_msleep(50);

  k_timer_stop(&timer);
  zassert_true(true, "blink_timer_start→sleep→k_timer_stop should not crash");
}

ZTEST(blink_integration, test_blink_once_returns_success) {
  gpio_pin_toggle_dt(&led);
  int initial_state = gpio_pin_get_dt(&led);
  zassert_true(initial_state >= 0, "gpio_pin_get_dt should succeed");

  zassert_equal(blink_once(led, 10), 0, "blink_once should succeed");

  int final_state = gpio_pin_get_dt(&led);
  zassert_equal(final_state, initial_state, "blink_once should restore initial state");
}

ZTEST(blink_integration, test_setup_then_timer_full_flow) {
  const struct gpio_dt_spec led2 = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
  zassert_equal(blink_setup(led2), 0, "setup should succeed on ready LED");

  static struct k_timer flow_timer;
  blink_timer_start(led2, &flow_timer, K_MSEC(5), K_MSEC(5));

  k_msleep(30);

  int rc = gpio_pin_get_dt(&led2);
  zassert_true(rc >= 0, "gpio_pin_get_dt should succeed after timer activity");

  k_timer_stop(&flow_timer);
}

ZTEST(blink_integration, test_repeated_timer_cycles) {
  static struct k_timer timer;
  blink_timer_start(led, &timer, K_NO_WAIT, K_MSEC(5));

  k_msleep(100);
  k_timer_stop(&timer);

  zassert_equal(k_timer_remaining_get(&timer), 0,
                "stopped timer should have zero remaining");
}

ZTEST_SUITE(blink_integration, NULL, suite_setup, NULL, NULL, NULL);
