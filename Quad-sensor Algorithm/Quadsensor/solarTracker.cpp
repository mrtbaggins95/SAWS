//This is the source file for the solarTracker Library
#include "Arduino.h"
#include "solarTracker.h"
#include <Stepper.h>

solarTracker::solarTracker(int baseStepAz, int baseStepAl)
{
  Stepper AzStepper = Stepper(400, 1, 2, 3, 4);
  Stepper AlStepper = Stepper(400, 5, 6, 7, 8);
  #define SensA A0
  #define SensB A1
  #define SensC A2
  #define SensD A3

  _baseStepAz = baseStepAz;
  _baseStepAl = baseStepAl;
  volatile int Home = false;
}



int solarTracker::readSun()
{
  // define local variable: int option = 0 by default
  int ValA = analogRead(SensA);
  int ValB = analogRead(SensB);
  int ValC = analogRead(SensC);
  int ValD = analogRead(SensD);
  int sunRead = ValA + ValB + ValC + ValD;
  int decision = 1;
  int threshold = 500; //determined by holding a hand at overhead, shading the photoresistor.
  int rightSide = ValA + ValD;
  int leftSide = ValB + ValC;

  if (sunRead < 1800)
  {
    return decision = 1; //light sensor is collectively low, go to sweepSun();
  }

  if (sunRead > 1800 && abs(rightSide - leftSide) > 100)
  {
    return decision = 2; //light sensor is above threshold but the left != the right
  }

  else
  {
    return decision = 3; //reading is OK, just standBy --> unused condition in the main program, standBy is default.
  }
}



int solarTracker::sweepSun() // this function always creates a new baseStepAz
{
  Serial.begin(9600);
  Stepper AzStepper(400, 4, 5, 6, 7);

  //variables for loop 1
  int sunData[181];
  int positionData[181];
  int SenseAvg = 0;

  for (int i = 0; i <= 180; i++) //this block does the physical sweep of the panel to collect sun data at each position.
  {
    AzStepper.step(1);
    delay(50); //allow time for the servo to reach the target position.
    SenseAvg = 0.25 * ( analogRead(SensA) + analogRead(SensB) + analogRead(SensC) + analogRead(SensD)); //use all 4 sensors to get an accurate reading of the sun.
    sunData[i] = SenseAvg; //save the data at that point to an array
    positionData[i] = i; //save the number of steps associated with that sun reading
  }

  // variables for loop 2
  int A = 0; // the trailing comparison
  int B = sunData[0]; // the leading comparison
  int sunMaxIndex = -1; // if this doesn't change, code will error on purpose to show that the if statement is dysfunctional
  int sunMaxOld = 0;
  int sunMaxNew = 0; // the result of comparing A and B
  int AIndex = -1; // pointer to extract value A from sunData (don't use until incremented into the scope of the array after 1 iteration of the loop)
  int BIndex = 0; // pointer to extract value B from sunData

  for (int j = 0; j <= 180; j++) //this block sorts through the 2 created arrays in order to find the highest value sun and turn to it.
  {
    sunMaxNew = max(A, B); // compare A to B and save the index of whichever is greater within the if statement
    if (sunMaxNew == B && sunMaxNew > sunMaxOld) // if the present (not past A) value was greater than the trailing, it is worth a look to see if it is greater than the old maximum. If so, we have a new high Sun value.
    {
      sunMaxIndex = j; // Think of the comparison as causal, if present index has a big value, save it and use to return to that degree position
      sunMaxOld = B; // Keep track of this new highest sun Value
    }
    //after comparison, move up the window.
    // Here's a visual: START [A] { [B] .. .. .. .. .. .. .. ---> .. .. END}
    //                  START .. { .. .. .. .. [A] [B] .. .. ---> .. .. END}
    AIndex++;
    BIndex++;
    A = sunData[AIndex]; // extract new A value
    B = sunData[BIndex]; // extract new B value
  }

  // After finding the index of the max sun, turn to it
  AzStepper.step(positionData[sunMaxIndex] - 180); // use negative complementary amount of steps to turn back to the high Sun position.
  _baseStepAz = positionData[sunMaxIndex]; // save amount we stepped away from initial home 0 steps. Altitude baseStepAl remains at zero and will be set by trackSunAltitude()
  return _baseStepAz;
}

