#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/shell/shell.h>
#include <zephyr/usb/usb_device.h>

extern "C" {
#include <pico/bootrom.h>
}

static int cmd_bootloader(const struct shell *sh, size_t argc, char **argv) {
  shell_print(sh, "Rebooting to mass storage mode...");
  k_msleep(100); // Allow time for the serial output to flush
  reset_usb_boot(0, 0);
  return 0;
}

SHELL_CMD_REGISTER(bootloader, NULL, "Reboot RP2040 into BOOTSEL mode",
                   cmd_bootloader);

int main(void) {
  if (usb_enable(NULL)) {
    return 0;
  }

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
