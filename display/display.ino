#include <LiquidCrystal_I2C.h>
#include <NeoSWSerial.h>
#include <BigCrystal.h>
//#include <SoftwareSerial.h>

// soft serial
#define rxPin 8
#define txPin 9
#define buttonPin 2
#define ledPin 13

// How long (in milliseconds) should lcd light stay on?
const uint32_t LCD_LIGHT_ON_TIME = 1000;
uint32_t lcdLightOn_Start = 0;
boolean isLcdLightOn = false;
const uint8_t bufsize = 33;
char buffer[bufsize];
boolean runOnce = false;

/* Only a subset of pins can be used for RX for some arduino boards:
 * mega, leonardo, micro. Uno can use all pins
 * https://www.arduino.cc/en/Reference/softwareSerial */
// SoftwareSerial mySerial(rxPin, txPin); // RX, TX
NeoSWSerial mySerial(rxPin, txPin);

// set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);
BigCrystal biglcd(&lcd);

int recieve_data(char* buffer, int bufsize){

    uint8_t i = 0;
    while (mySerial.available() > 0) {
        if (i < bufsize - 1){
            char ch = mySerial.read();
            buffer[i] = ch;
            if (ch == '\n'){
                // replace newline with null terminated
                buffer[i] = '\0';
                Serial.println("Recieved"); Serial.println(buffer);
                return 0;
            }
            i++;
        }
    }
    // error. Either too many chars or no newline char
    return -1;
}

// clear the display
void clear(){
  for (int i = 0; i < 5; i++) {
    lcd.setCursor(i, 0);
    lcd.print(' ');
    lcd.setCursor(i, 1);
    lcd.print(' ');
  }
}


void lcd_backlight(int buttonState) {
    // Serial.println(buttonState);
    // Keep in mind the pull-up means the pushbutton's logic is inverted. It goes
    // HIGH when it's open, and LOW when it's pressed. 
    if (buttonState == HIGH){
        if (isLcdLightOn &&
                (millis() - lcdLightOn_Start > LCD_LIGHT_ON_TIME)) {
            lcd.noBacklight();
            isLcdLightOn = false;
            digitalWrite(ledPin, LOW);
        }
    }
    else{
        Serial.println("Button pressed - LOW");
        lcd.backlight();
        lcdLightOn_Start = millis();
        isLcdLightOn = true;
        digitalWrite(ledPin, HIGH);
    }
}

void setup() {
    Serial.begin(9600);
    mySerial.begin(9600);
    mySerial.flush();

    pinMode(buttonPin, INPUT_PULLUP);
    pinMode(ledPin, OUTPUT); 

    biglcd.init();
    lcd.backlight();
    lcd.setCursor(0,0);
    lcd.print("Kilter Board  @");
    // Go to column 3, row 1
    lcd.setCursor(2,1);
    lcd.print("beta Boulders");

    Serial.println("Hello World");
    delay(2000);
}


void loop() {
    if (!runOnce) {
        runOnce = true;
        lcd.clear();
    }
    if (recieve_data(buffer, bufsize) == 0){
        size_t len = strlen(buffer);
        char pbuf[16];
        lcd.clear();
        // right justify string. Use %-8s for left justify
        // sprintf might add some overhead, but thats not important here
        sprintf(pbuf, "%8s", buffer);
        lcd.print(pbuf);
    }
    
    // read button and turn on backlight if pressed
    // int buttonState = digitalRead(buttonPin);
    // lcd_backlight(buttonState);

    // we need a delay to ensure correct recieving SoftwareSerial
    delay(2000);
}
