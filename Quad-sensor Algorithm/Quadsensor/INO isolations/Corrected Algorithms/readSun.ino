/*
 * readSun(); command
 * Garrett Bondoc, James Donahue, Jordan Hsu, Sakib Karim, Matthew Rosales
 * 
 * Description:
 * Photoresistors take a sun intensity reading and decides how to proceed based on sufficient lighting.
 */

#define photoA A0
#define photoB A1
#define photoC A2
#define photoD A3

//int threshold
//global readSun() Variables
//global sweepSun() Variables
//gloabl trackSun() Variables
//

void setup() 
{  
  Serial.begin(9600); // initialize serial communication at 9600 bits per second:
  readSun();
}

void loop() //This function returns 0 = sweepSun or 1 = homingSun or 2 = standBy
{

}

int readSun()
{
  // define local variable: int option = 0 by default
  int ValA = analogRead(photoA);
  int ValB = analogRead(photoB);
  int ValC = analogRead(photoC);
  int ValD = analogRead(photoD);
  int sunRead = ValA + ValB + ValC + ValD;
  int decision = 1;
  int threshold = 500; //determined by holding a hand at overhead, shading the photoresistor.
  int rightSide = ValA + ValD;
  int leftSide = ValB + ValC;
  
  if(sunRead < 1800)
  {
    return decision = 1; //light sensor is collectively low, go to sweepSun();
  }

  if(sunRead > 1800 && abs(rightSide - leftSide) > 100)
  {
    return decision = 2; //light sensor is above threshold but the left != the right
  }
  
  else
  {
    return decision = 3; //reading is OK, just standBy --> unused condition in the main program, standBy is default.
  }
  
}



