#pragma once

#include <Automaton.h>
#include "pinChange.h"
#include <Adafruit_RGBLCDShield.h>
#include <Automaton_Adafruit_RGBLCDShield_Display.h>

class Atm_Menu : public Automaton_Adafruit_RGBLCDShield_Display {

  public:
  Atm_Menu( void ) : Automaton_Adafruit_RGBLCDShield_Display() {};

  // Machine states
  enum { OPT00, OPT10, OPT11, OPT20, OPT21, OPT111} STATES;
  // Menu actions
  enum { ACT00, ACT10, ACT11, ACT20, ACT21, ACT11L, ACT21L, ACT111, UPD } ACTIONS;
  atm_pin pin;
  

  Atm_Menu & begin( void );
  virtual void menu_action( int id );
};


