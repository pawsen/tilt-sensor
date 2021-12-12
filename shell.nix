# https://github.com/boredom101/nixduino

{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  ARDMK_DIR = pkgs.arduino-mk;
  ARDUINO_DIR = "${pkgs.arduino}/share/arduino";
  ARDUINO_LIB_PATH = "${pkgs.arduino}/share/arduino/libraries";
  buildInputs = with pkgs; [
    (python3.withPackages (ps: with ps; [
      pyserial
    ]))
    arduino
    arduino-mk
    screen
  ];
}
