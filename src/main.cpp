#ifdef MAIN

#include <Arduino.h>
#include <math.h>

// lengtes in mm
#define Larm1 50
#define Larm2 210
#define Larmbase 100
#define Ltop 35

#define starthoek -10
#define eindhoek 80
#define marge 1

struct Coordinates
{
  int x;
  int y;
  int z;
};

float DegreeToRadian(int degree);
Coordinates ask_coordinates(void);
char calc_arm1(Coordinates coords);

void setup()
{
  Serial.begin(115200);

}

void loop()
{
  Coordinates coord = ask_coordinates();
  Serial.print("x: ");
  Serial.println(coord.x);
  Serial.print("y: ");
  Serial.println(coord.y);
  Serial.print("z: ");
  Serial.println(coord.z);
}

// geeft van een int in graden een float in radialen met 2 decimalen
float DegreeToRadian(int degree)
{
  return (float(degree) / 180.0000) * 3.14159265358979323846;
}

Coordinates ask_coordinates(void)
{
  Serial.println("Geef de x-coordinaat in mm");
  while (Serial.available() == 0)
  {
  }
  int x = Serial.parseInt();
  Serial.println("Geef de y-coordinaat in mm");
  while (Serial.available() == 0)
  {
  }
  int y = Serial.parseInt();
  Serial.println("Geef de z-coordinaat in mm");
  while (Serial.available() == 0)
  {
  }
  int z = Serial.parseInt();
  Coordinates coord = {x, y, z};
  return coord;
}

char calc_arm1(Coordinates coords)
{
  int x = coords.x + Ltop;
  int y = coords.y;
  int z = coords.z;
  
  int x_servo = Larmbase;
  int y_servo = 0;
  int z_servo = 0;

  for(int i = starthoek; i < eindhoek; i++)
  {
    int z_elleboog = z_servo + Larm1 * sin(DegreeToRadian(i));

    if((afstand - marge < Larm2) || (afstand + marge > Larm2))
    {
      return i;
    }
  }
  
}

#endif