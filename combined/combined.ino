// sensor
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
// display
#include <LiquidCrystal_I2C.h>
#include <BigCrystal.h>
// display extra sensor info
#include "extra.h"

// set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);
BigCrystal biglcd(&lcd);

/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

/* Calibrate accelerometer using two-point measurement
 *
 * meassure min and max "static" acceleration by taping the accelerometer to a
 * rectangular block and put it on all sides on a table. Tip: On the two sides
 * where the acc.meter and cables are in the way, press the block against the
 * bottom of the table. */

// gravity defined in Adafruit_Sensor to be 9.806 m/s^2
const float g = SENSORS_GRAVITY_EARTH;
// measured "static" accelerations
const float minX = -13.81;
const float maxX = 10.67;
const float minY = -10.87;
const float maxY = 12.87;
const float minZ = 0;
const float maxZ = 20.04;

float offset(float min_acc, float max_acc){
    return 0.5 * (min_acc + max_acc);
}
float gain(float min_acc, float max_acc){
    return 0.5 * ((max_acc - min_acc) / g);
}

// z-axis seems to be broken
const float offsetX = offset(minX, maxX);
const float offsetY = offset(minY, maxY);
const float offsetZ = 0; // offset(minZ, maxZ);

const float gainX = gain(minX, maxX);
const float gainY = gain(minY, maxY);
const float gainZ = 1; // gain(minZ, maxZ);

// buffer for displaying roll
const uint8_t bufsize = 33;
boolean runOnce = false;


/* get calibrated accelerations */
void get_cal_acceleration(sensors_event_t *event, sensors_vec_t *vec){
    vec->x = (event->acceleration.x - offsetX)/gainX;
    vec->y = (event->acceleration.y - offsetY)/gainY;
    vec->z = (event->acceleration.z - offsetZ)/gainZ;
}

/* get averaged accelerations */
void get_avg_acceleration(sensors_event_t *event, sensors_vec_t *vec, uint8_t n) {
    float ax,ay,az;
    uint8_t i;

    ax=0; ay=0; az=0;
    for(i=0; i<n; i++) {
        accel.getEvent(event);
        get_cal_acceleration(event, vec);
        ax += vec->x;
        ay += vec->y;
        az += vec->z;
        // Don't read faster than 100Hz: (10*100ms=1s 1s/10=100ms)
        delay(100);
    }
    vec->x = ax/n; vec->y = ay/n; vec->z = az/n;
}

void orientation(sensors_vec_t vec, sensors_vec_t *val){
    /* roll;
       Rotation around the longitudinal axis (the plane body, 'X axis').
       Roll is positive and increasing when moving downward. -90 degrees <= roll
       <= 90 degrees */
    /* pitch;
      Rotation around the lateral axis (the wing span, 'Y axis').
      Pitch is positive and increasing when moving upwards. -180 degrees <=
      pitch <= 180 degrees) */
    /* heading;
      Angle between the longitudinal axis (the plane body) and magnetic north,
      measured clockwise when viewing from the top of the device. 0-359
      degrees */

    // θ, theta: rotation of x-axis
    // ψ, psi: y-axis
    // φ, phi: z-axis
    val->roll = (int) (180/M_PI * ( M_PI/2 - atan2(vec.x, vec.y)));
    val->pitch = (int) (180/M_PI * ( M_PI/2 - atan2(
            (-vec.x) , sqrt(pow(vec.y,2) + pow(vec.z,2)))));

    // Angle from x,y axis to gravity vector. Alternative calculations
    // z-axis seems broken. Dont include
    float r = sqrt(vec.x*vec.x + vec.y*vec.y); // + vec.z*vec.z);
    int roll   = 180/M_PI * ( M_PI/2 - (acos(vec.y / r)));
    val->pitch  = (int) (180/M_PI * ( M_PI/2 - (acos(vec.x / r))));
}



void print_acceleration(sensors_vec_t vec){
    /* Display the results (acceleration is measured in m/s^2) */
    Serial.print("X: "); Serial.print(vec.x); Serial.print("  ");
    Serial.print("Y: "); Serial.print(vec.y); Serial.print("  ");
    Serial.print("Z: "); Serial.print(vec.z); Serial.print("  ");
    Serial.println("m/s^2 ");
}

void print_orientation(sensors_vec_t vec){
    Serial.print("θ: "); Serial.print(vec.roll); Serial.print("  ");
    Serial.print("φ: "); Serial.print(vec.pitch); Serial.print("  ");
    Serial.println("");
}

void setup(void) {
    Serial.begin(9600);

    biglcd.init();
    lcd.backlight();
    lcd.setCursor(0,0);
    lcd.print("Kilter Board  @");
    // Go to column 3, row 1
    lcd.setCursor(3,1);
    lcd.print("beta Boulders");

    /* Initialise the sensor */
    if(!accel.begin()) {
        Serial.println("no ADXL345 detected ... Check your wiring!");
        lcd.setCursor(0,1);
        lcd.print("Err");
        while(1);
    }

    /* Set the range to whatever is appropriate for your project */
    // accel.setRange(ADXL345_RANGE_16_G);
    // accel.setRange(ADXL345_RANGE_8_G);
    // accel.setRange(ADXL345_RANGE_4_G);
    accel.setRange(ADXL345_RANGE_2_G);
#ifdef __DEBUG__
    /* Display some basic information on this sensor */
    displaySensorDetails(&accel);
    /* Display additional settings (outside the scope of sensor_t) */
    displayDataRate(&accel);
    displayRange(&accel);
#endif

    Serial.println("ready");
    delay(2000);
}

void loop(void) {
    sensors_event_t event;
    sensors_vec_t acc;
    sensors_vec_t ori;

    /* Get a new sensor event */
    //accel.getEvent(&event);
    //acceleration(event, &acc);
    get_avg_acceleration(&event, &acc, 5);
    orientation(acc, &ori);

#ifdef __DEBUG__
    print_acceleration(acc);
    print_orientation(ori);
#endif

    // remember to send '\n' or parsing will pail on the other arduino
    int roll = (int)ori.roll;
    if (!runOnce) {
        runOnce = true;
        lcd.clear();
    }
    char pbuf[bufsize];
    char bbuf[bufsize];
    // right justify to center number
    sprintf(pbuf, "%2d", roll);
    // pad with spaces (left justify), to clear left-over text
    sprintf(bbuf, "%-6s", pbuf);
    biglcd.printBig(bbuf, 4, 0);
    Serial.println(bbuf);

}
