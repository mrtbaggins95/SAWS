#include <Stepper.h>

// change this to the number of steps on your motor
#define STEPS 400
// create an instance of the stepper class, specifying
// the number of steps of the motor and the pins it's
// attached to
Stepper stepperB(STEPS, 4, 5, 7, 6); // Ain2 Ain1 Bin1 Bin2
//Stepper stepperB(STEPS, 34, 38, 46, 48); // Ain2 Ain1 Bin1 Bin2


void setup()
{
  Serial.begin(9600);
  Serial.println("Stepper test!");
  //stepperA.setSpeed(135);
  stepperB.setSpeed(135);

}

void loop()
{
  //stepperA.step(40 * 400);
  //stepperB.step(40 * 400);


  for (int Speed = 100; Speed < 300; Speed = Speed + 10) {
    stepperB.setSpeed(Speed);
    Serial.println(Speed);
    stepperB.step(800);
  }

  for (int Speed = 300; Speed > 100; Speed = Speed - 10) {
    stepperB.setSpeed(Speed);
    Serial.println(Speed);
    stepperB.step(800);
  }





}
