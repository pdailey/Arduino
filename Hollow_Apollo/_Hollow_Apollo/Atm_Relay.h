#pragma once

#include <Automaton.h>

class Atm_Relay: public Machine {

 public:
  enum { HEAT_LEFT, HEAT_RIGHT, BOTH_OFF }; // STATES
  enum { EVT_TIMER_L, EVT_TIMER_R, EVT_TIMER_OFF, EVT_NEXT, EVT_HEAT_LEFT, EVT_HEAT_RIGHT, EVT_OFF, ELSE }; // EVENTS
  Atm_Relay( void ) : Machine() {};
  Atm_Relay& begin( int pin_left, int pin_right);
  Atm_Relay& trace( Stream & stream );
  Atm_Relay& trigger( int event );
  int state( void );
  Atm_Relay& automatic( unsigned long heat, unsigned long off );
  Atm_Relay& onChange( Machine& machine, int event = 0 );
  Atm_Relay& onChange( atm_cb_push_t callback, int idx = 0 );
  Atm_Relay& next( void );
  Atm_Relay& heat_left( void );
  Atm_Relay& heat_right( void );
  Atm_Relay& off( void );

 private:
  enum { ENT_HEAT_LEFT, ENT_HEAT_RIGHT, ENT_BOTH_OFF }; // ACTIONS
  enum { ON_CHANGE, CONN_MAX }; // CONNECTORS
  atm_connector connectors[CONN_MAX];
  int event( int id );
  void action( int id );
  int pin_left, pin_right;
  atm_timer_millis timer_off, timer_l, timer_r;
};

/* 
Automaton::ATML::begin - Automaton Markup Language

<?xml version="1.0" encoding="UTF-8"?>
<machines>
  <machine name="Atm_Relay">
    <states>
      <HEAT_LEFT index="0" on_enter="ENT_HEAT_LEFT">
        <EVT_TIMER_L>HEAT_RIGHT</EVT_TIMER_L>
        <EVT_NEXT>HEAT_RIGHT</EVT_NEXT>
        <EVT_HEAT_RIGHT>HEAT_RIGHT</EVT_HEAT_RIGHT>
        <EVT_OFF>BOTH_OFF</EVT_OFF>
      </HEAT_LEFT>
      <HEAT_RIGHT index="1" on_enter="ENT_HEAT_RIGHT">
        <EVT_TIMER_R>BOTH_OFF</EVT_TIMER_R>
        <EVT_NEXT>BOTH_OFF</EVT_NEXT>
        <EVT_HEAT_LEFT>HEAT_LEFT</EVT_HEAT_LEFT>
        <EVT_OFF>BOTH_OFF</EVT_OFF>
      </HEAT_RIGHT>
      <BOTH_OFF index="2" on_enter="ENT_BOTH_OFF">
        <EVT_TIMER_OFF>HEAT_LEFT</EVT_TIMER_OFF>
        <EVT_NEXT>HEAT_LEFT</EVT_NEXT>
        <EVT_HEAT_LEFT>HEAT_LEFT</EVT_HEAT_LEFT>
        <EVT_HEAT_RIGHT>HEAT_RIGHT</EVT_HEAT_RIGHT>
      </BOTH_OFF>
    </states>
    <events>
      <EVT_TIMER_L index="0" access="PRIVATE"/>
      <EVT_TIMER_R index="1" access="PRIVATE"/>
      <EVT_TIMER_OFF index="2" access="PRIVATE"/>
      <EVT_NEXT index="3" access="PUBLIC"/>
      <EVT_HEAT_LEFT index="4" access="PUBLIC"/>
      <EVT_HEAT_RIGHT index="5" access="PUBLIC"/>
      <EVT_OFF index="6" access="PUBLIC"/>
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
