#ifdef MAIN

#include <Arduino.h>
#include <math.h>
#include <servo.h>
#include <stepper.h>

// lengtes in mm
#define Larm1 50
#define Larm2 210
#define Larmbase 100
#define Ltop 35

#define starthoek -30
#define eindhoek 80
#define marge 1

#define motorspeed 5
#define fill 1
#define empty 0
#define millilitre 100

Stepper stepper1(1000, 8, 9, 10, 11);
Stepper stepper2(1000, 2, 7, 4, 1);

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
void move(Coordinates coords);
void stepper(int steps, char direction);
void pipette(char amount_ml, char direction);
void rinse(void);

Coordinates cup1 = {-70, 15, 200};
Coordinates cup1_above = {-70, 15, 180};
Coordinates cup2 = {-30, 50, 200};
Coordinates cup2_above = {-30, 50, 180};
Coordinates waste = {45, 30, 200};
Coordinates waste_above = {45, 30, 180};
Coordinates tube1 = {-16, -76, 200};
Coordinates tube1_above = {-16, -76, 180};
Coordinates tube2 = {5, -64, 200};
Coordinates tube2_above = {5, -64, 180};
Coordinates tube3 = {33, -48, 200};
Coordinates tube3_above = {33, -48, 180};

void setup()
{
  Serial.begin(9600);

  servo1.attach(3);
  servo2.attach(5);
  servo3.attach(6);

  stepper1.setSpeed(motorspeed);
  stepper2.setSpeed(motorspeed);

  move({0, 0, 180});
  delay(2000);
}

void loop()
{
  // tube 1 -> cup1 cup2 1:2
  move(cup1_above);
  move(cup1);
  pipette(1, fill);
  move(cup1_above);

  move(tube1_above);
  move(tube1);
  pipette(1, empty);
  move(tube1_above);

  rinse();

  move(cup2_above);
  move(cup2);
  pipette(2, fill);
  move(cup2_above);

  move(tube1_above);
  move(tube1);
  pipette(1, empty);
  move(tube1_above);

  rinse();

// tube 2 -> cup1 cup2 1:1
  move(cup1_above);
  move(cup1);
  pipette(1, fill);
  move(cup1_above);

  move(tube1_above);
  move(tube1);
  pipette(1, empty);
  move(tube1_above);

  rinse();

  move(cup2_above);
  move(cup2);
  pipette(1, fill);
  move(cup2_above);

  move(tube1_above);
  move(tube1);
  pipette(1, empty);
  move(tube1_above);

  rinse();

// tube 2 -> cup1 cup2 2:1
  move(cup1_above);
  move(cup1);
  pipette(2, fill);
  move(cup1_above);

  move(tube1_above);
  move(tube1);
  pipette(2, empty);
  move(tube1_above);

  rinse();

  move(cup2_above);
  move(cup2);
  pipette(1, fill);
  move(cup2_above);

  move(tube1_above);
  move(tube1);
  pipette(1, empty);
  move(tube1_above);

  rinse();

  move({0, 0, 180});
  delay(5000);
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

  for(char j = 0; j < 4; j++)
  {
    for(char i = starthoek; i < eindhoek; i++)
    {
      int z_elleboog = z_servo + Larm1 * sin(DegreeToRadian(i));
      int x_elleboog = x_servo + Larm1 * cos(DegreeToRadian(i));
      int afstand = sqrt(pow(x - x_elleboog, 2) + pow(z - z_elleboog, 2));

      if((afstand - (marge + j) < Larm2) && (afstand + (marge + j) > Larm2))
      {
        // Serial.print("Oplossing gevonden, hoek 1 is: ");
        // Serial.print(i);
        // Serial.println(" graden!");
        return i;
      }
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

  for(char j = 0; j < 4; j++)
  {
    for(char i = starthoek; i < eindhoek; i++)
    {
      int x_elleboog = x_servo - cos(DegreeToRadian(60)) * Larm1 * cos(DegreeToRadian(i));
      int y_elleboog = y_servo + sin(DegreeToRadian(60)) * Larm1 * cos(DegreeToRadian(i));
      int z_elleboog = z_servo + Larm1 * sin(DegreeToRadian(i));
      int afstand = sqrt(pow(x - x_elleboog, 2) + pow(y - y_elleboog, 2) + pow(z - z_elleboog, 2));

      if((afstand - (marge + j) < Larm2) && (afstand + (marge + j) > Larm2))
      {
        // Serial.print("Oplossing gevonden, hoek 2 is: ");
        // Serial.print(i + 0);
        // Serial.println(" graden!");
        return i;
      }
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

  for(char j = 0; j < 4; j++)
  {
    for(char i = starthoek; i < eindhoek; i++)
    {
      int x_elleboog = x_servo - cos(DegreeToRadian(60)) * Larm1 * cos(DegreeToRadian(i));
      int y_elleboog = y_servo - sin (DegreeToRadian(60)) * Larm1 * cos(DegreeToRadian(i));
      int z_elleboog = z_servo + Larm1 * sin(DegreeToRadian(i));
      int afstand = sqrt(pow(x - x_elleboog, 2) + pow(y - y_elleboog, 2) + pow(z - z_elleboog, 2));

      if((afstand - (marge + j) < Larm2) && (afstand + (marge + j) > Larm2))
      {
        // Serial.print("Oplossing gevonden, hoek 3 is: ");
        // Serial.print(i);
        // Serial.println(" graden!");
        return i;
      }
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

void move(Coordinates coords)
{
  Angles angles = calc_angles(coords);
  servos(angles);
  delay(500);
}

void stepper(int steps, char direction)
{
  if(direction == 0) // down
  {
    for(int i = 0; i < steps; i++)
    {
      stepper1.step(1);
      stepper2.step(1);
      delayMicroseconds(10);
    }
  }
  else if(direction == 1) // up
  {
    for(int i = 0; i < steps; i++)
    {
      stepper1.step(-1);
      stepper2.step(-1);
      delayMicroseconds(10);
    }
  }
}

void pipette(char amount_ml, char direction)
{
  int steps = amount_ml * millilitre;
  stepper(steps, direction);
}

void rinse()
{
  move(waste_above);
  move(waste);
  pipette(1, fill);
  pipette(1, empty);
  move(waste_above);
}

#endif