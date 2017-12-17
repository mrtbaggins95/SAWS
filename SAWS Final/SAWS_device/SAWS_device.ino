// include files for each breakout through downloads on sketch
// include custom cpp and h files by saving them in the same folder as this program: Quadsensor+RTC & Bluetooth/BME/UV/Blynk combo
// custom include files are constituent of other libraries and user defined programs
///////////////////////////////////////////////////////INCLUDES///////////////////////////////////////////////////////
#include <BlynkSimpleSerialBLE.h>
#include <BLEPeripheral.h>
#include "BLESerial.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "Adafruit_VEML6070.h"
#include <Stepper.h>
#include "solarTracker.h"
#include <RTClib.h>



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

///////////////////////////////////////////////////////Bluetooth/////////////////////////////////////////////////
char auth[] = "8963a3ec5af844e5b97898f6a0e01916";


///////////////////////////////////////////////////////GLOBAL VARIABLES/////////////////////////////////////////////////////// // define global variables needed to pass to the breakout objects
//////////////////RTC//////////////////
RTC_PCF8523 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};


//////////////////QUADSENSOR//////////////////
int globalStepAz = 0;
int globalStepAl = 0;
solarTracker quadsensor = solarTracker(globalStepAz,globalStepAl);
volatile int Home = false;

//////////////////UV//////////////////
Adafruit_VEML6070 uv = Adafruit_VEML6070();

///////////////////////////////BLUETOOTH////////////////////////////////
#define BLE_REQ   23
#define BLE_RDY   18
#define BLE_RST   25

///////////////////////////////BME/////////////////////////////////////
#define BME_SCK 52
#define BME_MISO 50
#define BME_MOSI 51
#define BME_CS A12
Adafruit_BME280 bme(BME_CS);
#define SEALEVELPRESSURE_HPA (1013.25)
BLESerial SerialBLE(BLE_REQ, BLE_RDY, BLE_RST);
///////////////////////////////BLYNK///////////////////////////////////
BlynkTimer timer;



void setup() 
// define all breakouts and objects
{
  Serial.begin(9600);
  
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
  attachInterrupt(digitalPinToInterrupt(3), pin_ISR, LOW); // Defining an active low interrupt on the switch pin 3
  
  
  //////////////////UV INDEX SENSOR//////////////////

  /////////////////BLUETOOTH/////////////////////////
  SerialBLE.setLocalName("Blynk");
  SerialBLE.setDeviceName("Blynk");
  SerialBLE.setAppearance(0x0080);
  SerialBLE.begin();

  ////////////////BLYNK,UV,BME TRANSFER/////////////
  //Intialize Blynk,UV,and BME to start data transfer
  Blynk.begin(SerialBLE, auth);
  uv.begin(VEML6070_1_T);
  bme.begin();
  //Sets timer interval. sendSensor refers to a function where UV data and BME data are sent
  timer.setInterval(1000L, sendSensor);

  Serial.println("Waiting for connections...");
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
      
      globalStepAz = 0;
      homeSun();
      globalStepAz = quadsensor.sweepSun(); //writes new globalStep
      globalStepAz = quadsensor.trackSunAzimuth(); //adjusts for more accurate sun position and thus more accurate globalStep
      //globalStepAl = quadsensor.trackSunAltitude(globalStepAl);
    }
    if (decision == 2)
    {
      globalStepAz = quadsensor.trackSunAzimuth();
      //globalStepAl = quadsensor.trackSunAltitude(globalStepAl);
    }
    
  }

  ////////////////////////////////data reading and transmission////////////////////////////// // will start reading after the first move upon turn on and continue to keep reading until user off.
  
  SerialBLE.poll();
  Blynk.run();
  timer.run();
}

////////////////////////////////////////////USER DEFINED FUNCTIONS NOT ALREADY IN CPP/H//////////////////////////////////////

void homeSun() 
{
#define STBY 8 // Pull this pin low to completely cut off power to the stepper
#define STEPS 400 // Number of steps per rotation, this is specific to our motor
#define HomeSwitch 2
  Stepper stepper(STEPS, 1, 2, 3, 4); // creates a stepper object called stepper, using pins 4-7
  stepper.setSpeed(100); // sets the speed of the motor in rpm
  pinMode(STBY, OUTPUT);
  digitalWrite(STBY, HIGH);
  pinMode(HomeSwitch, INPUT_PULLUP); //sets the HomeSwitch pin(2) as an input that uses internall pull ups
  while (Home == false) 
  { //This while loop continue to step the motor, while the swith is open(high)
    stepper.step(1); // steps the motor once
  }
  digitalWrite(STBY, LOW);
}

void pin_ISR() 
{ // this is the interrupt service routine. Basically the function that happens when interrupt occurs
  Home = true; // update the home variable to exit the while loop.
}

void sendBMEData()
{
  //The the bme.readExample() reads a desired measurement
  float bmeTemperature = bme.readTemperature();
  float bmePressure = bme.readPressure()/100.0F;
  float bmeHumidity = bme.readHumidity();
  //This writes the data to a virtual pin. 
  //Virtual Pins in Blynk act similar to regular pins, 
  //by wrtiing data to virtual pins we are able to read data on the app.
  Blynk.virtualWrite(V1,bmeTemperature);
  Blynk.virtualWrite(V2,bmePressure);
  Blynk.virtualWrite(V3,bmeHumidity);
}
//The function will return a string based on the values the UV sensor is reading.
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
//Sends Sensor data
void sendSensor()
{
  sendBMEData();
  Blynk.virtualWrite(V4,UVindex_val());
}
