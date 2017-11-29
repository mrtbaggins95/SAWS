//This is the header file for sunFunctions library

#ifndef sunFunctions_h
#define sunFunctions_h
#include <Stepper.h>
#include "Arduino.h"

class sunFunctions
{
  public:
  sunFunctions(int RTC, int baseStepAz, int baseStepAl);
  int readSun();
  int sweepSun();
  int trackSunAzimuth(int baseStepAz);
  int trackSunAltitude(int baseStepAl);
  void homeSun();  
  boolean pin_ISR(boolean Home);
  
  private:
  int _RTC;
  int _baseStepAz;
  int _baseStepAl;
};

#endif
