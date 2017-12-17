#include <Wire.h>
#include "Adafruit_VEML6070.h"
Adafruit_VEML6070 uv = Adafruit_VEML6070();

void setup() {
  Serial.begin(9600);
  Serial.println("VEML6070 UV Index");
  uv.begin(VEML6070_1_T);  // pass in the integration time constant
    
}


void loop() {
  Serial.print("UV index: "); Serial.println(UVindex_val()); 
  delay(1000);
}

String UVindex_val() {
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

