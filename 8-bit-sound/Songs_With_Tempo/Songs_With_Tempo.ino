/*
   Plays the Super Mario Theme without delay
   The tempo of the song is controled by a potentiometer
   The song can be repeated if desired.
   Connect the speaker to the speaker pin and a 1K resistor that leads to ground.
   Connect the center lead of a potentiometer to analog pin and the other two leads to +5V and ground.
*/

#include <NewTone.h>
#include "pitches.h"
#include "Led.h"

const byte ledPin = 13;
Led led = Led(ledPin);

const byte speakerPin =  9;
long prevMs = 0;

const byte potPin = A0;

boolean setTempo = true;
int thisTempo;

// min/max tempo factors
const byte minTempo = 5;
const byte maxTempo = 20;

const byte tempoFactor = 10;

byte thisSong = 0;
boolean repeatMelody = false;

// Set melodies - this creates the melodies. The numbers under 12 are the regular keys. Any higher number is a frequency that can't be made by pressing a single key (sharps and flats).
const int melody[][20] = {
  //{NOTE_D4,NOTE_G4,NOTE_FS4,NOTE_G4,NOTE_B4,NOTE_A4,NOTE_G4,NOTE_A4,NOTE_B4,NOTE_A4,NOTE_G4,NOTE_G4,NOTE_B4,NOTE_D5,NOTE_E5,0,NOTE_E5,NOTE_D5,NOTE_B4,NOTE_B4,NOTE_G4,NOTE_A4,NOTE_G4,NOTE_A4,NOTE_B4,NOTE_A4,NOTE_G4,NOTE_E4,NOTE_E4,NOTE_D4,NOTE_G4,0,NOTE_E5,NOTE_D5,NOTE_B4,NOTE_B4,NOTE_G4,NOTE_A4,NOTE_G4,NOTE_A4,NOTE_E5,NOTE_D5,NOTE_B4,NOTE_B4,NOTE_D5,NOTE_E5,0,NOTE_E5,NOTE_D5,NOTE_B4,NOTE_B4,NOTE_G4,NOTE_A4,NOTE_G4,NOTE_A4,NOTE_B4,NOTE_A4,NOTE_G4,NOTE_E4,NOTE_E4,NOTE_D4,NOTE_G4}, / // Auld Lang Syne
  //{NOTE_E4,NOTE_A4,NOTE_C5,NOTE_B4,NOTE_A4,NOTE_E5,NOTE_D5,NOTE_B4,NOTE_A4,NOTE_C5,NOTE_B4,NOTE_G4,NOTE_AS4,NOTE_E4,NOTE_E4,NOTE_A4,NOTE_C5,NOTE_B4,NOTE_A4,NOTE_E5,NOTE_G5,NOTE_FS5,NOTE_F5,NOTE_CS5,NOTE_F5,NOTE_E5,NOTE_DS5,NOTE_DS4,NOTE_C5,NOTE_A4,NOTE_C5}, // Harry Potter
  //{NOTE_AS4,NOTE_D5,NOTE_C5,NOTE_D5,NOTE_G4,0,NOTE_DS5,NOTE_D5,NOTE_DS5,NOTE_D5,NOTE_C5,0,NOTE_DS5,NOTE_D5,NOTE_DS5,NOTE_G4,0,NOTE_C5,NOTE_AS4,NOTE_C5,NOTE_AS4,NOTE_A4,NOTE_C5,NOTE_AS4,NOTE_D5,NOTE_C5,NOTE_D5,NOTE_G4,0,NOTE_DS5,NOTE_D5,NOTE_DS5,NOTE_D5,NOTE_C5,0,NOTE_DS5,NOTE_D5,NOTE_DS5,NOTE_G4,0,NOTE_C5,NOTE_AS4,NOTE_C5,NOTE_AS4,NOTE_A4,NOTE_C5,NOTE_AS4,NOTE_A4,NOTE_AS4,NOTE_C5,NOTE_AS4,NOTE_C5,NOTE_D5,NOTE_C5,NOTE_AS4,NOTE_A4,NOTE_G4,NOTE_DS5,NOTE_D5,NOTE_D5,NOTE_D5,NOTE_DS5,NOTE_D5,NOTE_C5,NOTE_D5}, // Final countdown
  //{NOTE_D4,NOTE_F4,NOTE_D5,NOTE_D4,NOTE_F4,NOTE_D5,NOTE_E5,NOTE_F5,NOTE_E5,NOTE_F5,NOTE_E5,NOTE_C5,NOTE_A4,NOTE_A4,NOTE_D4,NOTE_F4,NOTE_G4,NOTE_A4,NOTE_A4,NOTE_D4,NOTE_F4,NOTE_G4,NOTE_E4}, // Song of storms
  //{NOTE_C4,NOTE_G4,NOTE_F4,NOTE_E4,NOTE_D4,NOTE_C5,NOTE_G4,NOTE_F4,NOTE_E4,NOTE_D4,NOTE_C5,NOTE_G4,NOTE_F4,NOTE_E4,NOTE_F4,NOTE_D4}, // Star wars
  //{NOTE_F4,NOTE_A4,NOTE_B4,NOTE_F4,NOTE_A4,NOTE_B4,NOTE_F4,NOTE_A4,NOTE_B4,NOTE_E5,NOTE_D5,NOTE_B4,NOTE_C5,NOTE_B4,NOTE_G4,NOTE_E4,NOTE_D4,NOTE_E4,NOTE_G4,NOTE_E4}, // Lost woods
  //{ NOTE_C2, NOTE_F3, NOTE_C3, NOTE_A2, NOTE_C3, NOTE_F3, NOTE_C3, NOTE_C3, NOTE_F3, NOTE_C3, NOTE_F3, NOTE_AS3, NOTE_G3, NOTE_F3, NOTE_E3, NOTE_D3, NOTE_CS3, NOTE_C3, NOTE_F3, NOTE_C3, NOTE_A2, NOTE_C3, NOTE_F3, NOTE_C3, NOTE_AS3, 0, NOTE_G3, NOTE_F3, NOTE_E3, NOTE_D3, NOTE_CS3, NOTE_C3 }, // Jeopardy
  { NOTE_C4, NOTE_D4, NOTE_F4, NOTE_D4, NOTE_A4, 0, NOTE_A4, NOTE_G4, 0, NOTE_C4, NOTE_D4, NOTE_F4, NOTE_D4, NOTE_G4, 0, NOTE_G4, NOTE_F4, 0 } // Rick Roll
};

