#include "Atm_Relay.h"

/* Add optional parameters for the state machine to begin()
 * Add extra initialization code
 */

Atm_Relay& Atm_Relay::begin(int pin_c1, int pin_c2, int pin_h1, int pin_h2) {
  // clang-format off
  const static state_t state_table[] PROGMEM = {
    /*          ON_ENTER  ON_LOOP   ON_EXIT  EVT_HEAT  EVT_COOL  EVT_NEXT  EVT_TIMER_H  EVT_TIMER_C  ELSE */
    /*  COOL */ ENT_COOL,      -1, EXT_COOL,     HEAT,       -1,     HEAT,          -1,        HEAT,   -1,
    /*  HEAT */ ENT_HEAT,      -1, EXT_HEAT,       -1,     COOL,     COOL,        COOL,          -1,   -1,
  };
  // clang-format on
  Machine::begin( state_table, ELSE );
  this->pin_c1 = pin_c1; // Save the pins
  this->pin_c2 = pin_c2;
  this->pin_h1 = pin_h1;  
  this->pin_h2 = pin_h2;
  pinMode( pin_c1, OUTPUT ); // Set the pin modes
  pinMode( pin_c2, OUTPUT );
  pinMode( pin_h1, OUTPUT );
  pinMode( pin_h2, OUTPUT );
  timer_h.set( -1 ); // Initialize the timers
  timer_c.set( -1 );
  return *this;          
}

/* Add C++ code for each internally handled event (input) 
 * The code must return 1 to trigger the event
 */

int Atm_Relay::event( int id ) {
  switch ( id ) {
    case EVT_TIMER_H:
      return timer_h.expired( this );
    case EVT_TIMER_C:
      return timer_c.expired( this );
  }
  return 0;
}

/* Add C++ code for each action
 * This generates the 'output' for the state machine
 *
 * Available connectors:
 *   push( connectors, ON_CHANGE, 0, <v>, <up> );
 */

void Atm_Relay::action( int id ) {
  switch ( id ) {
    case ENT_COOL:
      digitalWrite( pin_c1, HIGH );
      digitalWrite( pin_c2, HIGH );
      push( connectors, ON_CHANGE, 0, 0, 0 );
      return;
    case EXT_COOL:
      digitalWrite( pin_c1, LOW );
      digitalWrite( pin_c2, LOW );
      return;
    case ENT_HEAT:
      digitalWrite( pin_h1, HIGH );
      digitalWrite( pin_h2, HIGH );
      push( connectors, ON_CHANGE, 0, 1, 0 );
      return;
    case EXT_HEAT:
      digitalWrite( pin_h1, LOW );
      digitalWrite( pin_h2, LOW );
      return;
  }
}

Atm_Relay& Atm_Relay::automatic( long c, long h ) {
  timer_c.set( c );
  timer_h.set( h );
  return *this;
}

/* Optionally override the default trigger() method
 * Control how your machine processes triggers
 */

Atm_Relay& Atm_Relay::trigger( int event ) {
  Machine::trigger( event );
  return *this;
}

/* Optionally override the default state() method
 * Control what the machine returns when another process requests its state
 */

int Atm_Relay::state( void ) {
  return Machine::state();
}

/* Nothing customizable below this line                          
 ************************************************************************************************
*/

/* Public event methods
 *
 */

Atm_Relay& Atm_Relay::heat() {
  trigger( EVT_HEAT );
  return *this;
}

Atm_Relay& Atm_Relay::cool() {
  trigger( EVT_COOL );
  return *this;
}

Atm_Relay& Atm_Relay::next() {
  trigger( EVT_NEXT );
  return *this;
}

/*
 * onChange() push connector variants ( slots 1, autostore 0, broadcast 0 )
 */

Atm_Relay& Atm_Relay::onChange( Machine& machine, int event ) {
  onPush( connectors, ON_CHANGE, 0, 1, 1, machine, event );
  return *this;
}

Atm_Relay& Atm_Relay::onChange( atm_cb_push_t callback, int idx ) {
  onPush( connectors, ON_CHANGE, 0, 1, 1, callback, idx );
  return *this;
}

/* State trace method
 * Sets the symbol table and the default logging method for serial monitoring
 */

Atm_Relay& Atm_Relay::trace( Stream & stream ) {
  Machine::setTrace( &stream, atm_serial_debug::trace,
    "RELAY\0EVT_HEAT\0EVT_COOL\0EVT_NEXT\0EVT_TIMER_H\0EVT_TIMER_C\0ELSE\0COOL\0HEAT" );
  return *this;
}



