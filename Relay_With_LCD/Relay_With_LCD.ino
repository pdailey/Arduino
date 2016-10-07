#include <Automaton.h>
#include <Wire.h>
#include <EEPROM.h>
#include <Adafruit_RGBLCDShield.h>
#include <Automaton_Adafruit_RGBLCDShield_Buttons.h>
#include "Atm_Relay.h"
#include "Atm_Menu.h"

// State Machines
Atm_Menu lcd_display;
Automaton_Adafruit_RGBLCDShield_Buttons lcd_buttons;
Atm_button state_btn, reset_btn;
Atm_digital counter_pulse;
Atm_Relay relay;

// Pin Variables
int relay_pins [4] = {4, 5, 6, 7};
int state_pin = 13;
int reset_pin = 12;

// EEPROM Variables
// EEPROM is memory whose values are kept when the board is turned off 
unsigned int count;   // Cycle count variable to store in EEPROM
int address = 0;      // EEPROM address of count variable.

// Timer Variables
unsigned long ms_heat = 10000; // Duration of heating cycle in ms
unsigned long ms_cool = 10000; // Duration of cooling cycle in ms

// TODO: jump to ground for eprom


void update_cycle_counter( int idx, int v, int up ) {
  count++;

  // Write to EEPROM every 10 cycles. 
  // According to the manufacturer Atmel, the EEPROM is good for 100,000 write/erase cycles, 
  // thus we increase the lifespan of the device tenfold with this little optimization. 
  if (count % 10 == 0) {
    EEPROM.put(address, count);
  }

  // Update the display
  char cycles[5];
  sprintf(cycles, "%05d", count);
  lcd_display.printXY(11, 0, cycles);
}

void reset_cycle_counter( int idx, int v, int up ) {
  count = 0;
  EEPROM.put(address, count);

  // Update the display
  char cycles[5];
  sprintf(cycles, "%05d", count);
  lcd_display.printXY(11, 0, cycles);
}


void setup() {
  Serial.begin(9600);

  // Retreive the cycle count from EEPROM. EEPROM saves variables between power cycles.
  EEPROM.get(address, count);

  // triggers the counter after given number of ms in a certain state. relay_pin[1] only goes HIGH in a cooling cycle, 
  // which indicates that a heating cycle has also occured, thus the system has finished one complete heat/cool cycle and the cycle count needs to be updated.
  // When the counter is triggered, the count variable is updated, reflecting the number of cycles the test has undergone.
  // Relay initially set to high on all pins, triggers our counter. As a result, the counter is initially off by one...
  counter_pulse.begin( relay_pins[1], 10000)
    .onChange(HIGH, update_cycle_counter);

  // Button to cycle the system between hold, heating and cooling. 
  state_btn.begin(state_pin)
    .onPress( relay, relay.EVT_BUTTON );

  // Button to reset the number of cycles
  reset_btn.begin(reset_pin)
    .onPress( reset_cycle_counter );

  // The relays set the states of the valves, thus the state of the system. 
  relay.begin(relay_pins)
    // Set the duration of the heat and cool cycle.
    .automatic(ms_heat, ms_cool)
    // Set to HOLD state.
    .trigger(relay.EVT_HOLD)
    .trace(Serial);
  
  // Setup the LCD display. Single window display shows the state of the machine, which relays are on, and the cycle count. 
  lcd_display.begin();
  lcd_buttons.begin( lcd_display ); // Link the buttons to the display. TODO: likely unnecessary...
  delay(1000);
  // ...and fix the off-by-one error encountered on startup
  count--;
}

void loop() {
  automaton.run();
}


