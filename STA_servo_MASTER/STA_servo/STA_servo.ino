

#include <Servo.h>
Servo Azimuth;
Servo Altitude;

#define photoA A0
#define photoB A1
#define photoC A2
#define photoD A3

//int threshold
//global readSun() Variables
//global sweepSun() Variables
//gloabl trackSun() Variables
//

void setup() {  
  Serial.begin(9600); // initialize serial communication at 9600 bits per second:
  Azimuth.attach(9);
  Altitude.attach(10);
}

void loop() 
{
  // if RTC is morning
  // if User console initiates 
  
readSun_servo();
// perform code based on 'option'


int readSun() //This function returns 0 = sweepSun or 1 = homingSun or 2 = standBy
{
  // define local variable: int option = 0 by default
  int ValA = analogRead(photoA);
  int ValB = analogRead(photoB);
  int ValC = analogRead(photoC);
  int ValD = analogRead(photoD); 
  // if ValA,B,C,D are all values below threshold, go to sweepSun() --> option == 0
  // if ValA,B,C,D are all values above threshold but NOT EQUAL, go to trackSun() --> option == 1
  // if ValA,B,C,D are all values above threshold and EQUAL, go to standBy() --> option == 2
  // return 'option' back to parent
}

void sweepSun() // --> contains shutDown() or AKA nightMode()
{
  // return to home stop switch
  // make full rotation along azimuth
  // use 1 sensor and save light intensity at each degree
  // go back to degree with highest value light
  // 
  // if no values are found above threshold, repeat
  // otherwise go to trackSun()
    // if attempted 3 times and sun not found, go to nightMode()
    // otherwise go to trackSun()
}

void trackSun()
{  
  // utilize PID error formulas
  // equalize Azimuth
  // equalize Altitude
  // enter standBy()

}

void nightMode()
{
  // go to this mode when RTC says night or sweep fails
  // compare with standBy()
  // motors off
}

void standBy()
{
  // when the panel finishes tracking and calibrating
  // exit standBy to recalibrate every 30 min (RTC)
  // motors off
}

}

