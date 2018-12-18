#include "max6675.h"

#define DP 1  // decimal places for output on serial port
#define D_MULT 0.001 // multiplier to convert temperatures from int to float

// used in main loop
float timestamp = 0;
boolean first;

int thermoDO = 8;
int thermoCLK = 10;

int thermoCS1 = 5;
int thermoCS2 = 9;

MAX6675 thermocouple1(thermoCLK, thermoCS1, thermoDO);
MAX6675 thermocouple2(thermoCLK, thermoCS2, thermoDO);
  
void setup() {
  Serial.begin(57600);
  Serial.println("temp1,temp2");
  // wait for MAX chip to stabilize
  delay(500);
}

// T1, T2 = temperatures x 1000
// t1, t2 = time marks, milliseconds
// ---------------------------------------------------
float calcRise( int32_t T1, int32_t T2, int32_t t1, int32_t t2 ) {
  int32_t dt = t2 - t1;
  if( dt == 0 ) return 0.0;  // fixme -- throw an exception here?
  float dT = (T2 - T1) * D_MULT;
  float dS = dt * 0.001; // convert from milli-seconds to seconds
  return ( dT / dS ) * 60.0; // rise per minute
}

// ------------------------------------------------------------------
void logger()
{
  int i;
  float RoR,t1,t2;
  float rx;

  // print timestamp from when samples were taken
  Serial.print( timestamp, DP );

  // print temperature, rate for each channel
  Serial.print(",");
   
  i = 0;
  Serial.print( t1 = D_MULT*temps[i], DP );
  Serial.print(",");
  RoR = calcRise( flast[i], ftemps[i], lasttimes[i], ftimes[i] );
  RoR = fRoR[i].doFilter( RoR /  D_MULT ) * D_MULT; // perform post-filtering on RoR values
  Serial.print( RoR , DP );
  i++;

  Serial.print(",");
  Serial.print( t2 = D_MULT * temps[i], DP );
  Serial.print(",");
  rx = calcRise( flast[i], ftemps[i], lasttimes[i], ftimes[i] );
  rx = fRoR[i].doFilter( rx / D_MULT ) * D_MULT; // perform post-filtering on RoR values
  Serial.print( rx , DP );
  i++;
  
  Serial.println();
};

void loop() {
   Serial.print(thermocouple1.readCelsius());
   Serial.print(",");
   Serial.println(thermocouple2.readCelsius());

   delay(1000);
}
