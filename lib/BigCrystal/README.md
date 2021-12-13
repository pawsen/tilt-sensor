BigCrystal
==========

Arduino Library for displaying double height characters on an LCD display. This library is
compatible with the standard LiquidCrystal library dictributed with the Arduino IDE and
also with the [New LiquidCrystal](https://bitbucket.org/fmalpartida/new-liquidcrystal/wiki/Home)
which is a drop in replacement for the standard library.
In Addition to the libraries mentioned above, it is also compatible with the [LiquidCrystal_I2C](https://github.com/marcoschwartz/LiquidCrystal_I2C) 
library for DFRobot I2C LCD Displays. Reason for this is, that this library is compatible with the Wemos D1 Mini.
This runs a 32bit µC and you get a lot of errors when using with the libs from the original BigCrystal library.


The New LiquidCrystal library supports connections to LCDs using
* 4 bit parallel interface
* 8 bit parallel interface
* I<sup>2</sup>C expansion boards
* Shift registers

Note that this version of the library is incompatible with version 1 to implement compatibility
with the New LiquidCrystal library.

Contributors
------------
Thanks to Bill Perry who reviewed the original code and suggested I base this library on
the New LiquidCrystal library by F. Malpartida. Thak you also to Tim Eckel who wrote the
[LCDBitmap](http://code.google.com/p/arduino-lcd-bitmap/) library whose code gave me insight
on how to make this library compatible with both the standard LiquidCrystal and New
LiquidCrystal libraries.

Usage
-----

You must install the BigCrystal library into your Arduino library directory. Optionally, you
can remove the standard LiquidCrystal library and replace it with the New LiquidCrystal library.

BigCrystal is implemented as a wrapper around an LCD object, created using either the standard or
new LiquidCrystal libraries. The lcd object is passed through to the BigCrystal constructor.

BigCrystal exposes all LiquidCrystal public methods and delegates to the underlying object. This
means that once the BigCrytal object is created, you just need to reference that instance instead
of using both the BigCrystal instance and the underling LiquidCrystal instance.

The following methods are available to display double height characters:
* writeBig(char c, uint8_t row, uint8_t col) - writes a single large character to the specified coordinates.
* printBig(char* str, uint8_t row, uint8_t col) - writes a String to the specified coordinates.
* widthBig(char c) - returns the width in characters of the specified large character.
  Includes the one column space following the character.

To compile the library into your project you have to make some small changes to your boards.txt which is
located inside the installation directory. E.g. C:\Program Files (x86)\Arduino\hardware\arduino\avr\boards.txt
You can just add a line like this:
mega.build.debug_level=-DUSE_I2C=1
Where "mega" stands for the type of board you can select inside the Arduino IDE.
If you are using the Arduino IDE for an ESP8266 based board, you will find the boards.txt file in your
user-directory \Documents\Arduino\hardware\esp8266com\esp8266\boards.txt where you can add a line like
d1_mini.build.debug_level=-DUSE_I2C=1
The reason for this workaround is, that if you don't have this global definition in place, the linker would
not be able to get it right. That's just a workaround for now until I find a better solution.