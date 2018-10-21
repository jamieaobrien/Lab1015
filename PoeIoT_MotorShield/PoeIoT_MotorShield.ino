// This is the IoT tutorial sort of modified to 
// our system and to motors. 

#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

// *****MOTOR CREATE******
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); //creating the motor shield object

Adafruit_DCMotor *myMotorL = AFMS.getMotor(3); //creating the object for Motor 1
Adafruit_DCMotor *myMotorR = AFMS.getMotor(4); //creating the object for Motor 2

String voice; //The input from the serial is the speech, it's stored as a string


// ** Functions to Turn the Motors on and off ***
void LeftOn(){
   myMotorR->setSpeed(0); 
   myMotorR->run(BACKWARD);
   myMotorL->setSpeed(100); 
   myMotorL->run(BACKWARD);
}

void RightOn(){
   myMotorL->setSpeed(0);
   myMotorL->run(BACKWARD);
   myMotorR->setSpeed(100); 
   myMotorR->run(BACKWARD);
    
}
void Back(){
   myMotorR->setSpeed(100); 
   myMotorL->setSpeed(100);
   myMotorL->run(FORWARD);
   myMotorR->run(FORWARD);
}
void BothOn(){
   myMotorL->setSpeed(100);
   myMotorR->setSpeed(100);
   myMotorL->run(BACKWARD);
   myMotorR->run(BACKWARD);
}
void BothOff(){
   myMotorL->setSpeed(0); 
   myMotorR->setSpeed(0); 
   myMotorL->run(FORWARD);
   myMotorR->run(FORWARD);
}

void setup() {
  Serial.begin(9600);
  //**Motor Setup**
  AFMS.begin(9600);  

}

void loop() {
  while(Serial.available()) {
    delay(10);
    voice =Serial.readString();
  }
    if (voice.length() > 0) { //If there's something to read, do some functions
      Serial.println(voice);  
        if (voice == "left" || voice =="turn left"){
          RightOn();
        }
        if (voice == "right" || voice =="turn right"){
          LeftOn();
        }
        if (voice == "forward" || voice=="go"){
          BothOn();
        }
        if (voice == "backward" || voice == "back"){

          Back();
        }
        if (voice == "stop" || voice=="off" || voice=="all off"){
          BothOff();
        }
voice="";
    }

}
