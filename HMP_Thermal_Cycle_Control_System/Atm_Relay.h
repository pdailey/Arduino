#pragma once

#include <Automaton.h>

class Atm_Relay: public Machine {

 public:
  enum { HOLD, HEAT, COOL, MEASURE }; // STATES
  enum { EVT_BTN, EVT_L_BTN, EVT_HOLD, EVT_HEAT, EVT_COOL, EVT_MEASURE, EVT_NEXT, EVT_TIMER_H, EVT_TIMER_C, EVT_TIMER_M, ELSE }; // EVENTS
  Atm_Relay( void ) : Machine() {};
  Atm_Relay& begin( int *pins );
  Atm_Relay& trace( Stream & stream );
  Atm_Relay& trigger( int event );
  int state( void );
  Atm_Relay& onChange( Machine& machine, int event = 0 );
  Atm_Relay& onChange( atm_cb_push_t callback, int idx = 0 );
  Atm_Relay& btn( void );
  Atm_Relay& l_btn( void );
  Atm_Relay& heat( void );
  Atm_Relay& cool( void );
  Atm_Relay& hold( void );
  Atm_Relay& next( void );
  Atm_Relay& automatic( unsigned long h, unsigned long c, unsigned long m); 

 private:
  int cold_bath, hot_bath, inlet, outlet;
  atm_timer_millis timer_h, timer_c, timer_m;
  enum { ENT_HOLD, ENT_HEAT, ENT_COOL, ENT_MEASURE }; // ACTIONS
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
        <EVT_BTN>HEAT</EVT_BTN>
        <EVT_L_BTN>MEASURE</EVT_L_BTN>
        <EVT_HEAT>HEAT</EVT_HEAT>
        <EVT_COOL>COOL</EVT_COOL>
        <EVT_NEXT>HEAT</EVT_NEXT>
      </HOLD>
      <HEAT index="1">
        <EVT_BTN>HOLD</EVT_BTN>
        <EVT_L_BTN>MEASURE</EVT_L_BTN>
        <EVT_COOL>COOL</EVT_COOL>
        <EVT_HOLD>HOLD</EVT_HOLD>
        <EVT_NEXT>COOL</EVT_NEXT>
        <EVT_TIMER_H>COOL</EVT_TIMER_H>
      </HEAT>
      <COOL index="2">
        <EVT_BTN>HOLD</EVT_BTN>
        <EVT_L_BTN>MEASURE</EVT_L_BTN>
        <EVT_HEAT>HEAT</EVT_HEAT>
        <EVT_HOLD>HOLD</EVT_HOLD>
        <EVT_NEXT>HEAT</EVT_NEXT>
        <EVT_TIMER_C>HEAT</EVT_TIMER_C>
      </COOL>
      <MEASURE index="3">
        <EVT_BTN>COOL</EVT_BTN>
        <EVT_L_BTN>COOL</EVT_L_BTN>
        <EVT_HEAT>HEAT</EVT_HEAT>
        <EVT_COOL>COOL</EVT_COOL>
        <EVT_NEXT>COOL</EVT_NEXT>
        <EVT_TIMER_M>HOLD</EVT_TIMER_M>
      </MEASURE>
    </states>
    <events>
      <EVT_BTN index="0" access="MIXED"/>
      <EVT_L_BTN index="1" access="MIXED"/>
      <EVT_HEAT index="2" access="PUBLIC"/>
      <EVT_COOL index="3" access="PUBLIC"/>
      <EVT_HOLD index="4" access="PUBLIC"/>
      <EVT_NEXT index="5" access="PUBLIC"/>
      <EVT_TIMER_H index="6" access="PRIVATE"/>
      <EVT_TIMER_C index="7" access="PRIVATE"/>
      <EVT_TIMER_M index="8" access="PRIVATE"/>
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

