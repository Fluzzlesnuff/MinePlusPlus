#include "includes.h"

id_t oldRenderMap[13][7];

void Screen::renderBitmap (const byte bitmap[], uint8_t rows, uint8_t columns, byte xPixel, byte yPixel) {
  const byte bitMasks[8] {
    B10000000, B1000000, B100000, B10000, B1000, B100, B10, B1
  };
  for (int rowIndex = 0; rowIndex < rows; rowIndex++) {
    for (int columnIndex = 0; columnIndex < columns; columnIndex++) {
      byte byteToRender = pgm_read_byte_near(bitmap + (rowIndex * columns + columnIndex));
      for (int bitIndex = 0; bitIndex < 8; bitIndex++) {
        GLCD.SetDot(xPixel + (columnIndex * 8) + bitIndex, yPixel + rowIndex, ((byteToRender & bitMasks[bitIndex]) ? BLACK : WHITE));
      }
    }
  }
}
void Screen::renderBlock (xcoord_t x, ycoord_t y, int8_t xPixelOffset = 0, int8_t yPixelOffset = 0) {
  id_t id;
  int8_t xRel = x - player.getCoords().x;
  int8_t yRel = y - player.getCoords().y;
  int8_t xPixelLocations[] = { -1, 9, 19, 29, 39, 49, 59, 69, 79, 89, 99, 109, 119};
  int8_t yPixelLocations[] = { -3, 7, 17, 27, 37, 47, 57};
  int8_t xPixel = xPixelLocations[xRel + 6];
  int8_t yPixel = yPixelLocations[(-yRel) + 3];
  const byte bitMasks[] {
    B10000, B01000, B00100, B00010, B00001
  };
  if (x < -xLimit || x > xLimit || y < 0 || y > yLimit)
    id = B_AIR;
  else
    id = block.get(x, y);
  byte* bitmapToRender = idToBitmap(id);
#ifdef RENDER_BLOCK_LOGGING
  com.out.log("Rendering block at relative coordinates (" + String(xRel) + ", " + String(yRel) + ")   \t[Top-left corner at pixel (" + String(xPixel) + ", " + String(yPixel) + "),\tID: " + String(id) + "].");
#endif
  ycoord_t yPixelOffsetForLoop = 0;
  for (index_t i{0}; i < 20; i++) {
    for (index_t j{0}; j < 5; j++) {
      int8_t specificPixelXCoord = (i % 2 ? j + 5 : j) + xPixel;
      int8_t specificPixelYCoord = yPixelOffsetForLoop + yPixel;
      byte byteToRender = pgm_read_byte_near(bitmapToRender + i);
      if (specificPixelXCoord >= 0 && specificPixelXCoord <= 127 && specificPixelYCoord >= 0 && specificPixelYCoord <= 63)
        GLCD.SetDot(specificPixelXCoord, specificPixelYCoord, ((byteToRender & bitMasks[j]) ? BLACK : WHITE));
    }
    if (i % 2)
      yPixelOffsetForLoop++;
  }
}

void Screen::renderBlock (CoordPair coords, int8_t xPixelOffset = 0, int8_t yPixelOffset = 0) {
  renderBlock(coords.x, coords.y, xPixelOffset, yPixelOffset);
}

void Screen::renderWorld () {
#ifdef RENDER_LOGGING
  com.out.log(F("Rendering World"));
#endif
  for (int8_t xIndex = 0; xIndex <= 6; xIndex++) {
    for (int8_t yIndex = 0; yIndex <= 3; yIndex++) {
      for (int8_t ySign = 0; ySign <= 1; ySign++) {
        for (int8_t xSign = 0; xSign <= 1; xSign++) {
          xcoord_t relX = xSign ? xIndex : -xIndex;
          ycoord_t relY = ySign ? yIndex : -yIndex;
          CoordPair blockCoords = player.getCoords(relX, relY);
          id_t blockIDToRender;
          if (blockCoords.x < -xLimit || blockCoords.x > xLimit || blockCoords.y < 0 || blockCoords.y > yLimit)
            blockIDToRender = C_VOID;
          else
            blockIDToRender = block.get(blockCoords);
          if (blockIDToRender != oldRenderMap[relX + 6][relY + 3]) {
            renderBlock(blockCoords);
            oldRenderMap[relX + 6][relY + 3] = blockIDToRender;
          }
        }
      }
    }
  }
#ifdef RENDER_LOGGING
  com.out.log(F("\tFinished"));
#endif
}

id_t* Screen::idToBitmap (id_t id) {
  switch (id) {
    case B_AIR:       return Textures::Blocks::light_7;
    case B_FIRE:      if (random() % 2)     return Textures::Blocks::fire1;   return Textures::Blocks::fire2;
    case B_DIRT:      return Textures::Blocks::dirt;
    case B_COBBLE:    return Textures::Blocks::cobblestone;
    case B_LAVA0:     return Textures::Blocks::lava_0;
    case B_LAVA1:     return Textures::Blocks::lava_1;
    case B_LAVA2:     return Textures::Blocks::lava_2;
    case B_LAVA3:     return Textures::Blocks::lava_3;
    case B_STONE:     return Textures::Blocks::stone;
    case B_LEAVES:    return Textures::Blocks::leaves;
    case B_GRAVEL:    return Textures::Blocks::gravel;
    case B_IRON_ORE:  return Textures::Blocks::iron_ore;
    case B_GOLD_ORE:  return Textures::Blocks::gold_ore;
    case B_DIA_ORE:   return Textures::Blocks::diamond_ore;
    case B_COAL_ORE:  return Textures::Blocks::coal_ore;
    case B_SNDSTN:    return Textures::Blocks::sandstone;
    case B_SAND:      return Textures::Blocks::sand;
    case B_PLANKS:    return Textures::Blocks::planks;
    case B_BRICKS:    return Textures::Blocks::bricks;
    case B_LADDER:    return Textures::Blocks::ladder;
    case C_LIGHT0:    return Textures::Blocks::light_0;
    case C_LIGHT1:    return Textures::Blocks::light_1;
    case C_LIGHT2:    return Textures::Blocks::light_2;
    case C_LIGHT3:    return Textures::Blocks::light_3;
    case C_LIGHT4:    return Textures::Blocks::light_4;
    case C_LIGHT5:    return Textures::Blocks::light_5;
    case C_LIGHT6:    return Textures::Blocks::light_6;
    case C_LIGHT7:    return Textures::Blocks::light_7;
    default:          return Textures::Blocks::error;
  }
}
