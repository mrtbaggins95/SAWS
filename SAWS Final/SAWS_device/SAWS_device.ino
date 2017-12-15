// include files for each breakout through downloads on sketch
// include custom cpp and h files by saving them in the same folder as this program: Quadsensor+RTC & Bluetooth/BME/UV/Blynk combo
// custom include files are constituent of other libraries and user defined programs
///////////////////////////////////////////////////////INCLUDES///////////////////////////////////////////////////////
#include <Blynk.h>
#include <BlynkSimpleSerialBLE.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BLE_UART.h>
#include <RTClib.h>
#include "Arduino.h"
#include "solarTracker.h"
#include <Stepper.h>
#include <Wire.h>
#include "weatherData.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "Adafruit_VEML6070.h"
#include <BLEPeripheral.h>
#include "BLESerial.h"
#include <SPI.h>


#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif
#include "Wire.h"

///////////////////////////////////////////////////////MISC. INITIALIZATION///////////////////////////////////////////////////////
//RTC
#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
   #define Serial SerialUSB
#endif

#define BLYNK_PRINT Serial
#define BLYNK_DEBUG 
#define BLYNK_USE_DIRECT_CONNECT

///////////////////////////////////////////////////////GLOBAL VARIABLES/////////////////////////////////////////////////////// // define global variables needed to pass to the breakout objects
//////////////////RTC//////////////////
RTC_PCF8523 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};


//////////////////QUADSENSOR//////////////////
int globalStepAz = 0;
int globalStepAl = 0;
solarTracker quadsensor = solarTracker(globalStepAz,globalStepAl);

void setup() 
// define all breakouts and objects
{
  //////////////////RTC//////////////////
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
  
  //////////////////QUADSENSOR//////////////////

  
  
  //////////////////UV INDEX SENSOR//////////////////
  Adafruit_VEML6070 uv = Adafruit_VEML6070();

}

void loop() 
// code blocks will be sectioned into parent code for break
{
  ///////////////////////local variables////////////////////////////////
  int decision = 0;
  boolean origin = false;
  ///////////////////////quadsensor&motion////////////////////////////// //will run every 30 minutes
  DateTime present = rtc.now();
  uint8_t min30 = present.minute();
  if (min30 % 30 == 0)
  {
    decision = quadsensor.readSun();
    
    if (decision == 1)
    {
      globalStepAz = quadsensor.homeSun(origin); //should reset the globalStep to 0
      globalStepAz = quadsensor.sweepSun(); //writes new globalStep
      globalStepAz = quadsensor.trackSunAzimuth(globalStepAz); //adjusts for more accurate sun position and thus more accurate globalStep
      //globalStepAl = quadsensor.trackSunAltitude(globalStepAl);
    }
    if (decision == 2)
    {
      globalStepAz = quadsensor.trackSunAzimuth(globalStepAz);
      //globalStepAl = quadsensor.trackSunAltitude(globalStepAl);
    }
    
  }

  ////////////////////////////////data reading and transmission////////////////////////////// // will start reading after the first move upon turn on and continue to keep reading until user off.
  

}
