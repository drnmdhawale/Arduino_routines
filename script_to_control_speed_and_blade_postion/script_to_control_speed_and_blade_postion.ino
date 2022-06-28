// A. Arduino routines
// #A-1. Arduino script to control speed and blade postion for the cutter on OSA

/* This example code is in the public domain.
Speed and postion control of OSA Cutter Motor
To forward, reverse and break OSA Cutter Motor
The circuit:
* CMD signal is attached on pin 3
* POS Signal is attached on A0
created 2014
by Nandkishor Dhawale
*/
String inputString = ""; // a string to hold incoming data
//String stringOne, stringTwo, stringThree;
boolean stringComplete = false; // whether the string is complete
// constants won't change and used here to set pin numbers:
const int motCmdPin = 3; // the number of the Motor pin
int motPosPin = A0; // select the input pin for the potentiometer
int motSloPin = 4; // to read in for slowing motor if 1
int motSloPinEn = 8; // to enable the pin 4 high
// Variables will change:
int motSloPinVal =0; //to store the value of motSloPin
int CutBlStat =0; // to inform cutter blade postion
int motCmdState = LOW; // ledState used to set the LED
unsigned long initial_time = millis(); //initial time
unsigned long time =0 ; // time
int PulseCmd=1500; // hh
int FWDCmd = 1550;
int REVCmd =1450;
int STPCmd =1500;
double SetPosValue = 490;
double ErrPosValue = 0.00;
int ChgSpd = 1;
// variables for filtering position data
const int numReadings = 10;
double readings[numReadings]; // read analog 0
double PosValue = 0.00; // variable to store the value coming from the position sensor
int index = 0;
double total = 0.00;
// tintialise
void setup() {
// set the digital pin as output:
pinMode(motCmdPin, OUTPUT);
pinMode(motSloPinEn, OUTPUT);
pinMode(motSloPin, INPUT_PULLUP);
digitalWrite(motSloPinEn, LOW);
Serial.begin(9600);
// reserve 200 bytes for the inputString:
inputString.reserve(50);
for (int thisReading = 0; thisReading < numReadings; thisReading++)
readings[thisReading] = 0;
}
// the main program
void loop()
{motSloPinVal = digitalRead(motSloPin); //read cutter slow command from magnetic switch
if (stringComplete) {
ChgSpd=inputString.toInt();
// clear the string:
inputString = "";
stringComplete = false;
}
if (motSloPinVal == 1 && ChgSpd == 1){
// Cutter motor stop
PulseCmd=1500;
motCmdState = HIGH;
digitalWrite(motCmdPin, motCmdState);
delayMicroseconds(PulseCmd);
motCmdState = LOW;
digitalWrite(motCmdPin, motCmdState);
delay(1);
}
if (motSloPinVal == 0 && ChgSpd == 0){
// Cutter motor stop
PulseCmd=1500;
motCmdState = HIGH;
digitalWrite(motCmdPin, motCmdState);
delayMicroseconds(PulseCmd);
motCmdState = LOW;
digitalWrite(motCmdPin, motCmdState);
delay(1);
}
if (motSloPinVal == 1 && ChgSpd == 0) {
// Motor fast forward Command
PulseCmd=1950;
motCmdState = HIGH;
digitalWrite(motCmdPin, motCmdState);
delayMicroseconds(PulseCmd);
motCmdState = LOW;
digitalWrite(motCmdPin, motCmdState);
delay(1);
}
if (motSloPinVal == 0 && ChgSpd == 1) {
STPCmd= 1500;
FWDCmd= 1535;
REVCmd= 1465;
ErrPosValue = PosValue-SetPosValue;
if (ErrPosValue >= -35 && ErrPosValue <= 35) {
PulseCmd= STPCmd;
delay(5);
if (ErrPosValue >= -45 && ErrPosValue <= 45) {
//delay(2);
PulseCmd= STPCmd;
CutBlStat =1;
//delay(2);
}
} //Stop
if (ErrPosValue > 46) {
PulseCmd= REVCmd;
CutBlStat =0; } // Reverse Speed
if (ErrPosValue < -46) {PulseCmd= FWDCmd;
CutBlStat =0; } // Fowward Speed
// Motor slow forward command
motCmdState = HIGH;
digitalWrite(motCmdPin, motCmdState);
delayMicroseconds(PulseCmd);
motCmdState = LOW;
digitalWrite(motCmdPin, motCmdState);
delay(1);
}
total= total - readings[index]; // read from position the sensor:
//readings[index] = ((analogRead(motPosPin)/10)*0.3381)-1.0177;
readings[index] = analogRead(motPosPin); // add the reading to the total:
total= total + readings[index]; // advance to the next position in the array:
index = index + 1;
if (index >= numReadings) {
// ...wrap around to the beginning:
index = 0;
// calculate the average:
//PosValue = (total / numReadings)/2.84; //Position in cm
PosValue = total / numReadings; //Position in cm
time = millis() - initial_time;
}
// send it to the computer as ASCII digits
Serial.print("$GPGGA"); Serial.print(',');
//Serial.print(time/1000); Serial.print(',');
//Serial.print(PulseCmd); Serial.print(',');
Serial.print(motSloPinVal); Serial.print(',');
//Serial.print(SetPosValue); Serial.print(',');
Serial.print(PosValue); Serial.print(',');
//Serial.println(ErrPosValue); Serial.print(',');
Serial.println(CutBlStat); //Serial.print(',');
delay(1); // delay in between reads for stability
}
void serialEvent() {
while (Serial.available()) {
// get the new byte:
char inChar = (char)Serial.read();
// add it to the inputString:
inputString += inChar;
// if the incoming character is a newline, set a flag
// so the main loop can do something about it:
if (inChar == '\n') {
stringComplete = true;
}
}
}
