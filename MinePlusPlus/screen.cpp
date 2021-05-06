#include "includes.h"

byte animationFrame2;
byte animationFrame3;
byte animationFrame4;
const uint16_t animationFramePeriod = 5000;

void Screen::renderBitmap (const byte bitmap[], uint8_t rows, uint8_t columns, byte xPixel, byte yPixel) const {
  const byte bitMasks[8] {
    B10000000, B1000000, B100000, B10000, B1000, B100, B10, B1
  };
  for (int rowIndex = 0; rowIndex < rows; ++rowIndex) {
    for (int columnIndex = 0; columnIndex < columns; ++columnIndex) {
      byte byteToRender = pgm_read_byte_near(bitmap + (rowIndex * columns + columnIndex));
      for (int bitIndex = 0; bitIndex < 8; ++bitIndex) {
        GLCD.SetDot(xPixel + (columnIndex * 8) + bitIndex, yPixel + rowIndex, ((byteToRender & bitMasks[bitIndex]) ? BLACK : WHITE));
      }
    }
  }
}
void Screen::renderBlock (xcoord_t x, ycoord_t y, int8_t xPixelOffset, int8_t yPixelOffset) const {
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
    id = Blocks::air;
  else
    id = block.get(x, y);
  const byte* bitmapToRender = idToBitmap(id);
#ifdef RENDER_BLOCK_LOGGING
  cout << prefix << F("Rendering block at relative coordinates (") << xRel << F(", ") << yRel << F(")   \t[Top-left corner at pixel (") << xPixel << F(", ") << yPixel << F("),\tID: ") << id << F("]") << endl;
#endif
  ycoord_t yPixelOffsetForLoop = 0;
  for (uint8_t i{0}; i < 20; ++i) {
    for (uint8_t j{0}; j < 5; ++j) {
      int16_t specificPixelXCoord = (i % 2 ? j + 5 : j) + xPixel;
      int8_t specificPixelYCoord = yPixelOffsetForLoop + yPixel;
      byte byteToRender = pgm_read_byte_near(bitmapToRender + i);
      if (specificPixelXCoord >= 0 && specificPixelXCoord <= 127 && specificPixelYCoord >= 0 && specificPixelYCoord <= 63)
        GLCD.SetDot(specificPixelXCoord, specificPixelYCoord, ((byteToRender & bitMasks[j]) ? BLACK : WHITE));
    }
    if (i % 2)
      yPixelOffsetForLoop++;
  }
}
void Screen::renderBlock (const CoordPair& coords, int8_t xPixelOffset, int8_t yPixelOffset) const {
  renderBlock(coords.x, coords.y, xPixelOffset, yPixelOffset);
}
void Screen::renderWorld (bool reRenderAnimatedBlocks) {
  static id_t oldRenderMap[13][7];
#ifdef RENDER_LOGGING
  cout << prefix << F("Rendering World") << endl;
#endif
  for (int8_t xIndex = 0; xIndex <= 6; ++xIndex) {
    for (int8_t yIndex = 0; yIndex <= 3; ++yIndex) {
      for (int8_t ySign = 0; ySign <= 1; ++ySign) {
        for (int8_t xSign = 0; xSign <= 1; ++xSign) {
          xcoord_t relX = xSign ? xIndex : -xIndex;
          ycoord_t relY = ySign ? yIndex : -yIndex;
          CoordPair blockCoords = player.getCoords(relX, relY);
          id_t blockIDToRender;
          if (blockCoords.x < -xLimit || blockCoords.x > xLimit || blockCoords.y < 0 || blockCoords.y > yLimit)
            blockIDToRender = Blocks::Runtime::empty;
          else
            blockIDToRender = block.get(blockCoords);
          if (blockIDToRender != oldRenderMap[relX + 6][relY + 3]) {
            renderBlock(blockCoords);
            oldRenderMap[relX + 6][relY + 3] = blockIDToRender;
          } else if (reRenderAnimatedBlocks && block.isAnimated(blockIDToRender))
            renderBlock(blockCoords);
        }
      }
    }
  }
#ifdef RENDER_LOGGING
  cout << prefix << F("\tFinished") << endl;
#endif
}
void Screen::forceRenderWorld () {
#ifdef RENDER_LOGGING
  cout << prefix << F("Rendering World") << endl;
#endif
  for (int8_t xIndex = 0; xIndex <= 6; ++xIndex) {
    for (int8_t yIndex = 0; yIndex <= 3; ++yIndex) {
      for (int8_t ySign = 0; ySign <= 1; ++ySign) {
        for (int8_t xSign = 0; xSign <= 1; ++xSign) {
          xcoord_t relX = xSign ? xIndex : -xIndex;
          ycoord_t relY = ySign ? yIndex : -yIndex;
          renderBlock(player.getCoords(relX, relY));
        }
      }
    }
  }
#ifdef RENDER_LOGGING
  cout << prefix << F("\tFinished") << endl;
#endif
}
void Screen::renderWorldOverview (xcoord_t center) const {
  for (uint8_t y = 0; y < 33; ++y) {
    for (uint8_t x = 0; x < 128; ++x) {
      id_t id = block.get(x - 64 + center, 32 - y);
      bool pixelColour = !(block.isAir(id) || id == Blocks::Generation::air || id == Blocks::Runtime::empty);
      GLCD.SetDot(x, y + 16, (pixelColour ? BLACK : WHITE));
    }
  }
}
const byte* Screen::idToBitmap (id_t id, byte version, FunctionCallContext context) {
	using namespace Blocks;
  if (context == FunctionCallContext::Generic) {
    switch (id) {
      case air:       return Textures::Blocks::light_7;
      case fire:      return (animationFrame2 ? Textures::Blocks::fire2 : Textures::Blocks::fire1);
      case dirt:      return Textures::Blocks::dirt;
      case cobblestone:    return Textures::Blocks::cobblestone;
      case lava0:     return Textures::Blocks::lava_0;
      case lava1:     return Textures::Blocks::lava_1;
      case lava2:     return Textures::Blocks::lava_2;
      case lava3:     return Textures::Blocks::lava_3;
      case water0:    return Textures::Blocks::water_0;
      case water1:    return Textures::Blocks::water_1;
      case water2:    return Textures::Blocks::water_2;
      case water3:    return Textures::Blocks::water_3;
      case water4:    return Textures::Blocks::water_4;
      case water5:    return Textures::Blocks::water_5;
      case water6:    return Textures::Blocks::water_6;
      case water7:    return Textures::Blocks::water_7;
      case farmland0:     //Allow to fall through to "return Textures::Blocks::farmland;"
      case farmland1:
      case farmland2:
      case farmland3:     return Textures::Blocks::farmland;
      case wheat0:    return Textures::Blocks::wheat_0;
      case wheat1:    return Textures::Blocks::wheat_1;
      case wheat2:    return Textures::Blocks::wheat_2;
      case wheat3:    return Textures::Blocks::wheat_3;
      case carrot0:   return Textures::Blocks::carrots_0;
      case carrot1:   return Textures::Blocks::carrots_1;
      case carrot2:   return Textures::Blocks::carrots_2;
      case carrot3:   return Textures::Blocks::carrots_3;
      case potato0:   return Textures::Blocks::potatoes_0;
      case potato1:   return Textures::Blocks::potatoes_1;
      case potato2:   return Textures::Blocks::potatoes_2;
      case potato3:   return Textures::Blocks::potatoes_3;
      case coalOre:  return Textures::Blocks::coal_ore;
      case ironOre:  return Textures::Blocks::iron_ore;
      case goldOre:  return Textures::Blocks::gold_ore;
      case diamondOre:   return Textures::Blocks::diamond_ore;
      case closedDoor:    return (version ? Textures::Blocks::door_closed_top : Textures::Blocks::door_closed_bottom);
      case openDoor:    return (version ? Textures::Blocks::door_open_top : Textures::Blocks::door_open_bottom);
      case closedTrapdoor:    return Textures::Blocks::trapdoor_closed;
      case openTrapdoor:    return Textures::Blocks::trapdoor_open;
      case stone:     return Textures::Blocks::stone;
      case sandstone:    return Textures::Blocks::sandstone;
      case gravel:    return Textures::Blocks::gravel;
      case sand:      return Textures::Blocks::sand;
      case wood:      return Textures::Blocks::wood;
      case planks:    return Textures::Blocks::planks;
      case leaves:    return Textures::Blocks::leaves;
      case obsidian:  return Textures::Blocks::obsidian;
      case glass:     return Textures::Blocks::glass;
      case stoneBricks:    return Textures::Blocks::bricks;
      case wool:      return Textures::Blocks::wool;
      case goldBlock: return Textures::Blocks::gold_block;
      case bed:       return (version ? Textures::Blocks::bed : Textures::Blocks::bed);
      case ladder:    return Textures::Blocks::ladder;
      case torch:     if (version == 0) return Textures::Blocks::torch_straight; else if (version == 1) return Textures::Blocks::torch_left; return Textures::Blocks::torch_right;
      case craftingTable:     return Textures::Blocks::crafting_table;
      case furnace:   return Textures::Blocks::furnace;
      case chest:     return Textures::Blocks::chest;
      case grass:     return Textures::Blocks::grass;
      case sapling:   return Textures::Blocks::sapling;
      case flower:    return Textures::Blocks::flower;
      case tnt:     return Textures::Blocks::tnt;
      case waterSource: return Textures::Blocks::water_7;
      case lavaSource: return Textures::Blocks::lava_3;
      case dryFarmland: return Textures::Blocks::dryFarmland;

      case Runtime::light0:    return Textures::Blocks::light_0;
      case Runtime::light1:    return Textures::Blocks::light_1;
      case Runtime::light2:    return Textures::Blocks::light_2;
      case Runtime::light3:    return Textures::Blocks::light_3;
      case Runtime::light4:    return Textures::Blocks::light_4;
      case Runtime::light5:    return Textures::Blocks::light_5;
      case Runtime::light6:    return Textures::Blocks::light_6;
      case Runtime::light7:    return Textures::Blocks::light_7;
      default:          return Textures::Blocks::error;
    }
  } else if (context == FunctionCallContext::Generation) {
    switch (id) {
      case Generation::air:     return Textures::Blocks::light_7;
      default:          return Textures::Blocks::error;
    }
  } else if (context == FunctionCallContext::Update) {
    switch (id) {
      default:          return Textures::Blocks::error;
    }
  }
  return Textures::Blocks::error;
}
void Screen::forceUpdateAnimations () {
  animationFrame2 = (animationFrame2 + 1) % 2;
  animationFrame3 = (animationFrame2 + 1) % 3;
  animationFrame4 = (animationFrame2 + 1) % 4;
}
void Screen::updateAnimations () {
  static uint32_t lastUpdateTime;
  if (millis() - lastUpdateTime >= animationFramePeriod) {
    forceUpdateAnimations();
    lastUpdateTime = millis();
    renderWorld(true);
  }
}
