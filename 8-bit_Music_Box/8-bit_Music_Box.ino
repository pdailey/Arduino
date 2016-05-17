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
#include <NewTone.h>
#include "NewPitches.h"
#include "Led.h"

bool debugFlag = 0;

//////////////////////////////////////
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

//////////////////////////////////////
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

//////////////////////////////////////
// Music
const byte speakerPin =  9;
long prevMs = 0;

// TEMPO
const byte potPin = A0;
boolean setTempo = true;
int thisTempo;

// min/max tempo factors
const byte minTempo = 5;
const byte maxTempo = 20;
const byte tempoFactor = 10;


// Melodies
// This creates the melodies using NewPitch.h by setting the note frequency
const int m0 [] = {D4, G4, FS4, G4, B4, A4, G4, A4, B4, A4, G4, G4, B4, D5, E5, 0, E5, D5, B4, B4, G4, A4, G4, A4, B4, A4, G4, E4, E4, D4, G4, 0, E5, D5, B4, B4, G4, A4, G4, A4, E5, D5, B4, B4, D5, E5, 0, E5, D5, B4, B4, G4, A4, G4, A4, B4, A4, G4, E4, E4, D4, G4}; // Auld Lang Syne
const int m1 [] = {E4, A4, C5, B4, A4, E5, D5, B4, A4, C5, B4, G4, AS4, E4, E4, A4, C5, B4, A4, E5, G5, FS5, F5, CS5, F5, E5, DS5, DS4, C5, A4, C5}; // Harry Potter
const int m2 [] = {AS4, D5, C5, D5, G4, 0, DS5, D5, DS5, D5, C5, 0, DS5, D5, DS5, G4, 0, C5, AS4, C5, AS4, A4, C5, AS4, D5, C5, D5, G4, 0, DS5, D5, DS5, D5, C5, 0, DS5, D5, DS5, G4, 0, C5, AS4, C5, AS4, A4, C5, AS4, A4, AS4, C5, AS4, C5, D5, C5, AS4, A4, G4, DS5, D5, D5, D5, DS5, D5, C5, D5}; // Final countdown
const int m3 [] = {D4, F4, D5, D4, F4, D5, E5, F5, E5, F5, E5, C5, A4, A4, D4, F4, G4, A4, A4, D4, F4, G4, E4}; // Song of storms
const int m4 [] = {C4, G4, F4, E4, D4, C5, G4, F4, E4, D4, C5, G4, F4, E4, F4, D4}; // Star wars
const int m5 [] = {F4, A4, B4, F4, A4, B4, F4, A4, B4, E5, D5, B4, C5, B4, G4, E4, D4, E4, G4, E4}; // Lost woods
const int m6 [] = {C2, F3, C3, A2, C3, F3, C3, C3, F3, C3, F3, AS3, G3, F3, E3, D3, CS3, C3, F3, C3, A2, C3, F3, C3, AS3, 0, G3, F3, E3, D3, CS3, C3 }; // Jeopardy
const int m7 [] = {C4, D4, F4, D4, A4, 0, A4, G4, 0, C4, D4, F4, D4, G4, 0, G4, F4, 0 }; // Rick Roll
const int* melodies[] = {m0, m1, m2, m3, m4, m5, m6, m7};


// Note Durations
// The duration to play each note. Notation: quarter note  = 4
const byte d0 [] = {3, 3, 8, 4, 4, 4, 8, 4, 8, 8, 4, 8, 4, 4, 2, 4, 4, 4, 8, 4, 4, 4, 8, 4, 8, 8, 4, 8, 4, 4, 2, 4, 4, 4, 8, 4, 4, 4, 8, 4, 4, 4, 8, 4, 4, 2, 4, 4, 4, 8, 4, 4, 4, 8, 4, 8, 8, 4, 8, 4, 4, 1}; // Auld Lang Syne
const byte d1 [] = {4, 4, 4, 4, 2, 4, 1.5, 1.5, 4, 4, 4, 2, 4, 1.5, 4, 4, 4, 4, 2, 4, 2, 4, 2, 4, 4, 4, 4, 2, 4, 1}; // Harry Potter
const byte d2 [] = {4, 16, 16, 4, 4, 4, 16, 16, 8, 8, 4, 4, 16, 16, 4, 4, 4, 16, 16, 8, 8, 8, 8, 4, 16, 16, 4, 4, 4, 16, 16, 8, 8, 4, 4, 16, 16, 4, 4, 4, 16, 16, 8, 8, 8, 8, 4, 16, 16, 4, 16, 16, 8, 8, 8, 8, 4, 4, 4, 8, 8, 4, 16, 16, 1}; // Final countdown
const byte d3 [] = {5, 5, 2, 5, 5, 2, 3, 8, 8, 8, 8, 8, 2, 4, 4, 8, 8, 2, 4, 4, 8, 8, 2}; // Song of storms
const byte d4 [] = {2, 2, 4, 4, 4, 2, 2, 4, 4, 4, 2, 2, 4, 4, 4, 1}; // Star wars
const byte d5 [] = {4, 4, 2, 4, 4, 2, 4, 4, 4, 4, 2, 4, 4, 4, 4, 1, 4, 4, 4, 1}; // Lost woods
const byte d6 [] = {4, 4, 4, 4, 4, 4, 2, 4, 4, 4, 4, 3, 8, 8, 8, 8, 8, 4, 4, 4, 4, 4, 4, 2, 4, 8, 8, 4, 4, 4, 4, 4, 4, 0}; // Jeopardy
const byte d7 [] = {4, 4, 4, 4, 4, 4, 1, 1, 2, 4, 4, 4, 4, 4, 4, 1, 1, 2}; // Rick Roll
const byte* noteDurations[] = {d0, d1, d2, d3, d4, d5, d6, d7};

