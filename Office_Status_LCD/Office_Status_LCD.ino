/*********************

  Example code for the Adafruit RGB Character LCD Shield and Library

  This code displays text on the shield, and also reads the buttons on the keypad.
  When a button is pressed, the backlight changes color.

**********************/

// include the library code:
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>


Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
char* locations[] = {"IN", "OUT", "APN", "OTHER"};
int no_locations = 4;

int pos = -1;

void setup() {
  // Debugging output
  Serial.begin(9600);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  // Print a message to the LCD.
  lcd.print("Peter Dailey");
}


void loop() {
  uint8_t buttons = lcd.readButtons();
  delay(100);
  if (buttons) {
    pos++;
    displayHeader();
    displayFooter(pos);
  }
}

void displayHeader() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Peter Dailey");
}

void displayFooter(int pos) {
  lcd.setCursor(0, 1);
  lcd.setCursor(0, 1);
  lcd.print(locations[pos % no_locations]);
}
