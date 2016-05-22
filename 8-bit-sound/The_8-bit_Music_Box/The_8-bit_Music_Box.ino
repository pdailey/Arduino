
/***
 *                                                                                                       
 *                                        bbbbbbbb                                                       
 *         888888888                      b::::::b              iiii          tttt                       
 *       88:::::::::88                    b::::::b             i::::i      ttt:::t                       
 *     88:::::::::::::88                  b::::::b              iiii       t:::::t                       
 *    8::::::88888::::::8                  b:::::b                         t:::::t                       
 *    8:::::8     8:::::8                  b:::::bbbbbbbbb    iiiiiiittttttt:::::ttttttt                 
 *    8:::::8     8:::::8                  b::::::::::::::bb  i:::::it:::::::::::::::::t                 
 *     8:::::88888:::::8                   b::::::::::::::::b  i::::it:::::::::::::::::t                 
 *      8:::::::::::::8   ---------------  b:::::bbbbb:::::::b i::::itttttt:::::::tttttt                 
 *     8:::::88888:::::8  -:::::::::::::-  b:::::b    b::::::b i::::i      t:::::t                       
 *    8:::::8     8:::::8 ---------------  b:::::b     b:::::b i::::i      t:::::t                       
 *    8:::::8     8:::::8                  b:::::b     b:::::b i::::i      t:::::t                       
 *    8:::::8     8:::::8                  b:::::b     b:::::b i::::i      t:::::t    tttttt             
 *    8::::::88888::::::8                  b:::::bbbbbb::::::bi::::::i     t::::::tttt:::::t             
 *     88:::::::::::::88                   b::::::::::::::::b i::::::i     tt::::::::::::::t             
 *       88:::::::::88                     b:::::::::::::::b  i::::::i       tt:::::::::::tt             
 *         888888888                       bbbbbbbbbbbbbbbb   iiiiiiii         ttttttttttt               
 *    MMMMMMMM               MMMMMMMM                                     iiii                           
 *    M:::::::M             M:::::::M                                    i::::i                          
 *    M::::::::M           M::::::::M                                     iiii                           
 *    M:::::::::M         M:::::::::M                                                                    
 *    M::::::::::M       M::::::::::Muuuuuu    uuuuuu      ssssssssss   iiiiiii     cccccccccccccccc     
 *    M:::::::::::M     M:::::::::::Mu::::u    u::::u    ss::::::::::s  i:::::i   cc:::::::::::::::c     
 *    M:::::::M::::M   M::::M:::::::Mu::::u    u::::u  ss:::::::::::::s  i::::i  c:::::::::::::::::c     
 *    M::::::M M::::M M::::M M::::::Mu::::u    u::::u  s::::::ssss:::::s i::::i c:::::::cccccc:::::c     
 *    M::::::M  M::::M::::M  M::::::Mu::::u    u::::u   s:::::s  ssssss  i::::i c::::::c     ccccccc     
 *    M::::::M   M:::::::M   M::::::Mu::::u    u::::u     s::::::s       i::::i c:::::c                  
 *    M::::::M    M:::::M    M::::::Mu::::u    u::::u        s::::::s    i::::i c:::::c                  
 *    M::::::M     MMMMM     M::::::Mu:::::uuuu:::::u  ssssss   s:::::s  i::::i c::::::c     ccccccc     
 *    M::::::M               M::::::Mu:::::::::::::::uus:::::ssss::::::si::::::ic:::::::cccccc:::::c     
 *    M::::::M               M::::::M u:::::::::::::::us::::::::::::::s i::::::i c:::::::::::::::::c     
 *    M::::::M               M::::::M  uu::::::::uu:::u s:::::::::::ss  i::::::i  cc:::::::::::::::c     
 *    MMMMMMMM               MMMMMMMM    uuuuuuuu  uuuu  sssssssssss    iiiiiiii    cccccccccccccccc     
 *    BBBBBBBBBBBBBBBBB                                                                                  
 *    B::::::::::::::::B                                                                                 
 *    B::::::BBBBBB:::::B                                                                                
 *    BB:::::B     B:::::B                                                                               
 *      B::::B     B:::::B   ooooooooooo xxxxxxx      xxxxxxx                                            
 *      B::::B     B:::::B oo:::::::::::oox:::::x    x:::::x                                             
 *      B::::BBBBBB:::::B o:::::::::::::::ox:::::x  x:::::x                                              
 *      B:::::::::::::BB  o:::::ooooo:::::o x:::::xx:::::x                                               
 *      B::::BBBBBB:::::B o::::o     o::::o  x::::::::::x                                                
 *      B::::B     B:::::Bo::::o     o::::o   x::::::::x                                                 
 *      B::::B     B:::::Bo::::o     o::::o   x::::::::x                                                 
 *      B::::B     B:::::Bo::::o     o::::o  x::::::::::x                                                
 *    BB:::::BBBBBB::::::Bo:::::ooooo:::::o x:::::xx:::::x                                               
 *    B:::::::::::::::::B o:::::::::::::::ox:::::x  x:::::x                                              
 *    B::::::::::::::::B   oo:::::::::::oox:::::x    x:::::x                                             
 *    BBBBBBBBBBBBBBBBB      ooooooooooo xxxxxxx      xxxxxxx                                            
 *  
 *  ============================================================================================
 *  ============================================================================================
 *  ============================================================================================
 */


