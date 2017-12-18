

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
#include <Stepper.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "Adafruit_VEML6070.h"
// define pin and global for the SPI SD Card Connection
#define SD_CS 53
String filename = "";
//  Stuff for the Real time clock
#include "RTClib.h"
RTC_DS1307 rtc;
//Auth token to connect to the mobile app
//char auth[] = "8963a3ec5af844e5b97898f6a0e01916";// Jordan's Ipad
//char auth[] = "050355e469a949ff8c29f7aeff0d4e2e"; // Sakib's Phone
char auth[] = "b509465b69104157938622e74725d7da"; // James' Phone
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
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SP
//Intitialize UV
Adafruit_VEML6070 uv = Adafruit_VEML6070();
//Initialize Blynk timer to poll data at a defined frequency based on the app
BlynkTimer timer;

struct Weather_t {
  float Temp;
  float Pres;
  float Hum;
  uint16_t Sun;
};
Weather_t Weather = {0, 0, 0, 0};

struct Time_t {
  uint16_t Year;
  uint8_t Month;
  uint8_t Day;
  uint8_t Hour;
  uint8_t Minute;
  uint8_t Second;
};
Time_t Time = {0, 0, 0, 0, 0, 0};

void setup()
{
  ////////////////////////////////////////////////////////////////////////////////////
  //Serial Console Setup

  Serial.begin(9600);
  ////////////////////////////////////////////////////////////////////////////////////
  //RTC INITIALIZATION
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    //while (1);
  }
  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  ////////////////////////////////////////////////////////////////////////////////////
  //SD CARD and FILE INITIALIZATION
  Serial.println("Initializing the SD card");
  if (!SD.begin(SD_CS)) {
    Serial.println(" Failed to connect to card, or card not present");
    //while(1){}
  }
  else {
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
  timer.setInterval(1000L, BlynkSend); // sendData is the only function that accesses the weather and time data. This function is called by a blynk timer every minute
  Serial.println("Waiting for connections...");
}
void loop()
{
  DateTime now = rtc.now();
  uint8_t Second = now.second();
  uint8_t Minute = now.minute();

  if (Second % 59 == 0 ) { // Every minute read sensors and save the data
    bool stateA = LOW;
    if (stateA != HIGH) {
      stateA == HIGH;
      GetData();
      SaveData();

    }
  }
  if ( Minute % 2 == 0) { // Every 30 minutes call the motor control function
    bool stateB = LOW;
    if (stateB != HIGH) {
      stateB == HIGH;
      trackSun();
    }
  }
  //Need these three lines to run Blynk stuff to transmit data.
  SerialBLE.poll(); // dont know what this does
  Blynk.run(); // dont know what this does
  timer.run(); // dont know what this does






}



void GetData() {
  Weather.Temp = bme.readTemperature();
  Weather.Pres = bme.readPressure() / 100.0F;
  Weather.Hum = bme.readHumidity();
  Weather.Sun =  uv.readUV();

  DateTime now = rtc.now();
  Time.Year = now.year();
  Time.Month = now.month();
  Time.Day = now.day();
  Time.Hour = now.hour();
  Time.Minute = now.minute();
  Time.Second = now.second();
}

void SaveData() {
  String DataString = String(Time.Year) + ", " + String(Time.Month) + ", " + String(Time.Day) + ", " + String(Time.Hour) + ", " + String(Time.Minute) + ", " + String(Time.Second) + ", " + String(Weather.Temp) + ", " + String(Weather.Pres) + ", " + String(Weather.Hum) + ", " + String(Weather.Sun) + ", ";
  File newFile = SD.open(filename, FILE_WRITE);
  if (newFile) {
    newFile.println(DataString);
    newFile.close();
    //Serial.println(DataString);
  }
  else {
    Serial.println("Error Writing Data string to SD");
  }
}
void BlynkSend()
{
  GetData(); // This function reads all of the sensors and modifies the global Weather structure
  //This writes the data to a virtual pin.
  //Virtual Pins in Blynk act similar to regular pins,
  //by wrtiing data to virtual pins we are able to read data on the app.
  Blynk.virtualWrite(V1, Weather.Sun);
  Blynk.virtualWrite(V2, Weather.Hum);
  Blynk.virtualWrite(V3, Weather.Pres);
  Blynk.virtualWrite(V4, Weather.Temp);
}
void trackSun()
{
#include <Stepper.h>
  int STEPS = 400;
  static Stepper AzStepper(STEPS, 34, 38, 46, 48);
#define SensA A8
#define SensB A9
#define SensC A10
#define SensD A11
#define Stby 36
  pinMode(Stby, OUTPUT);
  AzStepper.setSpeed(50);
  float baseStep = 0;  //later change this to the step amount obtained from sweepSun();
  unsigned long lastT = 0;
  float AzIerror = 0; // integral error
  float LastAzPerror = 0; // last saved proportional error
  float kp = 0.05; // proportional gain
  float ki = 0; // integral gain
  float kd = 0; // derivative gain
  float LeftAvg = 0.5 * (analogRead(SensB) + analogRead(SensC));
  float  RightAvg = 0.5 * (analogRead(SensA) + analogRead(SensD));
  float AzPerror =  (RightAvg - LeftAvg);
  //Azimuth Correction
  digitalWrite(Stby, HIGH);

  while (abs(AzPerror) > 24 )
  {
    unsigned long CurrentT = millis(); // current time
    float Tchange = double(CurrentT - lastT);  // time since last time through loop
    //PID formula, determine a number of steps to adjust
    //AzPerror = 0 - (LeftAvg - RightAvg)
    Serial.print(AzPerror);
    Serial.print(" Error   Steps ");
    AzIerror = (AzPerror) + AzIerror;
    float AzDerror = (AzPerror - LastAzPerror) / Tchange;
    LastAzPerror = AzPerror;
    lastT = CurrentT;
    int16_t AzDeltaSteps =  (kp * AzPerror + ki * AzIerror + kd * AzDerror);
    //extremity check
    //int AzStepCheck = baseStep + AzDeltaSteps;
    /*
      if (AzStepCheck > STEPS)
      {
      AzDeltaSteps = (AzStepCheck - STEPS) - baseStep;

      if (AzStepCheck < 0)
      {
      AzDeltaSteps = STEPS + AzStepCheck;
      }
    */
    //normal function
    if ((AzDeltaSteps < 20) & (AzDeltaSteps > 0)) {
      AzDeltaSteps = 20;
    }
    if ((AzDeltaSteps < 0) & (AzDeltaSteps > -20)) {
      AzDeltaSteps = -20;
    }
    AzStepper.step(-1 * AzDeltaSteps);
    baseStep = baseStep + AzDeltaSteps;
    Serial.println(AzDeltaSteps);
    LeftAvg = 0.5 * (analogRead(SensB) + analogRead(SensC));
    RightAvg = 0.5 * (analogRead(SensA) + analogRead(SensD));
    AzPerror = 0 - (LeftAvg - RightAvg);
  }
  digitalWrite(Stby, LOW);
}


