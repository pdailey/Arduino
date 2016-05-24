// This example checks the state of the rotary encoder in the loop() function.
// The current position is printed on output when changed.

#include <RotaryEncoder.h>
// Setup a RoraryEncoder for pins A2 and A3:
RotaryEncoder encoder(A2, A3);

void setup() {
  Serial.begin(57600);
  Serial.println("SimplePollRotator example for the RotaryEncoder library.");
} // setup()


// Read the current position of the encoder and print out when changed.
void loop() {
  static int pos = 0;
  encoder.tick();

  int newPos = encoder.getPosition();
  if (pos != newPos) {
    Serial.println(newPos);
    pos = newPos;
  }
} // loop ()


