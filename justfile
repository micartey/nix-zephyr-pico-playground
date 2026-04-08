build-01: cleanup
    west build -p always -b rpi_pico applications/01_led_blink -- \
        -DDTC_OVERLAY_FILE=$PWD/applications/01_led_blink/boards/app.overlay

build-02: cleanup
    nix develop -c west build -p always -b rpi_pico applications/02_button/ -- \
        -DDTC_OVERLAY_FILE=$PWD/applications/02_button/boards/button.overlay

build-03: cleanup
    nix develop -c west build -p always -b rpi_pico applications/03_button_driver/ -- \
        -DDTC_OVERLAY_FILE=$PWD/applications/03_button_driver/boards/rpi_pico.overlay

build-04: cleanup
    west build -p always -b rpi_pico applications/04_memory_arena -- \
        -DDTC_OVERLAY_FILE=$PWD/applications/04_memory_arena/boards/app.overlay

cleanup:
    rm -rf build

test-unit: cleanup
    nix develop -c sh -c 'west build -p always -b native_sim/native/64 applications/01_led_blink/tests/unit && ./build/zephyr/zephyr.exe'

test-integration: cleanup
    nix develop -c sh -c 'west build -p always -b native_sim/native/64 applications/01_led_blink/tests/integration && ./build/zephyr/zephyr.exe'

test: test-unit test-integration

flash:
    nix develop --command sudo picotool load -x build/zephyr/zephyr.uf2

# Generate a clangd config that works for nix based systems and enables correct LSP suport
clangd:
    ln -s build/compile_commands.json .
    python3 scripts/generate_clangd.py
