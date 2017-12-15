//Blynk Sensor Header file
#ifndef weatherData_h
#define weatherData_h

#include "Arduino.h"
#include <BlynkSimpleSerialBLE.h>
#include <BLEPeripheral.h>
#include "BLESerial.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
//#include "Adafruit_VEML6070.h"

class weatherData
{
  public:
  void start();
  void sendSensor();
  void sendBMEData();
  String UVindex_val();
};

#endif

