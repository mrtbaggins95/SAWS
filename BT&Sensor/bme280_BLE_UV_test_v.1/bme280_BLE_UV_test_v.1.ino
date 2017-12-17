#include <Wire.h>
#include <SPI.h>
#include "Adafruit_BLE_UART.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "Adafruit_VEML6070.h"

// Connect CLK/MISO/MOSI to hardware SPI
// e.g. On UNO & compatible: CLK = 13, MISO = 12, MOSI = 11
#define ADAFRUITBLE_REQ 23
#define ADAFRUITBLE_RDY 18    // This should be an interrupt pin, on Uno thats #2 or #3
#define ADAFRUITBLE_RST 25

Adafruit_BLE_UART BTLEserial = Adafruit_BLE_UART(ADAFRUITBLE_REQ, ADAFRUITBLE_RDY, ADAFRUITBLE_RST);
Adafruit_VEML6070 uv = Adafruit_VEML6070();

#define BME_SCK 52
#define BME_MISO 51
#define BME_MOSI 50
#define BME_CS A12
Adafruit_BME280 bme(BME_CS); // hardware SPI
#define SEALEVELPRESSURE_HPA (1013.25)

//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

unsigned long delayTime;
/**************************************************************************/
/*!
    Configure the Arduino and start advertising with the radio
*/
/**************************************************************************/
void setup()
{ 
  Serial.begin(9600);
  while(!Serial); // Leonardo/Micro should wait for serial init
  Serial.println(F("Adafruit nRF8001 BME280 VEML6070 UV Index test"));
  uv.begin(VEML6070_1_T);  // pass in the integration time constant

//bool status;
//    
//    // default settings
//    status = bme.begin();
//    if (!status) {
//        Serial.println("Could not find a valid BME280 sensor, check wiring!");
//        while (1);
//    }
//    
//    Serial.println("-- Default Test --");
//    delayTime = 100;
//
//    Serial.println();
//  // BTLEserial.setDeviceName("NEWNAME"); /* 7 characters max! */
//
//  BTLEserial.begin();
}

/**************************************************************************/
/*!
    Constantly checks for new events on the nRF8001
*/
/**************************************************************************/
//aci_evt_opcode_t laststatus = ACI_EVT_DISCONNECTED;

void loop()
{


    
  // Tell the nRF8001 to do whatever it should be working on.
  BTLEserial.pollACI();

  // Ask what is our current status
  aci_evt_opcode_t status = BTLEserial.getState();
  // If the status changed....
  if (status != laststatus) {
    // print it out!
    if (status == ACI_EVT_DEVICE_STARTED) {
        Serial.println(F("* Advertising started"));
    }
    if (status == ACI_EVT_CONNECTED) {
        Serial.println(F("* Connected!"));
    }
    if (status == ACI_EVT_DISCONNECTED) {
        Serial.println(F("* Disconnected or advertising timed out"));
    }
    // OK set the last status change to this one
    laststatus = status;
  }
  printValues();
  BTLEserial.print("UV index: ");
  printUVindex();
  delay(delayTime);
  
  if (status == ACI_EVT_CONNECTED) {
    // Lets see if there's any data for us! data from the device
    if (BTLEserial.available()) {
      Serial.print("* "); Serial.print(BTLEserial.available()); Serial.println(F(" bytes available from BTLE"));
    }
    // OK while we still have something to read, get a character and print it out
    while (BTLEserial.available()) {
      char c = BTLEserial.read();
      Serial.print(c);
    }


    // Next up, see if we have any data to get from the Serial console

    if (Serial.available()) {
      // Read a line from Serial
      Serial.setTimeout(1000); // 100 millisecond timeout
      String s = Serial.readString();

      // We need to convert the line to bytes, no more than 20 at this time
      uint8_t sendbuffer[20];
      s.getBytes(sendbuffer, 20);
      char sendbuffersize = min(20, s.length());

      Serial.print(F("\n*  \"")); ///this is where the data is sent to the bluetooth connected device
      Serial.print((char *)sendbuffer);///this is where the data is sent to the bluetooth connected device
      Serial.println("\"");  ///this is where the data is sent to the bluetooth connected device

      // write the data
      BTLEserial.write(sendbuffer, sendbuffersize);

//      delay(delayTime);
    }
  }
}


void printValues() {
    Serial.print("Temperature = ");
    Serial.print(bme.readTemperature());
    Serial.println(" *C");

    Serial.print("Pressure = ");
    Serial.print(bme.readPressure() / 100.0F);
    Serial.println(" hPa");

    Serial.print("Approx. Altitude = ");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.println(" m above sea level");

    Serial.print("Humidity = ");
    Serial.print(bme.readHumidity());
    Serial.println(" %");

    Serial.print("UV Intensity = ");
    Serial.print(printUVindex());


    Serial.println();
    
    }

String printUVindex() {
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


