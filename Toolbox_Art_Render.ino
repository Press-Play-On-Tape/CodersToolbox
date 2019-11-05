#include "src/utils/Arduboy2Ext.h"
#include "src/images/Images.h"



// ----------------------------------------------------------------------------
//  Render the state .. 
//

#define xOffset 35
#define previewXOffset 8
#define previewYOffset 46

void artScreen_Render() {

  Sprites::drawOverwrite(7, 3, Images::Frame_Small, 0);
  Sprites::drawSelfMasked(8, 4, Images::Art_Icon, 0);


  // Render coordinates ..

  arduboy.fillRect(0, 26, 31, 7, WHITE);
  arduboy.drawFastVLine(32, 0, HEIGHT, WHITE);

  font3x5.setCursor(1 + (imageVars.x < 10 && imageVars.y < 10 ? 3 : 0) + ((imageVars.x < 10 && imageVars.y >= 10) || (imageVars.x >= 10 && imageVars.y < 10) ? 2 : 0), 26);
  font3x5.setTextColor(BLACK);
  font3x5.print("X~");
  font3x5.print(imageVars.x);
  font3x5.print(":Y~");
  font3x5.print(imageVars.y);


  Sprites::drawSelfMasked(3, 36, Images::Preview, 0);


  // Render preview window ..

  arduboy.drawRect(previewXOffset - 2, previewYOffset - 2, imageVars.xDim + 4, imageVars.yDim + 4, WHITE);


  for (uint8_t y = 0; y < imageVars.yDim; y++) {

    arduboy.drawPixel(xOffset - 1, (y * 4) + 3, WHITE);

    for (uint8_t x = 0 ; x < imageVars.xDim; x++) {
      
      uint8_t z = imageVars.image[y / 8][x];

      if (z & (1 << (y % 8))) {

        Sprites::drawSelfMasked(xOffset + (x*4), y * 4, Images::Block, 0);
        arduboy.drawPixel(previewXOffset + x, previewYOffset + y, WHITE);

      }

      arduboy.drawPixel(xOffset + (x*4) + 3, (y * 4) + 3, WHITE);

    }

  }

  /*
  arduboy.drawHorizontalDottedLine(xOffset -1, xOffset - 1 + (imageVars.xDim * 4), (imageVars.y * 4) - 1);
  arduboy.drawHorizontalDottedLine(xOffset -1, xOffset - 1 + (imageVars.xDim * 4), (imageVars.y * 4) + 3);

  arduboy.drawVerticalDottedLine(1, (imageVars.yDim * 4), xOffset + (imageVars.x * 4) - 1);
  arduboy.drawVerticalDottedLine(1, (imageVars.yDim * 4), xOffset + (imageVars.x * 4) + 3);

  // sides
  arduboy.drawFastVLine(xOffset - 1, imageVars.y * 4, 3, WHITE);
  arduboy.drawFastVLine(xOffset - 1 + (imageVars.xDim * 4), imageVars.y * 4, 3, WHITE);
  arduboy.drawFastHLine(xOffset + (imageVars.x * 4), (imageVars.yDim * 4), 3, WHITE);
  */

  if (arduboy.getFrameCountHalf(64)) {
    arduboy.drawRect(xOffset + (imageVars.x * 4) - 1, (imageVars.y * 4) - 1, 5, 5, WHITE);
  }
  else {
    Sprites::drawSelfMasked(xOffset + (imageVars.x * 4) - 1, (imageVars.y * 4) - 1, Images::Block_Highlight, 0);
  }

  Sprites::drawOverwrite(102, 51, Images::HoldB, 0);


  // Render pop-up menu ..

  if (imageVars.menuCounter == MENU_DELAY) {

    arduboy.fillRect(53, 0, WIDTH - 53, HEIGHT, BLACK);
    arduboy.fillRect(54, 0, WIDTH - 54, 8, WHITE);
    arduboy.drawRect(54, 0, WIDTH - 54, HEIGHT, WHITE);

    Sprites::drawErase(117, 2, Images::Arrow_Left, 0);
    Sprites::drawErase(121, 2, Images::Arrow_Right, 0);
    
    font3x5.setCursor(56, 1);
    font3x5.setTextColor(BLACK);
    font3x5.print(F("TOOLBOX"));
    font3x5.setCursor(60, 11);
    font3x5.setTextColor(WHITE);

    switch (menu.image.page) {

      case 0:

        font3x5.print(F("Save to EEPROM\n"));

        uint8_t c;
        EEPROM.get(IMAGE_EEPROM, c);

        if (c == 'c') { 
          font3x5.print(F("Load from EEPROM\n")); 
          font3x5.print(F("Clear EEPROM\n"));
        }
        else {
          font3x5.print(F("\n\n"));
        }

        font3x5.setCursor(60, 39);
        font3x5.print(F("Clear Image\n")); 
        font3x5.print(F("Invert Image\n"));

        arduboy.drawFastVLine(57, pgm_read_byte(&yPos_2[menu.image.firstIndex]), 5);
        break;

      case 1:

        font3x5.print(F("Size X: "));
        
        if (menu.image.mode == MenuMode::XDim) {
          Sprites::drawOverwrite(87, 12, Images::Arrow_Left, 0);
          Sprites::drawOverwrite(114, 12, Images::Arrow_Right, 0);
          arduboy.fillRect(92, 11, 20, 7, WHITE);
  
          font3x5.setCursor(96, 11);
          font3x5.setTextColor(BLACK);
          font3x5.print(imageVars.xDim);
        }
        else {
          font3x5.print(imageVars.xDim);
          font3x5.print(F(" px"));
        }

        font3x5.setTextColor(WHITE);
        font3x5.setCursor(60, 19);
        font3x5.print(F("Size Y: "));
        
        if (menu.image.mode == MenuMode::YDim) {
          Sprites::drawOverwrite(87, 20, Images::Arrow_Left, 0);
          Sprites::drawOverwrite(114, 20, Images::Arrow_Right, 0);
          arduboy.fillRect(92, 19, 20, 7, WHITE);
  
          font3x5.setCursor(96, 19);
          font3x5.setTextColor(BLACK);
          font3x5.print(imageVars.yDim);
        }
        else {
          font3x5.print(imageVars.yDim);
          font3x5.print(F(" px"));
        }

        font3x5.setTextColor(WHITE);
        font3x5.setCursor(60, 31);
        font3x5.print(F("Export to Serial\nReturn to Menu"));

        arduboy.drawFastVLine(57, pgm_read_byte(&yPos[menu.image.secondIndex]), 5);
        break;

    }

  }

}
