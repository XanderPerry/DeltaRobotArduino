/*
#define accelx A0
#define accely A1
#define accelz A2

struct Coordinates
{
  int x;
  int y;
  int z;
};

void Calculateservos(int x, int y, int z);
*/

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>


Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

void setup(void) 
{
  Serial.begin(9600);
  
  accel.begin();
  accel.setRange(ADXL345_RANGE_4_G);
}

void loop(void) 
{
  sensors_event_t event; 
  accel.getEvent(&event);
 
  Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  ");Serial.println("m/s^2 ");
  delay(500);
}