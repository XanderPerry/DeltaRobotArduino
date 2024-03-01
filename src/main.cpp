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

float correctX, correctY, correctZ;

void setup(void) 
{
  Serial.begin(9600);
  
  accel.begin();
  accel.setRange(ADXL345_RANGE_4_G);

  float x = 0.0, y = 0.0, z = 0.0;
  for(int i = 0; i < 100; i++)
  {
    sensors_event_t event; 
    accel.getEvent(&event);
    x += event.acceleration.x;
    y += event.acceleration.y;
    z += event.acceleration.z;
  }
  correctX = x / 100.0;
  correctY = y / 100.0;
  correctZ = z / 100.0;
}

void loop(void) 
{
  sensors_event_t event; 
  accel.getEvent(&event);
 
  Serial.print("X: "); Serial.print(event.acceleration.x - correctX); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.acceleration.y - correctY); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.acceleration.z - correctZ); Serial.print("  ");Serial.println("m/s^2 ");
  delay(500);
}