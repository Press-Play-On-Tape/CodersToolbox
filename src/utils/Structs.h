#pragma once

#include "Constants.h"
#include "Arduboy2Ext.h"

struct Note {
  uint16_t freq;
  uint16_t duration;
  uint8_t getDurationInterval(uint16_t noteLength) { return (duration / noteLength); }
  uint8_t getDurationIndex(uint16_t noteLength) { return (duration / noteLength) - 1; }
};

union overlayData {
  Note notes[NUMBER_OF_NOTES] = {};
  uint8_t image[2][16];
};

enum GameState : uint8_t {
  SplashScreen,
  TitleScreen,
  Composer,
  Designer
};

enum MenuMode : uint8_t {
  Tempo,
  Range,
  XDim,
  YDim,
  Nothing
};

struct MusicVars {
  uint8_t hasBeenSaved;
  Note notes[NUMBER_OF_NOTES] = {};
  uint8_t x = 0;
  uint16_t y = 0;
  uint8_t menuCounter = 0;
  uint8_t restCounter = 0;
  uint16_t noteLength = 125;
  uint8_t range = 3;
};

struct ImageVars {
  uint8_t hasBeenSaved;
  uint8_t image[2][16];
  uint8_t x = 0;
  uint16_t y = 0;
  uint8_t xDim = 16;
  uint8_t yDim = 16;
  uint8_t menuCounter = 0;
};

struct Menu {
  uint8_t page = 0;
  uint8_t firstIndex = 0;
  uint8_t secondIndex = 0;
  MenuMode mode = MenuMode::Nothing;
};

struct MenuVars {
  Menu music;
  Menu image;
};
