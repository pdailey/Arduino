#include "Atm_Relay.h"

/* Add optional parameters for the state machine to begin()
   Add extra initialization code
*/
#define RELAY_ON 0
#define RELAY_OFF 1

Atm_Relay& Atm_Relay::begin(int pin_hp, int pin_hv)  {
  // clang-format off
  const static state_t state_table[] PROGMEM = {
    /*             ON_ENTER  ON_LOOP  ON_EXIT  EVT_HEAT_P  EVT_HEAT_V  EVT_OFF  EVT_TIMER_HP  EVT_TIMER_HV  EVT_TIMER_OFF  EVT_NEXT  ELSE */
    /* HEAT_P */ ENT_HEAT_P,      -1,      -1,         -1,     HEAT_V,     OFF,       HEAT_V,           -1,            -1,   HEAT_V,   -1,
    /* HEAT_V */ ENT_HEAT_V,      -1,      -1,     HEAT_P,         -1,     OFF,           -1,          OFF,            -1,      OFF,   -1,
    /*    OFF */    ENT_OFF,      -1,      -1,     HEAT_P,     HEAT_V,      -1,           -1,           -1,        HEAT_P,   HEAT_P,   -1,
  };
  // clang-format on
  Machine::begin( state_table, ELSE );
  this->pin_hp = pin_hp; // Save the pins
  this->pin_hv = pin_hv;

  digitalWrite( pin_hp, RELAY_OFF ); // Set the pin modes
  digitalWrite( pin_hv, RELAY_OFF );

  pinMode( pin_hp, OUTPUT ); // Set the pin modes
  pinMode( pin_hv, OUTPUT );

  timer_hp.set( -1 ); // Initialize the timers
  timer_hv.set( -1 );
  timer_off.set( -1 );
  return *this;
}

/* Add C++ code for each internally handled event (input)
   The code must return 1 to trigger the event
*/

int Atm_Relay::event( int id ) {
  switch ( id ) {
    case EVT_TIMER_HP:
      return timer_hp.expired(this);
    case EVT_TIMER_HV:
      return timer_hv.expired(this);
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
    case ENT_HEAT_P:
      digitalWrite( pin_hp, RELAY_ON );
      digitalWrite( pin_hv, RELAY_OFF );
      return;
    case ENT_HEAT_V:
      digitalWrite( pin_hp, RELAY_OFF );
      digitalWrite( pin_hv, RELAY_ON );
      return;
    case ENT_OFF:
      digitalWrite( pin_hp, RELAY_OFF );
      digitalWrite( pin_hv, RELAY_OFF );
      return;
  }
}

Atm_Relay& Atm_Relay::automatic( unsigned long h, unsigned long o ) {
  timer_hp.set( h );
  timer_hv.set( h );
  timer_off.set( o );
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

Atm_Relay& Atm_Relay::heat_p() {
  trigger( EVT_HEAT_P );
  return *this;
}

Atm_Relay& Atm_Relay::heat_v() {
  trigger( EVT_HEAT_V );
  return *this;
}

Atm_Relay& Atm_Relay::off() {
  trigger( EVT_OFF );
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
                     "RELAY\0EVT_HEAT_P\0EVT_HEAT_V\0EVT_OFF\0EVT_TIMER_HP\0EVT_TIMER_HV\0EVT_TIMER_OFF\0EVT_NEXT\0ELSE\0HEAT_P\0HEAT_V\0OFF" );
  return *this;
}




