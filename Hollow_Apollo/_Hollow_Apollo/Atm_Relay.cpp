#include "Atm_Relay.h"

/* Add optional parameters for the state machine to begin()
   Add extra initialization code
*/
#define RELAY_ON 0
#define RELAY_OFF 1

Atm_Relay& Atm_Relay::begin(int pin_left, int pin_right)  {
  // clang-format off
  const static state_t state_table[] PROGMEM = {
    /*                     ON_ENTER  ON_LOOP  ON_EXIT  EVT_TIMER_L  EVT_TIMER_R  EVT_TIMER_OFF    EVT_NEXT  EVT_HEAT_LEFT  EVT_HEAT_RIGHT   EVT_OFF  ELSE */
    /*  HEAT_LEFT */  ENT_HEAT_LEFT,      -1,      -1,  HEAT_RIGHT,          -1,            -1, HEAT_RIGHT,            -1,     HEAT_RIGHT, BOTH_OFF,   -1,
    /* HEAT_RIGHT */ ENT_HEAT_RIGHT,      -1,      -1,          -1,    BOTH_OFF,            -1,   BOTH_OFF,     HEAT_LEFT,             -1, BOTH_OFF,   -1,
    /*   BOTH_OFF */   ENT_BOTH_OFF,      -1,      -1,          -1,          -1,     HEAT_LEFT,  HEAT_LEFT,     HEAT_LEFT,     HEAT_RIGHT,       -1,   -1,
  };
  // clang-format on
  Machine::begin( state_table, ELSE );
  this->pin_left = pin_left; // Save the pins
  this->pin_right = pin_right;

  digitalWrite( pin_left, RELAY_OFF ); // Set the pin modes
  digitalWrite( pin_right, RELAY_OFF );

  pinMode( pin_left, OUTPUT ); // Set the pin modes
  pinMode( pin_right, OUTPUT );

  timer_l.set( -1 ); // Initialize the timers
  timer_r.set( -1 );
  timer_off.set( -1 );
  return *this;
}

/* Add C++ code for each internally handled event (input)
   The code must return 1 to trigger the event
*/

int Atm_Relay::event( int id ) {
  switch ( id ) {
    case EVT_TIMER_L:
      return timer_l.expired(this);
    case EVT_TIMER_R:
      return timer_r.expired(this);
    case EVT_TIMER_OFF:
      return timer_off.expired(this);
  }
  return 0;
}

/* Add C++ code for each action
   This generates the 'output' for the state machine

   Available connectors:
     push( connectors, ON_CHANGE, 0, <v>, <up> );
*/

void Atm_Relay::action( int id ) {
  switch ( id ) {
    case ENT_HEAT_LEFT:
      digitalWrite( pin_left, RELAY_ON );
      digitalWrite( pin_right, RELAY_OFF );
      push( connectors, ON_CHANGE, 0, 0, 0 );
      return;
    case ENT_HEAT_RIGHT:
      digitalWrite( pin_left, RELAY_OFF );
      digitalWrite( pin_right, RELAY_ON );
      push( connectors, ON_CHANGE, 0, 1, 0 );
      return;
    case ENT_BOTH_OFF:
      digitalWrite( pin_left, RELAY_OFF );
      digitalWrite( pin_right, RELAY_OFF );
      push( connectors, ON_CHANGE, 0, 2, 0 );
      return;
  }
}

Atm_Relay& Atm_Relay::automatic( unsigned long heat, unsigned long off ) {
  timer_l.set( heat );
  timer_r.set( heat );
  timer_off.set( off - heat ); // chambers are staggered. heat and off is in time per heater. off is an all off state.
  return *this;
}

/* Optionally override the default trigger() method
   Control how your machine processes triggers
*/

Atm_Relay& Atm_Relay::trigger( int event ) {
  Machine::trigger( event );
  return *this;
}

/* Optionally override the default state() method
   Control what the machine returns when another process requests its state
*/

int Atm_Relay::state( void ) {
  return Machine::state();
}

/* Nothing customizable below this line
 ************************************************************************************************
*/

/* Public event methods

Atm_Relay& Atm_Relay::next() {
  trigger( EVT_NEXT );
  return *this;
}

Atm_Relay& Atm_Relay::heat_left() {
  trigger( EVT_HEAT_LEFT );
  return *this;
}

Atm_Relay& Atm_Relay::heat_right() {
  trigger( EVT_HEAT_RIGHT );
  return *this;
}

Atm_Relay& Atm_Relay::off() {
  trigger( EVT_OFF );
  return *this;
}

/*
   onChange() push connector variants ( slots 1, autostore 0, broadcast 0 )
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
   Sets the symbol table and the default logging method for serial monitoring
*/

Atm_Relay& Atm_Relay::trace( Stream & stream ) {
  Machine::setTrace( &stream, atm_serial_debug::trace,
    "RELAY\0EVT_TIMER_L\0EVT_TIMER_R\0EVT_TIMER_OFF\0EVT_NEXT\0EVT_HEAT_LEFT\0EVT_HEAT_RIGHT\0EVT_OFF\0ELSE\0HEAT_LEFT\0HEAT_RIGHT\0BOTH_OFF" );
  return *this;
}
