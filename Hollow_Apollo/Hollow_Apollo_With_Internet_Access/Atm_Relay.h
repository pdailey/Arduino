#pragma once

#include <Automaton.h>

class Atm_Relay: public Machine {

 public:
  enum { HEAT_P, HEAT_V, OFF }; // STATES
  enum { EVT_HEAT_P, EVT_HEAT_V, EVT_OFF, EVT_TIMER_HP, EVT_TIMER_HV, EVT_TIMER_OFF, EVT_NEXT, ELSE }; // EVENTS
  Atm_Relay( void ) : Machine() {};
  Atm_Relay& begin( int pin_hp, int pin_hv);
  Atm_Relay& trace( Stream & stream );
  Atm_Relay& trigger( int event );
  int state( void );
  Atm_Relay& onChange( Machine& machine, int event = 0 );
  Atm_Relay& onChange( atm_cb_push_t callback, int idx = 0 );
  Atm_Relay& heat_p( void );
  Atm_Relay& heat_v( void );
  Atm_Relay& off( void );
  Atm_Relay& next( void );
  Atm_Relay& automatic( unsigned long c, unsigned long h );

 private:
  enum { ENT_HEAT_P, ENT_HEAT_V, ENT_OFF }; // ACTIONS
  enum { ON_CHANGE, CONN_MAX }; // CONNECTORS
  atm_connector connectors[CONN_MAX];
  int event( int id );
  void action( int id );
  int pin_hp, pin_hv;
  atm_timer_millis timer_off, timer_hp, timer_hv;
};

/*
Automaton::ATML::begin - Automaton Markup Language

<?xml version="1.0" encoding="UTF-8"?>
<machines>
  <machine name="Atm_Relay">
    <states>
      <HEAT_P index="0">
        <EVT_HEAT_V>HEAT_V</EVT_HEAT_V>
        <EVT_OFF>OFF</EVT_OFF>
        <EVT_TIMER_HP>HEAT_V</EVT_TIMER_HP>
        <EVT_NEXT>HEAT_V</EVT_NEXT>
      </HEAT_P>
      <HEAT_V index="1" on_enter="ENT_HEAT_V" on_exit="EXT_HEAT_V">
        <EVT_HEAT_P>HEAT_P</EVT_HEAT_P>
        <EVT_OFF>OFF</EVT_OFF>
        <EVT_TIMER_HV>OFF</EVT_TIMER_HV>
        <EVT_NEXT>OFF</EVT_NEXT>
      </HEAT_V>
      <OFF index="2" on_enter="ENT_OFF" on_exit="EXT_OFF">
        <EVT_HEAT_P>HEAT_P</EVT_HEAT_P>
        <EVT_HEAT_V>HEAT_V</EVT_HEAT_V>
        <EVT_TIMER_OFF>HEAT_P</EVT_TIMER_OFF>
        <EVT_NEXT>HEAT_P</EVT_NEXT>
      </OFF>
    </states>
    <events>
      <EVT_HEAT_P index="0" access="PUBLIC"/>
      <EVT_HEAT_V index="1" access="PUBLIC"/>
      <EVT_OFF index="2" access="PUBLIC"/>
      <EVT_TIMER_HP index="3" access="PRIVATE"/>
      <EVT_TIMER_HV index="4" access="PRIVATE"/>
      <EVT_TIMER_OFF index="5" access="PRIVATE"/>
      <EVT_NEXT index="6" access="PUBLIC"/>
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

