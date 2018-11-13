

//int myPin = 3;
//
//void setup() {
//  pinMode(myPin, OUTPUT);
//}
//
//void loop() {
//analogWrite(myPin, 128); // 50:50 square wave
//delayMicroseconds(263);
//
//analogWrite(myPin, 0);
//delayMicroseconds(2630);
//}

/* Control a Lutron Maestro light dimmer */
#define BIT_IS_SET(i, bits)  (1 << i & bits)

// LED connected to digital pin 13
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

void setup()
{
  pinMode(LED_PIN, OUTPUT);
}

/* Modulate pin at 39 kHz for give number of microseconds */
void on(int pin, int time) {
  static const int period = 25;
  // found wait_time by measuring with oscilloscope
  static const int wait_time = 9;  

  for (time = time/period; time > 0; time--) {
    digitalWrite(pin, HIGH);
    delayMicroseconds(wait_time);
    digitalWrite(pin, LOW);
    delayMicroseconds(wait_time);
  }
}

/* Leave pin off for time (given in microseconds) */
void off(int pin, int time) {
  digitalWrite(pin, LOW);
  delayMicroseconds(time);
}

/* Send a byte over the IR LED */
void send_byte(int bits) {
  for (int i = 7; i >= 0; i--)
  {
    if (BIT_IS_SET(i, bits)) {
      on(LED_PIN, PULSE_WIDTH);
    } else {
      off(LED_PIN, PULSE_WIDTH);
    }
  }
}

/* Send a full command */
void command(const int bytes[]) {
  for (int i = 0; i < COMMAND_LENGTH; i++) {
    send_byte(bytes[i]);
  }
  off(LED_PIN, 4 * PULSE_WIDTH);
}

void loop()
{
  command(UP);
  delay(1000);
  command(DOWN);
  delay(1000);
}
