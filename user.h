// user.h
// This file contains user definable compiler directives

// *************************************************************************************
// NOTE TO USERS: the following parameters should be
// be reviewed to suit your preferences and hardware setup.  
// First, load and edit this sketch in the Arduino IDE.
// Next compile the sketch and upload it to the Arduino.

// Set values 
#define SV1 205 //Set temperature for plate 1 (in Celsius)
#define SV2 200 //Set temperature for plate 2 (in Celsius)

#define RISEINERTIA 3 //how long it takes the system to stop rising (in min)
#define FALLINERTIA 1 //how long it takes the system to stop falling (in min)

// cycle time, in ms
#define LOOP_PERIOD_MS 1

// Delay time between temperature readings
// from the temperature sensor (ms).
// (must be larger than LOOP_PERIOD_MS)
// MAX6675 takes about 200 ms to convert
#define READING_PERIOD_MS 250

// How many readings are taken to determine a mean temperature.
// accounts for the thermocouple noise
#define NUMREADINGS 16

// delay between 2 control updates
// (must be larger than LOOP_PERIOD_MS)
#define CONTROL_UPDATE_PERIOD_MS 2000

// minimal time for the SSR to be on
#define MINIMAL_SSR_MS 100

// the temperature difference from the objective when a full heating cycle is needed, in Celsius
// when the temperature difference is lower than this value, the heating cycle length is proportional to this difference
#define PROPORTIONAL_TEMP_RANGE 20.0

// Pin mapping 
// Common SPI pins 
#define DOPIN 12
#define CLKPIN 13

// SPI Chip Select for each MAX6675 chip
#define CS1PIN 11
#define CS2PIN 10
// SSR control
#define SSR1PIN 8
#define SSR2PIN 9

// Other
#define BAUD 57600  // serial baud rate
// *************************************************************************************
