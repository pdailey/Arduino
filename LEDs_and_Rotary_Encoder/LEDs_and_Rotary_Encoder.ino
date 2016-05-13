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
#include "Led.h"

bool debugFlag = 0;

// RoraryEncoder for pins A2 and A3:
RotaryEncoder encoder(A2, A3);


// LED Bar Graph
const byte numberOfLeds = 10;
//byte ledPinArray[numberOfblinkers] = {2, 3, 4, 5, 6,
//                                        7, 8, 9,10,11};

Led leds[numberOfLeds] = {Led(2), Led(3), Led(4), Led(5), Led(6), \
                          Led(7), Led(8), Led(9), Led(10), Led(11)};

byte current_led = 0;
 
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

  int newPos = encoder.getPosition();
  if (pos != newPos) {
    if(newPos <= 0){
      encoder.setPosition(0);
      newPos = 0;
    } else if (newPos >= numberOfLeds - 1){
      encoder.setPosition(numberOfLeds - 1);
      newPos = numberOfLeds - 1;
    }
    
    Serial.println(newPos);
    pos = newPos;
  }

  // TODO: increment led
  current_led = abs(pos % numberOfLeds);

  for(int i = 0; i < numberOfLeds; i++){
    if(i == current_led){
     leds[i].on();
    } else {
      leds[i].off();
    }
  }

  if (debugFlag) {
    Serial.println("HI!");
  }
}


/* =========================================================================================
 * Function.
 * ========================================================================================*/

