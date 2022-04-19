#include "src/utils/Arduboy2Ext.h"
#include "src/images/Images.h"



// ----------------------------------------------------------------------------
//  Render the state .. 
//

// #define xOffset 35
// #define previewXOffset 8
// #define previewYOffset 46

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

  arduboy.fillRect(102, 26, 31, 7, WHITE);
  arduboy.drawFastVLine(100, 0, HEIGHT, WHITE);

  Sprites::drawSelfMasked(106, 16, Images::PressBArrow, 0);

  font3x5.setCursor(102, 26);
  font3x5.setTextColor(BLACK);
  font3x5.print("Image ");
  font3x5.print(imageVars.imageIdx + 1);

  Sprites::drawSelfMasked(3, 36, Images::Preview, 0);


  // Render preview window ..

  uint8_t previewXOffset = 8 + ((16 - imageVars.xDim) / 2);
  uint8_t previewYOffset = 46 + ((16 - imageVars.yDim) / 2);
  uint8_t xOffset = 35 + ((64 - (imageVars.xDim * 4)) / 2);
  uint8_t yOffset = (64 - (imageVars.yDim * 4)) / 2;

  arduboy.drawRect(previewXOffset - 2, previewYOffset - 2, imageVars.xDim + 4, imageVars.yDim + 4, WHITE);

  for (uint8_t y = 0; y < imageVars.yDim; y++) {

    arduboy.drawPixel(xOffset - 1, yOffset + (y * 4) + 3, WHITE);

    for (uint8_t x = 0 ; x < imageVars.xDim; x++) {

      arduboy.drawPixel(xOffset + (x*4) - 1, yOffset - 1, WHITE);
      arduboy.drawPixel(xOffset + (x*4) + 3, yOffset - 1, WHITE);
      
      uint8_t z = imageVars.image[imageVars.imageIdx][y / 8][x];

      if (z & (1 << (y % 8))) {

        Sprites::drawSelfMasked(xOffset + (x*4), yOffset + (y * 4), Images::Block, 0);
        arduboy.drawPixel(previewXOffset + x, previewYOffset + y, WHITE);

      }

      arduboy.drawPixel(xOffset + (x*4) + 3, yOffset + (y * 4) + 3, WHITE);

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
    arduboy.drawRect(xOffset + (imageVars.x * 4) - 1, yOffset + (imageVars.y * 4) - 1, 5, 5, WHITE);
  }
  else {
    Sprites::drawSelfMasked(xOffset + (imageVars.x * 4) - 1, yOffset + (imageVars.y * 4) - 1, Images::Block_Highlight, 0);
  }

  Sprites::drawOverwrite(102, 51, Images::HoldB, 0);


  // Render pop-up menu ..

  if (imageVars.menuCounter == MENU_DELAY) {

    arduboy.fillRect(52, 0, WIDTH - 52, HEIGHT, BLACK);
    arduboy.fillRect(53, 0, WIDTH - 53, 8, WHITE);
    arduboy.drawRect(53, 0, WIDTH - 53, HEIGHT, WHITE);

    Sprites::drawErase(117, 2, Images::Arrow_Left, 0);
    Sprites::drawErase(121, 2, Images::Arrow_Right, 0);
    
    font3x5.setCursor(55, 1);
    font3x5.setTextColor(BLACK);
    font3x5.print(F("TOOLBOX"));
    font3x5.setCursor(59, 11);
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

        font3x5.setTextColor(WHITE);
        font3x5.setCursor(59, 39);
        font3x5.print(F("Export to Serial\nReturn to Menu"));

        arduboy.drawFastVLine(56, pgm_read_byte(&yPos_21[menu.image.firstIndex]), 5);
        break;

      case 1:

        font3x5.print(F("Size X: "));
        
        if (menu.image.mode == MenuMode::XDim) {
          Sprites::drawOverwrite(86, 12, Images::Arrow_Left, 0);
          Sprites::drawOverwrite(113, 12, Images::Arrow_Right, 0);
          arduboy.fillRect(91, 11, 20, 7, WHITE);
  
          font3x5.setCursor(95, 11);
          font3x5.setTextColor(BLACK);
          font3x5.print(imageVars.xDim);
        }
        else {
          font3x5.print(imageVars.xDim);
          font3x5.print(F(" px"));
        }

        font3x5.setTextColor(WHITE);
        font3x5.setCursor(59, 19);
        font3x5.print(F("Size Y: "));
        
        if (menu.image.mode == MenuMode::YDim) {
          Sprites::drawOverwrite(86, 20, Images::Arrow_Left, 0);
          Sprites::drawOverwrite(113, 20, Images::Arrow_Right, 0);
          arduboy.fillRect(91, 19, 20, 7, WHITE);
  
          font3x5.setCursor(95, 19);
          font3x5.setTextColor(BLACK);
          font3x5.print(imageVars.yDim);
        }
        else {
          font3x5.print(imageVars.yDim);
          font3x5.print(F(" px"));
        }


        font3x5.setTextColor(WHITE);
        font3x5.setCursor(59, 31);
        font3x5.print(F("Copy Image\nClear Image\nClear All Images\nInvert Image\n"));
        
        if (menu.image.mode == MenuMode::Copy) {
          font3x5.setCursor(99, 31);
          font3x5.print(F("to"));
          Sprites::drawOverwrite(109, 32, Images::Arrow_Left, 0);
          Sprites::drawOverwrite(123, 32, Images::Arrow_Right, 0);
          arduboy.fillRect(114, 31, 7, 7, WHITE);
  
          font3x5.setCursor(116, 31);
          font3x5.setTextColor(BLACK);
          font3x5.print(imageVars.copy + 1);
        }

        arduboy.drawFastVLine(56, pgm_read_byte(&yPos_22[menu.image.secondIndex]), 5);
        break;

      case 2:
        font3x5.setCursor(57, 11);
        font3x5.print(F("A        Toggle pixel\n"));
        font3x5.print(F("A+      Toggle + move\n"));
        font3x5.print(F("A+      Toggle + move\n"));
        font3x5.print(F("B+      Image up\n"));
        font3x5.print(F("B+      Image down\n"));
        Sprites::drawSelfMasked(67, 19, Images::Arrows_UpDown, 0);
        Sprites::drawSelfMasked(66, 28, Images::Arrows_LeftRight, 0);
        Sprites::drawSelfMasked(67, 35, Images::Arrows_UpDown, 0);
        Sprites::drawSelfMasked(66, 44, Images::Arrows_LeftRight, 0);
        break;

    }

  }

}
