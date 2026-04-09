#include "zephyr/kernel/thread.h"
#include "zephyr/kernel/thread_stack.h"
#include "zephyr/logging/log.h"
#include "zephyr/sys/printk.h"
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

const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);

void blink_thread_start(void *arg_1, void *arg_2, void *arg_3) {
    int ret;
    int state = 0;

    while (1) {
        state = !state;

        ret = gpio_pin_set_dt(&led, state);
        if (ret < 0) {
            LOG_ERR("Error(%d): could not toggle pin", ret);
        }

        k_sleep(K_MSEC(500));
    }
}

int main(void) {
    if (!gpio_is_ready_dt(&led)) {
        LOG_ERR("Could not init led gpio");
        return -ENODEV;
    }

    gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);

    k_sleep(K_SECONDS(2)); // Led device stabalize

    printk("Starting...\r\n");

    /*
     * Note about priority:
     *
     * 1. The priority is always higher when the number is lower, meaning a thread with a "low" priority level
     *  has an actual higher priority!
     *
     * 2. When the number becomes negative the thread is "cooperative" which means it will only be suspended/interrupted when it finishes!
     *  It decides itself when it finishes!
     *
     * 3. A non-negative priority is a preemptible thread.
     *  It can be forcefully be interrupted at any point without the threads consent!
     */

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

    k_sleep(K_MSEC(5000));
    k_thread_suspend(blink_tid); // This only works because the thread is of positive priority -> Preemptible

    printk("Should be suspended now!\r\n");

    k_sleep(K_MSEC(10000));

    printk("And continue now!\r\n");
    k_thread_resume(blink_tid);


    while (true) {
        k_sleep(K_FOREVER);
    }

    (void)cmd_bootloader;
    return 0;
}
