#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/gpio.h>

#include "bootloader_cmd.h"
#include "hid_keycodes.h"
#include "usb_hid_keyboard.h"
#include "zephyr/shell/shell.h"

int main(void)
{
    int timeout = 50;

    printk("[APP] Starting, waiting for HID (max 5s)...\n");
    while (!hid_keyboard_is_connected() && timeout > 0) {
        k_sleep(K_MSEC(100));
        timeout--;
    }

    if (hid_keyboard_is_connected()) {
        printk("[APP] HID connected!\n");
    } else {
        printk("[APP] HID NOT connected after timeout, trying anyway...\n");
    }

    int key_count = 0;
    while (true) {
        k_sleep(K_MSEC(5000));
        key_count++;

        printk("[APP] Sending key #%d...\n", key_count);
        int ret = hid_keyboard_send_key(HID_KEY_A);
        printk("[APP] result=%d, connected=%d\n", ret, hid_keyboard_is_connected());
    }

    (void)cmd_bootloader;
    return 0;
}
