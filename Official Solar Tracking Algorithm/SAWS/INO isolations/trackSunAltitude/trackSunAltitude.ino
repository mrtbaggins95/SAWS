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

int trackSunAltitude(int baseStepAl)
{
  float _baseStepAl = baseStepAl;  //later change this to the step amount obtained from sweepSun();
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
    float AlSteps = baseStepAl + (AlPIDOUT) / 8;
    
    if (AlSteps > 90 ) 
    {
      AlSteps= 90;
    }
   
    if (AlSteps < 0) 
    {
      AlSteps = 0;
    }
    
    AlStepper.step(AlSteps);
    baseStepAl = AlSteps;
    delay(40);
  }
}

