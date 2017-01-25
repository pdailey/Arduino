#define RELAY_ON 0
#define RELAY_OFF 1


void setup() {
  const byte pin [] = [9, 10, 11, 12];
  const byte no_pins = 4;

  for(int i = 0; i < no_pins; i++){
    pinMode(pin[i], OUTPUT); // Set the pin modes
    digitalWrite(pin[i], RELAY_OFF); // Set the pin modes
  }
}

void loop() {
  // Turn relays ON
  for(int i = 0; i < no_pins; i++){
    digitalWrite(pin[i], RELAY_ON); // Set the pin modes
    delay(10000);
  }
  // Turn relays OFF
  for(int i = 0; i < no_pins; i++){
    digitalWrite(pin[i], RELAY_OFF); // Set the pin modes
    delay(10000);
  }
}
