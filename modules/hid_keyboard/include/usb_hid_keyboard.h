#ifndef USB_HID_KEYBOARD_H_
#define USB_HID_KEYBOARD_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief LED state structure
 */
struct hid_keyboard_led_state {
    uint8_t num_lock : 1;
    uint8_t caps_lock : 1;
    uint8_t scroll_lock : 1;
    uint8_t compose : 1;
    uint8_t kana : 1;
    uint8_t reserved : 3;
};

/**
 * @brief Send a single keystroke
 *
 * @param key HID keycode (use constants from hid_keycodes.h, e.g., HID_KEY_A)
 * @return 0 on success, negative error code on failure
 */
int hid_keyboard_send_key(uint8_t key);

/**
 * @brief Send a single keystroke with modifier
 *
 * @param key HID keycode (use constants from hid_keycodes.h, e.g., HID_KEY_A)
 * @param modifier HID modifier byte (use constants from hid_keycodes.h, e.g., HID_MOD_LSHIFT)
 * @return 0 on success, negative error code on failure
 */
int hid_keyboard_send_key_with_mod(uint8_t key, uint8_t modifier);

/**
 * @brief Check if keyboard is connected to host
 *
 * @return true if connected and ready
 */
bool hid_keyboard_is_connected(void);

/**
 * @brief Get current LED state from host
 *
 * @return LED state structure
 */
struct hid_keyboard_led_state hid_keyboard_get_led_state(void);

#ifdef __cplusplus
}
#endif

#endif /* USB_HID_KEYBOARD_H_ */
