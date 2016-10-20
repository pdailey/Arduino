#include <Automaton.h>

Atm_fade led1, led2;

void setup() {
  led1.begin( 3 ).blink( 2000, 2000 ).fade(200);  // Set up
  led2.begin( 9 ).blink( 2000, 2000 );
  led1.trigger( led1.EVT_BLINK );              // Start fading
  led2.trigger( led2.EVT_BLINK );
}

void loop() {
  automaton.run();
}
