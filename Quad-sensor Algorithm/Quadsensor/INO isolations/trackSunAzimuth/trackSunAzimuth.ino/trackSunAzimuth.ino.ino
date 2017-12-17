#include <Stepper.h>
Stepper AzStepper = Stepper(200,1,2,3,4);
Stepper AlStepper = Stepper(200,5,6,7,8);
#define SensA A0
#define SensB A1
#define SensC A2
#define SensD A3


void setup() 
{
  Serial.begin(9600);
}

void loop() 
{

}

int trackSunAzimuth(int baseStepAz)
{
  float _baseStepAz = baseStepAz;  //later change this to the step amount obtained from sweepSun();
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
    float AzStepCheck = baseStepAz + AzDeltaSteps;
    
    if (AzStepCheck > 200) 
    {
      AzDeltaSteps = (AzStepCheck - 200) - baseStepAz;
    }
    
    if (AzStepCheck < 0)
    {
      AzDeltaSteps = 200 + AzStepCheck;
    }
  
    //normal function  
    AzStepper.step(AzDeltaSteps);
    baseStepAz = baseStepAz + AzDeltaSteps;
    return baseStepAz;
    delay(40);
  }
}

