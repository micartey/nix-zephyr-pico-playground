#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/usb/usbd.h>
#include <zephyr/usb/class/usbd_hid.h>
#include <zephyr/usb/class/hid.h>
#include <zephyr/drivers/uart.h>
#include <string.h>

#include "usb_hid_keyboard.h"
#include "hid_keycodes.h"

#if defined(CONFIG_HID_KEYBOARD)

/* USB Device context */
USBD_DEVICE_DEFINE(usbd_kb,
                   DEVICE_DT_GET(DT_NODELABEL(zephyr_udc0)),
                   0x2FE3, 0x0100);

USBD_DESC_LANG_DEFINE(lang);
USBD_DESC_MANUFACTURER_DEFINE(mfr, "Zephyr Project");
USBD_DESC_PRODUCT_DEFINE(product, "HID Keyboard + CDC");
USBD_DESC_CONFIG_DEFINE(fs_cfg, "Config");

USBD_CONFIGURATION_DEFINE(fs_config, USB_SCD_SELF_POWERED, 100, &fs_cfg);

/* HID Report Descriptor */
static const uint8_t hid_report_desc[] = {
    HID_USAGE_PAGE(HID_USAGE_GEN_DESKTOP),
    HID_USAGE(HID_USAGE_GEN_DESKTOP_KEYBOARD),
    HID_COLLECTION(HID_COLLECTION_APPLICATION),
    HID_USAGE_PAGE(HID_USAGE_GEN_KEYBOARD),
    HID_USAGE_MIN8(224), HID_USAGE_MAX8(231),
    HID_LOGICAL_MIN8(0), HID_LOGICAL_MAX8(1),
    HID_REPORT_SIZE(1), HID_REPORT_COUNT(8),
    HID_INPUT(2),
    HID_REPORT_COUNT(1), HID_REPORT_SIZE(8),
    HID_INPUT(1),
    HID_REPORT_COUNT(5), HID_REPORT_SIZE(1),
    HID_USAGE_PAGE(HID_USAGE_GEN_LEDS),
    HID_USAGE_MIN8(1), HID_USAGE_MAX8(5),
    HID_OUTPUT(2),
    HID_REPORT_COUNT(1), HID_REPORT_SIZE(3),
    HID_OUTPUT(1),
    HID_REPORT_COUNT(6), HID_REPORT_SIZE(8),
    HID_LOGICAL_MIN8(0), HID_LOGICAL_MAX8(101),
    HID_USAGE_PAGE(HID_USAGE_GEN_KEYBOARD),
    HID_USAGE_MIN8(0), HID_USAGE_MAX8(101),
    HID_INPUT(0),
    HID_END_COLLECTION,
};

/* State */
static struct {
    uint8_t led_raw;
    bool ready;
} kb_state;

static const struct device *hid_dev;

/* HID callbacks */
static int kb_get_report(const struct device *dev,
                       const uint8_t type, const uint8_t id,
                       const uint16_t len, uint8_t *const buf)
{
    return 0;
}

static int kb_set_report(const struct device *dev,
                         const uint8_t type, const uint8_t id,
                         const uint16_t len, const uint8_t *const buf)
{
    if (type == HID_REPORT_TYPE_OUTPUT && buf && len > 0) {
        kb_state.led_raw = buf[0];
    }
    return 0;
}

static void kb_set_protocol(const struct device *dev, const uint8_t proto)
{
    printk("[USB] set_protocol: %d\n", proto);
}

static void kb_iface_ready(const struct device *dev, const bool ready)
{
    printk("[USB] kb_iface_ready: ready=%d\n", ready);
    kb_state.ready = ready;
}

static struct hid_device_ops kb_ops = {
    .iface_ready = kb_iface_ready,
    .get_report = kb_get_report,
    .set_report = kb_set_report,
    .set_protocol = kb_set_protocol,
};

/* Public API */
int hid_keyboard_send_key(uint8_t key)
{
    uint8_t report[8] = {0};

    if (!kb_state.ready) {
        return -ENODEV;
    }

    report[2] = key;
    hid_device_submit_report(hid_dev, sizeof(report), report);
    k_msleep(10);

    memset(report, 0, sizeof(report));
    hid_device_submit_report(hid_dev, sizeof(report), report);
    k_msleep(10);

    return 0;
}

bool hid_keyboard_is_connected(void)
{
    return kb_state.ready;
}

struct hid_keyboard_led_state hid_keyboard_get_led_state(void)
{
    struct hid_keyboard_led_state state = {0};
    state.num_lock = kb_state.led_raw & 0x01;
    state.caps_lock = kb_state.led_raw & 0x02;
    state.scroll_lock = kb_state.led_raw & 0x04;
    state.compose = kb_state.led_raw & 0x08;
    state.kana = kb_state.led_raw & 0x10;
    return state;
}

/* Initialization */
static int hid_keyboard_init(void)
{
    int ret;

    printk("[USB] HID init starting\n");

    hid_dev = DEVICE_DT_GET_ONE(zephyr_hid_device);
    printk("[USB] HID dev lookup: %p\n", hid_dev);
    if (!device_is_ready(hid_dev)) {
        printk("[USB] HID device not ready (dev=%p)\n", hid_dev);
        return -ENODEV;
    }
    printk("[USB] HID device ready (dev=%p)\n", hid_dev);

    ret = hid_device_register(hid_dev, hid_report_desc, sizeof(hid_report_desc), &kb_ops);
    if (ret == -EINVAL) {
        printk("[USB] HID register returned EINVAL - device tree may handle it, continuing...\n");
    } else if (ret) {
        printk("[USB] HID register failed: %d\n", ret);
        return ret;
    }
    printk("[USB] HID registered\n");

    ret = usbd_add_descriptor(&usbd_kb, &lang);
    if (ret) printk("[USB] lang desc: %d\n", ret);
    ret = usbd_add_descriptor(&usbd_kb, &mfr);
    if (ret) printk("[USB] mfr desc: %d\n", ret);
    ret = usbd_add_descriptor(&usbd_kb, &product);
    if (ret) printk("[USB] product desc: %d\n", ret);

    ret = usbd_add_configuration(&usbd_kb, USBD_SPEED_FS, &fs_config);
    if (ret) {
        printk("[USB] config failed: %d\n", ret);
        return ret;
    }
    printk("[USB] Config added\n");

    ret = usbd_register_all_classes(&usbd_kb, USBD_SPEED_FS, 1, NULL);
    if (ret) {
        printk("[USB] register classes failed: %d\n", ret);
        return ret;
    }
    printk("[USB] Classes registered\n");

    ret = usbd_init(&usbd_kb);
    if (ret) {
        printk("[USB] init failed: %d\n", ret);
        return ret;
    }
    printk("[USB] USBD init OK\n");

    ret = usbd_enable(&usbd_kb);
    if (ret) {
        printk("[USB] enable failed: %d\n", ret);
        return ret;
    }
    printk("[USB] USBD enabled\n");

    printk("[USB] HID Keyboard + CDC ACM ready\n");
    return 0;
}

SYS_INIT(hid_keyboard_init, POST_KERNEL, 50);

#endif
