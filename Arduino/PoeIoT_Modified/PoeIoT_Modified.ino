// This is the IoT tutorial sort of modified to 
// our system and to motors. 

#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

// *****MOTOR CREATE******
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); //creating the motor shield object

Adafruit_DCMotor *myMotorL = AFMS.getMotor(1); //creating the object for Motor 1
Adafruit_DCMotor *myMotorR = AFMS.getMotor(2); //creating the object for Motor 2

String voice; //The input from the serial is the speech, it's stored as a string


// ** Functions to Turn the Motors on and off ***
void LeftOn(){
   myMotorL->run(FORWARD);
   myMotorL->setSpeed(50); 
}
void LeftOff(){
   myMotorL->setSpeed(0); 
}
void RightOn(){
   myMotorR->run(FORWARD);
   myMotorR->setSpeed(50); 
}
void RightOff(){
   myMotorR->setSpeed(0); 
}
void BothOn(){
   myMotorL->run(FORWARD);
   myMotorR->run(FORWARD);
}
void BothOff(){
   myMotorL->setSpeed(0); 
   myMotorR->setSpeed(0); 
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
        if (voice == "left" || voice =="left on"){
          LeftOn();
        }
        if (voice == "right" || voice =="right on"){
          RightOn();
        }
        if (voice == "stop left" || voice == "left off"){
          LeftOff();
        }
        if (voice == "stop right" || voice == "right off"){
          RightOff();
        }
        if (voice == "forward" || voice=="go"){
          BothOn();
        }
        if (voice == "stop" || voice=="off" || voice=="all off"){
          BothOff();
        }
voice="";
    }

}
