/* James Donahue
    10/22/17
    SAWS Fall 2017 Senior Design Project
    ***********************************************************
    ALL THIS DOES IS TURN A STEPPER MOTOR BACK TO A HOME POSITION, AND STOP. IT DOES NOT UPDATE A VARIABLE SAYING WE ARE ARE 0 DEGREES!!

    This sketch demonstrates the use of a stepper home function. The program needs to keep track of the current position of the gantry.
    Since we are using bipolar stepper motors, we must calibrate the stepper position, with the virtual program position. 3D printers
    use home switches that close when the stepper motor hits a known position. This program uses interrupts to detect when the a normally open
    homing switch closes.
*/

#include <Stepper.h>
volatile int Home = false; // variable that is modified by the Interrupt, on pin 2. Needs to be global

void setup() {
  attachInterrupt(digitalPinToInterrupt(3), pin_ISR, LOW); // Defining an active low interrupt on the switch pin 2
  HomeStepper();
}

void loop() {}

void HomeStepper() 
{
#define STBY 8 // Pull this pin low to completely cut off power to the stepper
#define STEPS 400 // Number of steps per rotation, this is specific to our motor
#define HomeSwitch 3
  Stepper stepper(STEPS, 34, 38, 46, 48); // creates a stepper object called stepper, using pins 4-7
  stepper.setSpeed(135); // sets the speed of the motor in rpm
  pinMode(STBY, OUTPUT);
  digitalWrite(STBY, HIGH);
  pinMode(HomeSwitch, INPUT_PULLUP); //sets the HomeSwitch pin(2) as an input that uses internall pull ups
  while (Home == false) 
  { //This while loop continue to step the motor, while the swith is open(high)
    stepper.step(1); // steps the motor once
  }
  digitalWrite(STBY, LOW);
}

void pin_ISR() { // this is the interrupt service routine. Basically the function that happens when interrupt occurs
  Home = true; // update the home variable to exit the while loop.
}
