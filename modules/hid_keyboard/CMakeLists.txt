if(CONFIG_HID_KEYBOARD)

  zephyr_include_directories(.)

  zephyr_library_sources(src/usb_hid_keyboard.c)

  zephyr_include_directories(include)

endif()
