#include <Wire.h>
#include "Adafruit_VEML6070.h"

Adafruit_VEML6070 uv = Adafruit_VEML6070();

void setup() {
  Serial.begin(9600);
  Serial.println("VEML6070 Test");
  uv.begin(VEML6070_1_T);  // pass in the integration time constant
    //VEML6070_HALF_T ~62.5ms
    //VEML6070_1_T ~125ms
    //VEML6070_2_T ~250ms
    //VEML6070_4_T ~500ms
}


void loop() {
  Serial.print("UV light level: "); Serial.println(uv.readUV());
  
  delay(1000);
}
