#include "src/utils/Arduboy2Ext.h"
#include "src/images/Images.h"

uint8_t counter = 0;

// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
void splashScreen_Update() { 

	auto justPressed = arduboy.justPressedButtons();

  if (justPressed > 0 && counter > 0) {

    counter = 124;

  }

  if (justPressed > 0 && counter == 0) {

    counter = 1;

  }

  if (counter > 0) {

    counter++;

    if (counter == 125) gameState = GameState::TitleScreen; 

  }

}


// ----------------------------------------------------------------------------
//  Render the state .. 
//
void splashScreen_Render() {

  Sprites::drawOverwrite(47, 17, Images::Ppot_Buttons, 0);
  Sprites::drawOverwrite(43, 26, Images::Ppot_ButtonUp, 0);
  Sprites::drawOverwrite(73, 26, Images::Ppot_ButtonUp, 0);

  if (counter == 0) {

    Sprites::drawOverwrite(58, 26, Images::Ppot_ButtonUp, 0);
    Sprites::drawOverwrite(26, 46, Images::Ppot_Caption, 0);

  }
  else {

    Sprites::drawOverwrite(58, 26, Images::Ppot_ButtonDown, 0);
    Sprites::drawOverwrite(44, 46, Images::Ppot_Loading, 0);

    uint8_t i = (counter / 15) % 4;

    for (uint8_t j = 0; j < i; j++) {
      
        arduboy.drawPixel(79 + (j * 2), 49, WHITE);

    }

  }

}
