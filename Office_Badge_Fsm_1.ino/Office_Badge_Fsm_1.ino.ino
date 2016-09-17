#include <Automaton.h>

Atm_step step;
Atm_fan fan;
Atm_button button;
Atm_led led_flash[4], led_solid[4];
Atm_timer countdown;

const int onTime = 200;
const int waitTime = 300;
// TODO
const int 15_min = 1000 * 15;

void setup() {
  Serial.begin( 9600 );

  // Button triggers a change in the step machine
  button.begin( 2 )
    .onPress(step, step.EVT_STEP )
    .debounce(10);


  // Define a timer
  stepTimer.begin( 1000 ) // Each step takes 1 second
    .repeat( 15 ) // Set to 70 seconds
    .onTimer( [] ( int idx, int v, int up ) {
      char buffer[50];
      sprintf( buffer, "Time left: %02d:%02d", v / 60, v % 60 );      
      Serial.println( buffer );
    })
    .onFinish( led[i], relay.EVT_ON );
  
  // Setup the step machine
  step.begin();
  
  step.trace( Serial );
  
  // Setup the leds
  for ( int i = 0; i < 4; i++ ) {
    led_flash[i].begin( i + 10 ).blink(onTime, waitTime);
    led_solid[i].begin( i + 10 );
  }

  




}

void loop() {
  automaton.run();
}
