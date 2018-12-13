int sensorPin1 = A2;
//int sensorPin2 = A4;
int sensorValue1 = 0;
//int sensorValue2 = 0;
//int redPin = 8;
//int bluePin = 7;
void setup() {
  // put your setup code here, to run once:
  TCCR2B = TCCR2B & B11111000 | B00000001; // for PWM frequency of 31372.55 Hz
  pinMode(3, OUTPUT);
  pinMode(sensorPin1, INPUT);
//  pinMode(sensorPin2, INPUT);
//  pinMode(redPin, OUTPUT);
//  pinMode(bluePin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  analogWrite(3, 255);
//  digitalWrite(redPin,LOW);
//  digitalWrite(bluePin,LOW);
  sensorValue1 = analogRead(sensorPin1);
//  sensorValue2 = analogRead(sensorPin2);
  Serial.print("X: "); Serial.print(sensorValue1); Serial.print("  ");
//  Serial.print("Y: "); Serial.print(sensorValue2); Serial.print("  ");
  Serial.println("uT");
  
  if(sensorValue1 == 0){
//    digitalWrite(redPin, HIGH);
  //  Serial.println("Here! red");Serial.print(sensorValue1);
    //delay(500);
    //digitalWrite(redPin, LOW);
    }
//   if(sensorValue2 == 0){
//    digitalWrite(bluePin, HIGH);
//   // Serial.println("beep beep lettuce");Serial.print(sensorValue2);
//    //delay(500);
//    //digitalWrite(bluePin, LOW);
//    }
//    Serial.print(sensorValue1);
//    Serial.print(",");
//    Serial.print(sensorValue2);
//    Serial.print('\n');
}
