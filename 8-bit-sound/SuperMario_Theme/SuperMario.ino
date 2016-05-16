/*
  Arduino Mario Bros Tunes

*/

#include <NewTone.h>
#include "pitches.h"

const byte speakerPin =  9;
long prevMs = 0;

//Mario main theme melody
int melody[] = {
  NOTE_E7, NOTE_E7, 0, NOTE_E7,
  0, NOTE_C7, NOTE_E7, 0,
  NOTE_G7, 0, 0,  0,
  NOTE_G6, 0, 0, 0,

  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,

  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0,

  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,

  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0
};

//Mario main them tempo
byte noteDurations[] = {
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
};

void setup(void)
{
  pinMode(speakerPin, OUTPUT);

  Serial.begin(57600);
  Serial.println("Begin Mario!");
}

void loop()
{
  updateMelody(current_song);
  

  //Debugging
  //Serial.println(millis());
  
}

void updateMelody() {
  static int thisNote = 0;
  int totalNotes = sizeof(noteDurations) / sizeof(byte);


  if (thisNote < totalNotes) {
    unsigned long nowMs = millis();
    int noteDuration = 1000/noteDurations[thisNote];
    
    // if the duration of the note has passed, move to next note
    if (nowMs - prevMs > noteDuration) {
      prevMs = nowMs;

      NewTone(speakerPin, melody[thisNote], noteDuration);
      thisNote++;
    }
  }
  if(thisNote == totalNotes){
    Serial.println("Done!");  
  }
}

