// By John Main © best-microcontroller-projects.com
// ADXL345 Self Test, show g and roll pitch values
#include <math.h>
#include <Wire.h>
#include <I2Cdev.h> // Has MIT license.
#include <ADXL345.h>

ADXL345 accel;

#define LED_PIN LED_BUILTIN // (Arduino is 13, Teensy is 6, nano? so use LED_BUILTIN)
bool toggleLED = false;
static byte showAngles = 0; // Display operation

void setup() {
    Wire.begin();
    Serial.begin(115200);

    // initialize device
    Serial.println( F("Starting I2C devices...") );
    accel.initialize();

    Serial.println( F("Checking ADXL345...") );
    if(accel.testConnection())
      Serial.println( F("ADXL345 comms Ok.") );
    else {
       Serial.println( F("ADXL345 comms failed.") );
       while(1); // Hang on error.
    }
    Serial.println(F("Enter t - Self test, s - Show angles"));

    accel.setFullResolution(1); // 0 => 10 bit mode.
    accel.setRate(ADXL345_RATE_100);  // This is default but shows the value.
    accel.setLowPowerEnabled(0);
    accel.setRange(0); // 0 => 2g, 3 => 16g

    pinMode(LED_PIN, OUTPUT);
}

// Get readings for self test - A set of n averaged results.
void ADXL345GetAvgN(int16_t *xi,int16_t *yi,int16_t *zi,uint8_t n) {
int16_t x,y,z;
int32_t ax,ay,az;
uint8_t i;

    ax=0; ay=0; az=0;
    for(i=0;i<n;i++) {
      accel.getAcceleration(&x, &y, &z);
      ax += x; ay += y; az +=z;
      delay(100); // Don't read faster than 100Hz: (10*100ms=1s 1s/10=100ms)
    }
    *xi = ax/n; *yi = ay/n; *zi = az/n;
}

void print_minmax(int vmin,int vmax) {
  Serial.print('(');Serial.print(vmin);Serial.print(" ~ ");Serial.print(vmax);Serial.print(')');
}

// This function setsup the relevant parameters while preserving them e.g. range
void ADXL345SelfTest(void) {
uint16_t fr,st,rt,lp,ra;
    fr = accel.getFullResolution();
    rt = accel.getRate();
    lp = accel.getLowPowerEnabled();
    ra = accel.getRange();

    int16_t avg_x=0, avg_y=0, avg_z=0;
    int16_t avg_stx=0, avg_sty=0, avg_stz=0;
    int16_t ax,ay,az;
    int16_t x,y,z;
    uint8_t i;

    // Setup operating mode required for self test.
    accel.setFullResolution(0); // 0 => 10 bit mode.
    accel.setRate(ADXL345_RATE_100);
    accel.setLowPowerEnabled(0);
    accel.setRange(3); // 3 => 16g

    ADXL345GetAvgN(&avg_x, &avg_y, &avg_z, 10);

    accel.setSelfTestEnabled(1);
    ADXL345GetAvgN(&avg_stx,&avg_sty,&avg_stz,5);    // Let device settle for >=4 samples.
    ADXL345GetAvgN(&avg_stx, &avg_sty, &avg_stz, 10);
    accel.setSelfTestEnabled(0);

    // Display results
    x = avg_stx-avg_x; y = avg_sty-avg_y; z = avg_stz-avg_z;
    Serial.print("\nstx-x\t");Serial.print(x);Serial.print("\t");
    Serial.print("sty-y\t");Serial.print(y);Serial.print("\t");
    Serial.print("stz-z\t");Serial.print(z);Serial.print("\t");

    // 3V3 error limit scale factors.
    #define scale_factor_xy  1.77;
    #define scale_factor_z   1.47;
    float vmin,vmax;

    Serial.println("");

    //Value limits are from ADXL345 datasheet - Tables 14 and 18.
    vmin = 6 * scale_factor_xy; vmax = 67 * scale_factor_xy;
    Serial.print( (x>=vmin && x <vmax) ? "X PASS " : "X FAIL " );
    print_minmax(vmin,vmax);
    Serial.println("");

    vmin = -67 * scale_factor_xy; vmax = -6 * scale_factor_xy;
    Serial.print( (y>=vmin && y <vmax) ? "Y PASS " : "Y FAIL " );
    print_minmax(vmin,vmax);
    Serial.println("");

    vmin = 10 * scale_factor_z; vmax = 110 * scale_factor_z;
    Serial.print( (z>=vmin && z <vmax) ? "Z PASS " : "Z FAIL " );
    print_minmax(vmin,vmax);
    Serial.println("");

    accel.setFullResolution(fr);
    accel.setRate(rt);
    accel.setLowPowerEnabled(lp);
    accel.setRange(ra);
}

void showAccelAngles(void) {
  float x,y,z;
  int16_t ax,ay,az;
   // Show angles
   // Datasheet: OPERATION AT VOLTAGES OTHER THAN 2.5 V
   // 3v3 X,Y 25mg too high, z 20mg too low
   // 3V3 lsb value 265/g   (g/265)=0.03698
   // 2V5 lsb value 256/g   (g/256)=0.03828 z axis unaffected by voltage supply.
   #define ADXL345_LSBVAL_3V3 3.698E-3
   #define ADXL345_LSBVAL_2V5 3.828E-3

   accel.getAcceleration(&ax, &ay, &az);
   x = ax*ADXL345_LSBVAL_3V3 - 25E-3;
   y = ay*ADXL345_LSBVAL_3V3 - 25E-3;
   z = az*ADXL345_LSBVAL_2V5 + 20e-3;

   float fNum;
   Serial.print(F("g values: "));
   fNum=x;Serial.print(fNum); Serial.print("\t");
   fNum=y;Serial.print(fNum); Serial.print("\t");
   fNum=z;Serial.print(fNum); Serial.print("\t");

   float r = sqrt(x*x + y*y + z*z);

   // Angle from x,y axis to gravity vector.
   int roll   = 180/M_PI * ( M_PI/2 - (acos(y/r) ) );
   int pitch  = 180/M_PI * ( M_PI/2 - (acos(x/r) ) );

   Serial.print(F("Roll ")); Serial.print(roll); Serial.print(' ');
   Serial.print(F("Pitch "));Serial.print(pitch);Serial.print(' ');
   Serial.println("");
}

void loop() {
float x,y,z;
static byte ch;

   if(Serial.available()) {
     ch=Serial.read();
   }

   if(ch=='t') {
      Serial.println(F("Testing..."));
      ADXL345SelfTest();
      showAngles = 0;
   }
   else if(ch=='s')
      showAngles = 1;

   if (showAngles) showAccelAngles();

   toggleLED = !toggleLED;
   digitalWrite(LED_PIN, toggleLED);
   delay(200);
}

 
