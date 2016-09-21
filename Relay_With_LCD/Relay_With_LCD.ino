#include <Automaton.h>
#include <Wire.h>
#include <EEPROM.h>
#include <Adafruit_RGBLCDShield.h>
#include <Automaton_Adafruit_RGBLCDShield_Buttons.h>
#include "Atm_Relay.h"
#include "Atm_Menu.h"

Atm_Menu lcd_display;
Automaton_Adafruit_RGBLCDShield_Buttons lcd_buttons;
Atm_button button;
Atm_digital counter_pulse;
Atm_Relay relay;


int relay_pins [4] = {4, 5, 6, 7};
int btn_pin = 13;
unsigned int count;   //Variable to store data read from EEPROM.
int address = 0; //EEPROM address to start reading from



// Do something when a pulse of at least 20 ms is detected on pin A0
void update_counter( int idx, int v, int up ) {
  char temp[5];
  count++;
  EEPROM.put(address, count);
  sprintf(temp,"%05d",count);
  lcd_display.printXY(11, 0, temp);
}

void setup() {
  Serial.begin(9600);
  
  EEPROM.get(address, count);

  // triggers the counter after given number of ms
  counter_pulse.begin( relay_pins[1], 1000)
    .onChange(HIGH, update_counter);

  button.begin(btn_pin)
    .onPress( relay, relay.EVT_BUTTON);

  relay.begin(relay_pins)
    .automatic(10000, 10000)
    .trigger(relay.EVT_HOLD)
    .trace(Serial);

  lcd_display.begin();
  lcd_buttons.begin( lcd_display ); // Link the buttons to the display


}

void loop() {
  automaton.run();
}


