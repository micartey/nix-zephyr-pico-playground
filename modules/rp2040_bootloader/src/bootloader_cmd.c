#include <zephyr/kernel.h>
#include <zephyr/shell/shell.h>

#include "../../hal/rpi_pico/src/rp2_common/pico_bootrom/include/pico/bootrom.h"

#if defined(CONFIG_RP2040_BOOTLOADER)
static int cmd_bootloader(const struct shell *sh, int argc, char **argv) {
  ARG_UNUSED(argc);
  ARG_UNUSED(argv);

  shell_print(sh, "Rebooting to mass storage mode...");
  k_msleep(100);
  reset_usb_boot(0, 0);
  return 0;
}

SHELL_CMD_REGISTER(bootloader, NULL, "Reboot RP2040 into BOOTSEL mode",
                   cmd_bootloader);
#endif
