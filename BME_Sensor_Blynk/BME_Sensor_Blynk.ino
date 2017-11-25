

#define BLYNK_PRINT Serial
#include <Adafruit_BME280.h>
#include <Blynk.h>




void setup() {
  Serial.begin(9600);
  Serial.println(F("BME280 test"));


  if (!bme.begin()) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
}

void loop() {
  BLYNK_READ(V1)
    {
    Blynk.virtualWrite(V1, bme.readTemperature());
    }
  BLYNK_READ(V2)
    {
    Blynk.virtualWrite(V2,bme.readPressure()/100.0F);
    }
    BLYNK_READ(V3)
    {
    Blynk.virtualWrite(V3,bme.readHumidty());
    }
    delay(2000);
}
