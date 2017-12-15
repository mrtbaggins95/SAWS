//Source file for BlynkSensorOutput

#include "Arduino.h"
#include "weatherData.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "Adafruit_VEML6070.h"

weatherData::weatherData()
{
  //define all breakout objects here ex: UV,BME,etc
  //define all global variables such as the sensor pins that were used all throughout track Sun methods
  BlynkTimer timer;
}

void weatherData::sendBMEData()
{
  float bmeTemperature = bme.readTemperature();
  float bmePressure = bme.readPressure()/100.0F;
  float bmeHumidity = bme.readHumidity();
  Blynk.virtualWrite(V1,bmeTemperature);
  Blynk.virtualWrite(V2,bmePressure);
  Blynk.virtualWrite(V3,bmeHumidity);
}

String weatherData::UVindex_val()
{
  uint16_t reading = uv.readUV();
  if (reading >= 2055)
  {
    return "Extreme";
  }
  else if ( reading >= 1494)
  {
    return "Very High";
  }
  else if ( reading >= 1121)
  {
    return "High";
  }
  else if (reading >= 561)
  {
    return "Moderate";
  }
  else
  {
    return "Low";
  }
}

void weatherData::sendSensor()
{
  sendBMEData();
  Blynk.virtualWrite(V4,UVindex_val());
 
}
void weatherData::start()
{
  SerialBLE.setLocalName("Blynk");
  SerialBLE.setDeviceName("Blynk");
  SerialBLE.setAppearance(0x0080);
  SerialBLE.begin();
  
  Blynk.begin(SerialBLE, auth);
  uv.begin(VEML6070_1_T);
  bme.begin();
  timer.setInterval(1000L, sendSensor);
  Serial.println("Waiting for connections...");
}
