build: cleanup
    nix develop -c west build -p always -b rpi_pico applications/01_hello_world -- -DDTC_OVERLAY_FILE=boards/app.overlay

build-example:
    nix develop -c west build -b rpi_pico zephyr/samples/basic/blinky -- -DCONFIG_NEWLIB_LIBC=y

setup:
    ln -s build/compile_commands.json .

cleanup:
    rm -rf build
