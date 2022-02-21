#include "src/utils/Arduboy2Ext.h"

#include "src/ArduboyTones/ArduboyTones.h"
#include "src/images/Images.h"
#include "src/fonts/Font3x5.h"
#include "src/utils/Structs.h"
#include "src/utils/Constants.h"
#include <EEPROM.h>

Arduboy2Ext arduboy;
ArduboyTones sound(arduboy.audio.enabled);
Font3x5 font3x5;

MusicVars musicVars;
ImageVars imageVars;
MenuVars menu;

GameState gameState = GameState::SplashScreen_Init;


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Setup
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void setup() {

  arduboy.boot(); 
  arduboy.display(); 
  arduboy.flashlight(); 

  arduboy.systemButtons();
  arduboy.audio.begin();
  arduboy.initRandomSeed();
  arduboy.waitNoButtons(); 

  resetAll();

}

/* -----------------------------------------------------------------------------------------------------------------------------
 *  Main loop
 * -----------------------------------------------------------------------------------------------------------------------------
 */

void loop() {

  if (!(arduboy.nextFrame())) return;

  arduboy.clear();
  arduboy.pollButtons();

  switch (gameState) {

    case GameState::SplashScreen_Init:
      splashScreen_Init();
      break;

    case GameState::SplashScreen:
      splashScreen();
      break;

    case GameState::TitleScreen:
      titleScreen_Update();
      titleScreen_Render();
      break;

    case GameState::Composer:
      composer_Update();
      composer_Render();
      break;

    case GameState::Designer:
      artScreen_Update();
      artScreen_Render();
      break;
      
  }

  arduboy.display(true);

}
