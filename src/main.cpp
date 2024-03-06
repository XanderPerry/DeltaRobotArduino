#ifdef MAIN

#include <Arduino.h>
#include <math.h>

// lengtes in mm
#define Larm1 50
#define Larm2 210
#define Larmbase 100
#define Ltop 35

#define starthoek -10
#define eindhoek 90
#define marge 1

float DegreeToRadian(int degree);

struct Coordinates
{
  int x;
  int y;
  int z;
};

void setup()
{
  Serial.begin(9600);

}

void loop()
{
  
}

// geeft van een int in graden een float in radialen met 2 decimalen
float DegreeToRadian(int degree)
{
  return (float(degree) / 180.0000) * 3.14159265358979323846;
}
#endif