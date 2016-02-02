#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

Adafruit_7segment disp = Adafruit_7segment(); // Sets up the 7-segment display

void setup() {
  disp.begin(0x70); // Initialize the 7-segment display
}

void loop() {
  // print a floating point 
  disp.print(25);
  disp.writeDisplay();
  delay(1000);
  disp.writeDisplay();
  Bean.sleep(10000);
}
