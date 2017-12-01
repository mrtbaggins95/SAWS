
#define BLYNK_PRINT Serial
#define BLYNK_USE_DIRECT_CONNECT
#include <BlynkSimpleSerialBLE.h>
#include <BLEPeripheral.h>
#include "BLESerial.h"
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = " db551ad1ea9b465592128b64f96b294a";

// define pins (varies per shield/board)
//BLUETOOTH SHIT
#define BLE_REQ   10
#define BLE_RDY   2
#define BLE_RST   9

//BME SHIT
#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 8
#define SEALEVELPRESSURE_HPA (1013.25)


// create ble serial instance, see pinouts above
BLESerial SerialBLE(BLE_REQ, BLE_RDY, BLE_RST);
Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

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

