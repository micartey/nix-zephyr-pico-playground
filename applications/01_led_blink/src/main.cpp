#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/shell/shell.h>
#include "zephyr/sys/printk.h"

#include "bootloader_cmd.h"
#include "blink.hpp"

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

  if (blink_setup(led) != 0) {
    return -1;
  }

  static struct k_timer blink_timer;

  blink_timer_start(led, &blink_timer, K_SECONDS(1), K_SECONDS(1));

  while (true) {
    k_sleep(K_FOREVER);
  }

  (void)cmd_bootloader;
  return 0;
}
