#include <Wire.h>
#include "Adafruit_VEML6070.h"
//VEML6070_HALF_T// ~62.5ms
//VEML6070_1_T// ~125ms
//VEML6070_2_T// ~250ms
//VEML6070_4_T //~500ms
Adafruit_VEML6070 uv = Adafruit_VEML6070();

void setup() {
  Serial.begin(9600);
  Serial.println("VEML6070 Test");
  uv.begin(VEML6070_1_T);  // pass in the integration time constant
    
}


void loop() {
  Serial.print("UV light level: "); Serial.println(uv.readUV());
  delay(1000);
}

String UVindex_val() {
  reading = uv.readUV;
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

