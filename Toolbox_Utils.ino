#include "src/utils/Arduboy2Ext.h"

#include "src/ArduboyTones/ArduboyTones.h"
#include "src/images/Images.h"
#include "src/fonts/Font3x5.h"
#include "src/utils/Structs.h"
#include "src/utils/Constants.h"
#include <EEPROM.h>



// -----------------------------------------------------------------------------
//  Return the frequency of the note or the previous, valid frequency ..

uint16_t getFreq(uint16_t x) {

  for (uint16_t i = x; i >= 0; i--) {

    Note note = musicVars.notes[i];
    if (note.freq != TONES_END) return note.freq;

  }

  return musicVars.notes[x].freq;

}


// -----------------------------------------------------------------------------
//  Return the index in the selected range of the note ..

uint8_t getNoteIndex(uint8_t range, uint16_t currNote) {

  currNote = currNote & 0x7FFF;

  for (uint8_t x = 0; x < 15; x++) {
    if (pgm_read_word(&noteSeq[range][x]) == currNote) {
      return x;
    }
  } 

  return 0;

}


// -----------------------------------------------------------------------------
//  Return the Y value of the nominated note for rendering ..

uint8_t getNoteYVal(uint8_t range, uint16_t currNote) {

  currNote = currNote & 0x7FFF;

  for (uint8_t x = 0; x < 15; x++) {
    if (pgm_read_word(&noteSeq[range][x]) == currNote) {
      return 58 - (x * 4);
    }
  } 

  return 0;

}


// -----------------------------------------------------------------------------
//  Return the number of lines to render above or below the staff ..

int8_t getNoteLinesAboveBelow(uint8_t range, uint16_t currNote) {

  currNote = currNote & 0x7FFF;

  for (uint8_t x = 0; x < 15; x++) {

    if (pgm_read_word(&noteSeq[range][x]) == currNote) {

      if (x < 2) return -(3 - x);
      if (x > 11) return x - 11;
      break;

    }

  } 

  return 0;

}


// -----------------------------------------------------------------------------
//  Return the note above the nominated note in the same range ..

uint16_t getNoteAbove(uint8_t range, uint16_t currNote) {

  currNote = currNote & 0x7FFF;

  for (uint8_t x = 0; x < 15; x++) {
    if (pgm_read_word(&noteSeq[range][x]) == currNote) {
      if (x == 14) {
        return pgm_read_word(&noteSeq[range][14]);
      }
      else {
        return pgm_read_word(&noteSeq[range][x + 1]);
      }
    }
  } 

  return 0;

}


// -----------------------------------------------------------------------------
//  Return the note below the nominated note in the same range ..

uint16_t getNoteBelow(uint8_t range, uint16_t currNote) {

  currNote = currNote & 0x7FFF;
  
  for (uint8_t x = 0; x < 15; x++) {
    if (pgm_read_word(&noteSeq[range][x]) == currNote) {
      if (x == 0) {
        return pgm_read_word(&noteSeq[range][0]);
      }
      else {
        return pgm_read_word(&noteSeq[range][x - 1]);
      }
    }
  } 

  return 0;

}


// -----------------------------------------------------------------------------
//  Return the middle note of the selected range ..

uint16_t getNoteMiddle(uint8_t range) {

  return pgm_read_word(&noteSeq[range][7]);
  
}


// -----------------------------------------------------------------------------
//  Reset ready for a new tune!

void resetAll() {

  if (gameState == GameState::Composer) {

    for (uint8_t x = 0; x < NUMBER_OF_NOTES; x++) {
      musicVars.notes[x].freq = 0;
      musicVars.notes[x].duration = musicVars.noteLength;
    }

    musicVars.range = 3;
    musicVars.notes[0].freq = TONES_END;
    musicVars.y = getNoteMiddle(musicVars.range);
    musicVars.x = 0;

  }
  else {

    for (uint8_t y = 0; y < 2; y++) {

      for (uint8_t x = 0; x < 16; x++) {
        imageVars.image[y][x]= 0;
      }

    }
    
    imageVars.y = 0;
    imageVars.x = 0;

  }

}


// -----------------------------------------------------------------------------
//  Make a note when moving to the right ,,

void makeNextNote(Note &note) {

  if (note.freq == TONES_END) {
    musicVars.notes[musicVars.x + 1].freq = TONES_END;
    note.freq = musicVars.y;
    sound.tone(note.freq, NOTE_SAMPLE_DURATION);
  }
  else {
    note.freq = musicVars.y;
    if (note.duration == 0)
      note.duration = musicVars.noteLength;
  }

}


// -----------------------------------------------------------------------------
//  Export the current tune to the serial port ..

void exportToSerial() {

  switch (gameState) {

    case GameState::Composer:
      {
      Serial.println(F("const uint16_t theme[] PROGMEM = {"));

      for (uint16_t x = 0; x < NUMBER_OF_NOTES; x++) {

        Note note = musicVars.notes[x];
        
        if (note.freq == TONES_END) break;
        if (note.freq > 0x8000) note.freq = 0;

        Serial.print(note.freq);
        Serial.print(F(","));
        Serial.print(note.duration);
        Serial.print(F(", "));

        if (x > 0 && x % 8 == 0) Serial.println(F("")); 

      }
        
      Serial.println(F("TONES_END };"));
      }
      break;

    case GameState::Designer:
      {
      const uint8_t masks[] = {0, 1, 3, 7, 15, 31, 63, 127 };
      uint8_t yMax = (imageVars.yDim % 8 == 0 ? imageVars.yDim / 8 : (imageVars.yDim / 8) + 1);

Serial.print(imageVars.yDim % 8);
Serial.print(" ");
Serial.println(masks[imageVars.yDim % 8]);

      uint8_t yMax_Mask = (imageVars.yDim % 8 == 0 ? 255 : masks[imageVars.yDim % 8]);

      Serial.println(F("const uint8_t picture[] PROGMEM = {"));

      for (uint8_t y = 0; y < yMax; y++) {

        for (uint8_t x = 0; x < imageVars.xDim; x++) {

          if (y < yMax - 1) {
            Serial.print(imageVars.image[y][x]);
          }
          else {
            Serial.print(imageVars.image[y][x] & yMax_Mask);
          }
          Serial.print(F(","));

        }

        if (y < (imageVars.yDim / 2) - 1) Serial.println(F("")); 
      }
        
      Serial.println(F(" };"));

      }
      break;

  }

}


// -----------------------------------------------------------------------------
//  Increase the tempo ..

uint16_t getTempo_Above(uint16_t tempo) {

  for (uint8_t x = 0; x < 7; x++) {

    if (tempo == pgm_read_byte(&tempos[x])) {
      
      return (x < 6 ? pgm_read_byte(&tempos[x + 1]) : pgm_read_byte(&tempos[x])); 

    }

  }

  return tempo;

}



// -----------------------------------------------------------------------------
//  Decrease the tempo ..

uint16_t getTempo_Below(uint16_t tempo) {

  for (uint8_t x = 0; x < 7; x++) {

    if (tempo == pgm_read_byte(&tempos[x])) {
      
      return (x > 0 ? pgm_read_byte(&tempos[x - 1]) : pgm_read_byte(&tempos[x])); 

    }

  }

  return tempo;

}
