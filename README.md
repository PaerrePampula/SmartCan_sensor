# SmartCan_sensor
 A project demonstrating the concept of a trashcan that is able to measure and display information about its' fullness. Made using MBED-OS
 Also creates data on an IoT platform. This platform however is no longer active.
 
# Usage 
Built using MBED OS, so download the development software for that.
Needed parts:

distance sensor, sonar. Two DIY stereos could even work with the other one configured to be a microphone (or just a microphone), but you could also make it a bit easier on yourself if you just buy a 1-2 euro sonar made for projects like these off the internet

An OLED screen, built on the Adafruit_SSD1331, so that is preferrable

A magnetic contact

A wifi adapter, the ESP82266 is preferrable

nucleo l432kc board.

soldering tools for the magnetic contact.

and ofcourse the wiring, more info on those in the source code.

My version was built on a breadboard, but you could ofcourse, solder the thing together

The magnetic contacts should be placed so that they carry current whenever the lid is closed, and not when it opens, place them like so.

The system starts up whenever the lid gets closed, measuring the distance to the bottom of the container, meaning that the sonar should be placed under the lid of the container. Max distance is about 30cm, so the container must be smaller or equals to 30cm in depth
