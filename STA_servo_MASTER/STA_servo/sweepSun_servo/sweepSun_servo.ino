#include <Servo.h>
Servo AzServo;
Servo ElServo;

#define SensA A0
#define SensB A1
#define SensC A2
#define SensD A3


int sunData[181];
int positionData[181];

void setup() {
  Serial.begin(9600);
  AzServo.attach(9);
  ElServo.attach(10);
  
  for (int i = 0; i < 181; i++) //this block does the physical sweep of the panel to collect sun data at each position.
  {
    AzServo.write(i);
    delay(50); //allow time for the servo to reach the target position.
    int SenseAvg = 0.25*( analogRead(SensA) +analogRead(SensB)+ analogRead(SensC)+ analogRead(SensD)); //use all 4 sensors to get an accurate reading of the sun.
    sunData[i] = SenseAvg; //save the data at that point to an array
    positionData[i] = i;
  }

  int A = 0; // the trailing comparison
  int AIndex = -1;
  int BIndex = 0;
  int B = sunData[0]; // the leading comparison
  int sunMaxIndex = -1; // if this doesn't change, code will error on purpose to show that the if statement is dysfunctional
  
  for(int j = 0; j < 181; j++) //this block sorts through the 2 created arrays in order to find the highest value sun and turn to it.
  {
    int sunMax = max(A,B); // compare A to B and save the index of whichever is greater
    if (sunMax == B) // if the present (not past A) value was greater...
    {
      sunMaxIndex = j; // Think of the comparison as causal, if present index has a big value, save it and use to return to that degree position
    }
    //after comparison, move up the window.
    // Here's a visual: START [] { [] .. .. .. .. .. .. .. ---> .. .. END}
    //                  START .. { .. .. .. .. [] [] .. .. ---> .. .. END}

    //move up the indices and analyze the next two saved sun data values.
    AIndex++;
    BIndex++;
    A = sunData[AIndex];
    B = sunData[BIndex];
    
  }

  // After finding the index of the max sun, turn to it
  AzServo.write(positionData[sunMaxIndex]);

  //we have found the sun... transition to trackSun_servo();
}

void loop() {
}
