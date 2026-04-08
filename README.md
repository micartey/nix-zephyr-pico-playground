# zephyr pico

https://docs.zephyrproject.org/latest/introduction/index.html

1. Figure out why zephyr may be used

> Zephyr uses Devicetree to describe hardware. Your application code calls generic APIs like gpio_pin_set(). To move to a different board, you just change the build target. The OS handles the underlying register-level differences

2. Why not use micro python or tinygo

> Zephyr is a Real-Time OS (RTOS). You have precise control over thread priorities. MicroPython has a "Garbage Collector" that can pause your code at random times to clean up memory—this is a dealbreaker for timing-critical tasks (like high-speed motor control)

> Zephyr has an incredibly sophisticated power management subsystem. It can automatically put the CPU into deep sleep between RTOS ticks and wake up only when a timer or interrupt fires. Doing this manually in Go or Python is significantly harder and less efficient

> MicroPython needs ~256KB Flash and 16KB RAM just to start. Zephyr can be stripped down to run on tiny chips with <32KB RAM while still providing multi-threading

<!--

### Threats

#### Priority-Based Preemption

Zephyr uses a priority-based scheduler. To ensure an "Important Task" always runs immediately when needed, you assign it a Negative Priority.

- **Cooperative Threads (Negative Priorities):** These are the most critical. Once they start running, the scheduler will not swap them out for any "normal" thread (Preemptive threads), regardless of what is happening.

- **Preemptive Threads (Positive Priorities):** These are for your background tasks like Bluetooth or logging.

#### High-Precision Timing with `k_timer`

If your task must run every X milliseconds exactly, use a hardware-backed kernel timer.

```cpp
struct k_timer my_timer;

void my_expiry_function(struct k_timer *timer_id) {
    /* This runs in ISR (Interrupt) context! */
    /* Highest possible priority. Handle the deadline here 
       or give a semaphore to a high-priority thread. */
}

k_timer_init(&my_timer, my_expiry_function, NULL);
k_timer_start(&my_timer, K_MSEC(10), K_MSEC(10)); // 100Hz fixed frequency
```

#### The "Deadline" Pattern: Semaphore + High-Priority Thread

For complex tasks that take too long for an Interrupt Service Routine (ISR) but have a strict deadline, use the Semaphore synchronization pattern.

```cpp
#define MY_PRIORITY -5  // High-priority cooperative thread
K_SEM_DEFINE(deadline_sem, 0, 1);

void important_task_thread(void) {
    while (1) {
        // Wait here until the timer or hardware trigger fires
        k_sem_take(&deadline_sem, K_FOREVER);

        // --- Start of Critical Task ---
        do_realtime_math(); 
        drive_hardware_pins();
        // --- End of Critical Task ---
    }
}

// Define and start the thread at boot
K_THREAD_DEFINE(my_tid, 1024, important_task_thread, NULL, NULL, NULL, MY_PRIORITY, 0, 0);
```

#### Zero-Latency Interrupts

If your deadline is in the microsecond range (e.g., stopping a motor before it hits a physical limit), Zephyr allows you to bypass the kernel entirely using BT_ISR_DIRECT_DECLARE. This creates a direct connection between the hardware vector and your code, skipping the RTOS scheduler overhead.
