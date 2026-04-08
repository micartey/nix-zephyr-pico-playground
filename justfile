build: cleanup
    nix develop -c west build -p always -b rpi_pico applications/01_led_blink -- -DDTC_OVERLAY_FILE=$PWD/applications/01_led_blink/boards/app.overlay

build-example:
    nix develop -c west build -b rpi_pico zephyr/samples/basic/blinky -- -DCONFIG_NEWLIB_LIBC=y

setup:
    ln -s build/compile_commands.json .

cleanup:
    rm -rf build

test-unit: cleanup
    nix develop -c sh -c 'west build -p always -b native_sim/native/64 applications/01_led_blink/tests/unit && ./build/zephyr/zephyr.exe'

test-integration: cleanup
    nix develop -c sh -c 'west build -p always -b native_sim/native/64 applications/01_led_blink/tests/integration && ./build/zephyr/zephyr.exe'

test: test-unit test-integration

flash:
    nix develop --command sudo picotool load -x build/zephyr/zephyr.uf2
