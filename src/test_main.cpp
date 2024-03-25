#ifdef TEST
#include <Arduino.h>
#include <Stepper.h>
int motorspeed = 10;
Stepper myStepper(2048, 8, 9, 10, 11);

void setup()
{
    myStepper.setSpeed(motorspeed);
}

void loop()
{
    myStepper.step(2048);
    delay(1000);
    myStepper.step(-2048);
    delay(1000);
}
#endif