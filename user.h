// user.h
// This file contains user definable compiler directives

// *************************************************************************************
// NOTE TO USERS: the following parameters should be
// be reviewed to suit your preferences and hardware setup.  
// First, load and edit this sketch in the Arduino IDE.
// Next compile the sketch and upload it to the Arduino.

// Set values 
#define SV1 190 //Set temperature for plate 1 (in Celsius)
#define SV2 190 //Set temperature for plate 2 (in Celsius)

#define RISEINERTIA 3 //how long it takes the system to stop rising (in min)
#define FALLINERTIA 1 //how long it takes the system to stop rising (in min)

#define SAMPLING 5 //loops to calculate ror

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
