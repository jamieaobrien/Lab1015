// This is pretty much the IoT tutorial code, which is
// linked in the Links docs
String voice;
int RED = 2;
int GREEN = 3;
int BLUE = 4;
void RedOn(){
digitalWrite (RED, HIGH);
}
void RedOff(){
digitalWrite (RED, LOW);
}
void GreenOn(){
digitalWrite (GREEN, HIGH);
}
void GreenOff(){
digitalWrite (GREEN, LOW);
}
void BlueOn(){
digitalWrite (BLUE, HIGH);
}
void BlueOff(){
digitalWrite (BLUE, LOW);
}
void allon() {
digitalWrite (RED, HIGH);
digitalWrite (GREEN, HIGH);
digitalWrite (BLUE, HIGH);
}
void alloff() {
digitalWrite (RED, LOW);
digitalWrite (GREEN, LOW);
digitalWrite (BLUE, LOW);
}
void setup() {

pinMode(RED, OUTPUT);
pinMode(GREEN, OUTPUT);
pinMode(BLUE, OUTPUT);
}
void loop() {
while(Serial.available()) {
delay(10);
char c=Serial.read();
if(c=='#')
{break; }
voice += c;
}
if (voice.length() > 0) {
Serial.println(voice);
if (voice == "left" || voice == "all")
{
allon() ;
}
else if (voice == "right" || voice=="all off")
{
alloff() ;
}
else if(voice =="red" || voice =="red on"){
RedOn();
}
else if(voice =="red off"){
RedOff();
}
else if(voice =="forward" || voice =="go"){
GreenOn();
}
else if( voice =="green off" ){
GreenOff();
}
else if(voice =="back" || voice =="blue on"){
BlueOn();
}
else if(voice =="blue off"){
BlueOff();
}
voice="";
}
}
