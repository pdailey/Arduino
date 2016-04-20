/*
  Serial Event example using 32u4 microprocesor
 https://www.arduino.cc/en/Guide/ArduinoLeonardoMicro
 
 When new serial data arrives, this sketch adds it to a String.
 When a newline is received, the loop prints the string and
 clears it.

 A good test for this is to try it with a GPS receiver
 that sends out NMEA 0183 sentences.
 */
 
#include <SPI.h>
#include <SD.h>

const double IMU_BAUD = 115200;
const double GPS_BAUD = 57600;

const int chipSelect = 4;


String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

void setup() {
  // initialize serial:
  Serial.begin(115200);
  Serial1.begin(GPS_BAUD);
  //Serial1.begin(IMU_BAUD);
  
  while (!Serial) {
    ; // wait for serial port to connect. DO NOT REMOVE!
  }
  
  Serial.println("Up and running BABY!");
  
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
}

void loop() {
  // print the string when a newline arrives:
  if (stringComplete) {
    Serial.println(inputString);
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
  
  while (Serial1.available()) {
    // get the new byte:
    char inChar = (char)Serial1.read();
    //Serial.println(inChar);
    
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
