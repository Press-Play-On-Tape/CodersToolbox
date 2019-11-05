#include "src/utils/Arduboy2Ext.h"
#include "src/ArduboyTones/ArduboyTones.h"
#include "src/images/Images.h"
#include "src/utils/Constants.h"
#include <EEPROM.h>


void composer_Update() {

  // Get focus note ..

  Note &note = musicVars.notes[musicVars.x];

  if (musicVars.menuCounter < MENU_DELAY) {


    // -----------------------------------------------------------------------------------
    //  A Pressed

    if (arduboy.justPressed(A_BUTTON)) {

      makeNextNote(note);
      musicVars.restCounter = 1;

    }
    else if (arduboy.pressed(A_BUTTON)) {

      if (musicVars.restCounter > 0) musicVars.restCounter++;

      if (arduboy.justPressed(LEFT_BUTTON)) {

        musicVars.restCounter = 0;

        if (note.duration > 1 * musicVars.noteLength) {
          note.duration = note.duration - musicVars.noteLength;
        }

      }
      else if (arduboy.justPressed(RIGHT_BUTTON)) {

        musicVars.restCounter = 0;

        if (note.duration < 8 * musicVars.noteLength)
          note.duration = note.duration + musicVars.noteLength;
      }
      else if (arduboy.justPressed(UP_BUTTON)) {

        if (musicVars.y > 0) {
          note.freq = getNoteAbove(musicVars.range, note.freq);
          musicVars.y = getNoteAbove(musicVars.range, musicVars.y);
          musicVars.restCounter = 0;
          sound.tone(note.freq, NOTE_SAMPLE_DURATION);
        }
        
      }
      else if (arduboy.justPressed(DOWN_BUTTON)) {

        if (musicVars.y > 0) {
          note.freq = getNoteBelow(musicVars.range, note.freq);
          musicVars.y = getNoteBelow(musicVars.range, musicVars.y);
          musicVars.restCounter = 0;
          sound.tone(note.freq, NOTE_SAMPLE_DURATION);
        }

      }
      else if (musicVars.restCounter > REST_DELAY) {

        if (note.freq > 0x8000) {
          note.freq = note.freq & 0x7FFF;
        }
        else {
          note.freq = note.freq | 0x8000;
        }

        musicVars.restCounter = 0;

      }

    }

    if (arduboy.notPressed(A_BUTTON)) {
      musicVars.restCounter = 0;
    }


    // -----------------------------------------------------------------------------------
    //  B Pressed

    if (arduboy.justPressed(B_BUTTON)) {

      musicVars.menuCounter = 1;
      // Serial.println("1 _________________ ");
      // for (uint16_t x = 0; x < NUMBER_OF_NOTES; x++)
      // {
      //   Serial.print(musicVars.notes[x].freq);
      //   Serial.print(",");
      //   Serial.print(musicVars.notes[x].duration);
      //   Serial.print(" ");
      // }
      // Serial.println(" ");

    }
    else if (arduboy.pressed(B_BUTTON)) {

      if (arduboy.justPressed(RIGHT_BUTTON)) {

        musicVars.menuCounter = 0;

        for (uint16_t x = NUMBER_OF_NOTES - 2; x >= musicVars.x; x--) {

          musicVars.notes[x + 1] = musicVars.notes[x];

        }

        musicVars.notes[musicVars.x].freq = musicVars.notes[musicVars.x + 1].freq;
        musicVars.notes[musicVars.x].duration = musicVars.noteLength;

      }

      else if (arduboy.justPressed(LEFT_BUTTON) && musicVars.x > 0) {

        musicVars.menuCounter = 0;

        for (uint16_t x = musicVars.x; x < NUMBER_OF_NOTES - 1; x++) {

          musicVars.notes[x] = musicVars.notes[x + 1];

        }

        musicVars.notes[NUMBER_OF_NOTES].freq = 0;
        musicVars.notes[NUMBER_OF_NOTES].duration = musicVars.noteLength;

        musicVars.x--;
        musicVars.y = musicVars.notes[musicVars.x].freq;

      }

      else  if (musicVars.menuCounter < MENU_DELAY && arduboy.pressed(B_BUTTON)) {
        musicVars.menuCounter++;
      }

    }

    if (arduboy.notPressed(B_BUTTON)) {
      musicVars.menuCounter = 0;
    }


    // -----------------------------------------------------------------------------------
    //  A + B Not Pressed

    if (arduboy.notPressed(A_BUTTON) && arduboy.notPressed(B_BUTTON)) { 

      if (arduboy.justPressed(LEFT_BUTTON)) {

        if (musicVars.x > 0) {
          musicVars.x--;
          musicVars.y = getFreq(musicVars.x);
        }

      }

      if (arduboy.justPressed(RIGHT_BUTTON)) {

        if ((musicVars.x == 0 && musicVars.notes[musicVars.x].freq != TONES_END) || (musicVars.x > 0 && musicVars.x < NUMBER_OF_NOTES - 2)) {

          musicVars.x++;
          musicVars.y = getFreq(musicVars.x);
          
          if (musicVars.notes[musicVars.x].freq == TONES_END) {

            Note &note = musicVars.notes[musicVars.x];
            makeNextNote(note);

          }
          
        }

        if (musicVars.y == 0) { musicVars.y = getNoteMiddle(musicVars.range); }

      }

      if (arduboy.justPressed(UP_BUTTON)) {

        if ((musicVars.x == 0 && musicVars.notes[musicVars.x].freq != TONES_END) || (musicVars.x < NUMBER_OF_NOTES - 2 && musicVars.notes[musicVars.x + 1].freq == TONES_END)) {

          musicVars.x++;
          musicVars.y = getNoteAbove(musicVars.range, getFreq(musicVars.x));
          Note &note = musicVars.notes[musicVars.x];
          makeNextNote(note);
          
        }

        if (musicVars.y == 0) { musicVars.y = getNoteMiddle(musicVars.range); }

      }

      if (arduboy.justPressed(DOWN_BUTTON)) {

        if ((musicVars.x == 0 && musicVars.notes[musicVars.x].freq != TONES_END) || (musicVars.x < NUMBER_OF_NOTES - 2 && musicVars.notes[musicVars.x + 1].freq == TONES_END)) {

          musicVars.x++;
          musicVars.y = getNoteBelow(musicVars.range, getFreq(musicVars.x));
          Note &note = musicVars.notes[musicVars.x];
          makeNextNote(note);
          
        }

        if (musicVars.y == 0) { musicVars.y = getNoteMiddle(musicVars.range); }

      }

    }

  }
  else {


    // -----------------------------------------------------------------------------------
    //  Menu Controls ..

    switch (menu.music.page) {

      case 0:

        if (arduboy.justPressed(LEFT_BUTTON)) {

          menu.music.page = 3;
          
        }

        if (arduboy.justPressed(RIGHT_BUTTON)) {

          menu.music.page = 1;
          
        }

        if (arduboy.justPressed(UP_BUTTON) && menu.music.firstIndex > 0) {

            menu.music.firstIndex--;

        }

        if (arduboy.justPressed(DOWN_BUTTON) && menu.music.firstIndex < 4) {

          if (menu.music.firstIndex == 2) {

            uint8_t c;
            EEPROM.get(MUSIC_EEPROM, c);

            if (c == 'c') { 
              menu.music.firstIndex++;
            }

          }
          else {
            menu.music.firstIndex++;
          }

        }
        
        if (arduboy.justPressed(B_BUTTON)) {
          musicVars.menuCounter = 0;
        }

        if (arduboy.justPressed(A_BUTTON)) {

          switch (menu.music.firstIndex) {

            case 0:
              sound.tonesInRAM(static_cast<uint16_t *>(&musicVars.notes[0].freq));
              break;

            case 1:
              sound.tonesInRAM(static_cast<uint16_t *>(&musicVars.notes[musicVars.x].freq));
              break;

            case 2:
              musicVars.hasBeenSaved = 'c';
              EEPROM.put(MUSIC_EEPROM, musicVars);
              break;

            case 3:
              {
                uint8_t c;
                EEPROM.get(MUSIC_EEPROM, c);
                if (c == 'c') EEPROM.get(MUSIC_EEPROM, musicVars);
              }
              break;

            case 4:
              EEPROM.put(MUSIC_EEPROM, 'd');
              musicVars.hasBeenSaved = ' ';
              menu.music.firstIndex = 2;
              break;

          }

        }

        break;
  
      case 1:

        if (arduboy.justPressed(LEFT_BUTTON) && (menu.music.mode == MenuMode::Nothing)) {

          menu.music.page = 0;
          
        }

        if (arduboy.justPressed(RIGHT_BUTTON) && menu.music.mode == MenuMode::Nothing) {

          menu.music.page = 2;
          
        }

        if (arduboy.justPressed(UP_BUTTON) && menu.music.secondIndex > 0 && menu.music.mode == MenuMode::Nothing) {

          menu.music.secondIndex--;

        }

        if (arduboy.justPressed(DOWN_BUTTON) && menu.music.secondIndex < 4 && menu.music.mode == MenuMode::Nothing) {

          menu.music.secondIndex++;

        }

        if (menu.music.mode != MenuMode::Nothing && arduboy.justPressed(LEFT_BUTTON)) {

          switch (menu.music.secondIndex) {

            case 0:
              {
                uint16_t noteLength = musicVars.noteLength;
                uint16_t noteLengthAbove = getTempo_Above(noteLength);

                if (noteLength != noteLengthAbove) {

                  musicVars.noteLength = noteLengthAbove;

                  for (uint8_t x = 0; x < NUMBER_OF_NOTES; x++) {

                    musicVars.notes[x].duration = musicVars.notes[x].getDurationInterval(noteLength) * noteLengthAbove;

                  }

                }

              }
              break;

            case 1:
              {
                uint16_t range = musicVars.range;

                if (range > 0) {

                  musicVars.range--;

                  for (uint8_t x = 0; x < NUMBER_OF_NOTES; x++) {

                    if (musicVars.notes[x].freq != TONES_END && musicVars.notes[x].freq != NOTE_REST) {
                      musicVars.notes[x].freq = pgm_read_word( &noteSeq[range - 1][getNoteIndex(range, musicVars.notes[x].freq)] );
                      musicVars.y = getFreq(musicVars.x);
                    }

                  }

                }

              }
              break;            

          }
                    
        }

        if (menu.music.mode != MenuMode::Nothing && arduboy.justPressed(RIGHT_BUTTON)) {

          switch (menu.music.secondIndex) {

            case 0:
              {
                uint16_t noteLength = musicVars.noteLength;
                uint16_t noteLengthBelow = getTempo_Below(noteLength);

                if (noteLength != noteLengthBelow) {

                  musicVars.noteLength = noteLengthBelow;

                  for (uint8_t x = 0; x < NUMBER_OF_NOTES; x++) {

                    musicVars.notes[x].duration = musicVars.notes[x].getDurationInterval(noteLength) * noteLengthBelow;

                  }

                }

              }
              break;

            case 1:
              {
                uint16_t range = musicVars.range;

                if (range < 7) {

                  musicVars.range++;

                  for (uint8_t x = 0; x < NUMBER_OF_NOTES; x++) {

                    if (musicVars.notes[x].freq != TONES_END && musicVars.notes[x].freq != NOTE_REST) {
                      musicVars.notes[x].freq = pgm_read_word(&noteSeq[range + 1][getNoteIndex(range, musicVars.notes[x].freq)]);
                      musicVars.y = getFreq(musicVars.x);
                    }

                  }

                }

              }
              break;            

          }
                    
        }

        if (arduboy.justPressed(A_BUTTON)) {

          if (menu.music.mode == MenuMode::Nothing) {

            switch (menu.music.secondIndex) {

              case 0 ... 1:
                menu.music.mode = static_cast<MenuMode>(menu.music.secondIndex);
                break;

              case 2:
                resetAll();
                break;

              case 3:
                exportToSerial();
                break;

              case 4:
                menu.music.mode = MenuMode::Nothing;
                menu.music.page = 0;
                menu.music.firstIndex = 0;
                menu.music.secondIndex = 0;
                musicVars.menuCounter = 0;
                gameState = GameState::TitleScreen;
                break;

            }

          }
          else {

            menu.music.mode = MenuMode::Nothing;

          }

        }
    
        if (arduboy.justPressed(B_BUTTON)) {
          musicVars.menuCounter = 0;
          menu.music.mode = MenuMode::Nothing;
        }

        break; 

      case 2:

        if (arduboy.justPressed(LEFT_BUTTON)) {

          menu.music.page = 1;
          
        }

        if (arduboy.justPressed(RIGHT_BUTTON)) {

          menu.music.page = 3;
          
        }
        
        if (arduboy.justPressed(B_BUTTON)) {

          musicVars.menuCounter = 0;

        }
        break;

      case 3:

        if (arduboy.justPressed(LEFT_BUTTON)) {

          menu.music.page = 2;
          
        }

        if (arduboy.justPressed(RIGHT_BUTTON)) {

          menu.music.page = 0;
          
        }
        
        if (arduboy.justPressed(B_BUTTON)) {

          musicVars.menuCounter = 0;
          
        }
        break;

    }

  }

}
