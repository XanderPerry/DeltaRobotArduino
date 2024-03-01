#ifdef MAIN

#include <Arduino.h>
#include <math.h>

#define Larm1 10
#define Larm2 100
#define Larmbase 50
#define Ltop 5

#define starthoek 0
#define eindhoek 90
#define marge 2

struct Coordinates
{
  int x;
  int y;
  int z;
};

void Calculateservos(int x, int y, int z);
void Calculate1(int x, int z);
void Calculate2(int x, int y, int z);
void Calculate3(int x, int y, int z);
int DegreeToRadian(int degree);

void setup()
{
  Serial.begin(9600);

}

void loop()
{
  Coordinates coordinates;
  /*
  coordinates.x = scanf("Geef x coordinaat: %d", &coordinates.x);
  coordinates.y = scanf("Geef y coordinaat: %d", &coordinates.y);
  coordinates.z = scanf("Geef z coordinaat: %d", &coordinates.z);
  */

  coordinates.x = 10;
  coordinates.y = 20;
  coordinates.z = 30;
  Calculateservos(coordinates.x, coordinates.y, coordinates.z);
  delay(500);
}

void Calculateservos(int x, int y, int z)
{
  Calculate1(x, z);
}

void Calculate1(int x, int z)
{
  int i = starthoek;
  int xcalc = 0;
  int zcalc = 0;
  int distance = 0;

  while((distance < (Larm2 - marge)) || (distance > (Larm2 + marge)))
  {
    zcalc = Larm1 + sin(DegreeToRadian(i));
    xcalc = Larm1 + cos(DegreeToRadian(i));
    distance = sqrt(pow(x - xcalc, 2) + pow(z - zcalc, 2));
    i++;
  }
  Serial.println(i);
}

int DegreeToRadian(int degree)
{
  return (degree * 71) / 4068;
}
#endif