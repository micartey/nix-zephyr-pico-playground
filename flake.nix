{
  description = "Zephyr RTOS development environment for Raspberry Pi Pico";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-25.11";
    utils.url = "github:numtide/flake-utils";
  };

  outputs =
    {
      self,
      nixpkgs,
      utils,
    }:
    utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = import nixpkgs { inherit system; };
      in
      {
        devShells.default = pkgs.mkShell {
          buildInputs = with pkgs; [
            # Build Tools
            cmake
            ninja
            dtc
            gperf
            dfu-util
            gcc-arm-embedded

            # Python 3.12 with required build modules
            (python312.withPackages (
              ps: with ps; [
                west
                jsonschema
                pyyaml
                pyelftools
                setuptools
                canopen
                packaging
              ]
            ))

            # Flashing/Terminal Tools
            picotool
            openocd
            screen
            tio
          ];

          shellHook = ''
            echo "--- Zephyr 4.x Development Environment (NixOS 25.11) ---"

            export ZEPHYR_TOOLCHAIN_VARIANT=gnuarmemb
            export GNUARMEMB_TOOLCHAIN_PATH=${pkgs.gcc-arm-embedded}

            if [ -z "$ZEPHYR_BASE" ]; then
              # Automatically try to set ZEPHYR_BASE if we are in the project root
              if [ -d "zephyr" ]; then
                export ZEPHYR_BASE="$(pwd)/zephyr"
                echo "ZEPHYR_BASE set to $ZEPHYR_BASE"
              fi
            fi
          '';
        };
      }
    );
}
