#include <Automaton.h>

Atm_step step;
Atm_fan fan;
Atm_button button;
Atm_led led[4];
Atm_timer countdown;

void setup() {
  Serial.begin( 9600 );
  step.trace( Serial );

  // Setup the step machine
  step.begin() 
    .onStep( []( int idx, int v, int up ) {        
      
      // TODO: really noobish, but turns off the light 
      Serial.print( step.state() );
      if(step.state() == 0) led[3].trigger(led[3].EVT_OFF);
      if(step.state() == 1) led[0].trigger(led[0].EVT_OFF);
      if(step.state() == 2) led[1].trigger(led[1].EVT_OFF);
      if(step.state() == 3) led[2].trigger(led[2].EVT_OFF);
    });

  
  for ( int i = 0; i < 4; i++ ) {
    led[i].begin( i + 9 );
    step.onStep( i, led[i], led[i].EVT_ON );
  }
  
  // Button triggers a change in the step machine
  button.begin( 3 )
    .onPress(step, step.EVT_STEP )
    .debounce(10);
  
}

void loop() {
  automaton.run();
}
