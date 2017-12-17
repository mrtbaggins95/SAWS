#include <Stepper.h>
#include "solarTracker.h"
#include <RTClib.h>
//#include user defined functions.

int globalStepAz = 0;
int globalStepAl = 0;
solarTracker quadsensor = solarTracker(globalStepAz,globalStepAl);


//RTC
#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
   #define Serial SerialUSB
#endif

RTC_PCF8523 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

volatile int Home = false;

void setup() 
{
  //homeStepper//
    attachInterrupt(digitalPinToInterrupt(2), pin_ISR, LOW); // Defining an active low interrupt on the switch pin 2
  
  #ifndef ESP8266
  while (!Serial); // for Leonardo/Micro/Zero
  #endif

  Serial.begin(57600);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (! rtc.initialized()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  Serial.begin(9600);
}

void loop() 
{
  //local variables//
  int decision = 0;
  boolean origin = false;
  DateTime present = rtc.now();
  uint8_t min30 = present.minute();
  
  if (min30 % 30 == 0)
  {
    decision = quadsensor.readSun();
    
    if (decision == 1)
    {
      homeSun(); //should reset the globalStep to 0
      globalStepAz = quadsensor.sweepSun(); //writes new globalStep
      globalStepAz = quadsensor.trackSunAzimuth(); //adjusts for more accurate sun position and thus more accurate globalStep
      //globalStepAl = quadsensor.trackSunAltitude(globalStepAl);
    }
    if (decision == 2)
    {
      globalStepAz = quadsensor.trackSunAzimuth();
      //globalStepAl = quadsensor.trackSunAltitude(globalStepAl);
    } 
  }
}

void homeSun() 
{
#define STBY 8 // Pull this pin low to completely cut off power to the stepper
#define STEPS 400 // Number of steps per rotation, this is specific to our motor
#define HomeSwitch 2
  Stepper stepper(STEPS, 1, 2, 3, 4); // creates a stepper object called stepper, using pins 4-7
  stepper.setSpeed(100); // sets the speed of the motor in rpm
  pinMode(STBY, OUTPUT);
  digitalWrite(STBY, HIGH);
  pinMode(HomeSwitch, INPUT_PULLUP); //sets the HomeSwitch pin(2) as an input that uses internall pull ups
  while (Home == false) 
  { //This while loop continue to step the motor, while the swith is open(high)
    stepper.step(1); // steps the motor once
  }
  digitalWrite(STBY, LOW);
}

void pin_ISR() 
{ // this is the interrupt service routine. Basically the function that happens when interrupt occurs
  Home = true; // update the home variable to exit the while loop.
}
