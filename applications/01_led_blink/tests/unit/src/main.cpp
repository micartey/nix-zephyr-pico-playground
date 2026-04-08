#include <zephyr/ztest.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>

#include "blink.hpp"

ZTEST(blink_unit, test_blink_setup_ready) {
  const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
  zassert_equal(blink_setup(led), 0, "blink_setup should succeed on ready LED");
}

ZTEST(blink_unit, test_blink_once) {
  const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
  blink_setup(led);
  zassert_equal(blink_once(led, 1), 0, "blink_once should succeed");
}

ZTEST(blink_unit, test_blink_timer_start) {
  const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
  blink_setup(led);
  static struct k_timer timer;
  blink_timer_start(led, &timer, K_NO_WAIT, K_MSEC(100));
  k_msleep(50);
  k_timer_stop(&timer);
}

ZTEST_SUITE(blink_unit, NULL, NULL, NULL, NULL, NULL);