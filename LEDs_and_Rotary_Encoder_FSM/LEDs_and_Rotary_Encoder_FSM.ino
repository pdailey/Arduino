/*
   ============================================================================================
   ============================================================================================
   ============================================================================================
*/


/* ============================================================================
                               GLOBAL VARIABLES!
                                    (/^▽^)/
   ===========================================================================*/
#include <RotaryEncoder.h>
#include <Button.h>
#include "Led.h"

// RoraryEncoder for pins A2 and A3:
RotaryEncoder encoder(A2, A3);

// Push button on encoder
const byte BUTTON_PIN = 2;
const bool PULLUP = true;        //To keep things simple, we use the Arduino's internal pullup resistor.
const bool INVERT = true;        //Since the pullup resistor will keep the pin high unless the
//switch is closed, this is negative logic, i.e. a high state
//means the button is NOT pressed. (Assuming a normally open switch.)
const byte DEBOUNCE_MS = 20;     //A debounce time of 20 milliseconds usually works well for tactile button switches.

Button button(BUTTON_PIN, PULLUP, INVERT, DEBOUNCE_MS);    //Declare the button

// LED Bar Graph
const byte numberOfLeds = 10;
Led leds[numberOfLeds] = {
  Led(3),
  Led(4),
  Led(5),
  Led(6),
  Led(7),
  Led(8),
  Led(10),
  Led(11),
  Led(12),
  Led(13),
};

// State Variables
bool debugFlag = true;

int pos = 0;
int note = 0;

bool button_pressed = false;
bool encoder_moved = false;

// Serial Communication Variables
const unsigned int BAUD = 115200;


// State machine variables
#define SELECT_SONG 0
#define START_SONG 1
#define CONTINUE_SONG 2
byte fsm_state = SELECT_SONG;

/* ============================================================================
                                     SETUP!
                                 (ﾉ◕ヮ◕)ﾉ*:･ﾟ✧
   ==========================================================================*/
void setup() {
  // USB and Board Serial
  Serial.begin(BAUD);
}


/* ============================================================================
                                      LOOP!
                                     ᕕ( ᐛ )ᕗ
   ==========================================================================*/
void loop() {
  // Check for external input
  buttonRead();
  encoderRead();

  switch (fsm_state) {
    case SELECT_SONG:
      updateLeds(pos);

      if (button_pressed) {
        if (note == 0)
          fsm_state = START_SONG;
        else 
          fsm_state = CONTINUE_SONG;
        
      }
      break;

    case START_SONG:
      note = 0;
      fsm_state = CONTINUE_SONG;
      break;

    case CONTINUE_SONG:
      updateLedsInverted(pos);
      updateMelody();
      
      if (button_pressed || note == 0) {
        // reset the triggers
        fsm_state = SELECT_SONG;
      } else if (encoder_moved) {
        note = 0;
        fsm_state = SELECT_SONG;
      }
      
      break;

    default:
      Serial.println(F("ERROR: Reached default state"));
      break;
  }

  if (debugFlag) {
    Serial.println(F("DEBUGGING:"));
    Serial.print(F("State: "));
    Serial.println(fsm_state);
    Serial.print(F("note: "));
    Serial.println(note);
    Serial.print(F("song: "));
    Serial.println(pos);
  }
  
}


/* =========================================================================================
   Function.
   ========================================================================================*/
void buttonRead() {
  button.read();

  if (button.wasReleased()) {
    button_pressed = true;
  } else {
    button_pressed = false;
  }
}

// Handle literal edge cases
byte setEncoderInRange(int newPos) {
  if (newPos <= 0) {
    encoder.setPosition(0);
  } else if (newPos >= numberOfLeds - 1) {
    encoder.setPosition(numberOfLeds - 1);
  }

  return encoder.getPosition();
}

void encoderRead() {
  encoder.tick();
  int newPos = encoder.getPosition();

  if (pos != newPos) {
    pos = setEncoderInRange(newPos);
    encoder_moved = true;
  } else {
    encoder_moved = false;
  }
}

void updateMelody() {
  note++;
}


void updateLeds(byte n) {
  for (byte i = 0; i < numberOfLeds; i++)
    (i == n) ? leds[i].on() : leds[i].off();
}

void updateLedsInverted(byte n) {
  for (byte i = 0; i < numberOfLeds; i++)
    (i == n) ? leds[i].off() : leds[i].on();
}


