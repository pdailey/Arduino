#include <Automaton.h>

// Run a countdown timer for 70 seconds, display the remaining time on 
// the serial monitor and activate a relay when the timer finishes

Atm_timer countdown;
Atm_led relay;

void setup() {
  Serial.begin( 9600 );

  relay.begin( 4 ); // Led on pin 4

  countdown.begin( 1000 ) // Each step takes 1 second
    .repeat( 70 ) // Set to 70 seconds
    .onTimer( [] ( int idx, int v, int up ) {
      char buffer[50];
      sprintf( buffer, "Time left: %02d:%02d", v / 60, v % 60 );      
      Serial.println( buffer );
    })
    .onFinish( relay, relay.EVT_ON )
    .start();
}

void loop() {
  automaton.run();
}
