
#define DT_DRV_COMPAT custom_button

#include "button.h"
#include <errno.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

extern "C" {

LOG_MODULE_REGISTER(button);

static int button_init(const struct device *dev);
static int button_state_get(const struct device *dev, uint8_t *state);

static const struct button_api button_api_funcs = {
    .get = button_state_get,
};

#define BUTTON_INIT(n)                                                         \
                                                                               \
  static const struct button_config button_config_##n = {                      \
      .btn = GPIO_DT_SPEC_GET_OR(DT_DRV_INST(n), gpios, {0}),                  \
      .id = n,                                                                 \
  };                                                                           \
                                                                               \
  static struct button_data button_data_##n;                                   \
  DEVICE_DT_INST_DEFINE(n, button_init, NULL, &button_data_##n,                \
                        &button_config_##n, POST_KERNEL,                       \
                        CONFIG_GPIO_INIT_PRIORITY, &button_api_funcs);

DT_INST_FOREACH_STATUS_OKAY(BUTTON_INIT)

static int button_init(const struct device *dev) {
  int ret;
  const struct button_config *config =
      reinterpret_cast<const struct button_config *>(dev->config);

  LOG_DBG("Initializing button (Instance ID: %u)", config->id);

  if (!gpio_is_ready_dt(&config->btn)) {
    LOG_ERR("GPIO is not ready");
    return -ENODEV;
  }

  ret = gpio_pin_configure_dt(&config->btn, GPIO_INPUT);
  if (ret < 0) {
    LOG_ERR("Could not configure GPIO as input");
    return ret;
  }

  return 0;
}

static int button_state_get(const struct device *dev, uint8_t *state) {
  int ret;
  const struct button_config *config =
      reinterpret_cast<const struct button_config *>(dev->config);

  ret = gpio_pin_get_dt(&config->btn);
  if (ret < 0) {
    LOG_ERR("Error (%d): failed to read button", ret);
    return ret;
  }

  *state = static_cast<uint8_t>(ret);
  return 0;
}
}
