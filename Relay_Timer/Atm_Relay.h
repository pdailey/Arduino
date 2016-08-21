#pragma once

#include <Automaton.h>

class Atm_Relay: public Machine {

 public:
  enum { COOL, HEAT }; // STATES
  enum { EVT_HEAT, EVT_COOL, EVT_NEXT, EVT_TIMER_H, EVT_TIMER_C, ELSE }; // EVENTS
  Atm_Relay( void ) : Machine() {};
  Atm_Relay& begin( int pin_c1, int pin_c2, int pin_h1, int pin_h2 );
  Atm_Relay& trace( Stream & stream );
  Atm_Relay& trigger( int event );
  int state( void );
  Atm_Relay& onChange( Machine& machine, int event = 0 );
  Atm_Relay& onChange( atm_cb_push_t callback, int idx = 0 );
  Atm_Relay& heat( void );
  Atm_Relay& cool( void );
  Atm_Relay& next( void );
  Atm_Relay& automatic( long c, long h );

 private:
  enum { ENT_COOL, EXT_COOL, ENT_HEAT, EXT_HEAT }; // ACTIONS
  enum { ON_CHANGE, CONN_MAX }; // CONNECTORS
  atm_connector connectors[CONN_MAX];
  int event( int id ); 
  void action( int id ); 
  int pin_c1, pin_c2, pin_h1, pin_h2;
  atm_timer_millis timer_c, timer_h;

};

/*
Automaton::ATML::begin - Automaton Markup Language

<?xml version="1.0" encoding="UTF-8"?>
<machines>
  <machine name="Atm_Relay">
    <states>
      <COOL index="0" on_enter="ENT_COOL" on_exit="EXT_COOL">
        <EVT_HEAT>HEAT</EVT_HEAT>
        <EVT_NEXT>HEAT</EVT_NEXT>
        <EVT_TIMER_C>HEAT</EVT_TIMER_C>
      </COOL>
      <HEAT index="1" on_enter="ENT_HEAT" on_exit="EXT_HEAT">
        <EVT_COOL>COOL</EVT_COOL>
        <EVT_NEXT>COOL</EVT_NEXT>
        <EVT_TIMER_H>COOL</EVT_TIMER_H>
      </HEAT>
    </states>
    <events>
      <EVT_HEAT index="0" access="PUBLIC"/>
      <EVT_COOL index="1" access="PUBLIC"/>
      <EVT_NEXT index="2" access="PUBLIC"/>
      <EVT_TIMER_H index="3" access="PRIVATE"/>
      <EVT_TIMER_C index="4" access="PRIVATE"/>
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

