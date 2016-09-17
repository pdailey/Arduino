#include <Automaton.h>
#include "Atm_Menu.h"
#include "Atm_Relay.h"

extern int relay_pins[4];

Atm_Menu & Atm_Menu::begin( void )
{
  const static state_t state_table[] PROGMEM = {
    /*             ON_ENTER     ON_LOOP  ON_EXIT  EVT_SELECT  EVT_RIGHT  EVT_DOWN    EVT_UP   EVT_LEFT    ELSE */
    /* OPT00  */      ACT00,         -1,      -1,         -1,        -1,       -1,       -1,        -1,  OPT10, // Main menu title, jumps straight to OPT10
    /* OPT10  */      ACT10,        UPD,      -1,         -1,     OPT11,    OPT20,    OPT20,     OPT00,     -1, // First menu option
    /* OPT11  */      ACT11,     ACT11L,      -1,     OPT111,        -1,       -1,       -1,     OPT10,     -1, // View the system state
    /* OPT20  */      ACT20,        UPD,      -1,         -1,     OPT21,    OPT10,    OPT10,     OPT00,     -1, // 2nd Menu option
    /* OPT21  */      ACT21,     ACT21L,      -1,         -1,        -1,       -1,       -1,     OPT20,     -1, // View individual relay status
    /* OPT111 */     ACT111,         -1,      -1,         -1,        -1,       -1,       -1,        -1,  OPT11, // Actions without corresponding menu options
  };
  Machine::begin( state_table, ELSE );
  const static char menu_definition[] PROGMEM =
    /*            1234567890123456 */
    /* ACT00 */  "00 Main menu    "
    /* ACT10 */  "10 System State>"
    /* ACT11 */  "????            "
    /* ACT20 */  "20 Relay Status>"
    /* ACT21 */  "L1? L2? L3? L4? ";
  menu( menu_definition, ACT00, ACT21, 3, 12 );
  return *this;
}

void Atm_Menu::menu_action( int id )
{
  switch ( id ) {

    case UPD :
      if ( updateDisplay( 1 ) == 0 ) sleep( 1 );
      return;
    case ACT11L :
    {
      // Print the current system state
      int n = 0; 
      for (int i = 0; i < 4; i++) {
        n += !digitalRead( relay_pins[i] );
      }
      if (n == 0) printXY( 0, 1, "HOLD               ");
      if (n == 4) printXY( 0, 1, "N HOLD             ");
      if (n == 1) printXY( 0, 1, "HEAT               ");
      if (n == 3) printXY( 0, 1, "COOL               ");
      updateDisplay( 1 );
      return;
    }
      return;
    case ACT11 :
    {
      // Print the current system state
      int n = 0; 
      for (int i = 0; i < 4; i++) {
        n += !digitalRead( relay_pins[i] );
      }
      if (n == 0) printXY( 0, 1, "HOLD               ");
      else if (n == 1) printXY( 0, 1, "HEAT               ");
      else if (n == 3) printXY( 0, 1, "COOL               ");
      updateDisplay( 1 );
      return;
    }
    case ACT21L :
      if ( pin.change( relay_pins[0] ) ) printXY(  2, 1, !digitalRead( relay_pins[0] ) ? "+" : " " ); // Dependency on pin use!!!
      if ( pin.change( relay_pins[1] ) ) printXY(  6, 1, !digitalRead( relay_pins[1] ) ? "+" : " " );
      if ( pin.change( relay_pins[2] ) ) printXY( 10, 1, !digitalRead( relay_pins[2] ) ? "+" : " " );
      if ( pin.change( relay_pins[3] ) ) printXY( 14, 1, !digitalRead( relay_pins[3] ) ? "+" : " " );
      updateDisplay( 1 );
      return;

    case ACT21 :
      printXY(  2, 1, digitalRead( relay_pins[0] ) ? "+" : " " );
      printXY(  6, 1, digitalRead( relay_pins[1] ) ? "+" : " " );
      printXY( 10, 1, digitalRead( relay_pins[2] ) ? "+" : " " );
      printXY( 14, 1, digitalRead( relay_pins[3] ) ? "+" : " " );
      return;
  }
}
