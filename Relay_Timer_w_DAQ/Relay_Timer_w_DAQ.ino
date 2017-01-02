/*
 * Triggers a 2 relay module for a set period fo time using the Automaton library.
 */

#include <Automaton.h>
#include "Atm_Relay.h"
#include <SPI.h>
#include <SD.h>

Atm_Relay relay;
const unsigned long ms_cool = 5400000;
const unsigned long ms_heat =  900000;

// Pins atached to the relay
const byte relay_pin_1 = 3;
const byte relay_pin_2 = 4;

void setup() {
  Serial.begin( 9600 );

  // Assign the pins to the relays
  relay.begin(relay_pin_1, relay_pin_2)
    // Display changes in state over serial
    .trace( Serial )
    // Set the time of the cooling and heating cycle.
    .automatic(ms_cool, ms_heat)
    // Trigger the heating cycle to run first
    .trigger(relay.EVT_HEAT);
}

void loop() {
  automaton.run();
}


