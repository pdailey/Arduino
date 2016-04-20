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

const int chipSelect = 4;


String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

void setup() {
  // initialize serial:
  Serial1.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. DO NOT REMOVE!
  }
  
  
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

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
 // SerialEvent does not work on 32u4 boards!
/*void serialEvent() {
  while (Serial1.available()) {
    Serial1.print("SerialEvent!");
    // get the new byte:
    char inChar = (char)Serial1.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}*/
