//detach stepper motors

/*
 * readSun(); command
 * Garrett Bondoc, James Donahue, Jordan Hsu, Sakib Karim, Matthew Rosales
 * 
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
  standBy();
}

void loop() //This function returns 0 = sweepSun or 1 = homingSun or 2 = standBy
{

}

void standBy()
{
  //detach_stepper_motors_please
}



