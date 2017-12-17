#include <Stepper.h>
Stepper AzStepper(STEPS, 1,2,3,4);
//Stepper AzStepper(STEPS, 34, 38, 46, 48);
#define SensA A0
#define SensA A1
#define SensA A2
#define SensA A3

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
  float baseStep = 100;  //later change this to the step amount obtained from sweepSun();
  unsigned long lastT = 0;
  float AzIerror = 0; // integral error
  float AlIerror = 0; 
  float LastAzPerror = 0; // last saved proportional error
  float LastElPerror = 0; 
  float kp = 0.38; // proportional gain
  float ki = 0.015; // integral gain
  float kd = 0.08; // derivative gain

  // THIS WAS OUR PERIPHERAL OPTION
  /*define scope view of PID
  int photoSight[] = {baseStep - 5,baseStep - 4,baseStep - 3,baseStep - 2,baseStep - 1,baseStep,baseStep + 1,baseStep + 2,baseStep + 3,baseStep + 4,baseStep + 5,}
  if(baseStep - 5 < 0 || baseStep + 5 > 359)
  {
    for
  }

  int LeftAvg = 0.5 * (analogRead(SensA) + analogRead(SensD));
  int RightAvg = 0.5 * (analogRead(SensB) + analogRead(SensC));
  float AzPerror = 0 - (LeftAvg - RightAvg);

     A|B
     D|C
  */


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
    float AzStepCheck = baseStep + AzDeltaSteps;
    
    if (AzStepsCheck > 200) 
    {
      AzDeltaSteps = (AzStepCheck - 200) - baseStep;
    }
    
    if (AzStepCheck < 0)
    {
      AzDeltaSteps = 200 + AzStepCheck;
    }
  
    //normal function  
    AzStepper.step(AzDeltaSteps);
    baseStep = baseStep + AzDeltaSteps;
    delay(40);
  }

  //Altitude Correction
  baseStep = 90;
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
    float AlSteps = baseStep + (AlPIDOUT) / 8;
    
    if (AlSteps > 90 ) 
    {
      AlSteps= 90;
    }
   
    if (AlSteps < 0) 
    {
      AlSteps = 0;
    }
    
    AlServo.write(AlSteps);
    baseStep = AlSteps;
    delay(40);
  }
}

