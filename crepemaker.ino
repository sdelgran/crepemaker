
// The user.h file contains user-definable compiler options
// It must be located in the same folder as this file
#include "user.h"

// libraries to be installed from the library manager
#include "max6675.h"

#include "math.h"

// global variables and objects
MAX6675 thermocouple1(CLKPIN, CS1PIN, DOPIN);
MAX6675 thermocouple2(CLKPIN, CS2PIN, DOPIN);

// variables for the heating control
float previoustemp1 = 0;
float previoustemp2 = 0;
unsigned long lastControlUpdate = 0; // last time controls were updated
float control1 = 0; // how much (0..1) of the heating cycle is the 1st heater switched on
float control2 = 0;
byte on1 = 0; // whether the 1st heater is currently switched on
byte on2 = 0;

// variables for the moving averages on temperature readings
float tempReadings1[NUMREADINGS]; // the temperature readings from the analog input
float tempReadings2[NUMREADINGS];
int readIndex = 0; // the index of the current reading
unsigned long lastRead = 0; // last time readings were updated
float tempReadingsTotal1 = 0; // the running total
float tempReadingsTotal2 = 0;
float temp1; // the average
float temp2;

void seriallogger(float temp1, float temp2, float ror1, float ror2, float projectedtemp1, float projectedtemp2, float control1, float control2) {
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
  Serial.print(projectedtemp2);
  Serial.print(",");
  Serial.print(control1);
  Serial.print(",");
  Serial.print(control2);
}

void setup() {
  pinMode(SSR1PIN, OUTPUT);
  pinMode(SSR2PIN, OUTPUT);

  Serial.begin(BAUD);

  // wait for MAX chips to stabilize
  delay(500);

  // initialize the readings array to zero
  for (int i = 0; i < NUMREADINGS; i++) {
    tempReadings1[i] = 0;
    tempReadings2[i] = 0;
  }

  // initialize the temperature readings and averages with actual values
  while (readIndex < NUMREADINGS - 1) {
    updateTemperatureReadings();
    delay(LOOP_PERIOD_MS);
  }
}

void loop() {
  updateTemperatureReadings();
  updateControls();
  delay(LOOP_PERIOD_MS);
}

void updateTemperatureReadings() {
  if (millis() - lastRead < READING_PERIOD_MS) {
    return;
  }

  lastRead += READING_PERIOD_MS;

  // subtract the last reading
  tempReadingsTotal1 = tempReadingsTotal1 - tempReadings1[readIndex];
  tempReadingsTotal2 = tempReadingsTotal2 - tempReadings2[readIndex];

  // read from the sensor
  tempReadings1[readIndex] = thermocouple1.readCelsius();
  tempReadings2[readIndex] = thermocouple2.readCelsius();

  // add the reading to the total
  tempReadingsTotal1 = tempReadingsTotal1 + tempReadings1[readIndex];
  tempReadingsTotal2 = tempReadingsTotal2 + tempReadings2[readIndex];

  // advance to the next position in the array
  readIndex = readIndex + 1;

  // if we are at the end of the array...
  if (readIndex >= NUMREADINGS) {
    // ...wrap around to the beginning
    readIndex = 0;
  }

  // calculate the average
  temp1 = tempReadingsTotal1 / NUMREADINGS;
  temp2 = tempReadingsTotal2 / NUMREADINGS;
}

void updateControls()
{
  unsigned long timeelapsed = 0;
  float ror1;
  float ror2;
  float projectedtemp1;
  float projectedtemp2;

  timeelapsed = (millis() - lastControlUpdate);

  if (timeelapsed < CONTROL_UPDATE_PERIOD_MS) {
    if (on1 == 1 && timeelapsed > control1 * CONTROL_UPDATE_PERIOD_MS) {
      stop1();

      if (control2 > 0) {
        start2();
      }
    }

    if (on2 == 1 && timeelapsed > (control1 + control2) * CONTROL_UPDATE_PERIOD_MS) {
      stop2();
    }

    return;
  }

  lastControlUpdate += CONTROL_UPDATE_PERIOD_MS; // do not use timeelapsed here to avoid drift

  ror1 = (((temp1 - previoustemp1) / timeelapsed) * 1000 * 60); //Raise of rise in C/min
  ror2 = (((temp2 - previoustemp2) / timeelapsed) * 1000 * 60); //Raise of rise in C/min
  previoustemp1 = temp1;
  previoustemp2 = temp2;

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

  // proportional controls
  control1 = max(0, min(1, (SV1 - projectedtemp1) / PROPORTIONAL_TEMP_RANGE));
  control2 = max(0, min(1, (SV2 - projectedtemp2) / PROPORTIONAL_TEMP_RANGE));

  // 1st heater takes priority, 2nd heater takes what is left
  control2 = min(control2, 1 - control1);

  // use steps of MINIMAL_SSR_MS to avoid short switches
  control1 = roundf(control1 * CONTROL_UPDATE_PERIOD_MS / MINIMAL_SSR_MS) * MINIMAL_SSR_MS / CONTROL_UPDATE_PERIOD_MS;
  control2 = roundf(control2 * CONTROL_UPDATE_PERIOD_MS / MINIMAL_SSR_MS) * MINIMAL_SSR_MS / CONTROL_UPDATE_PERIOD_MS;

  if (control1 > 0) {
    start1();
    stop2();
  }
  else {
    stop1();

    if (control2 > 0) {
      start2();
    }
    else {
      stop2();
    }
  }

  seriallogger(temp1, temp2, ror1, ror2, projectedtemp1, projectedtemp2, control1, control2);
}

void start1() {
  digitalWrite(SSR1PIN, HIGH);
  on1 = 1;
}

void stop1() {
  digitalWrite(SSR1PIN, LOW);
  on1 = 0;
}

void start2() {
  digitalWrite(SSR2PIN, HIGH);
  on2 = 1;
}

void stop2() {
  digitalWrite(SSR2PIN, LOW);
  on2 = 0;
}
