#include <Automaton.h>
#include "Atm_Menu.h"
#include "Atm_Relay.h"

// TODO: Could be optimized, as no menu is needed....

extern int relay_pins[4];

Atm_Menu & Atm_Menu::begin( void )
{
  const static state_t state_table[] PROGMEM = {
    /*             ON_ENTER     ON_LOOP  ON_EXIT  EVT_SELECT  EVT_RIGHT  EVT_DOWN    EVT_UP   EVT_LEFT    ELSE */
    /* OPT00  */      ACT00,         -1,      -1,         -1,        -1,       -1,       -1,        -1,  OPT10, // Main title, jumps straight to OPT10
    /* OPT10  */      ACT10,         -1,      -1,         -1,        -1,       -1,       -1,        -1,  OPT11, // Main subtitle, jumps straight to OPT11
    /* OPT11  */      ACT11,     ACT11L,      -1,         -1,        -1,       -1,       -1,        -1,     -1, // View the system state
  };
  Machine::begin( state_table, ELSE );
  const static char menu_definition[] PROGMEM =
    // ? marks will be replaced by variables that are updated.
    /*            0123456789012345 */
    /* ACT00 */  "00 LOADING...   "
    /* ACT10 */  "10 SYS:????   -----"
    /* ACT11 */  "CB? HB? IN? OUT?";

  menu( menu_definition, ACT00, ACT11, 4, 12 );
  printXY(0,0, "SYS:????   ?????");
  printXY(0,1, "CB? HB? IN? OUT?");
  //updateDisplay();
  return *this;
  
}

void Atm_Menu::menu_action( int id )
{ 
  switch ( id ) {
    case UPD:
      if ( updateDisplay(1) == 0 ) sleep(1);
      return;

    case ACT11L :
      {

        // Top screen
        byte n = 0;
        for (int i = 0; i < 4; i++) {
          n += !digitalRead( relay_pins[i] );
        }
        
        if      (n == 0) printXY( 4, 0, "HOLD");
        else if (n == 3) printXY( 4, 0, "HEAT");
        else if (n == 1) printXY( 4, 0, "COOL");
        else             printXY( 4, 0, "ERR ");

        //Bottom Screen:
        if ( pin.change( relay_pins[0] ) ) printXY(  2, 1, !digitalRead( relay_pins[0] ) ? "+" : " " );
        if ( pin.change( relay_pins[1] ) ) printXY(  6, 1, !digitalRead( relay_pins[1] ) ? "+" : " " );
        if ( pin.change( relay_pins[2] ) ) printXY( 10, 1, !digitalRead( relay_pins[2] ) ? "+" : " " );
        if ( pin.change( relay_pins[3] ) ) printXY( 15, 1, !digitalRead( relay_pins[3] ) ? "+" : " " );
        updateDisplay(4);
        
        return;
      }
    case ACT11 :
      {
        // TOP Screen:
        int n = 0;
        for (int i = 0; i < 4; i++) {
          n += !digitalRead( relay_pins[i] );
        }

        if      (n == 0) printXY( 4, 0, "HOLD");
        else if (n == 3) printXY( 4, 0, "HEAT");
        else if (n == 1) printXY( 4, 0, "COOL");
        else             printXY( 4, 0, "ERR ");
        
        // Bottom Screen:
        printXY(  2, 1, digitalRead( relay_pins[0] ) ? "+" : " " );
        printXY(  6, 1, digitalRead( relay_pins[1] ) ? "+" : " " );
        printXY( 10, 1, digitalRead( relay_pins[2] ) ? "+" : " " );
        printXY( 15, 1, digitalRead( relay_pins[3] ) ? "+" : " " );

        updateDisplay(4);
        return;
      }
  }
}
