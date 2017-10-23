#include <Stepper.h>
Stepper AzStepper = Stepper(200,1,2);
Stepper AlStepper = Stepper(200,3,4);
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
  float baseStep = 99;  //later change this to the step amount obtained from sweepSun();
  unsigned long lastT = 0;
  float AzIerror = 0; // integral error
  float AlIerror = 0; 
  float LastAzPerror = 0; // last saved proportional error
  float LastElPerror = 0; 
  float kp = 0.38; // proportional gain
  float ki = 0.015; // integral gain
  float kd = 0.08; // derivative gain

  //define scope view of PID
  int photoSight[] = {baseStep - 5,baseStep - 4,baseStep - 3,baseStep - 2,baseStep - 1,baseStep,baseStep + 1,baseStep + 2,baseStep + 3,baseStep + 4,baseStep + 5,}
  if(baseStep - 5 < 0 || baseStep + 5 > 359)
  {
    for
  }

  int LeftAvg = 0.5 * (analogRead(SensA) + analogRead(SensD));
  int RightAvg = 0.5 * (analogRead(SensB) + analogRead(SensC));
  float AzPerror = 0 - (LeftAvg - RightAvg);

  
  /*
     A|B
     D|C
  */


  while ((AzPerror > 2) | (AzPerror < -2)) 
  {
    LeftAvg = 0.5 * (analogRead(SensA) + analogRead(SensD));
    RightAvg = 0.5 * (analogRead(SensB) + analogRead(SensC));
    unsigned long now = millis(); // current time
    float Tchange = double(now - lastT);  // time since last time through loop
    AzPerror = 0 - (LeftAvg - RightAvg);
    AzIerror = (AzPerror) + AzIerror;
    float AzDerror = (AzPerror - LastAzPerror) / Tchange;
    float AzPIDOUT = kp * AzPerror + ki * AzIerror + kd * AzDerror;
    LastAzPerror = AzPerror;
    lastT = now;
    float AzSteps = baseStep + (AzPIDOUT) / 8;
    
    if (AzSteps > 180 ) 
    {
      AzSteps = 180;
    }
    
    if (AzSteps < 0)
    {
      AzSteps = 0;
    }
    
    AzServo.write(AzSteps);
    baseStep = AzSteps;
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
    
    if (AlSteps > 180 ) 
    {
      AlSteps= 180;
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

