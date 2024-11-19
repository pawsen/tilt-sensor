# use with
# nix develop -c $SHELL
{
  inputs = { nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable"; };

  outputs = { nixpkgs, ... }:
    let forAllSystems = nixpkgs.lib.genAttrs [ "aarch64-linux" "x86_64-linux" ];
    in {
      devShells = forAllSystems (system:
        let pkgs = import nixpkgs { inherit system; };
        in with pkgs; rec {
          default = mkShell {

            ARDMK_DIR = pkgs.arduino-mk;
            ARDUINO_DIR = "${pkgs.arduino-core-unwrapped}/share/arduino";
            ARDUINO_LIB_PATH =
              "${pkgs.arduino-core-unwrapped}/share/arduino/libraries";
            buildInputs = with pkgs; [
              (python3.withPackages (ps: with ps; [ pyserial ]))
              arduino-core-unwrapped
              arduino-mk
              # use tio instead of screen, tio -b 9600 /dev/ttyUSB0
              tio
							# keep screen for make monitor
              screen
            ];

          };
        });
      packages = forAllSystems (system: {
        default =
          nixpkgs.legacyPackages.${system}.callPackage ./default.nix { };
      });
    };
}
