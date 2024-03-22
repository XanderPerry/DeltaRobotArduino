#ifdef MAIN

#include <Arduino.h>
#include <math.h>
#include <servo.h>

// lengtes in mm
#define Larm1 50
#define Larm2 210
#define Larmbase 100
#define Ltop 35

#define starthoek -30
#define eindhoek 70
#define marge 2

struct Coordinates
{
  int x;
  int y;
  int z;
};

struct Angles
{
  int8_t hoek1;
  int8_t hoek2;
  int8_t hoek3;
};

Servo servo1;
Servo servo2;
Servo servo3;

float DegreeToRadian(int degree);
Coordinates ask_coordinates(void);
char calc_arm1(Coordinates coords);
char calc_arm2(Coordinates coords);
char calc_arm3(Coordinates coords);
Angles calc_angles(Coordinates coords);
void servos(Angles angles);

void setup()
{
  Serial.begin(9600);

  servo1.attach(9);
  servo2.attach(10);
  servo3.attach(11);

  Angles angles_test = {0, 0, 0};
  servos(angles_test);

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

  Angles angles = calc_angles(coord);
  Serial.print("hoek1: ");
  Serial.println(angles.hoek1);
  Serial.print("hoek2: ");
  Serial.println(angles.hoek2);
  Serial.print("hoek3: ");
  Serial.println(angles.hoek3);
  Serial.println("");

  servos(angles);

  delay(2000);
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
  int z = coords.z;
  
  int x_servo = 0 + Larmbase;
  int z_servo = 0;

  for(char i = starthoek; i < eindhoek; i++)
  {
    int z_elleboog = z_servo + Larm1 * sin(DegreeToRadian(i));
    int x_elleboog = x_servo + Larm1 * cos(DegreeToRadian(i));
    int afstand = sqrt(pow(x - x_elleboog, 2) + pow(z - z_elleboog, 2));

    if((afstand - marge < Larm2) && (afstand + marge > Larm2))
    {
      // Serial.print("Oplossing gevonden, hoek 1 is: ");
      // Serial.print(i);
      // Serial.println(" graden!");
      return i;
    }
  }
  Serial.println("Geen oplossing gevonden");
  return 0;  
}

char calc_arm2(Coordinates coords)
{
  int x = coords.x - cos(DegreeToRadian(60)) * Ltop;
  int y = coords.y + sin(DegreeToRadian(60)) * Ltop;
  int z = coords.z;
  
  int x_servo = -cos(DegreeToRadian(60)) * Larmbase;
  int y_servo = sin(DegreeToRadian(60)) * Larmbase;
  int z_servo = 0;

  for(char i = starthoek; i < eindhoek; i++)
  {
    int x_elleboog = x_servo - cos(DegreeToRadian(60)) * Larm1 * cos(DegreeToRadian(i));
    int y_elleboog = y_servo + sin(DegreeToRadian(60)) * Larm1 * cos(DegreeToRadian(i));
    int z_elleboog = z_servo + Larm1 * sin(DegreeToRadian(i));
    int afstand = sqrt(pow(x - x_elleboog, 2) + pow(y - y_elleboog, 2) + pow(z - z_elleboog, 2));

    if((afstand - marge < Larm2) && (afstand + marge > Larm2))
    {
      // Serial.print("Oplossing gevonden, hoek 2 is: ");
      // Serial.print(i + 0);
      // Serial.println(" graden!");
      return i;
    }
  }
  Serial.println("Geen oplossing gevonden");
  return 0;
}

char calc_arm3(Coordinates coords)
{
  int x = coords.x - cos(DegreeToRadian(60)) * Ltop;
  int y = coords.y - sin(DegreeToRadian(60)) * Ltop;
  int z = coords.z;
  
  int x_servo = -cos(DegreeToRadian(60)) * Larmbase;
  int y_servo = -sin(DegreeToRadian(60)) * Larmbase;
  int z_servo = 0;

  for(char i = starthoek; i < eindhoek; i++)
  {
    int x_elleboog = x_servo - cos(DegreeToRadian(60)) * Larm1 * cos(DegreeToRadian(i));
    int y_elleboog = y_servo - sin (DegreeToRadian(60)) * Larm1 * cos(DegreeToRadian(i));
    int z_elleboog = z_servo + Larm1 * sin(DegreeToRadian(i));
    int afstand = sqrt(pow(x - x_elleboog, 2) + pow(y - y_elleboog, 2) + pow(z - z_elleboog, 2));

    if((afstand - marge < Larm2) && (afstand + marge > Larm2))
    {
      // Serial.print("Oplossing gevonden, hoek 3 is: ");
      // Serial.print(i);
      // Serial.println(" graden!");
      return i;
    }
  }
  Serial.println("Geen oplossing gevonden");
  return 0;
}

Angles calc_angles(Coordinates coords)
{
  Angles angles;
  angles.hoek1 = calc_arm1(coords);
  angles.hoek2 = calc_arm2(coords);
  angles.hoek3 = calc_arm3(coords);
  return angles;
}

void servos(Angles angles)
{
  servo1.write(-angles.hoek1 + 105);
  servo2.write(-angles.hoek2 + 105);
  servo3.write(-angles.hoek3 + 105);
}

#endif