#include <Stepper.h>
int STEPS = 400;
Stepper AzStepper(STEPS, 34, 38, 46, 48);

#define SensA A8
#define SensB A9
#define SensC A10
#define SensD A11

void setup()
{
 
  Serial.begin(9600);
  trackSun();

}

void loop()
{

}

void trackSun()
{
  
  float baseStep = 0;  //later change this to the step amount obtained from sweepSun();
  unsigned long lastT = 0;

  float AzIerror = 0; // integral error
  float LastAzPerror = 0; // last saved proportional error

  float kp = 1; // proportional gain
  float ki = 0; // integral gain
  float kd = 0; // derivative gain

  float LeftAvg = 0.5 * (analogRead(SensB) + analogRead(SensC));
  float  RightAvg = 0.5 * (analogRead(SensA) + analogRead(SensD));
  float AzPerror = (LeftAvg - RightAvg);

  //Azimuth Correction
  
  while ((AzPerror > 2) | (AzPerror < -2))
    {
      //compare averages
      LeftAvg = 0.5 * (analogRead(SensB) + analogRead(SensC));
      RightAvg = 0.5 * (analogRead(SensA) + analogRead(SensD));

      //timestamp
      unsigned long now = millis(); // current time
      float Tchange = double(now - lastT);  // time since last time through loop

      //PID formula, determine a number of steps to adjust
      AzPerror = 0 - (LeftAvg - RightAvg);
      Serial.print(AzPerror);
      Serial.print(" Error   Steps ");
      AzIerror = (AzPerror) + AzIerror;
      float AzDerror = (AzPerror - LastAzPerror) / Tchange;
      LastAzPerror = AzPerror;
      lastT = now;
      int AzDeltaSteps =  (kp * AzPerror + ki * AzIerror + kd * AzDerror)*0.5;

      //extremity check
      //int AzStepCheck = baseStep + AzDeltaSteps;
      /*
      if (AzStepCheck > STEPS)
      {
        AzDeltaSteps = (AzStepCheck - STEPS) - baseStep;
      }

      if (AzStepCheck < 0)
      {
        AzDeltaSteps = STEPS + AzStepCheck;
      }
      */

      //normal function
      
      AzStepper.step(AzDeltaSteps);
      baseStep = baseStep + AzDeltaSteps;
      Serial.println(AzDeltaSteps);
      
      
      
      
      
    }

  
}

