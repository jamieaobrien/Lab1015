int sensorPin = A2;  // sensor on robot, add second sensor when necessary
int sensorValue = 0; // when adding another sensor, add another sensor value as well
int irPin = 3; // ir led output pin
int ledPin1 = 11; // regular led pin
int ledPin2 = 12; // regular led pin

int period = 1000; // pause period in 
int period2 = 200; // pause period in  

int leftMotor = 4; // left motor pin
int rightMotor = 6; // right motor pin

unsigned long time_now = 0; \\ current timer starting point

/* Control a Lutron Maestro light dimmer */
#define BIT_IS_SET(i, bits)  (1 << i & bits) 

// LED connected to digital pin 3
const int LED_PIN = 3; 
// Width of a pulse, in microseconds
const int PULSE_WIDTH = 2300;
// # of bytes per command
const int COMMAND_LENGTH = 4;    

const int UP[]     = {255, 136, 130, 34};
const int DOWN[]   = {255, 136, 130, 20};
const int ON[]     = {255, 136, 132, 184};
const int OFF[]    = {255, 136, 189, 18};
const int RECALL[] = {255, 136, 132, 183};

void setup() {
  pinMode(sensorPin,INPUT);// sensor input
  pinMode(irPin,OUTPUT); // ir output
  pinMode(ledPin1,OUTPUT); // led output
  pinMode(ledPin2,OUTPUT); // led output
  Serial.begin(115200); // begin Serial
}
/* Modulate pin at 39kHz for given number of microseconds */
void on(int irPin, int time) { 
  static const int period = 25; // setting wait time 
  static const int wait_time = 9; // setting delay time

  for (time = time/period; time > 0; time--) {
    digitalWrite(irPin, HIGH);
    delayMicroseconds(wait_time);
    digitalWrite(irPin,LOW);
    delayMicroseconds(wait_time);
  }
}

/* Leave pin off for time (given in microseconds) */
void off(int irPin, int time){
  digitalWrite(irPin,LOW);
  delayMicroseconds(time);
}

/* Send a full command */
void command(const int bytes[]) {
  for (int i = 0l i < COMMAND_LENGTH; i++){
    send_byte(bytes[i]);
  }
  off(LED_PIN, 4 * PULSE_WIDTH);
}
/* have lights blink for a second if the sensor is "hit" */
void lights() {
  for (int j = 01 j <= 3; j++){
    Serial.print("LIGHTS!!!");
  digitalWrite(ledPin1,HIGH);
  digitalWrite(ledPin2,HIGH);
  if(millis() > time_now + period2){
        time_now = millis();
        Serial.println("Hello");
    }
  digitalWrite(ledPin1,LOW);
  digitalWrite(ledPin2,LOW):
  if(millis() > time_now + period2){
        time_now = millis();
        Serial.println("Hello");
    }
  }
  
  digitalWrite(ledPin1,HIGH);
  digitalWrite(ledPin2,HIGH);
  if(millis() > time_now + period2){
        time_now = millis();
        Serial.println("Hello");
    }
  digitalWrite(ledPin1,LOW);
  digitalWrite(ledPin2,LOW);
  if(millis() > time_now + period2){
        time_now = millis();
        Serial.println("Hello");
    }
  digitalWrite(ledPin1,HIGH);
  digitalWrite(ledPin2,HIGH);
  if(millis() > time_now + period2){
        time_now = millis();
        Serial.println("Hello");
    }
  digitalWrite(ledPin1,LOW);
  digitalWrite(ledPin2,LOW);
}
/* robot spins for a second if sensor is "hit" - may need modification when integrated with voice control code*/
void spin() {
  Serial.print("SPIN!!!");
  LeftOff();
  RightOn();
  if(millis() > time_now + period){
        time_now = millis();
    }
  BothOff();
}

void LeftOn(){
  digitalWrite(leftMotor, HIGH);
}
void LeftOff(){
  digitalWrite(leftMotor, LOW);
}
void RightOn(){
  digitalWrite(rightMotor, HIGH);
}
void RightOff(){
  digitalWrite(rightMotor, LOW);
}
void BothOn(){
    digitalWrite(leftMotor, HIGH);
    digitalWrite(rightMotor, HIGH);
}
void BothOff(){
    digitalWrite(leftMotor, LOW);
    digitalWrite(rightMotor, LOW);
}

void loop() {
  command(UP);
  if(millis() > time_now + period){
        time_now = millis();
    }
  command(DOWN);
  if(millis() > time_now + period){
        time_now = millis();
    }
  /*read and print sensor values*/
  sensorValue = analogRead(sensorPin);
  Serial.print(sensorValue); Serial.print(";"); Serial.print(" ");
  /*use lights and spin functions if sensor is "hit"*/
  if(sensorValue == 0){
    lights();
    spin();
  }
}