int solarTracker::trackSunAzimuth()
{
  Stepper AzStepper(400, 1, 2, 3, 4);
  unsigned long lastT = 0;
  float AzIerror = 0; // integral error
  float LastAzPerror = 0; // last saved proportional error
  float kp = 0.38; // proportional gain
  float ki = 0.015; // integral gain
  float kd = 0.08; // derivative gain

  int LeftAvg = 0.5 * (analogRead(SensA) + analogRead(SensD));
  int RightAvg = 0.5 * (analogRead(SensB) + analogRead(SensC));
  float AzPerror = 0 - (LeftAvg - RightAvg);

  //Azimuth Correction
  while ((AzPerror > 2) | (AzPerror < -2))
  {
    //compare averages
    LeftAvg = 0.5 * (analogRead(SensA) + analogRead(SensD));
    RightAvg = 0.5 * (analogRead(SensB) + analogRead(SensC));

    //timestamp
    unsigned long now = millis(); // current time
    float Tchange = double(now - lastT);  // time since last time through loop

    //PID formula, determine a number of steps to adjust
    AzPerror = 0 - (LeftAvg - RightAvg);
    AzIerror = (AzPerror) + AzIerror;
    float AzDerror = (AzPerror - LastAzPerror) / Tchange;
    LastAzPerror = AzPerror;
    lastT = now;
    float AzDeltaSteps = (kp * AzPerror + ki * AzIerror + kd * AzDerror) / 8;

    //extremity check
    float AzStepCheck = _baseStepAz + AzDeltaSteps;

    if (AzStepCheck > 400)
    {
      AzDeltaSteps = (AzStepCheck - 400) - _baseStepAz;
    }

    if (AzStepCheck < 0)
    {
      AzDeltaSteps = 400 + AzStepCheck;
    }

    //normal function
    AzStepper.step(AzDeltaSteps);
    _baseStepAz = _baseStepAz + AzDeltaSteps;
    return _baseStepAz;
    delay(40);
  }
}



int solarTracker::trackSunAltitude()
{
  Stepper AlStepper(400, 34, 38, 46, 48 ); // Ain2 Ain1 Bin1 Bin2
  unsigned long lastT = 0;
  float AlIerror = 0;
  float LastAlPerror = 0; // last saved proportional error
  float kp = 0.38; // proportional gain
  float ki = 0.015; // integral gain
  float kd = 0.08; // derivative gain

  //Altitude Correction
  int TopAvg = 0.5 * (analogRead(SensA) + analogRead(SensB));
  int BotAvg = 0.5 * (analogRead(SensC) + analogRead(SensD));
  float AlPerror = 0 - (TopAvg - BotAvg);

  while ((AlPerror > 2) | (AlPerror < -2))
  {
    TopAvg = 0.5 * (analogRead(SensA) + analogRead(SensB));
    BotAvg = 0.5 * (analogRead(SensC) + analogRead(SensD));
    unsigned long now = millis(); // current time
    float Tchange = double(now - lastT);  // time since last time through loop
    AlPerror = 0 - (TopAvg - BotAvg);
    AlIerror = (AlPerror) + AlIerror;
    float AlDerror = (AlPerror - LastAlPerror) / Tchange;
    float AlPIDOUT = kp * AlPerror + ki * AlIerror + kd * AlDerror;
    LastAlPerror = AlPerror;
    lastT = now;
    float AlSteps = _baseStepAl + (AlPIDOUT) / 8;

    if (AlSteps > 90 )
    {
      AlSteps = 90;
    }

    if (AlSteps < 0)
    {
      AlSteps = 0;
    }

    AlStepper.step(AlSteps);
    _baseStepAl = AlSteps;
    delay(40);
  }
}




