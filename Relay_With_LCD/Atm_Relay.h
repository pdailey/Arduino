#pragma once

#include <Automaton.h>

class Atm_Relay: public Machine {

 public:
  enum { HOLD, HEAT, COOL }; // STATES
  enum { EVT_BUTTON, EVT_HEAT, EVT_COOL, EVT_HOLD, EVT_NEXT, EVT_TIMER_H, EVT_TIMER_C, ELSE }; // EVENTS
  Atm_Relay( void ) : Machine() {};
  Atm_Relay& begin( int *pins );
  Atm_Relay& trace( Stream & stream );
  Atm_Relay& trigger( int event );
  int state( void );
  Atm_Relay& onChange( Machine& machine, int event = 0 );
  Atm_Relay& onChange( atm_cb_push_t callback, int idx = 0 );
  Atm_Relay& button( void );
  Atm_Relay& heat( void );
  Atm_Relay& cool( void );
  Atm_Relay& hold( void );
  Atm_Relay& next( void );
  Atm_Relay& automatic( unsigned long h, unsigned long c); 

 private:
  int cold_bath, hot_bath, inlet, outlet;
  atm_timer_millis timer_h, timer_c;
  enum { ENT_HOLD, ENT_HEAT, ENT_COOL }; // ACTIONS
  enum { ON_CHANGE, CONN_MAX }; // CONNECTORS
  atm_connector connectors[CONN_MAX];
  int event( int id ); 
  void action( int id ); 

};

/*
Automaton::ATML::begin - Automaton Markup Language

<?xml version="1.0" encoding="UTF-8"?>
<machines>
  <machine name="Atm_Relay">
    <states>
      <HOLD index="0">
        <EVT_BUTTON>HEAT</EVT_BUTTON>
        <EVT_HEAT>HEAT</EVT_HEAT>
        <EVT_COOL>COOL</EVT_COOL>
        <EVT_NEXT>HEAT</EVT_NEXT>
      </HOLD>
      <HEAT index="1">
        <EVT_BUTTON>HOLD</EVT_BUTTON>
        <EVT_COOL>COOL</EVT_COOL>
        <EVT_HOLD>HOLD</EVT_HOLD>
        <EVT_NEXT>COOL</EVT_NEXT>
        <EVT_TIMER_H>COOL</EVT_TIMER_H>
      </HEAT>
      <COOL index="2">
        <EVT_BUTTON>HOLD</EVT_BUTTON>
        <EVT_HEAT>HEAT</EVT_HEAT>
        <EVT_HOLD>HOLD</EVT_HOLD>
        <EVT_NEXT>HEAT</EVT_NEXT>
        <EVT_TIMER_C>HEAT</EVT_TIMER_C>
      </COOL>
    </states>
    <events>
      <EVT_BUTTON index="0" access="PUBLIC"/>
      <EVT_HEAT index="1" access="PUBLIC"/>
      <EVT_COOL index="2" access="PUBLIC"/>
      <EVT_HOLD index="3" access="PUBLIC"/>
      <EVT_NEXT index="4" access="PUBLIC"/>
      <EVT_TIMER_H index="5" access="PRIVATE"/>
      <EVT_TIMER_C index="6" access="PRIVATE"/>
    </events>
    <connectors>
      <CHANGE autostore="0" broadcast="0" dir="PUSH" slots="1"/>
    </connectors>
    <methods>
    </methods>
  </machine>
</machines>

Automaton::ATML::end
*/

