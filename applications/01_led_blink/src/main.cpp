#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>

#include "bootloader_cmd.h"

int main(void) {
  const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);

  if (!gpio_is_ready_dt(&led)) {
    return -1;
  }

  gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);

  while (true) {
    gpio_pin_toggle_dt(&led);
    k_msleep(1000);
  }

  return 0;
}
