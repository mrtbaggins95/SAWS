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
#define BLYNK_USE_DIRECT_CONNECT
#include <Adafruit_BME280.h>
#include <BlynkSimpleSerialBLE.h>
#include <BLEPeripheral.h>
#include "BLESerial.h"
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <SPI.h>

#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME280 bme;

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

// define pins (varies per shield/board)
#define BLE_REQ   10
#define BLE_RDY   2
#define BLE_RST   9

 // I2C
// create ble serial instance, see pinouts above
BLESerial SerialBLE(BLE_REQ, BLE_RDY, BLE_RST);



BLYNK_WRITE(V1)
{
  int pinValue1 = param.asInt();
  Serial.print("Temperature = ");
  Serial.print(bme.readTemperature());
  Serial.println(" *C");
  
}
BLYNK_WRITE(V2)
{
  int pinValue2 = param.asInt();
   Serial.print("Pressure = ");
   Serial.print(bme.readPressure() / 100.0F);
   Serial.println(" hPa");
}
BLYNK_WRITE(V3)
{
  int pinValue3 = param.asInt();
  Serial.print("Humidity = ");
  Serial.print(bme.readHumidity());
  Serial.println(" %");

  
}


void setup()
{
  // Debug console
  Serial.begin(9600);

  SerialBLE.setLocalName("Blynk");
  SerialBLE.setDeviceName("Blynk");
  SerialBLE.setAppearance(0x0080);
  SerialBLE.begin();

  Blynk.begin(SerialBLE, auth);

  Serial.println("Waiting for connections...");
}

void loop()
{
  SerialBLE.poll();

  if (SerialBLE) {    // If BLE is connected...

  






    
    Blynk.run();
  }
}

