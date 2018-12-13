#define BIT_IS_SET(i, bits)  (1 << i & bits) // variable that has two inputs, which are parameterized

const int led_pin = 3;
const int pulse_width = 2000;
//number of times to go through the cycles, no cycles loop
const int command_length = 4;

//  distinct pattern of different voltages (intensities) to differentiate commands

const int DRAGON[]     = {255, 136, 130, 34};
const int BEE[]   = {255, 136, 130, 20};
void setup() {

  pinMode(led_pin, OUTPUT);

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
void loop() {

  command(DRAGON);
  delay(1000);
}
