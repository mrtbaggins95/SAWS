// include files for each breakout through downloads on sketch
// include custom cpp and h files by saving them in the same folder as this program: Quadsensor+RTC & Bluetooth/BME/UV/Blynk combo
// custom include files are constituent of other libraries and user defined programs
#include <Blynk.h>
#include <Adafruit_BLE_UART.h>
#include <Adafruit_BME280.h>
#include <RTClib.h>
#include "Arduino.h"
#include "solarTracker.h"
#include <Stepper.h>
#include <Wire.h>
#include "Adafruit_VEML6070.h"
#include "WProgram.h"
#include "BLESerial.h"

// define global variables needed to pass to the breakout objects

void setup() 
// define all breakouts and objects
{
  //solarTracker quadsensor = solarTracker(
  Adafruit_VEML6070 uv = Adafruit_VEML6070();

}

void loop() 
// code blocks will be sectioned into parent code for break
{
  // quadsensor

  // data reading and transmission

}
