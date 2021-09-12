#include <LiquidCrystal_I2C.h>
#include <NeoSWSerial.h>
#include <BigCrystal.h>

// soft serial
#define rxPin 8
#define txPin 9

const uint8_t bufsize = 33;
char buffer[bufsize];
boolean runOnce = false;

/* Only a subset of pins can be used for RX for some arduino boards:
 * (mega, leonardo, micro). Uno can use all pins
 * https://www.arduino.cc/en/Reference/softwareSerial */
NeoSWSerial mySerial(rxPin, txPin);

// set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);
BigCrystal biglcd(&lcd);

// check for data on a given port.
// Most interfaces that derives from Stream (Serial, SoftwareSerial, etc) can be used
int recieve_data(Stream &port, char* buffer, int bufsize){

    uint8_t i = 0;
    while (port.available() > 0) {
        if (i < bufsize - 1){
            char ch = port.read();
            buffer[i] = ch;
            if (ch == '\n' && i > 0){
                // replace newline with null terminated
                buffer[i] = '\0';
                Serial.println("Recieved"); Serial.println(buffer);
                return 0;
            } else if (ch == '\n' && i == 0) {
                // only recieved newline
                return -1;
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
    // check if there's data on the Serial interface
    if (recieve_data(Serial, buffer, bufsize) == 0){
        // clear the initial message now that we recieved data
        if (!runOnce) {
            runOnce = true;
            lcd.clear();
        }
        char pbuf[bufsize];
        // right justify to center number
        sprintf(pbuf, "%2s", buffer);
        // pad with spaces (left justify), to clear left-over text
        sprintf(buffer, "%-6s", pbuf);
        biglcd.printBig(buffer, 4, 0);
    }
    
    // we need a delay to ensure correct recieving SoftwareSerial
    delay(100);
}
