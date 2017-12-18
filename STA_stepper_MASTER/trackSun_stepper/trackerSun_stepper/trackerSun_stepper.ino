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
  trackSun();
  delay(5000);
}

void trackSun()
{
#define Stby 36
  pinMode(Stby, OUTPUT);
  AzStepper.setSpeed(50);
  float baseStep = 0;  //later change this to the step amount obtained from sweepSun();
  unsigned long lastT = 0;
  float AzIerror = 0; // integral error
  float LastAzPerror = 0; // last saved proportional error
  float kp = 0.05; // proportional gain
  float ki = 0; // integral gain
  float kd = 0; // derivative gain
  float LeftAvg = 0.5 * (analogRead(SensB) + analogRead(SensC));
  float  RightAvg = 0.5 * (analogRead(SensA) + analogRead(SensD));
  float AzPerror =  (RightAvg - LeftAvg);
  //Azimuth Correction
  digitalWrite(Stby, HIGH);

  while (abs(AzPerror) > 24 )
  {    
    unsigned long CurrentT = millis(); // current time
    float Tchange = double(CurrentT - lastT);  // time since last time through loop
    //PID formula, determine a number of steps to adjust
    //AzPerror = 0 - (LeftAvg - RightAvg)
    Serial.print(AzPerror);
    Serial.print(" Error   Steps ");
    AzIerror = (AzPerror) + AzIerror;
    float AzDerror = (AzPerror - LastAzPerror) / Tchange;
    LastAzPerror = AzPerror;
    lastT = CurrentT;
    int16_t AzDeltaSteps =  (kp * AzPerror + ki * AzIerror + kd * AzDerror);
    //extremity check
    //int AzStepCheck = baseStep + AzDeltaSteps;
    /*
      if (AzStepCheck > STEPS)
      {
      AzDeltaSteps = (AzStepCheck - STEPS) - baseStep;

      if (AzStepCheck < 0)
      {
      AzDeltaSteps = STEPS + AzStepCheck;
      }
    */
    //normal function
    if ((AzDeltaSteps < 20) & (AzDeltaSteps > 0)) {
      AzDeltaSteps = 20;
    }
    if ((AzDeltaSteps < 0) & (AzDeltaSteps > -20)) {
      AzDeltaSteps = -20;
    }
    AzStepper.step(-1 * AzDeltaSteps);
    baseStep = baseStep + AzDeltaSteps;
    Serial.println(AzDeltaSteps);
    LeftAvg = 0.5 * (analogRead(SensB) + analogRead(SensC));
    RightAvg = 0.5 * (analogRead(SensA) + analogRead(SensD));
    AzPerror = 0 - (LeftAvg - RightAvg);
  }
  digitalWrite(Stby, LOW);
}

