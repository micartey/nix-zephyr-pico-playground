#ifndef HID_KEYCODES_H_
#define HID_KEYCODES_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Modifier keys
#define HID_MOD_LCTRL   0x01
#define HID_MOD_LSHIFT  0x02
#define HID_MOD_LALT    0x04
#define HID_MOD_LGUI    0x08
#define HID_MOD_RCTRL   0x10
#define HID_MOD_RSHIFT  0x20
#define HID_MOD_RALT    0x40
#define HID_MOD_RGUI    0x80

// Standard keys (letters and numbers)
#define HID_KEY_NONE    0x00
#define HID_KEY_A       0x04
#define HID_KEY_B       0x05
#define HID_KEY_C       0x06
#define HID_KEY_D       0x07
#define HID_KEY_E       0x08
#define HID_KEY_F       0x09
#define HID_KEY_G       0x0A
#define HID_KEY_H       0x0B
#define HID_KEY_I       0x0C
#define HID_KEY_J       0x0D
#define HID_KEY_K       0x0E
#define HID_KEY_L       0x0F
#define HID_KEY_M       0x10
#define HID_KEY_N       0x11
#define HID_KEY_O       0x12
#define HID_KEY_P       0x13
#define HID_KEY_Q       0x14
#define HID_KEY_R       0x15
#define HID_KEY_S       0x16
#define HID_KEY_T       0x17
#define HID_KEY_U       0x18
#define HID_KEY_V       0x19
#define HID_KEY_W       0x1A
#define HID_KEY_X       0x1B
#define HID_KEY_Y       0x1C
#define HID_KEY_Z       0x1D

// Numbers (top row)
#define HID_KEY_1       0x1E
#define HID_KEY_2       0x1F
#define HID_KEY_3       0x20
#define HID_KEY_4       0x21
#define HID_KEY_5       0x22
#define HID_KEY_6       0x23
#define HID_KEY_7       0x24
#define HID_KEY_8       0x25
#define HID_KEY_9       0x26
#define HID_KEY_0       0x27

// Special characters
#define HID_KEY_ENTER       0x28
#define HID_KEY_ESC         0x29
#define HID_KEY_BACKSPACE   0x2A
#define HID_KEY_TAB         0x2B
#define HID_KEY_SPACE       0x2C
#define HID_KEY_MINUS       0x2D
#define HID_KEY_EQUAL       0x2E
#define HID_KEY_LEFTBRACE   0x2F
#define HID_KEY_RIGHTBRACE  0x30
#define HID_KEY_BACKSLASH   0x31
#define HID_KEY_HASH        0x32  // Non-US # and ~
#define HID_KEY_SEMICOLON   0x33
#define HID_KEY_APOSTROPHE  0x34
#define HID_KEY_GRAVE       0x35  // ` and ~
#define HID_KEY_COMMA       0x36
#define HID_KEY_DOT         0x37
#define HID_KEY_SLASH       0x38
#define HID_KEY_CAPSLOCK    0x39

// Function keys
#define HID_KEY_F1      0x3A
#define HID_KEY_F2      0x3B
#define HID_KEY_F3      0x3C
#define HID_KEY_F4      0x3D
#define HID_KEY_F5      0x3E
#define HID_KEY_F6      0x3F
#define HID_KEY_F7      0x40
#define HID_KEY_F8      0x41
#define HID_KEY_F9      0x42
#define HID_KEY_F10     0x43
#define HID_KEY_F11     0x44
#define HID_KEY_F12     0x45

// Control keys
#define HID_KEY_PRINT       0x46
#define HID_KEY_SCROLLLOCK  0x47
#define HID_KEY_PAUSE       0x48
#define HID_KEY_INSERT      0x49
#define HID_KEY_HOME        0x4A
#define HID_KEY_PAGEUP      0x4B
#define HID_KEY_DELETE      0x4C
#define HID_KEY_END         0x4D
#define HID_KEY_PAGEDOWN    0x4E
#define HID_KEY_RIGHT       0x4F
#define HID_KEY_LEFT        0x50
#define HID_KEY_DOWN        0x51
#define HID_KEY_UP          0x52

// Numpad
#define HID_KEY_NUMLOCK     0x53
#define HID_KEY_KPSLASH     0x54
#define HID_KEY_KPASTERISK  0x55
#define HID_KEY_KPMINUS     0x56
#define HID_KEY_KPPLUS      0x57
#define HID_KEY_KPENTER     0x58
#define HID_KEY_KP1         0x59
#define HID_KEY_KP2         0x5A
#define HID_KEY_KP3         0x5B
#define HID_KEY_KP4         0x5C
#define HID_KEY_KP5         0x5D
#define HID_KEY_KP6         0x5E
#define HID_KEY_KP7         0x5F
#define HID_KEY_KP8         0x60
#define HID_KEY_KP9         0x61
#define HID_KEY_KP0         0x62
#define HID_KEY_KPDOT       0x63

// Additional keys
#define HID_KEY_102ND       0x64  // Non-US \ and |
#define HID_KEY_COMPOSE     0x65
#define HID_KEY_POWER       0x66
#define HID_KEY_KPEQUAL     0x67

// QWERTZ-specific characters (accessed with modifiers)
// Note: These are the scan codes; actual characters depend on OS keyboard layout
// On QWERTZ (German): Z and Y are swapped compared to QWERTY

// Common QWERTZ symbols and their HID equivalents:
// German keyboard layout uses these scan codes:
// - Ä: Apostrophe (0x34) with Shift
// - Ö: Semicolon (0x33) with Shift
// - Ü: Left bracket (0x2F) with Shift
// - ß: Minus (0x2D)
// - §: 3 (0x20) with AltGr (Right Alt)
// - €: E (0x08) with AltGr
// - µ: M (0x10) with AltGr
// - ^: Grave (0x35) - dead key
// - `: Grave accent (0x35) - dead key
// - ´: Equal (0x2E) - dead key
// - <>: 102nd key (0x64)

// Helper macros for QWERTZ special characters
// These work with German (de-DE) keyboard layout on the host

// Umlauts (ä, ö, ü)
#define HID_SCAN_AE   HID_KEY_APOSTROPHE  // ä
#define HID_SCAN_OE   HID_KEY_SEMICOLON   // ö
#define HID_SCAN_UE   HID_KEY_LEFTBRACE   // ü

// Other German characters
#define HID_SCAN_SZ             HID_KEY_MINUS       // ß
#define HID_SCAN_DEGREE         HID_KEY_DOT         // ° (with Shift)
#define HID_SCAN_CIRCUMFLEX     HID_KEY_GRAVE       // ^ (dead key)
#define HID_SCAN_ACUTE          HID_KEY_EQUAL       // ´ (dead key)
#define HID_SCAN_LESS_THAN      HID_KEY_102ND       // <
#define HID_SCAN_GREATER_THAN   HID_KEY_102ND       // > (with Shift)
#define HID_SCAN_PIPE           HID_KEY_102ND       // | (with AltGr)

// Helper function to create HID report
struct hid_key_report {
    uint8_t modifiers;
    uint8_t reserved;
    uint8_t keycodes[6];
};

#ifdef __cplusplus
}
#endif

#endif /* HID_KEYCODES_H_ */
