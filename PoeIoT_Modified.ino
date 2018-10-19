// This is the IoT tutorial sort of modified to 
// our system and to motors. 

String voice; //The input from the serial is the speech, it's stored as a string
int leftMotor = 2;
int rightMotor = 3;

// ** Functions to Turn the Motors on and off ***
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

void setup() {
  Serial.begin(9600);
  pinMode(leftMotor, OUTPUT);
  pinMode(rightMotor, OUTPUT);
}

void loop() {
  while(Serial.available()) {
    delay(10);
    char c=Serial.read();
    if(c=='#') //Not sure about this part, if the character is a pound sign? 
    {break;}
    voice +=c;
  }
    if (voice.length() > 0) { //If there's something to read, do some functions
      Serial.println(voice);  //I'm also not sure why it can be reading it, when serial.read returns bytes and serial.readString() prints the string
        if (voice == "left" || voice =="left on"){
          LeftOn();
        }
        if (voice == "right" || voice =="right on"){
          RightOn();
        }
        if (voice == "stop left"){
          LeftOff();
        }
        if (voice == "stop right"){
          RightOff();
        }
        if (voice == "forward" || voice=="go"){
          BothOn();
        }
        if (voice == "stop" || voice=="off"){
          BothOff();
        }
voice="";
    }

}
