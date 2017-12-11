

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

int loop() //This function returns 0 = sweepSun or 1 = homingSun or 2 = standBy
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



