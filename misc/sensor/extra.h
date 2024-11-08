#ifndef __EXTRA_H
#define __EXTRA_H

#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
// for Serial.print
#include <Arduino.h>


/* display info */
void displaySensorDetails(Adafruit_ADXL345_Unified *accel);
void displayDataRate(Adafruit_ADXL345_Unified *accel);
void displayRange(Adafruit_ADXL345_Unified *accel);



#endif  // __EXTRA_H
