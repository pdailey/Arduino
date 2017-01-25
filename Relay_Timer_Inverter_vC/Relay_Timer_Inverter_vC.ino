#include <Automaton.h>
#include "Atm_Relay.h"

// Relays
Atm_Relay relay;
// time entered in ms
//const unsigned long ms_off  = 4500000;
//const unsigned long ms_heat = 900000;
// TODO: times will change
const unsigned long ms_off  = 15000;
const unsigned long ms_heat =  5000;
// Pins atached to the relay
const byte pin_p = 0;
const byte pin_v = 16;

// timers
Atm_timer sensor_timer;

void setup() {
  Serial.begin( 9600 );

  setupRelays();
  setupTimers();
}

void loop() {
  automaton.run();
}


bool setupTimers() {
  Serial.print("\tInitializing Timers...");
  // Assign the pins to the relays
  sensor_timer.interval_seconds(300) // 5 minutes
  .onTimer( collectSensorData )
  .repeat( ATM_TIMER_OFF )  // Set timer not to expire at all
  .trace( Serial );


  Serial.print("timers initialized.\n");
  return true;
}

bool setupRelays() {
  Serial.print("\tInitializing Relays...");
  // Assign the pins to the relays
  relay.begin(pin_p, pin_v)
  // Display changes in state over serial
  .trace( Serial )
  // Set the time of the cooling and heating cycle.
  .automatic(ms_heat, ms_off)
  // Trigger the heating cycle to run first
  .trigger(relay.EVT_HEAT_P);

  Serial.print("relays initialized.\n");
  return true;
}

void collectSensorData(int idx, int v, int up) {
  // read the sensor data and save it to a string
  String str = "hjl";
  // save the data to the SD
  //writeStringToSD(str, file_name);
}


