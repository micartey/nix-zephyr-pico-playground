if(CONFIG_RP2040_BOOTLOADER)

  zephyr_include_directories(.)

  zephyr_library_sources(src/bootloader_cmd.c)

  zephyr_include_directories(include)

endif()
