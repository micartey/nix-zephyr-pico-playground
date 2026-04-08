#ifndef ZEPHYR_DRIVERS_BUTTON_H_
#define ZEPHYR_DRIVERS_BUTTON_H_

#include <stdint.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>

struct button_api {
	int (*get)(const struct device *dev, uint8_t *state);
};

struct button_config {
	struct gpio_dt_spec btn;
	uint32_t id;
};

struct button_data {
	uint8_t _unused;
};

#endif /* ZEPHYR_DRIVERS_BUTTON_H_ */
