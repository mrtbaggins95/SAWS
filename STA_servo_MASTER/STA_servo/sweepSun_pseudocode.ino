

#include <Servo.h>
Servo Azimuth;
Servo Altitude;

#define photoA A0
#define photoB A1
#define photoC A2
#define photoD A3

unsigned long lastT = 0;
float AzHome = 90;
float AlHome = 0;
int positionMemory[//numDegrees];
int solarMemory[//sunValue];
int newPosition = AzHome;

void setup() 
{  
  Serial.begin(9600); // initialize serial communication at 9600 bits per second:
  Azimuth.attach(9);
  Altitude.attach(10);
}

void loop()
{
  
  //go to home
  Azimuth.write(AzHome);
  Altitude.write(AlHome);

  //only need to find strong sun on azimuth, then homeSun will adjust
  //this loop gets all available sun data
  for(int deg = 0; deg <= 180; deg++)
  {
      solarMemory[deg] = analogRead(photoA); //acquire solar value and save
      positionMemory[deg] = newPosition; //save position at which that solar reading was taken
      newPosition++; //update position
      Azimuth.write(newPosition); //move servo to new position and repeat for all degrees
  }

  //COMPUTE: this loop finds the greatest sun Value and goes to its position
  //if attempts != 3
  for(int deg = 0; deg <=180; deg++)
  {
    //do comparative sort algorithm? if pres>prev then prev = 0
    //@ solarMemory valueMax, save deg as index
    //if a value isn't found in the solarMemory, then do the sweep loop again
  }
  //else shutDown();

  //go to as a Azimuth.write(positionMemory[degIndex]);
  //panel has gone to strongest sun, initiate homeSun();
}



