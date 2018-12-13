// motor shield import 
#include <Wire.h> 
#include <Adafruit_MotorShield.h> 
#include "utility/Adafruit_MS_PWMServoDriver.h"
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
<<<<<<< HEAD
Adafruit_DCMotor *myMotorL = AFMS.getMotor(3);
Adafruit_DCMotor *myMotorR = AFMS.getMotor(2);
#define BIT_IS_SET(i, bits)  (1 << i & bits) // variable that has two inputs, which are parameterized
=======
Adafruit_DCMotor *myMotorL = AFMS.getMotor(2);
Adafruit_DCMotor *myMotorR = AFMS.getMotor(3);
>>>>>>> d2e0e98703334f45cab8788090408669cd1b7a5e

#include <SoftwareSerial.h> // bluetooth import

SoftwareSerial BTSerial(10,11); //setting up bluetooth
int lifecount=3;

long currtim=millis();
long prevtim=0;

const int led_pin = 3;
int laser = 8;
<<<<<<< HEAD
int checkbluetooth = 5;
int checkcommand = 6;
const int pulse_width = 2000;
//number of times to go through the cycles, no cycles loop
const int command_length = 4;
=======
>>>>>>> d2e0e98703334f45cab8788090408669cd1b7a5e


//pins for sensing
int sensorPin1 = A2;
int sensorPin2 = A4;
int sensorValue1 = 0;
int sensorValue2 = 0;

String voice; 
int dance_step = 1;

<<<<<<< HEAD
//notes for songs
const int c = 261;
const int d = 294;
const int e = 329;
const int f = 349;
const int g = 391;
const int gS = 415;
const int a = 440;
const int aS = 455;
const int b = 466;
const int cH = 523;
const int cSH = 554;
const int dH = 587;
const int dSH = 622;
const int eH = 659;
const int fH = 698;
const int fSH = 740;
const int gH = 784;
const int gSH = 830;
const int aH = 880;
 
const int buzzerPin = 8;
const int ledPin1 = 12;
const int ledPin2 = 13;
 
int counter = 0;
 
=======
// *** Functions for IR LED ************************
#define BIT_IS_SET(i, bits)  (1 << i & bits) // variable that has two inputs, which are parameterized
const int pulse_width = 2000;
//number of times to go through the cycles, no cycles loop
const int command_length = 4;

//  distinct pattern of different voltages (intensities) to differentiate commands

const int DRAGON[]     = {255, 136, 130, 34};
const int BEE[]   = {255, 136, 130, 20};

>>>>>>> d2e0e98703334f45cab8788090408669cd1b7a5e

