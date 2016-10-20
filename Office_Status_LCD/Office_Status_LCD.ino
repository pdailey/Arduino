#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>


Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
char* locations[] = {
  "IN      ", 
  "OUT     ", 
  "APN     ", 
  "VACATION", 
  "OTHER   "
};
  
int no_locations = 4;
int pos = 1;

void setup() {
  // Debugging output
  Serial.begin(9600);
  
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  // Print a message to the LCD.
  displayHeader();
  displayFooter(pos);
}


void loop() {
  uint8_t buttons = lcd.readButtons();
  if (buttons) {
    pos++;
    displayFooter(pos);
  }
  
  delay(100);
}

void displayHeader() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Peter Dailey");
}

void displayFooter(int pos) {
  lcd.setCursor(0, 1);
  lcd.print(locations[pos % no_locations]);
}
