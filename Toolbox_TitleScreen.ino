#include "src/utils/Arduboy2Ext.h"
#include "src/images/Images.h"

bool title_index = 0;
uint8_t title_left_arrow = 0;
uint8_t title_right_arrow = 0;


// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
void titleScreen_Update() { 

	auto justPressed = arduboy.justPressedButtons();

  if (title_left_arrow > 0)   title_left_arrow--;
  if (title_right_arrow > 0)  title_right_arrow--;

  if (arduboy.justPressed(LEFT_BUTTON) || arduboy.justPressed(RIGHT_BUTTON)) {

    if (arduboy.justPressed(LEFT_BUTTON)) {

      title_left_arrow = 8;

    }

    if (arduboy.justPressed(RIGHT_BUTTON)) {

      title_right_arrow = 8;

    }

    title_index = !title_index;
    
  }

  if (arduboy.justPressed(A_BUTTON) || arduboy.justPressed(B_BUTTON)) {

    switch (title_index) {

      case 0:
        gameState = GameState::Composer; 
        break;

      case 1:
        gameState = GameState::Designer; 
        break;

    }

    resetAll();

  }

}


// ----------------------------------------------------------------------------
//  Render the state .. 
//
void titleScreen_Render() {

  Sprites::drawOverwrite(8, 8, Images::Toolbox, 0);

  arduboy.drawFastVLine(69, 0, HEIGHT);
  arduboy.drawFastVLine(71, 0, HEIGHT);
  arduboy.drawFastVLine(72, 0, HEIGHT);

  Sprites::drawOverwrite(title_left_arrow > 0 ? 79 : 80, 21, Images::Arrow_Big_Left, 0);
  Sprites::drawOverwrite(title_right_arrow > 0 ? 115 : 114, 21, Images::Arrow_Big_Right, 0);
  Sprites::drawOverwrite(91, 17, Images::Frame_Small, 0);

  switch (title_index) {

    case 0:
      Sprites::drawSelfMasked(92, 18, Images::Music_Icon, 0);
      Sprites::drawSelfMasked(84, 41, Images::Music_Word, 0);
      break;

    case 1:
      Sprites::drawSelfMasked(92, 18, Images::Art_Icon, 0);
      Sprites::drawSelfMasked(90, 41, Images::Art_Word, 0);
      break;
      
  }

}
