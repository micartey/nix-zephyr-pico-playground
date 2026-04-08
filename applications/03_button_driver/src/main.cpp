#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include "zephyr/device.h"
#include "button.h"
#include "bootloader_cmd.h"
#include "zephyr/sys/printk.h"

// Constants
static const int32_t sleep_time_ms = 100;

static const struct device *btn_1 = DEVICE_DT_GET(DT_ALIAS(my_button_1));
static const struct device *btn_2 = DEVICE_DT_GET(DT_ALIAS(my_button_2));

int main(void)
{
    printk("Starting...\r\n");

    int ret;
    uint8_t state1;
    uint8_t state2;

    if (!(device_is_ready(btn_1) && device_is_ready(btn_2))) {
        printk("Error: buttons are not ready!\r\n");
        return -1;
    }

    const struct button_api *btn_api = (const struct button_api *)btn_1->api; // Just a pointer to a function

    while (1) {
        ret = btn_api->get(btn_1, &state1);
        if (ret < 0) {
            printk("Error (%d): failed to read button 1 pin\r\n", ret);
            continue;
        }

        ret = btn_api->get(btn_2, &state2);
        if (ret < 0) {
            printk("Error (%d): failed to read button 1 pin\r\n", ret);
            continue;
        }

        printk("%d %d \r\n", state1, state2);

        k_sleep(K_MSEC(sleep_time_ms));
    }

    (void)cmd_bootloader; // Prevent LSP error
    return 0;
}
