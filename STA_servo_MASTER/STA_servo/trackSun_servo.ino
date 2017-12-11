

#include <Servo.h>
Servo Azimuth;
Servo Altitude;

#define photoA A0
#define photoB A1
#define photoC A2
#define photoD A3

unsigned long lastT = 0;
float AzAngle = 90;
float AlAngle = 0;


float Azkp = 1; // proportional gain
float Azki = 1; // integral gain
float Azkd = 1; // derivative gain



void setup() 
{  
  Serial.begin(9600); // initialize serial communication at 9600 bits per second:
  Azimuth.attach(9);
  Altitude.attach(10);

void trackSun()
{
  //local variables
  float AzIerror = 0; // integral error
  float AlIerror = 0; // integral error
  float LastAzABPerror = 0; // last saved proportional error AB
  float LastAzCDPerror = 0; // last saved proportional error CD
  float LastAlACPerror = 0; // last saved proportional error AC
  float LastAlBDPerror = 0; // last saved proportional error BD
  
  //timing block
  unsigned long now = millis(); // current time
  float Tchange = double(now - lastT);  // time since last time through loop  

  //AB block --> Adjust Servo 1, Azimuth
  float AzABPerror = 0 - (ValA - ValB); // calculates proportional error 
  AzABIerror = (AzABPerror) + AzABIerror; // calculates integral error
  float AzABDerror = (AzABPerror - LastAzABPerror)/Tchange; // calculates derivative error
  float PIDOUT = Azkp*AzABPerror + Azki*AzABIerror + Azkd*AzABDerror; // calculates total PID error
  LastAzABPerror = AzABPerror; // save error value for derivative error check

  //CD block -- redundant
  float AzCDPerror = 0 - (ValC - ValD); // calculates proportional error 
  AzCDIerror = (AzCDPerror) + AzCDIerror; // calculates integral error
  float AzCDDerror = (AzCDPerror - LastAzCDPerror)/Tchange; // calculates derivative error
  float PIDOUT = Azkp*AzCDPerror + Azki*AzCDIerror + Azkd*AzCDDerror; // calculates total PID error
  LastAzCDPerror = AzCDPerror; // save error value for derivative error check
  
 
  
  //Serial.print(AzPerror);
  //Serial.print(|   |);  
  //Serial.print(Azkp*AzPerror);
  //Serial.print(|   |);  
  //Serial.print(Azki*AzIerror);
  //Serial.print(|   |);  
  //Serial.print(Azkd*AzDerror);
  //Serial.print(|   |);
  //Serial.println(PIDOUT);


  AzAngle = AzAngle + 0.1*PIDOUT; // turns left for -PIDOUT. turns right for -PIDOUT
  // This algorithm is continuous but it does not account for both pairs of sensors and cannot adjust to extreme angles becuase PIDOUT maxes out since there is not a complete presence or absence of light.
  Azimuth.write(AzAngle);

  // check if X=X and Y=Y
  // if yes, lock in position and proceed to track altitude. if not, repeat azimuth tracking.

  //AC block --> Adjust Servo 2, Altitude
  float AlACPerror = 0 - (ValA - ValC); // calculates proportional error 
  AlACIerror = (AlACPerror) + AlACIerror; // calculates integral error
  float AlACDerror = (AlACPerror - LastAlACPerror)/Tchange; // calculates derivative error
  float PIDOUT = Azkp*AlACPerror + Azki*AlACIerror + Azkd*AlACDerror; // calculates total PID error
  LastAlACPerror = AlACPerror; // save error value for derivative error check

  //BD block -- redundant
  float AlBDPerror = 0 - (ValB - ValD); // calculates proportional error 
  AlBDIerror = (AlBDPerror) + AlBDIerror; // calculates integral error
  float AlBDDerror = (AlBDPerror - LastAlBDPerror)/Tchange; // calculates derivative error
  float PIDOUT = Azkp*AlBDPerror + Azki*AlBDIerror + Azkd*AlBDDerror; // calculates total PID error
  LastAlBDPerror = AlBDPerror; // save error value for derivative error check

  lastT = now; // make new time stamp

  //Serial.print(AzPerror);
  //Serial.print(|   |);  
  //Serial.print(Azkp*AzPerror);
  //Serial.print(|   |);  
  //Serial.print(Azki*AzIerror);
  //Serial.print(|   |);  
  //Serial.print(Azkd*AzDerror);
  //Serial.print(|   |);
  //Serial.println(PIDOUT);

  AlAngle = AlAngle + 0.1*PIDOUT; // turns left for -PIDOUT. turns right for -PIDOUT
  // This algorithm is continuous but it does not account for both pairs of sensors and cannot adjust to extreme angles becuase PIDOUT maxes out since there is not a complete presence or absence of light.
  Altitude.write(AlAngle);

  // check if Z=Z=Z=Z.
  // if yes, lock in altitude and enter stand-by mode
  
  delay(10); // delay in between reads for stability
}

}

