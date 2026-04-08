# hello world example

Inside of the nix develop shell, run:

```bash
west build -p always -b rpi_pico applications/01_hello_world -- -DDTC_OVERLAY_FILE=boards/app.overlay

# To flash the device
sudo picotool load -x build/zephyr/zephyr.uf2
```

## LED in Device-Tree

```bash
cat zephyr/boards/raspberrypi/common/rpi_pico-led.dtsi | grep "led" | grep ":"

# led0: led_0 {
# pwm_led0: pwm_led_0 {
```

As you can see, there are two lables which we can access in the code:

- `led0`
- `pwm_led0`

```cpp
const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);

if (!gpio_is_ready_dt(&led)) {
    return -1;
}

gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);

while (true) {
    gpio_pin_toggle_dt(&led);
    k_msleep(1000);
}
```

## Troubleshooting

```bash
sudo dmesg -w
```

### Shell

The following command will start a uart shell where you can read logs, but also execute commands:

```bash
tio -f none /dev/ttyACM0
```

- `bootloader` will reset the device into boot mode
- `device list` will list the hardware configured devices
