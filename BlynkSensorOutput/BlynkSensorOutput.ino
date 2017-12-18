

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

//Initialize Blynk Bluetooth interface. You do not BLYNK_DEBUG, but it is there to see if data is transmitting,
#define BLYNK_PRINT Serial
#define BLYNK_DEBUG
#define BLYNK_USE_DIRECT_CONNECT

//These include files are necessary. The UV include file and the BLESerial include are local, use the Add file method to include the library.
#include <BlynkSimpleSerialBLE.h>
#include <BLEPeripheral.h>
#include "BLESerial.h"
#include <SPI.h>
#include <Wire.h>
#include <SD.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "Adafruit_VEML6070.h"

// define pin and global for the SPI SD Card Connection
#define SD_CS 53
String filename = "";


//Auth token to connect to the mobile app
char auth[] = "8963a3ec5af844e5b97898f6a0e01916";//ipad blynk


//define pins for BLE for Blynk
#define BLE_REQ   23
#define BLE_RDY   18
#define BLE_RST   25


// define Pressure constant
#define SEALEVELPRESSURE_HPA (1013.25)

// create ble serial instance, see pinouts above, comes from the BLESerial library
BLESerial SerialBLE(BLE_REQ, BLE_RDY, BLE_RST);

//Define BME pins using Hardware SPI
#define BME_SCK 52
#define BME_MISO 50
#define BME_MOSI 51
#define BME_CS A12
Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

//Intitialize UV
Adafruit_VEML6070 uv = Adafruit_VEML6070();

//Initialize Blynk timer to poll data at a defined frequency based on the app
BlynkTimer timer;
void setup()
{
  // Debug console
  Serial.begin(9600);
  ////////////////////////////////////////////////////////////////////////////////////
  //SD CARD and FILE INITIALIZATION
  while (!Serial) { // do nothing while there is no serial activity.
  }
  Serial.println("Initializing the SD card");
  if (!SD.begin(SD_CS)) {
    Serial.println(" Failed to connect to card, or card not present");
    //while(1){}
  }
  else if (SD.begin(SD_CS)) {
    Serial.println("Card initialized");
  }
  int n = 1;
  while (1) {
    filename = "Data_";
    filename += String(n);
    filename += ".txt";
    if (!SD.exists(filename)) { // File does not exist, create a new file name. Evenutally this file will be created
      break;
    }
    n = n + 1;
  }

  /////////////////////////////////////////////////////////////////////////////////////
//Setup the BLE to advertise as "Blynk"
SerialBLE.setLocalName("Blynk");
SerialBLE.setDeviceName("Blynk");
SerialBLE.setAppearance(0x0080);
SerialBLE.begin();

//Intialize Blynk,UV,and BME to start data transfer
Blynk.begin(SerialBLE, auth);
uv.begin(VEML6070_1_T);
bme.begin();
//Sets timer interval. sendSensor refers to a function where UV data and BME data are sent
timer.setInterval(1000L, sendSensor);

Serial.println("Waiting for connections...");
}




//Will send Temperature, Pressure, and Humidity Data
void sendBMEData()
{
  //The the bme.readExample() reads a desired measurement
  float bmeTemperature = bme.readTemperature();
  float bmePressure = bme.readPressure() / 100.0F;
  float bmeHumidity = bme.readHumidity();
  uint16_t vemlVal =  UVindex_val();

  //This writes the data to a virtual pin.
  //Virtual Pins in Blynk act similar to regular pins,
  //by wrtiing data to virtual pins we are able to read data on the app.
  Blynk.virtualWrite(V1,vemlVal);
  Blynk.virtualWrite(V2, bmeHumidity);
  //Blynk.virtualWrite(V3,bmePressure);
  //Blynk.virtualWrite(V4,bmeTemperature);

      ///////////////////////////////////////////////////////////////////////////////////
  //WRITE DATA TO SD CARD
  String DataString = String(bmeTemperature) + ", " + String(bmePressure) + ", " + String(bmeHumidity) + ", " +String(vemlVal) + ", ";  
  File newFile = SD.open(filename, FILE_WRITE);
  if (newFile) {
      newFile.println(DataString);
      newFile.close();
      //Serial.println(DataString);
    }
    else{
      Serial.println("Error Writing Data string to SD");
    }

  //////////////////////////////////////////////////////////////////////////////////
}
//The function will return a string based on the values the UV sensor is reading.
uint16_t UVindex_val()
{
  uint16_t reading = uv.readUV();
  //String output;
  /*
    if (reading >= 2055)
    {
    output =  "Extreme";
    return output;
    }
    else if ( reading >= 1494)
    {
    output = "Very High";
    return output;
    }
    else if ( reading >= 1121)
    {
    output = "High";
    return output;
    }
    else if (reading >= 561)
    {
    output = "Moderate";
    return output;
    }
    else
    {
    output = "Low";
    return output;
    }
  */
  return reading;
}
//Sends Sensor data
void sendSensor()
{
  sendBMEData();
  Blynk.virtualWrite(V4, UVindex_val());

}


void loop()
{
  //Need these three lines to run Blynk stuff to transmit data.
  SerialBLE.poll();
  Blynk.run();
  timer.run();



}

