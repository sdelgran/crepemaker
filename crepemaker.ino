
// The user.h file contains user-definable compiler options
// It must be located in the same folder as this file
#include "user.h"

// libraries to be installed from the library manager
#include "max6675.h"

// other compile directives
#define LOOPTIME 500 // cycle time, in ms

// global variables and objects
MAX6675 thermocouple1(CLKPIN, CS1PIN, DOPIN);
MAX6675 thermocouple2(CLKPIN, CS2PIN, DOPIN);

float previoustemp1 = 0;
float previoustemp2 = 0;
unsigned long timestamp = 0;
int counter = 0;

void seriallogger(float temp1, float temp2, float ror1, float ror2, float projectedtemp1, float projectedtemp2) {
  Serial.print(temp1);
  Serial.print(",");
  Serial.print(temp2);
  Serial.print(",");
  Serial.print(ror1);
  Serial.print(",");
  Serial.print(ror2);
  Serial.print(",");
  Serial.print(projectedtemp1);
  Serial.print(",");
  Serial.println(projectedtemp2);
}

void setup() {
  pinMode(SSR1PIN, OUTPUT);
  pinMode(SSR2PIN, OUTPUT);

  Serial.begin(BAUD);
  
  // wait for MAX chips to stabilize
  delay(500);
}

void loop() {  
  unsigned long timeelapsed = 0;
  float temp1;
  float temp2;
  float ror1;
  float ror2;
  float projectedtemp1;
  float projectedtemp2;

  temp1 = thermocouple1.readCelsius();
  temp2 = thermocouple2.readCelsius();

  if (counter >= SAMPLING) {
    timeelapsed = (millis() - timestamp);
    ror1 = (((temp1 - previoustemp1) / timeelapsed) * 1000 * 60); //Raise of rise in C/min  
    ror2 = (((temp2 - previoustemp2) / timeelapsed) * 1000 * 60); //Raise of rise in C/min  
    timestamp = millis();
    counter = 0;
    previoustemp1 = temp1; // We should really calculate an average rather...
    previoustemp2 = temp2;    
  }
  
  counter++;

  if (ror1 >= 0) {
    projectedtemp1 = (temp1 + (ror1 * RISEINERTIA));
  }
  else {
    projectedtemp1 = (temp1 + (ror1 * FALLINERTIA));
  }

  if (ror2 >= 0) {
    projectedtemp2 = (temp2 + (ror2 * RISEINERTIA));
  }
  else {
    projectedtemp2 = (temp2 + (ror2 * FALLINERTIA));
  }

  seriallogger(temp1,temp2,ror1,ror2,projectedtemp1,projectedtemp2);

  if (projectedtemp1  <= SV1) {
   digitalWrite(SSR2PIN, LOW); 
   digitalWrite(SSR1PIN, HIGH); 
  } 
  else {
     digitalWrite(SSR1PIN, LOW); 

    if (projectedtemp2 <= SV2) {
     digitalWrite(SSR2PIN, HIGH); 
    }
    else {
      digitalWrite(SSR2PIN, LOW); 
    }
  }
  delay(LOOPTIME);
}
