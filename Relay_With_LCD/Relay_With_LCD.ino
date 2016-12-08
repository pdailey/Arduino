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
unsigned long ms_heat = 65000; // Duration of heating cycle in ms
unsigned long ms_cool = 80000; // Duration of cooling cycle in ms

// Button and trigger variables
unsigned long ms_pulse = 30000;  // Duration system must be cooling before a cycle is triggered
unsigned long ms_debounce = 500; // Duration a button must be pressed before it is recognized

void update_display_count(int count){
  // reset cycle count in case of interference
  lcd_display.printXY(11, 0, "-----");
  lcd_display.updateDisplay(5);
  
  char cycles[5];
  sprintf(cycles, "%05d", count);
  lcd_display.printXY(11, 0, cycles);
  lcd_display.updateDisplay(5);
}

void update_cycle_counter( int idx, int v, int up ) {
  count++;

  // Write to EEPROM every 10 cycles. 
  // According to the manufacturer Atmel, the EEPROM is good for 100,000 write/erase cycles, 
  // thus we increase the lifespan of the device tenfold with this little optimization. 
  if (count % 10 == 0) {
    EEPROM.put(address, count);
  }
  update_display_count(count);
}

void reset_cycle_counter( int idx, int v, int up ) {
  count = 0;
  EEPROM.put(address, count);
  update_display_count(count);
}


void setup() {
  Serial.begin(9600);

  // Retrieve the cycle count from EEPROM. EEPROM saves variables between power cycles.
  EEPROM.get(address, count);

  // triggers the counter after given number of ms in a certain state. relay_pin[1] only goes HIGH in a cooling cycle, 
  // which indicates that a heating cycle has also occurred, thus the system has finished one complete heat/cool cycle and the cycle count needs to be updated.
  // When the counter is triggered, the count variable is updated, reflecting the number of cycles the test has undergone.
  // Relay initially set to high on all pins, triggers our counter.
  counter_pulse.begin( relay_pins[0], ms_pulse)
    .onChange(LOW, update_cycle_counter);

  // Button to cycle the system between hold, heating and cooling. 
  // Occasionally the system needs to set in the heating position for measurement purposes. 
  // 0.5 second press and release - cycle between states
  // 3 second press and release - hold in heating state
  state_btn.begin( state_pin )
    .onPress( [] ( int idx, int v, int up ) {
      switch ( v ) {
        case 1:
          return; // debounce, prevent unwanted switching
        case 2:
        case 3:
        case 4:
        case 5:
          relay.trigger( relay.EVT_BTN );      // cycle between states
          update_display_count(count); 
          return;
        case 6:
          relay.trigger( relay.EVT_L_BTN ); // enter measuring heating period
          lcd_display.printXY(11, 0, ".....");
          lcd_display.updateDisplay(5);
          return;
      }
    })
    // 6 cases (really 2), 500 ms debounce
    .longPress( 6, 500 );
  
  // Button to reset the number of cycles
  reset_btn.begin(reset_pin)
    .debounce( ms_debounce )
    .onPress( reset_cycle_counter );

  // The relays set the states of the valves, thus the state of the system. 
  relay.begin(relay_pins)
    // Set the duration of the heat and cool cycle.
    .automatic(ms_heat, ms_cool, ms_measure)
    // Set to HOLD state.
    .trigger(relay.EVT_HOLD)
    .trace(Serial);
  
  // Setup the LCD display. Single window display shows the state of the machine, which relays are on, and the cycle count. 
  lcd_display.begin();
  lcd_buttons.begin( lcd_display ); // Link the buttons to the display. TODO: likely unnecessary...
  update_display_count(count);  
}

void loop() {
  automaton.run();
}


