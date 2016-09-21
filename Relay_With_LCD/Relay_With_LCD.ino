#include <Automaton.h>
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <Automaton_Adafruit_RGBLCDShield_Buttons.h>
#include "Atm_Relay.h"
#include "Atm_Menu.h"

Atm_Menu lcd_display;
Automaton_Adafruit_RGBLCDShield_Buttons lcd_buttons;
Atm_button button;
Atm_Relay relay;

int relay_pins [4] = {4, 5, 6, 7};
int btn_pin = 13;

void setup() {
  Serial.begin(9600);

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


