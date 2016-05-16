/*
 * ============================================================================================
 * ============================================================================================
 * ============================================================================================
 */




/* ============================================================================
 *                             GLOBAL VARIABLES!
 *                                  (/^▽^)/
 * ===========================================================================*/
#include <RotaryEncoder.h>
#include <Button.h>
#include "Led.h"

bool debugFlag = 0;

// RoraryEncoder for pins A2 and A3:
RotaryEncoder encoder(A2, A3);


// Push button on encoder
const byte BUTTON_PIN = 13;       //Connect a tactile button switch (or something similar)
                                  //from Arduino pin 13 to ground.
const bool PULLUP = true;        //To keep things simple, we use the Arduino's internal pullup resistor.
const bool INVERT = true;        //Since the pullup resistor will keep the pin high unless the
                                 //switch is closed, this is negative logic, i.e. a high state
                                 //means the button is NOT pressed. (Assuming a normally open switch.)
const byte DEBOUNCE_MS = 20;     //A debounce time of 20 milliseconds usually works well for tactile button switches.

Button button(BUTTON_PIN, PULLUP, INVERT, DEBOUNCE_MS);    //Declare the button

// LED Bar Graph
const byte numberOfLeds = 10;
//byte ledPinArray[numberOfblinkers] = {2, 3, 4, 5, 6,
//                                        7, 8, 9,10,11};

Led leds[numberOfLeds] = {Led(2), Led(3), Led(4), Led(5),  Led(6), \
                          Led(7), Led(8), Led(9), Led(10), Led(12)};

// State Variables
byte current_led = 0;
bool invert_leds = false;

// Serial Communication Variables
const unsigned int BAUD = 57600;


/* ============================================================================
 *                                   SETUP!
 *                               (ﾉ◕ヮ◕)ﾉ*:･ﾟ✧
 * ==========================================================================*/
void setup() {
  // USB and Board Serial
  Serial.begin(BAUD);
  
  // Debugging: Arduino will not run without USB Serial connected
  //while (!Serial) {;}
}


/* ============================================================================
 *                                    LOOP!
 *                                   ᕕ( ᐛ )ᕗ
 * ==========================================================================*/
void loop() {
  static int pos = 0;
  encoder.tick();
  button.read(); 

  // Button Logic
  if (button.wasReleased()) {       //If the button was released, change the LED state
    
    // switch the button pressed flag
    invert_leds = !invert_leds;
    Serial.println("TRIGGERED!!!");
  }

  // Encoder Logic
  int newPos = encoder.getPosition();
  
  // update position
  if (pos != newPos) {
    // Handle edge cases
    // already at first led
    if(newPos <= 0){
      encoder.setPosition(0);
      newPos = 0;
    // already last led
    } else if (newPos >= numberOfLeds - 1){
      encoder.setPosition(numberOfLeds - 1);
      newPos = numberOfLeds - 1;
    }
   
    pos = newPos;
  }

  current_led = abs(pos % numberOfLeds);

  if(invert_leds){
    for(int i = 0; i < numberOfLeds; i++){
      if(i == current_led){
       leds[i].off();
      } else {
        leds[i].on();
      }
    }
  } else { 
    for(int i = 0; i < numberOfLeds; i++){
      if(i == current_led){
       leds[i].on();
      } else {
       leds[i].off();
      }
    }
  }
  

  if (debugFlag) {
    Serial.println("HI!");
  }
}


/* =========================================================================================
 * Function.
 * ========================================================================================*/