void setup() {

  
  Serial.begin(9600); //Serial setup
  BTSerial.begin(38400); //Bluetooth setup
  AFMS.begin(9600);   //Motor Setup

  TCCR2B = TCCR2B & B11111000 | B00000001; // for PWM frequency of 31372.55 Hz
  pinMode(led_pin, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(sensorPin1, INPUT);
  pinMode(sensorPin2, INPUT);
  pinMode(checkbluetooth, OUTPUT);
  pinMode(checkcommand, OUTPUT);
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
   myMotorR->run(FORWARD);
   myMotorL->setSpeed(100); 
   myMotorL->run(FORWARD);

}

void RightOn(){
   myMotorL->setSpeed(0);
   myMotorL->run(FORWARD);
   myMotorR->setSpeed(100); 
   myMotorR->run(FORWARD);
    
}
void Back(){
   myMotorR->setSpeed(100); 
   myMotorL->setSpeed(100);
   myMotorL->run(BACKWARD);
   myMotorR->run(BACKWARD);
}
void BothOn(){
   myMotorL->setSpeed(100);
   myMotorR->setSpeed(100);
   myMotorL->run(FORWARD);
   myMotorR->run(FORWARD);
}
void BothOff(){
   myMotorL->setSpeed(0); 
   myMotorR->setSpeed(0); 
   myMotorL->run(FORWARD);
   myMotorR->run(FORWARD);
}

void RightOff(){ 
   myMotorR->setSpeed(0); 
   myMotorR->run(BACKWARD);
}

void LeftOff(){ 
   myMotorL->setSpeed(0); 
   myMotorL->run(FORWARD);
}

void Left90(){
  BothOff();
  RightOn();
  delay(2170);
  RightOff();
}

void Right90(){
  BothOff();
  LeftOn();
  delay(2170);
  LeftOff();
}

void Turn180(){
  BothOff();
  LeftOn();
  delay(4340);
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

<<<<<<< HEAD
//This is for song stuff
void firstSection()
{
  beep(a, 500);
  beep(a, 500);    
  beep(a, 500);
  beep(f, 350);
  beep(cH, 150);  
  beep(a, 500);
  beep(f, 350);
  beep(cH, 150);
  beep(a, 650);
 
  delay(500);
 
  beep(eH, 500);
  beep(eH, 500);
  beep(eH, 500);  
  beep(fH, 350);
  beep(cH, 150);
  beep(gS, 500);
  beep(f, 350);
  beep(cH, 150);
  beep(a, 650);
 
  delay(500);
}
void beep(int note, int duration)
{
  //Play tone on buzzerPin
  tone(buzzerPin, note, duration);
 
  //Play different LED depending on value of 'counter'
  if(counter % 2 == 0)
  {
    delay(duration);
  }else
  {
    delay(duration);
  }
 
  //Stop tone on buzzerPin
  noTone(buzzerPin);
 
  delay(50);
 
  //Increment counter
  counter++;
}
 

=======
>>>>>>> d2e0e98703334f45cab8788090408669cd1b7a5e

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
<<<<<<< HEAD
    voice = BTSerial.readString();
    Serial.print("Plaz ");
    Serial.println(voice);
=======
    voice =BTSerial.readString(); 

>>>>>>> d2e0e98703334f45cab8788090408669cd1b7a5e
  }
if (voice.length() > 0) { //If there's something to read, execute functions
    Serial.println(voice);  
<<<<<<< HEAD
      digitalWrite(checkbluetooth, HIGH);
      delay(100);
      digitalWrite(checkbluetooth, LOW);
=======

    // **** If the string matches one of the functions, execute that function *** 
     
>>>>>>> d2e0e98703334f45cab8788090408669cd1b7a5e
      if (voice == "left" || voice =="turn left"){
        digitalWrite(checkcommand, HIGH);
        delay(10);
        RightOn();
        digitalWrite(checkcommand, LOW);
        BTSerial.flush();
        Serial.println("backlog"+String(voice)); 
      }
      if (voice == "left 45"){
        BothOff();
        RightOn();
        delay(1050); // used to be 383
        RightOff();
        BTSerial.flush();
      Serial.println("backlog"+String(voice)); 
      }
      if (voice == "right 45"){
        BothOff();
        LeftOn();
        delay(1050);
        LeftOff();
        BTSerial.flush();
        Serial.println("backlog"+String(voice)); 
      }
      if(voice == "left 90"){
        BothOff();
        Left90();
        BTSerial.flush();
  Serial.println("backlog"+String(voice)); 
      }
      if (voice == "right 90"){
        BothOff();
        Right90();
        BTSerial.flush();
        Serial.println("backlog"+String(voice)); 
      }

      if (voice == "right 180" | voice == "turn 180"){
        BothOff();
        Turn180();
        BTSerial.flush();
        Serial.println("backlog"+String(voice)); 
      }

      if (voice == "left 180") {
        BothOff();
        BothOff();
        RightOn();
        delay(4340);
        RightOff();
        BTSerial.flush();
        Serial.println("backlog"+String(voice)); 
      }
      
      if (voice == "right" || voice =="turn right"){
        LeftOn();
        BTSerial.flush();
        Serial.println("backlog"+String(voice)); 
      }
      if (voice == "forward" || voice=="go"){
      digitalWrite(checkcommand, HIGH);
      delay(100);
        BothOn();
      digitalWrite(checkcommand, LOW);
      BTSerial.flush();
      Serial.print(BTSerial.readString());
      Serial.println("backlog"); 
      }
      if (voice == "backward" || voice == "back"){
        BothOff();
        Back();
        BTSerial.flush();
        Serial.println("backlog"+String(voice)); 
      }
      if (voice == "stop" || voice=="off" || voice=="all off"){
        BothOff();
        BTSerial.flush();
        Serial.println("backlog"+String(voice)); 
      }
      if (voice == "shoot" || voice=="pewpew" || voice=="fire"){
 
        Shootybootymcscooty();
        BTSerial.flush();
        Serial.println("backlog"+String(voice)); 
      }
      if (voice == "spin"){
        LeftOn();
        delay(10500);
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
<<<<<<< HEAD
      if (voice == "sing") {
        firstSection();
      }
=======
      // ** Beware, dance has issues with the bluetooth
>>>>>>> d2e0e98703334f45cab8788090408669cd1b7a5e
      if (voice == "dance") { // kinda does the macarena
        GASGASGAS();
        firstSection();
        if (dance_step == 1) {
          Right90();
          
        }
        if (dance_step == 2) {
          BothOn();
          
          //delay(800);
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
//    if (voice != "dance") {
    voice = "";
//    }
  }
}
