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

// -----------------------------

#define CYCLE_PERIOD_MS 1000
#define TASK_A_BUDGET_MS 20

/* Define synchronization semaphores for each task */
K_SEM_DEFINE(sync_sem_a, 0, 1);
K_SEM_DEFINE(sync_sem_b, 0, 1);
// ...

void cycle_timer_handler(struct k_timer *timer_id) {
    k_sem_give(&sync_sem_a);
    k_sem_give(&sync_sem_b);
    // ...
}

K_TIMER_DEFINE(cycle_timer, cycle_timer_handler, NULL);

// -----------------------------

void task_a_entry(void *p1, void *p2, void *p3) {
    while (1) {
        /* Block until the timer signals the start of a new 100ms cycle */
        k_sem_take(&sync_sem_a, K_FOREVER);

        uint32_t start_time = k_uptime_get_32();
        uint8_t index = 0;

        /*
         * Work loop
         * This is where the logic for thread_a will be executed and automatically stopped once the time slice
         *  is exhausted!
         */
        while ((k_uptime_get_32() - start_time) < TASK_A_BUDGET_MS) {
            printk("Hello World! %d \r\n", ++index);
            k_sleep(K_MSEC(10)); // We should not do that but this is easier to follow
        }

        /*
         * Budget exhausted. Do not call k_sleep().
         * Loop back and wait on the semaphore to stay locked to the timer's phase.
         * The scheduler will see that we are waiting for a semaphore and will schedule other threads
         */
    }
}

/*
 * Statically defined thread for task_a (Prio 5)
 * Other threads can be defined here as well and should share the same priority
 */
K_THREAD_DEFINE(task_a, 1024, task_a_entry, NULL, NULL, NULL, 5, 0, 0);

int main(void) {
    printk("Starting...\r\n");

    /*
     * This hardware timer gives the semaphores back the amount of locks (we could use mutex as well)
     * The threads are waiting for this to happen
     */
    k_timer_start(&cycle_timer, K_MSEC(CYCLE_PERIOD_MS), K_MSEC(CYCLE_PERIOD_MS));

    (void)cmd_bootloader;
    return 0;
}
