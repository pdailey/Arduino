#include "Atm_Relay.h"

#define RELAY_OFF 1
#define RELAY_ON 0
#define VALVE_DELAY 1000

/* Add optional parameters for the state machine to begin()
   Add extra initialization code
*/

Atm_Relay& Atm_Relay::begin(int * pins) {
  // clang-format off
  const static state_t state_table[] PROGMEM = {
    /*          ON_ENTER  ON_LOOP  ON_EXIT  EVT_BUTTON  EVT_HEAT  EVT_COOL  EVT_HOLD  EVT_NEXT  EVT_TIMER_H  EVT_TIMER_C  ELSE */
    /*  HOLD */ ENT_HOLD,      -1,      -1,       HEAT,     HEAT,     COOL,       -1,     HEAT,          -1,          -1,   -1,
    /*  HEAT */ ENT_HEAT,      -1,      -1,       HOLD,       -1,     COOL,     HOLD,     COOL,        COOL,          -1,   -1,
    /*  COOL */ ENT_COOL,      -1,      -1,       HOLD,     HEAT,       -1,     HOLD,     HEAT,          -1,        HEAT,   -1,
  };
  // clang-format on
  Machine::begin( state_table, ELSE );
  this->p1 = pins[0]; // Save the pins
  this->p2 = pins[1];
  this->p3 = pins[2];
  this->p4 = pins[3];
  pinMode( p1, OUTPUT ); // Set the pin modes
  pinMode( p2, OUTPUT );
  pinMode( p3, OUTPUT );
  pinMode( p4, OUTPUT );
  digitalWrite( p1, RELAY_OFF ); 
  digitalWrite( p2, RELAY_OFF );
  digitalWrite( p3, RELAY_OFF );
  digitalWrite( p4, RELAY_OFF );
  timer_h.set( -1 ); // set the timers
  timer_c.set( -1 );
  return *this;
}

/* Add C++ code for each internally handled event (input)
   The code must return 1 to trigger the event
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
   This generates the 'output' for the state machine

   Available connectors:
     push( connectors, ON_CHANGE, 0, <v>, <up> );
*/

void Atm_Relay::action( int id ) {
  switch ( id ) {
    case ENT_HOLD:
      digitalWrite( p1, RELAY_OFF );
      delay(VALVE_DELAY);
      digitalWrite( p2, RELAY_OFF );
      delay(VALVE_DELAY);
      digitalWrite( p3, RELAY_OFF );
      delay(VALVE_DELAY);
      digitalWrite( p4, RELAY_OFF );
      push( connectors, ON_CHANGE, 0, 0, 0 );
      return;
      
    case ENT_HEAT:
      digitalWrite( p1, RELAY_ON );
      delay(VALVE_DELAY);
      digitalWrite( p2, RELAY_OFF );
      delay(VALVE_DELAY);
      digitalWrite( p3, RELAY_OFF );
      delay(VALVE_DELAY);
      digitalWrite( p4, RELAY_OFF );
      push( connectors, ON_CHANGE, 0, 1, 0 );
      return;
      
    case ENT_COOL:
      digitalWrite( p1, RELAY_OFF );
      delay(VALVE_DELAY);
      digitalWrite( p2, RELAY_ON );
      delay(VALVE_DELAY);
      digitalWrite( p3, RELAY_ON );
      delay(VALVE_DELAY);
      digitalWrite( p4, RELAY_ON );
      push( connectors, ON_CHANGE, 0, 2, 0 );
      return;
  }
}

Atm_Relay& Atm_Relay::automatic( int h, int c) {
  timer_h.set( h );
  timer_c.set( c );
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

*/

Atm_Relay& Atm_Relay::button() {
  trigger( EVT_BUTTON );
  return *this;
}

Atm_Relay& Atm_Relay::heat() {
  trigger( EVT_HEAT );
  return *this;
}

Atm_Relay& Atm_Relay::cool() {
  trigger( EVT_COOL );
  return *this;
}

Atm_Relay& Atm_Relay::hold() {
  trigger( EVT_HOLD );
  return *this;
}

Atm_Relay& Atm_Relay::next() {
  trigger( EVT_NEXT );
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
                     "RELAY\0EVT_BUTTON\0EVT_HEAT\0EVT_COOL\0EVT_HOLD\0EVT_NEXT\0EVT_TIMER_H\0EVT_TIMER_C\0ELSE\0HOLD\0HEAT\0COOL" );
  return *this;
}



