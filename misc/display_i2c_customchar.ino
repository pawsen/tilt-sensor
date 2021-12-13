#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <BigCrystal.h>

// Set the LCD I2C address - important that it matches the I2C module
LiquidCrystal_I2C lcd(0x27, 16, 2);
BigCrystal bigCrystal(&lcd);

void clear() {
    for (int i = 0; i < 5; i++) {
        bigCrystal.setCursor(i, 0);
        bigCrystal.print(' ');
        bigCrystal.setCursor(i, 1);
        bigCrystal.print(' ');
    }
}

void setup() {
    // It is important to call bigCrystal.init instead of lcd.init
    // as the former calls bigCrystal.createCustomChars()
    bigCrystal.init();
    lcd.backlight();
}

void loop() {
    // Displays all characters is big front from 0x00 (space) to 0x5A (Z)
    for (char c = 0x20; c <= 0x5A; c++) {
        // Clear out the maximum width so that pars of wider
        // characters are removed
        clear();

        bigCrystal.writeBig(c, 0, 0);
        bigCrystal.setCursor(7, 0);
        bigCrystal.write(c);
        delay(1000);
    }
}

