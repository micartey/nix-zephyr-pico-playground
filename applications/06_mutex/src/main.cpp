#include "zephyr/kernel/thread.h"
#include "zephyr/kernel/thread_stack.h"
#include "zephyr/logging/log.h"
#include "zephyr/sys/printk.h"
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <memory>
#include <memory_resource>
#include <print>
#include <vector>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/shell/shell.h>

#include "bootloader_cmd.h"

LOG_MODULE_REGISTER(main);

// Stack size settings
#define BLINK_THREAD_STACK_SIZE 256

K_THREAD_STACK_DEFINE(blink_stack, BLINK_THREAD_STACK_SIZE);

static struct k_thread blink_thread;

// ---

K_MUTEX_DEFINE(my_mutex);

static int32_t sleep_rate = 500;

const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);

void blink_thread_start(void *arg_1, void *arg_2, void *arg_3) {
    int ret;
    int state = 0;

    while (1) {
        state = !state;

        // Very useless read but ok
        k_mutex_lock(&my_mutex, K_FOREVER);
        auto delay = std::max(sleep_rate, 0); // Actually 0 will starve the shell cmd as the main thread will also run indefinetly
        k_mutex_unlock(&my_mutex);

        ret = gpio_pin_set_dt(&led, state);
        if (ret < 0) {
            LOG_ERR("Error(%d): could not toggle pin", ret);
        }

        k_sleep(K_MSEC(delay));
    }
}

int main(void) {
    SHELL_CMD_REGISTER(plus, NULL, "Increase sleep time", +[](const struct shell *sh, int argc, char **argv) -> int {
        k_mutex_lock(&my_mutex, K_FOREVER);

        sleep_rate += 200;

        k_mutex_unlock(&my_mutex);
        return 0;
    });

    SHELL_CMD_REGISTER(minus, NULL, "Decrease sleep time", +[](const struct shell *sh, int argc, char **argv) -> int {
        k_mutex_lock(&my_mutex, K_FOREVER);

        sleep_rate -= 200;

        k_mutex_unlock(&my_mutex);
        return 0;
    });

    if (!gpio_is_ready_dt(&led)) {
        LOG_ERR("Could not init led gpio");
        return -ENODEV;
    }

    gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);

    k_sleep(K_SECONDS(2)); // Led device stabalize

    printk("Starting...\r\n");

    auto blink_tid = k_thread_create(
        &blink_thread,                      // Thread
        blink_stack,                        // Stack
        K_THREAD_STACK_SIZEOF(blink_stack), // Size of Stack?
        blink_thread_start,                 // entrypoint
        NULL,                               // Pointer of value (the value is free to use)
        NULL,
        NULL,
        3,                                  // Priority -> Non Negative -> Preemptible
        0,                                  // Options (ToC: Thread Options)
        K_NO_WAIT                           // Startup delay
    );
    k_thread_name_set(blink_tid, "Blink Thread");


    while (true) {
        k_sleep(K_FOREVER);
    }

    (void)cmd_bootloader;
    return 0;
}
