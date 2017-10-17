#include <PID_v1.h>
#include <Servo.h>
Servo AzServo;
Servo ElServo;

#define SensA A0
#define SensB A1
#define SensC A2
#define SensD A3

double Setpoint, Input, Output;
//Specify the links and initial tuning parameters
PID azPID(&Input, &Output, &Setpoint,2,5,1, DIRECT);

void setup()
{
  uint16_t LeftAvg = 0.5 * (analogRead(SensA) + analogRead(SensD));
  uint16_t RightAvg = 0.5 * (analogRead(SensB) + analogRead(SensC));
  uint8_t SampleFreq = 100; // Hz
  Input =  RightAvg - LeftAvg;

  Setpoint = 0;

  //turn the PID on 
  
  azPID.SetMode(AUTOMATIC);
  //SetSampleTime(1000/SampleFreq);
 
}

void loop()
{
  uint16_t LeftAvg = 0.5 * (analogRead(SensA) + analogRead(SensD));
  uint16_t RightAvg = 0.5 * (analogRead(SensB) + analogRead(SensC));
  Input =  RightAvg - LeftAvg;
  azPID.Compute();
  Serial.println(Output);
  
}
