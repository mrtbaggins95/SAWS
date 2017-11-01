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

int sweepSun() // this function always creates a new baseStepAz
{
  Serial.begin(9600);

    //variables for loop 1
  int sunData[181];
  int positionData[181];
  int SenseAvg = 0;

  for (int i = 0; i <= 180; i++) //this block does the physical sweep of the panel to collect sun data at each position.
  {
    AzStepper.step(1);
    delay(50); //allow time for the servo to reach the target position.
    SenseAvg = 0.25 * ( analogRead(SensA) + analogRead(SensB) + analogRead(SensC) + analogRead(SensD)); //use all 4 sensors to get an accurate reading of the sun.
    sunData[i] = SenseAvg; //save the data at that point to an array
    positionData[i] = i; //save the number of steps associated with that sun reading
  }

  // variables for loop 2
  int A = 0; // the trailing comparison
  int B = sunData[0]; // the leading comparison
  int sunMaxIndex = -1; // if this doesn't change, code will error on purpose to show that the if statement is dysfunctional
  int sunMaxOld = 0;
  int sunMaxNew = 0; // the result of comparing A and B
  int AIndex = -1; // pointer to extract value A from sunData (don't use until incremented into the scope of the array after 1 iteration of the loop)
  int BIndex = 0; // pointer to extract value B from sunData

  for (int j = 0; j <= 180; j++) //this block sorts through the 2 created arrays in order to find the highest value sun and turn to it.
  {
    sunMaxNew = max(A, B); // compare A to B and save the index of whichever is greater within the if statement
    if (sunMaxNew == B && sunMaxNew > sunMaxOld) // if the present (not past A) value was greater than the trailing, it is worth a look to see if it is greater than the old maximum. If so, we have a new high Sun value.
    {
      sunMaxIndex = j; // Think of the comparison as causal, if present index has a big value, save it and use to return to that degree position
      sunMaxOld = B; // Keep track of this new highest sun Value
    }
    //after comparison, move up the window.
    // Here's a visual: START [A] { [B] .. .. .. .. .. .. .. ---> .. .. END}
    //                  START .. { .. .. .. .. [A] [B] .. .. ---> .. .. END}
    AIndex++;
    BIndex++;
    A = sunData[AIndex]; // extract new A value
    B = sunData[BIndex]; // extract new B value
  }

  // After finding the index of the max sun, turn to it
  AzStepper.step(positionData[sunMaxIndex] - 180); // use negative complementary amount of steps to turn back to the high Sun position.
  int baseStepAz = positionData[sunMaxIndex]; // save amount we stepped away from initial home 0 steps. Altitude baseStepAl remains at zero and will be set by trackSunAltitude()
  return baseStepAz;
}

