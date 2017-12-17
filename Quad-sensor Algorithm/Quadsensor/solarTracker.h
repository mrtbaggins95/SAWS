//This is the header file for solarTracker library

#ifndef solarTracker_h
#define solarTracker_h
#include <Stepper.h>
#include "Arduino.h"

class solarTracker
{
  public:
  solarTracker(int baseStepAz, int baseStepAl);
  int readSun();
  int sweepSun();
  int trackSunAzimuth();
  int trackSunAltitude();
  
  private:
  int _baseStepAz;
  int _baseStepAl;
};

#endif
