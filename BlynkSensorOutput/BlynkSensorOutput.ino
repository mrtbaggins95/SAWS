

/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Social networks:            http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  For this example you need BLEPeripheral library
    from http://librarymanager/all#BLEPeripheral
    or https://github.com/sandeepmistry/arduino-BLEPeripheral

  Warning: Bluetooth support is in beta!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial
#define BLYNK_DEBUG 
#define BLYNK_USE_DIRECT_CONNECT

#include <BlynkSimpleSerialBLE.h>
#include <BLEPeripheral.h>
#include "BLESerial.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "Adafruit_VEML6070.h"





// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "8963a3ec5af844e5b97898f6a0e01916";
//char auth[] = "2b4d2aca7b4e4533b51e172d44aa30dc";
//char auth[] = "cc56e7bc08e046d4bc44e3c1906aa103";

// define pins (varies per shield/board)
#define BLE_REQ   23
#define BLE_RDY   18
#define BLE_RST   25



#define SEALEVELPRESSURE_HPA (1013.25)

// create ble serial instance, see pinouts above
BLESerial SerialBLE(BLE_REQ, BLE_RDY, BLE_RST);


#define BME_SCK 52
#define BME_MISO 50
#define BME_MOSI 51
#define BME_CS A12
Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

Adafruit_VEML6070 uv = Adafruit_VEML6070();

BlynkTimer timer;
void setup()
{
  // Debug console
  Serial.begin(9600);

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

void sendBMEData()
{
  float bmeTemperature = bme.readTemperature();
  float bmePressure = bme.readPressure()/100.0F;
  float bmeHumidity = bme.readHumidity();
  //Blynk.virtualWrite(V1,bmeTemperature);
  //Blynk.virtualWrite(V2,bmePressure);
  Blynk.virtualWrite(V1,bmeHumidity);
}
String UVindex_val()
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
void sendSensor()
{
  sendBMEData();
  Blynk.virtualWrite(V7,UVindex_val());
 
}

void loop()
{
  SerialBLE.poll();
  Blynk.run();
  timer.run();
  
  
}

