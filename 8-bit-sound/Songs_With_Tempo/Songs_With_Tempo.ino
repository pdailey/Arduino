/*
   Plays the Super Mario Theme without delay
   The tempo of the song is controled by a potentiometer
   The song can be repeated if desired.
   Connect the speaker to the speaker pin and a 1K resistor that leads to ground.
   Connect the center lead of a potentiometer to analog pin and the other two leads to +5V and ground.
*/

#include <NewTone.h>
#include "NewPitches.h"
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
const int melody[][100] = {
  {D4,G4,FS4,G4,B4,A4,G4,A4,B4,A4,G4,G4,B4,D5,E5,0,E5,D5,B4,B4,G4,A4,G4,A4,B4,A4,G4,E4,E4,D4,G4,0,E5,D5,B4,B4,G4,A4,G4,A4,E5,D5,B4,B4,D5,E5,0,E5,D5,B4,B4,G4,A4,G4,A4,B4,A4,G4,E4,E4,D4,G4}, // Auld Lang Syne
  {E4,A4,C5,B4,A4,E5,D5,B4,A4,C5,B4,G4,AS4,E4,E4,A4,C5,B4,A4,E5,G5,FS5,F5,CS5,F5,E5,DS5,DS4,C5,A4,C5}, // Harry Potter
  {AS4,D5,C5,D5,G4,0,DS5,D5,DS5,D5,C5,0,DS5,D5,DS5,G4,0,C5,AS4,C5,AS4,A4,C5,AS4,D5,C5,D5,G4,0,DS5,D5,DS5,D5,C5,0,DS5,D5,DS5,G4,0,C5,AS4,C5,AS4,A4,C5,AS4,A4,AS4,C5,AS4,C5,D5,C5,AS4,A4,G4,DS5,D5,D5,D5,DS5,D5,C5,D5}, // Final countdown
  {D4,F4,D5,D4,F4,D5,E5,F5,E5,F5,E5,C5,A4,A4,D4,F4,G4,A4,A4,D4,F4,G4,E4}, // Song of storms
  {C4,G4,F4,E4,D4,C5,G4,F4,E4,D4,C5,G4,F4,E4,F4,D4}, // Star wars
  {F4,A4,B4,F4,A4,B4,F4,A4,B4,E5,D5,B4,C5,B4,G4,E4,D4,E4,G4,E4}, // Lost woods
  {C2, F3, C3, A2, C3, F3, C3, C3, F3, C3, F3, AS3, G3, F3, E3, D3, CS3, C3, F3, C3, A2, C3, F3, C3, AS3, 0, G3, F3, E3, D3, CS3, C3 }, // Jeopardy
  {C4, D4, F4, D4, A4, 0, A4, G4, 0, C4, D4, F4, D4, G4, 0, G4, F4, 0 } // Rick Roll
};

// Note Durations
// Notation: quarter note  = 4
const byte noteDurations[][100] = { // This is the duration to play each note. 1 = whole note, 2 = half note, 4 = quater note, etc.
  {3,3,8,4,4,4,8,4,8,8,4,8,4,4,2,4,4,4,8,4,4,4,8,4,8,8,4,8,4,4,2,4,4,4,8,4,4,4,8,4,4,4,8,4,4,2,4,4,4,8,4,4,4,8,4,8,8,4,8,4,4,1}, // Auld Lang Syne
  {4,4,4,4,2,4,1.5,1.5,4,4,4,2,4,1.5,4,4,4,4,2,4,2,4,2,4,4,4,4,2,4,1}, // Harry Potter
  {4,16,16,4,4,4,16,16,8,8,4,4,16,16,4,4,4,16,16,8,8,8,8,4,16,16,4,4,4,16,16,8,8,4,4,16,16,4,4,4,16,16,8,8,8,8,4,16,16,4,16,16,8,8,8,8,4,4,4,8,8,4,16,16,1}, // Final countdown
  {5,5,2,5,5,2,3,8,8,8,8,8,2,4,4,8,8,2,4,4,8,8,2}, // Song of storms
  {2,2,4,4,4,2,2,4,4,4,2,2,4,4,4,1}, // Star wars
  {4,4,2,4,4,2,4,4,4,4,2,4,4,4,4,1,4,4,4,1}, // Lost woods
  {4,4,4,4,4,4,2,4,4,4,4,3,8,8,8,8,8,4,4,4,4,4,4,2,4,8,8,4,4,4,4,4,4,0}, // Jeopardy
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

