/*
 * Script used to clear EEPROM. Sets all of the bytes 
 * of the EEPROM to 0. Will turn on an LED when complete.
 */

#include <EEPROM.h>

void setup()
{
  Serial.begin(9600);
  pinMode(0, OUTPUT);
  digitalWrite(13, HIGH);

  EEPROM.begin(512);
  // write a 0 to all 512 bytes of the EEPROM
  for (int i = 0; i < 512; i++) {
    EEPROM.write(i, 0);
    Serial.println(i);
  }
  // turn the LED on when we're done
  digitalWrite(0, LOW);
  EEPROM.end();
}

void loop()
{
}
