#define BIT_IS_SET(i, bits)  (1 << i & bits) // variable that has two inputs, which are parameterized

int milkylife=3;

long currtim=millis();
long prevtim=0;

const int led_pin = 3;
const int pulse_width = 2000;
//number of times to go through the cycles, no cycles loop
const int command_length = 4;

//  distinct pattern of different voltages (intensities) to differentiate commands

const int DRAGON[]     = {255, 136, 130, 34};
const int BEE[]   = {255, 136, 130, 20};

//pins for sensing
int sensorPin1 = A2;
int sensorPin2 = A4;
int sensorValue1 = 0;
int sensorValue2 = 0;
int redPin = 8;
int bluePin = 7;

void setup() {

  pinMode(led_pin, OUTPUT);
  
  Serial.begin(9600);
  //**Motor Setup**
  AFMS.begin(9600);

  TCCR2B = TCCR2B & B11111000 | B00000001; // for PWM frequency of 31372.55 Hz
  pinMode(3, OUTPUT);
  pinMode(sensorPin1, INPUT);
  pinMode(sensorPin2, INPUT);

  Serial.begin(9600);
}
// *** Writing all the helper functions **** 

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

void Shootybootymcscooty(){ 
  command(DRAGON);
  delay(1000);
}

void GASGASGAS(){
  LeftOn();
  RightOff();
  delay(1000)
  RightOn();
  LeftOff();
  delay(1000);
  RightOff();
  LeftOff(); 
}

void loop() {
  if (milkylife==0){
    GASGASGAS();
    exit(0);
  }
  analogWrite(3, 255);
  sensorValue1 = analogRead(sensorPin1);
  sensorValue2 = analogRead(sensorPin2);
  
  if(sensorValue1 == 0 && currtim-prevtim>=3000){
    GASGASGAS(); 
    prevtim=currtim;
    milkylife-=1;
    }
   if(sensorValue2 == 0 && currtim-prevtim>=3000){
    GASGASGAS(); 
    prevtim=currtim 
    milkylife-=1;  
    }
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
      if (voice == "shoot" || voice=="pewpew" || voice=="fire"){
        Shootybootyscooty();
      }
    voice="";
  }

}
