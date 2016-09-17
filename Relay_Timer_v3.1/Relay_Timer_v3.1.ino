/*
 * 6 Relay Automaton
 *
 * Triggers a 6 relay module for a set period fo time using the Automaton library.
 * Relay starts automatically, alternating between running 2 fans for 15 min, and 4 heaters for 30 min.
 *
 * Pins and cycle times can be configured within the sketch.
 *
 * Based off of this tutorial:
 * https://github.com/tinkerspy/Automaton/wiki/Machine-building-tutorial-2#adding-a-push-connector
 */

#include <Automaton.h>
#include "Atm_Relay.h"

// Basic Arduino sketch - instantiates the state machine and nothing else
Atm_Relay Relay;

// Time that the cycles will be on. Can handle 4,294,967,294 ms or ~50 Days
// time entered in ms
const unsigned long min_cool = 60000;
const unsigned long min_heat = 60000;

// Pins atached to the relay
const byte heat_pin_1 = 8;
const byte heat_pin_2 = 9;
const byte heat_pin_3 = 10;
const byte heat_pin_4 = 11;
const byte heat_pin_5 = 12;
const byte heat_pin_6 = 13;

void setup() {
  Serial.begin( 9600 );
  // Display changes in state over serial
  Relay.trace( Serial );

  // Assign the pins to the relays
  Relay.begin(heat_pin_1, heat_pin_2, heat_pin_3, heat_pin_4, heat_pin_5, heat_pin_6)
    // Set the time of the cooling and heating cycle. Cooling cycle runs first
    .automatic(min_cool, min_heat);
}

void loop() {
  automaton.run();
}


