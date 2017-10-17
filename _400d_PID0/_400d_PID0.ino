
/*

  Name: _400d_PID0
  Author: James Donahue
  Date: 10/7/17
  Version 1.0
  This program is an initial test sketch to demonstrate PID control of a single axis, azimuth sun tracker. 
  The sun intensity is recorded using 4 phototransister voltage dividers. The sensors output a light proportional
  voltage between 0 and 5 volts. Each sensor is separated from the others with a opaque divider.  
  The sensor orientation is shown below:
              Aximuth        
                | 
              A | B
  Elevation -------
              D | C

 The following PID controller monitors sunlight intensity values from each sensor and generates a PID output
 that controls an azimuth servo. The 'quad' sensor is mounted on top of the azimuth servo. The PID controller 
 continually updates a servo angle to minimize the difference between the left and right average sun intensity.          
*****************************************************************************************************************
*/
#include <Servo.h>
#define SensA A0            // Sensor A, upper right sensor
#define SensB A1            // Sensor B, upper left sensor
#define SensC A2            // Sensor C, lower right sensor
#define SensD A3            // Sensor D, lower left sensor 
float BaseAngle = 90;       // Initial servo angle 
Servo AzServo; 
unsigned long lastT = 0;    // 
float AzIerror = 0;         // stores the integral error
float LastAzPerror = 0;     // stores last saved proportional error
float kp = 0.38;            // proportional gain
float ki = 0.018;           // integral gain
float kd = 0.08;            // derivative gain

void setup() {
  AzServo.attach(9);        // Attach servo AzServo to pin 9 

}
void loop() {
  int LeftAvg = 0.5 * (analogRead(SensA) + analogRead(SensD));    // calc the avg light value from the left sensors A and D
  int RightAvg = 0.5 * (analogRead(SensB) + analogRead(SensC));   // calc the avg light value from the right sensors B and C
  float AzPerror = 0 - (LeftAvg - RightAvg);                      // AzPerror is the proportional error. 
  unsigned long now = millis();                                   // current time
  float Tchange = double(now - lastT);                            // 
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
