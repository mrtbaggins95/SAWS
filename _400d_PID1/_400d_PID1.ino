#include <Servo.h>
Servo AzServo;
Servo ElServo;

#define SensA A0
#define SensB A1
#define SensC A2
#define SensD A3


void setup() {
  Serial.begin(9600);
  trackSun();

}

// the loop routine runs over and over again forever:
void loop() {

}

void trackSun() {
  AzServo.attach(9);
  ElServo.attach(10);
  float BaseAngle = 90;
  unsigned long lastT = 0;
  float AzIerror = 0; // integral error
  float ElIerror = 0; 
  float LastAzPerror = 0; // last saved proportional error
  float LastElPerror = 0; 
  float kp = 0.38; // proportional gain
  float ki = 0.015; // integral gain
  float kd = 0.08; // derivative gain
  /*
     A|B
     D|C
  */


  int LeftAvg = 0.5 * (analogRead(SensA) + analogRead(SensD));
  int RightAvg = 0.5 * (analogRead(SensB) + analogRead(SensC));
  float AzPerror = 0 - (LeftAvg - RightAvg);

  while ((AzPerror > 2) | (AzPerror < -2)) {
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
    float AzServoAngle = BaseAngle + (AzPIDOUT) / 8;
    if (AzServoAngle > 180 ) {
      AzServoAngle = 180;
    }
    if (AzServoAngle < 0) {
      AzServoAngle = 0;
    }
    AzServo.write(AzServoAngle);
    BaseAngle = AzServoAngle;
    delay(40);
  }
  
  BaseAngle = 90;
  int TopAvg = 0.5 * (analogRead(SensA) + analogRead(SensB));
  int BotAvg = 0.5 * (analogRead(SensC) + analogRead(SensD));
  float ElPerror = 0 - (TopAvg - BotAvg);
  while ((ElPerror > 2) | (ElPerror < -2)) {
    TopAvg = 0.5 * (analogRead(SensA) + analogRead(SensB));
    BotAvg = 0.5 * (analogRead(SensC) + analogRead(SensD));
    unsigned long now = millis(); // current time
    float Tchange = double(now - lastT);  // time since last time through loop
    ElPerror = 0 - (TopAvg - BotAvg);
    ElIerror = (ElPerror) + ElIerror;
    float ElDerror = (ElPerror - LastElPerror) / Tchange;
    float ElPIDOUT = kp * ElPerror + ki * ElIerror + kd * ElDerror;
    LastElPerror = ElPerror;
    lastT = now;
    float ElServoAngle = BaseAngle + (ElPIDOUT) / 8;
    if (ElServoAngle > 180 ) {
      ElServoAngle = 180;
    }
    if (ElServoAngle < 0) {
      ElServoAngle = 0;
    }
    ElServo.write(ElServoAngle);
    BaseAngle = ElServoAngle;
    delay(40);
  }
}





