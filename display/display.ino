#include <LiquidCrystal_I2C.h>
#include <NeoSWSerial.h>
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

/* Only a subset of pins can be used for RX for some arduino boards:
 * mega, leonardo, micro. Uno can use all pins
 * https://www.arduino.cc/en/Reference/softwareSerial */
// SoftwareSerial mySerial(rxPin, txPin); // RX, TX
NeoSWSerial mySerial(rxPin, txPin);

// set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27,16,2);

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

void setup() {
    Serial.begin(9600);
    mySerial.begin(9600);
    mySerial.flush();

    pinMode(buttonPin, INPUT_PULLUP);
    pinMode(ledPin, OUTPUT); 

    lcd.init();
    // lcd.backlight();
    // Go to column 3, row 0
    lcd.setCursor(3,0);
    lcd.print("Hello, world!");
    lcd.setCursor(0,1);
    lcd.print("All hail Arduino");
}


void loop() {

    if (recieve_data(buffer, bufsize) == 0){
        size_t len = strlen(buffer);
        char pbuf[16];
        lcd.clear();
        // right justify string. Use %-8s for left justify
        // sprintf might add some overhead, but thats not important here
        sprintf(pbuf, "%8s", buffer);
        lcd.print(pbuf);
    }

    int buttonState = digitalRead(buttonPin);
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

    // we need a delay to ensure correct recieving SoftwareSerial
    delay(100);
}
