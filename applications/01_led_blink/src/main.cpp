#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/shell/shell.h>
#include "zephyr/sys/printk.h"

#include "bootloader_cmd.h"

static int cmd_hello(const struct shell *sh, int argc, char **argv) {
  printk("World!\n");
  return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(sub_demo,
        SHELL_CMD(ping, NULL, "Ping command.", +[](const struct shell *sh, int argc, char **argv) -> int {
            ARG_UNUSED(sh);
            ARG_UNUSED(argc);
            ARG_UNUSED(argv);

            printk("Pong!\n");
            return 0;
        }),
        SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(hello, &sub_demo, "Print \"World!\"", cmd_hello);

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