/* ============================================================================
                               GLOBAL VARIABLES!
                                    (/^▽^)/
   ===========================================================================*/
#include <RotaryEncoder.h>
#include <Button.h>
#include <NewTone.h>
#include "pitches.h"
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
const int m0 [] = {NOTE_D4, NOTE_G4, NOTE_FS4, NOTE_G4, NOTE_B4, NOTE_A4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_A4, NOTE_G4, NOTE_G4, NOTE_B4,  NOTE_D5, NOTE_E5, 0, NOTE_E5, NOTE_D5, NOTE_B4, NOTE_B4, NOTE_G4, NOTE_A4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_A4, NOTE_G4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_G4, 0, NOTE_E5, NOTE_D5, NOTE_B4, NOTE_B4, NOTE_G4, NOTE_A4, NOTE_G4, NOTE_A4, NOTE_E5, NOTE_D5, NOTE_B4, NOTE_B4, NOTE_D5, NOTE_E5, 0, NOTE_E5, NOTE_D5, NOTE_B4, NOTE_B4, NOTE_G4, NOTE_A4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_A4, NOTE_G4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_G4}; // Auld Lang Syne
const int m1 [] = {NOTE_E4, NOTE_A4, NOTE_C5, NOTE_B4, NOTE_A4, NOTE_E5, NOTE_D5, NOTE_B4, NOTE_A4, NOTE_C5, NOTE_B4, NOTE_G4, NOTE_AS4, NOTE_E4, NOTE_E4, NOTE_A4, NOTE_C5, NOTE_B4, NOTE_A4, NOTE_E5, NOTE_G5, NOTE_FS5, NOTE_F5, NOTE_CS5, NOTE_F5, NOTE_E5, NOTE_DS5, NOTE_DS4, NOTE_C5, NOTE_A4, NOTE_C5}; // Harry Potte/AS
const int m2 [] = {NOTE_AS4, NOTE_D5, NOTE_C5, NOTE_D5, NOTE_G4, 0, NOTE_DS5, NOTE_D5, NOTE_DS5, NOTE_D5, NOTE_C5, 0, NOTE_DS5, NOTE_D5, NOTE_DS5, NOTE_G4, 0, NOTE_C5, NOTE_AS4, NOTE_C5, NOTE_AS4, NOTE_A4, NOTE_C5, NOTE_AS4, NOTE_D5, NOTE_C5, NOTE_D5, NOTE_G4, 0, NOTE_DS5, NOTE_D5, NOTE_DS5, NOTE_D5, NOTE_C5, 0, NOTE_DS5, NOTE_D5, NOTE_DS5, NOTE_G4, 0, NOTE_C5, NOTE_AS4, NOTE_C5, NOTE_AS4, NOTE_A4, NOTE_C5, NOTE_AS4, NOTE_A4, NOTE_AS4, NOTE_C5, NOTE_AS4, NOTE_C5, NOTE_D5, NOTE_C5, NOTE_AS4, NOTE_A4, NOTE_G4, NOTE_DS5, NOTE_D5, NOTE_D5, NOTE_D5, NOTE_DS5, NOTE_D5, NOTE_C5, NOTE_D5}; // Final countdown
const int m3 [] = {NOTE_D4, NOTE_F4, NOTE_D5, NOTE_D4, NOTE_F4, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_E5, NOTE_F5, NOTE_E5, NOTE_C5, NOTE_A4, NOTE_A4, NOTE_D4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_D4, NOTE_F4, NOTE_G4, NOTE_E4}; // Song of storms
const int m4 [] = {NOTE_C4, NOTE_G4, NOTE_F4, NOTE_E4, NOTE_D4, NOTE_C5, NOTE_G4, NOTE_F4, NOTE_E4, NOTE_D4, NOTE_C5, NOTE_G4, NOTE_F4, NOTE_E4, NOTE_F4, NOTE_D4}; // Star wars
const int m5 [] = {NOTE_F4, NOTE_A4, NOTE_B4, NOTE_F4, NOTE_A4, NOTE_B4, NOTE_F4, NOTE_A4, NOTE_B4, NOTE_E5, NOTE_D5, NOTE_B4, NOTE_C5, NOTE_B4, NOTE_G4, NOTE_E4, NOTE_D4, NOTE_E4, NOTE_G4, NOTE_E4}; // Lost woods
const int m6 [] = {NOTE_C2, NOTE_F3, NOTE_C3, NOTE_A2, NOTE_C3, NOTE_F3, NOTE_C3, NOTE_C3, NOTE_F3, NOTE_C3, NOTE_F3, NOTE_AS3, NOTE_G3, NOTE_F3, NOTE_E3, NOTE_D3, NOTE_CS3, NOTE_C3, NOTE_F3, NOTE_C3, NOTE_A2, NOTE_C3, NOTE_F3, NOTE_C3, NOTE_AS3, 0, NOTE_G3, NOTE_F3, NOTE_E3, NOTE_D3, NOTE_CS3, NOTE_C3 }; // Jeopardy
const int m7 [] = {NOTE_C4, NOTE_D4, NOTE_F4, NOTE_D4, NOTE_A4,       0, NOTE_A4, NOTE_G4,       0, NOTE_C4, NOTE_D4, NOTE_F4, NOTE_D4, NOTE_G4, 0, NOTE_G4, NOTE_F4, 0 }; // Rick Roll
// Future expansion, currently Rick Rolls
//const int m8 [] = {0}; // Rick Roll
//const int m9 [] = {0}; // Rick Roll
const int* melodies[] = {m0, m1, m2, m3, m4, m5, m6, m7, m7, m7};


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
// Future expansion, currently Rick Rolls
//const byte d8 [] = {0}; // Rick Roll
//const byte d9 [] = {0}; // Rick Roll
const byte* noteDurations[] = {d0, d1, d2, d3, d4, d5, d6, d7, d7, d7};

const byte totalNotes[] = {62, 30, 65, 23, 16, 20, 34, 18, 18, 18};


//////////////////////////////////////
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
  pinMode(speakerPin, OUTPUT);

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
        fsm_state = START_SONG;
      }
      break;

    case START_SONG:
      note = 0;
      fsm_state = CONTINUE_SONG;
      break;

    case CONTINUE_SONG:
      updateLedsInverted(pos);
      updateMelody();

      if (button_pressed || note == 0 || encoder_moved) {
        // reset the triggers
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

void updateLeds(byte n) {
  for (byte i = 0; i < numberOfLeds; i++)
    (i == n) ? leds[i].on() : leds[i].off();
}

void updateLedsInverted(byte n) {
  for (byte i = 0; i < numberOfLeds; i++)
    (i == n) ? leds[i].off() : leds[i].on();
}

/*
   Changes the note played according to Plays a melody one time through
*/
void updateMelody() {
  unsigned long nowMs = millis();
  // convert seconds to ms, then
  int noteDuration = 1000 / noteDurations[pos][note];

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
    NewTone(speakerPin, melodies[pos][note], noteDuration);
    prevMs = nowMs;
    note++;

    if (note == totalNotes[pos]) {
      note = 0;
    }
  }
}

