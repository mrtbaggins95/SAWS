//This is the header file for sunFunctions library

#ifndef sunFunctions_h
#define sunFunctions_h
#include <Stepper.h>

class sunFunctions
{
  public:
  sunFunctions(int RTC, int baseStepAz, int baseStepAl);
  void homeIn();
  int readSun();
  int sweepSun();
  int trackSunAzimuth();
  int trackSunAltitude();
  void homeIn();  

  Stepper AzStepper = Stepper(200,1,2,3,4);
  Stepper AlStepper = Stepper(200,5,6,7,8);
  #define SensA A0
  #define SensB A1
  #define SensC A2
  #define SensD A3
  _RTC = RTC;
  _baseStepAz = baseStepAz;
  _baseStepAl = baseStepAl;
}

void sunFunctions::readSun()
{
  
}
#endif