const byte totalNotes[] = {62, 30, 65, 23, 16, 20, 34, 18};

//////////////////////////////////////
// State Variables
bool invert_leds = false;
bool update_position = true;
byte thisSong = 0;
boolean play_melody = false;
boolean repeat_melody = false;

// Serial Communication Variables
const unsigned int BAUD = 57600;


/* ============================================================================
                                     SETUP!
                                 (ﾉ◕ヮ◕)ﾉ*:･ﾟ✧
   ==========================================================================*/
void setup() {
  pinMode(speakerPin, OUTPUT);

  Serial.begin(BAUD);
  // Debugging: Arduino will not run without USB Serial connected
  //while (!Serial) {;}
}


/* ============================================================================
                                      LOOP!
                                     ᕕ( ᐛ )ᕗ
   ==========================================================================*/
void loop() {
  static int pos = 0;

  // Check for changes in state
  encoder.tick();
  button.read();

  // update encoder position
  int newPos = encoder.getPosition();

  if (pos != newPos) {
    pos = getEncoderPosition(pos, newPos);
    //TODO updateLedPosition();
    if (pos < sizeof(melodies)) {
      thisSong = pos;
    }
  }

  // Button Logic
  if (button.wasReleased()) {
    invert_leds = !invert_leds;
    play_melody = !play_melody;
  }

  //TODO:invertLeds();
  if (invert_leds) {
    for (byte i = 0; i < numberOfLeds; i++)
      (i == pos) ? leds[i].off() : leds[i].on();
  } else {
    // upddateLedPosition(leds, pos)
    for (byte i = 0; i < numberOfLeds; i++)
      (i == pos) ? leds[i].on() : leds[i].off();
  }

  if (play_melody) {
    updateMelody();
  }

  if (debugFlag) {
    Serial.println(F("DEBUGGING:"));
  }
}


/* =========================================================================================
   Function.
   ========================================================================================*/

// Handle literal edge cases
byte getEncoderPosition(int oldPos, int newPos) {
  if (newPos <= 0) {
    encoder.setPosition(0);
    newPos = 0;
  } else if (newPos >= numberOfLeds - 1) {
    encoder.setPosition(numberOfLeds - 1);
    newPos = numberOfLeds - 1;
  }
  oldPos = newPos;

  return oldPos;
}

/*
   Changes the note played according to Plays a melody one time through
*/
void updateMelody() {
  static int thisNote = 0;

  // Is the song still going?
  if (thisNote < totalNotes[thisSong]) {
    unsigned long nowMs = millis();

    // convert seconds to ms, then
    int noteDuration = 1000 / noteDurations[thisSong][thisNote];

    if (setTempo) {
      // Update the tempo of the song
      int pot = analogRead(potPin);                      // Read the potentiometer to get the current tempo
      thisTempo = map(pot, 0, 1023, minTempo, maxTempo); // Convert pot analog values to a range from min to max tempo.
      //Serial.println(thisTempo);
      // map() outputs interger values, but we want floats
      noteDuration = noteDuration * thisTempo / tempoFactor;
    }

    // if the duration of the note has passed, move to next note
    if (nowMs - prevMs > noteDuration) {
      prevMs = nowMs;

      // Change the tone, updating the tempo and pitch
      NewTone(speakerPin, melodies[thisSong][thisNote], noteDuration);
      thisNote++;

      if (repeat_melody && thisNote == totalNotes[thisSong]) {
        thisNote = 0;
      }
    } else {
      //led.off();
    }
  }
}
