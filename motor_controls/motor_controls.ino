/*
 Stepper Motor Control - position control

 This program aims to control the position set by some value given
 in UART(Serial Terminal). 

 This uses a modified version of the AccelStepper library to control the
 power given to the motor by using analogWrite on each pin's output.

 Nathan Wong

 */

#include <../AccelStepper.h>


// stepsPerRevolution is calculated by 360/(degrees per step)
//  This should be listed on your motor
//const int stepsPerRevolution = 200;  

// initialize the stepper library on pins 2 through 5:
AccelStepper stepper1(AccelStepper::FULL4WIRE, 2, 3, 4, 5); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5

AccelStepper stepper2(AccelStepper::FULL4WIRE, 8, 9, 10, 11);
//AccelStepper stepper3(AccelStepper::FULL2WIRE, 10, 11);

//variables to hold certain values
//x coordinate value
String inString = "";
int inChar;
int x,y;

//flag for triggering current position printout
bool dmFlag = false;

//counter to poll motor
unsigned short int counter = 0;

//value to hold what is being read on a0 port
//this is used to determine the intensity of the
//uv led. (PWM)
unsigned short int uvledval;

//led pin set for 11 (PWM)
unsigned short int uvledpin = 12;

void setup() {

  Serial.begin(9600);
  //set initialization parameters for stepper1
  stepper1.setMaxSpeed(400.0);
  stepper1.setAcceleration(100.0);
//  stepper1.setMinPulseWidth(20);

  //set initialization parameters for stepper2
  stepper2.setMaxSpeed(400.0);
  stepper2.setAcceleration(100.0);
//  stepper2.setMinPulseWidth(20);

  //disable outputs so it doesn't draw and break stuff
  stepper1.moveTo(0);
  stepper2.moveTo(0);
//  stepper1.disableOutputs();
//  stepper2.disableOutputs();
  
  Serial.println("UV(0-255)|x-pos,y-pos");
}

void loop() {
  while (Serial.available() > 0) {
    inChar = Serial.read();
    if (inChar == '|') {
      uvledval = inString.toInt();
      inString = "";
      Serial.print("UV LED PWM at: ");
      Serial.println(uvledval);
    } else if (inChar == ',') {
      
      x = inString.toInt();
      inString = "";
      
      //printouts to see input x and y position pt.1
//      Serial.print("<x,y> = <");
//      Serial.print(x);
      
    } else if (inChar == '\n') {
      
      y = inString.toInt();
      inString = "";
      
      //printouts to see input x and y position pt.2
//      Serial.print(",");
//      Serial.print(y);
//      Serial.println(">");
      
      stepper1.enableOutputs();
      stepper1.moveTo(x);
      
      stepper2.enableOutputs();
      stepper2.moveTo(y);

      dmFlag = true;
      
      //printouts to see current x and y position
//      Serial.print("Stepper 1 (x) is currently at: ");
//      Serial.println(stepper1.currentPosition());
//
//      Serial.print("Stepper 2 (y) is currently at: ");
//      Serial.println(stepper2.currentPosition());
      
    } else inString += (char)inChar;
    
   
  }

  if (dmFlag && stepper1.distanceToGo()==0 && stepper2.distanceToGo() == 0) {
    Serial.print("x Position:");
    Serial.println(stepper1.currentPosition());
    Serial.print("y Position:");
    Serial.println(stepper2.currentPosition());
    dmFlag = false;
  }

  if (stepper1.distanceToGo() == 0) {
    stepper1.disableOutputs();
  }

  if (stepper2.distanceToGo() == 0) {
    stepper2.disableOutputs();
  }
  
  stepper1.run();
  stepper2.run();

  analogWrite(uvledpin,uvledval);

}
