// motor shield import 
#include <Wire.h> 
#include <Adafruit_MotorShield.h> 
#include "utility/Adafruit_MS_PWMServoDriver.h"
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *myMotorL = AFMS.getMotor(2);
Adafruit_DCMotor *myMotorR = AFMS.getMotor(3);

#include <SoftwareSerial.h> // bluetooth import

SoftwareSerial BTSerial(10,11); //setting up bluetooth
int lifecount=3;

long currtim=millis();
long prevtim=0;

const int led_pin = 3;
int laser = 8;


//pins for sensing
int sensorPin1 = A2;
int sensorPin2 = A4;
int sensorValue1 = 0;
int sensorValue2 = 0;

String voice; 
int dance_step = 1;

// *** Functions for IR LED ************************
#define BIT_IS_SET(i, bits)  (1 << i & bits) // variable that has two inputs, which are parameterized
const int pulse_width = 2000;
//number of times to go through the cycles, no cycles loop
const int command_length = 4;

//  distinct pattern of different voltages (intensities) to differentiate commands

const int DRAGON[]     = {255, 136, 130, 34};
const int BEE[]   = {255, 136, 130, 20};


void setup() {

  
  Serial.begin(9600); //Serial setup
  BTSerial.begin(38400); //Bluetooth setup
  AFMS.begin(9600);   //Motor Setup

  TCCR2B = TCCR2B & B11111000 | B00000001; // for PWM frequency of 31372.55 Hz
  pinMode(led_pin, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(sensorPin1, INPUT);
  pinMode(sensorPin2, INPUT);

}


// *** Helper functions **** 
void turnon(int pin, int time) {
  // the time it takes to go from peak to peak
  static const int period = 25;
  // this is the empirically found time at 0 V, found by the author of tutorial
  static const int wait_time = 9;

  // actual command to create the pulse
  for (time = time/period; time > 0; time --){ // the time is going down until it reaches 0
    digitalWrite(pin, HIGH);
    delayMicroseconds(wait_time);
    digitalWrite(pin, LOW);
    delayMicroseconds(wait_time);
  }
}
// this is the "gap time" between consecutive pulses
void turnoff(int pin, int time) {
  digitalWrite(pin, LOW);
  delayMicroseconds(time);
}

// this is using the previous two commands to write the distinct pattern for ONE part of the dragon and bee patterns
void send_byte(int bits) { // for each part of the pattern 
  for (int i = 7; i >= 0; i--) // for seven iterations
  {
    if (BIT_IS_SET(i, bits)) { //if the pattern part has more pulses left
      turnon(led_pin, pulse_width); //send the pattern
    } else {
      turnoff(led_pin, pulse_width); // otherwise go into the "gap time" 
    }
  }
}

//this is using the whole iteration put together
void command(const int bytes[]) { // for the number of distinct patterns
  for (int i = 0; i < command_length; i++) { // for each part of the pattern
    send_byte(bytes[i]); //send one part of the pattern 
  }
  turnoff(led_pin, 4 * pulse_width); //"gap time" 
}
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

void RightOff(){ 
   myMotorR->setSpeed(0); 
   myMotorR->run(FORWARD);
}

void LeftOff(){ 
   myMotorL->setSpeed(0); 
   myMotorL->run(FORWARD);
}

void Left90(){
  RightOn();
  delay(1200);
  RightOff();
}

void Right90(){
  LeftOn();
  delay(800);
  LeftOff();
}

void Turn180(){
  LeftOn();
  delay(1800);
  LeftOff();
}

void Ram(){
  myMotorL->setSpeed(250);
  myMotorR->setSpeed(250);
  myMotorL->run(BACKWARD);
  myMotorR->run(BACKWARD);
}

void shoot(){ 
  digitalWrite(12, HIGH);
  delay(500);
  command(DRAGON);
  command(DRAGON);
  command(DRAGON);
  command(DRAGON);
  command(DRAGON);
  command(DRAGON);
  command(DRAGON);
  command(DRAGON);
  command(DRAGON);

  digitalWrite(12, LOW);
}

void gotShot(){
  LeftOn();
  RightOff();
  delay(1000);
  RightOn();
  LeftOff();
  delay(1000);
  RightOff();
  LeftOff(); 
}

void loop() {
 
  if (lifecount==0){ // check if the robot has any lives
    gotShot();
    exit(0);
  }
  
// check if sensors got shot
  sensorValue1 = analogRead(sensorPin1);
  sensorValue2 = analogRead(sensorPin2);
  
// if sensors are shot, respond
  if(sensorValue1 < 100){ //100 is limit, value will get lower the more IR light is present
    gotShot();  
    prevtim=currtim; //update time
    lifecount-=1; // lose life
    }
   if(sensorValue2 < 100){
    gotShot(); 
    prevtim=currtim; 
    lifecount-=1;  
    }

  // ** Listening to Commands 
  while(BTSerial.available()) {
    delay(10);
    voice =BTSerial.readString(); 

  }
  if (voice.length() > 0) { //If there's something to read, execute functions
    Serial.println(voice);  

    // **** If the string matches one of the functions, execute that function *** 
    
      if (voice == "left" || voice =="turn left"){
        RightOn();

      }
      if (voice == "left 45"){
        RightOn();
        delay(600); // used to be 383
        RightOff();
      }

      if (voice == "right 45"){
        LeftOn();
        delay(400);
        LeftOff();
      }
      if(voice == "left 90"){
        Left90();
      }
      if (voice == "right 90"){
        Right90();
      }

      if (voice == "right 180" | voice == "turn 180"){
        Turn180();
      }

      if (voice == "left 180") {
        RightOn();
        delay(1600);
        RightOff();
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
      if (voice == "shoot" || voice=="pewpew" || voice=="fire"){
 
        shoot();

      }
      if (voice == "spin"){
        LeftOn();
        delay(3600);
        LeftOff();
      }
      if (voice == "run") {
        Turn180();
        BothOn();
      }
      if (voice == "ram") {
        Ram();
        delay(1000);
        BothOn();
      }

      // ** Beware, dance has issues with the bluetooth
      if (voice == "dance") { // kinda does the macarena
        if (dance_step == 1) {
          Right90();
        }
        if (dance_step == 2) {
          BothOn();
          delay(800);
        }
        if (dance_step == 3) {
          Turn180();
        }
        if (dance_step == 4) {
          BothOn();
        }
        if (dance_step == 5) {
          gotShot();
        }
        dance_step += 1;
        if (dance_step == 6) {
          dance_step = 1;
        }
      }
    if (voice != "dance") {
      voice = "";
    }
  }
}