// Note Durations
// Notation: quarter note  = 4
const byte noteDurations[][20] = { // This is the duration to play each note. 1 = whole note, 2 = half note, 4 = quater note, etc.
  //{3,3,8,4,4,4,8,4,8,8,4,8,4,4,2,4,4,4,8,4,4,4,8,4,8,8,4,8,4,4,2,4,4,4,8,4,4,4,8,4,4,4,8,4,4,2,4,4,4,8,4,4,4,8,4,8,8,4,8,4,4,1}, // Auld Lang Syne
  //{4,4,4,4,2,4,1.5,1.5,4,4,4,2,4,1.5,4,4,4,4,2,4,2,4,2,4,4,4,4,2,4,1}, // Harry Potter
  //{4,16,16,4,4,4,16,16,8,8,4,4,16,16,4,4,4,16,16,8,8,8,8,4,16,16,4,4,4,16,16,8,8,4,4,16,16,4,4,4,16,16,8,8,8,8,4,16,16,4,16,16,8,8,8,8,4,4,4,8,8,4,16,16,1}, // Final countdown
  //{5,5,2,5,5,2,3,8,8,8,8,8,2,4,4,8,8,2,4,4,8,8,2}, // Song of storms
  //{2,2,4,4,4,2,2,4,4,4,2,2,4,4,4,1}, // Star wars
  //{4,4,2,4,4,2,4,4,4,4,2,4,4,4,4,1,4,4,4,1}, // Lost woods
  //{4,4,4,4,4,4,2,4,4,4,4,3,8,8,8,8,8,4,4,4,4,4,4,2,4,8,8,4,4,4,4,4,4,0}, // Jeopardy
  {4,4,4,4,4,4,1,1,2,4,4,4,4,4,4,1,1,2}, // Rick Roll
};

void setup() {
  pinMode(speakerPin, OUTPUT);

  Serial.begin(57600);
  Serial.println("Begin Mario!");
}

void loop() {
  updateMelody();

  //DEBUGGING
  //Serial.println(millis());
  delay(10);
}

/*
   Changes the note played according to Plays a melody one time through
*/
void updateMelody() {
  static int thisNote = 0;
  int totalNotes = sizeof(noteDurations[thisSong]) / sizeof(byte);

  // Is the song still going?
  if (thisNote < totalNotes) {
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

    Serial.println(noteDuration);

    // if the duration of the note has passed, move to next note
    if (nowMs - prevMs > noteDuration) {
      prevMs = nowMs;
      led.on();
      // Change the tone, updatign the tempo and pitch
      NewTone(speakerPin, melody[thisSong][thisNote], noteDuration);
      thisNote++;

      if (repeatMelody && thisNote == totalNotes) {
        thisNote = 0;
      }
    } else {
      led.off();
    }
  }

  // DEBUGGING
  if (thisNote == totalNotes) {
    Serial.println("Done!");
  }
}

