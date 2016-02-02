#include <Wire.h>

// Class primarily used for debugging
class Led {
    int ledPin;      // LED pin number

    // Constructor - creates an LED with an assigned pin
  public:
    Led(int pin) {
      ledPin = pin;
      pinMode(ledPin, OUTPUT);
      set(LOW); // default is OFF
    }

    // Turn the LED ON or OFF
    void set(int state) {
      digitalWrite(ledPin, state);
    }
};

// Set up the lights
Led ledLights(3); // yellow -> Will be a relay



void setup() {
 ledLights.set(HIGH);
}

uint8_t i = 0;
void loop() {

}



