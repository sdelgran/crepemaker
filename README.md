# crepemaker
Arduino-based temperature control of a commercial double crepe machine

This started as a project to enhance the temperature control of a SODIR 400ED crepe machine. The OEM on/off control with bulb thermostats has a 100F hysteresis which makes the equipment difficult to use. Also, the machine requires a 50A circuit (7000W @220V). We would like to control the heating elements so that we only power one at a time and can use a 20A circuit. Classic disctinct PID would not work for that purpose.
