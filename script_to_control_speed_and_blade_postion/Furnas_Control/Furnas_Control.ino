// this example is public domain. enjoy!
// www.ladyada.net/learn/sensors/thermocouple

// A. Arduino routines
// #A-1. Arduino script to read thermocouple readings via 
// the breakout board and to control the on/off of the furnace

/* This example code is in the public domain.
The circuit:
* SENS-THERMO Thermocouple Amplifier MAX6675 Breakout Board
* SEN-00251 Thermocouple Type-K Glass Braid Insulated
created March 2016
by Nandkishor Dhawale
*/

#include "max6675.h"

int thermoDO = 4;
int thermoCS = 5;
int thermoCLK = 6;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

void setup() {
  Serial.begin(9600);
  Serial.println("MAX6675 test");
  // wait for MAX chip to stabilize
  delay(500);
}

void loop() {
  // basic readout test, just print the current temp
  
   Serial.print("C = "); 
   Serial.println(thermocouple.readCelsius());
   //Serial.print("F = "); 
   //Serial.println(thermocouple.readFahrenheit());
  
   delay(1000);
}
