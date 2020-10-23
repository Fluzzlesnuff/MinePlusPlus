#include "includes.h"

id_t* blockDB;

id_t Block::get (const xcoord_t x, const ycoord_t y) {
#ifdef BLOCK_GET_LOGGING
  com.out.log("Getting block at (" + String(x) + ", " + String(y) + ").");
#endif
  if (x < -xLimit || x > xLimit)
    com.out.throwError(XCOORD_OOB);
  else if (y < 0 || y > yLimit)
    com.out.throwError(YCOORD_OOB);
  else
    return blockDB[coordsToAddress(x, y)];
}
id_t Block::get (const CoordPair coords) {
  return get(coords.x, coords.y);
}

bool Block::set (const xcoord_t x, const ycoord_t y, const id_t id) {
  blockDB[coordsToAddress(x, y)] = id;
#ifdef BLOCK_SET_LOGGING
  com.out.log("Set block at (" + String(x) + ", " + String(y) + ") with ID: " + id);
#endif
}

bool Block::set (const CoordPair coords, const id_t id) {
  set(coords.x, coords.y, id);
}

bool Block::isMineable (const id_t id) {
  if (block.isAir(id))
    return false;
  if (block.isWater(id))
    return false;
  if (block.isLava(id))
    return false;
  if (id == B_TNT_L)
    return false;
  return true;
}

bool Block::dropsItem (const id_t id, const id_t toolUsed) {
  const id_t cobbleTT[TT_ARRAY_SIZE] {I_PICK_WOOD, I_PICK_STONE, I_PICK_GOLD, I_PICK_IRON, I_PICK_DIA};
  const id_t coal_oreTT[TT_ARRAY_SIZE] {I_PICK_WOOD, I_PICK_STONE, I_PICK_GOLD, I_PICK_IRON, I_PICK_DIA};
  const id_t iron_oreTT[TT_ARRAY_SIZE] {I_PICK_STONE, I_PICK_GOLD, I_PICK_IRON, I_PICK_DIA};
  const id_t gold_oreTT[TT_ARRAY_SIZE] {I_PICK_IRON, I_PICK_DIA};
  const id_t dia_oreTT[TT_ARRAY_SIZE] {I_PICK_IRON, I_PICK_DIA};
  const id_t stoneTT[TT_ARRAY_SIZE] {I_PICK_WOOD, I_PICK_STONE, I_PICK_GOLD, I_PICK_IRON, I_PICK_DIA};
  const id_t sndstnTT[TT_ARRAY_SIZE] {I_PICK_WOOD, I_PICK_STONE, I_PICK_GOLD, I_PICK_IRON, I_PICK_DIA};
  const id_t obsidianTT[TT_ARRAY_SIZE] {I_PICK_DIA};
  const id_t bricksTT[TT_ARRAY_SIZE] {I_PICK_WOOD, I_PICK_STONE, I_PICK_GOLD, I_PICK_IRON, I_PICK_DIA};
  const id_t gold_blockTT[TT_ARRAY_SIZE] {I_PICK_IRON, I_PICK_DIA};
  const id_t furnaceTT[TT_ARRAY_SIZE] {I_PICK_WOOD, I_PICK_STONE, I_PICK_GOLD, I_PICK_IRON, I_PICK_DIA};
  if (!isMineable(id))
    return false;
  switch (id) {
    case B_FIRE:
      return false;
    case B_COBBLE:
      return scanToolTable(cobbleTT, toolUsed);
    case B_COAL_ORE:
      return scanToolTable(coal_oreTT, toolUsed);
    case B_IRON_ORE:
      return scanToolTable(iron_oreTT, toolUsed);
    case B_GOLD_ORE:
      return scanToolTable(gold_oreTT, toolUsed);
    case B_DIA_ORE:
      return scanToolTable(dia_oreTT, toolUsed);
    case B_STONE:
      return scanToolTable(stoneTT, toolUsed);
    case B_SNDSTN:
      return scanToolTable(sndstnTT, toolUsed);
    case B_OBSIDIAN:
      return scanToolTable(obsidianTT, toolUsed);
    case B_BRICKS:
      return scanToolTable(bricksTT, toolUsed);
    case B_GOLD_BLOCK:
      return scanToolTable(gold_blockTT, toolUsed);
    case B_FURNACE:
      return scanToolTable(furnaceTT, toolUsed);
    default:
      return true;
  }
}

bool Block::isAnimated(const id_t id) {
  return id == B_FIRE;
}

bool Block::place (const CoordPair coords, const id_t blockType) {

}

bool Block::isOpaque(const id_t id) {

}
bool Block::isSolid(const id_t id) {
  if (block.isWater(id) || block.isLava(id) || block.isAir(id))
    return false;
  if (block.isFarmland(id))
    return true;
  id_t otherSolidBlocks[] {B_DIRT, B_COBBLE, B_COAL_ORE, B_IRON_ORE, B_GOLD_ORE, B_DIA_ORE, B_DOOR_C, B_TRAP_C, B_STONE, B_SNDSTN, B_GRAVEL, B_SAND, B_WOOD, B_PLANKS, B_LEAVES, B_OBSIDIAN, B_GLASS, B_BRICKS, B_WOOL, B_GOLD_BLOCK, B_SPONGE, B_TNT_U};
  for (byte i = 0; i < 22; i++)
    if (otherSolidBlocks[i] == id)
      return true;
  return false;
}
bool Block::isWater(const id_t id) {
  id_t waterBlocks[] {B_WATER0, B_WATER1, B_WATER2, B_WATER3, B_WATER4, B_WATER5, B_WATER6, B_WATER7};
  for (byte i = 0; i < 8; i++)
    if (waterBlocks[i] == id)
      return true;
  return false;
}
bool Block::isLava(const id_t id) {
  id_t lavaBlocks[] {B_LAVA0, B_LAVA1, B_LAVA2, B_LAVA3};
  for (byte i = 0; i < 4; i++)
    if (lavaBlocks[i] == id)
      return true;
  return false;
}
bool Block::isFlammable(const id_t id) {
  id_t flammableBlocks[] {B_DOOR_C, B_DOOR_O, B_TRAP_C, B_TRAP_O, B_WOOD, B_PLANKS, B_LEAVES, B_WOOL, B_LADDER, B_GRASS, B_SAPLING, B_FLOWER};
  for (byte i = 0; i < 12; i++)
    if (flammableBlocks[i] == id)
      return true;
  return false;
}
bool Block::isFarmland(const id_t id) {
  id_t farmlandBlocks[] {B_FARM0, B_FARM1, B_FARM2, B_FARM3};
  for (byte i = 0; i < 4; i++)
    if (farmlandBlocks[i] == id)
      return true;
  return false;
}
bool Block::isAir(const id_t id) {
  id_t airBlocks[] {B_AIR, C_LIGHT0, C_LIGHT1, C_LIGHT2, C_LIGHT3, C_LIGHT4, C_LIGHT5, C_LIGHT6, C_LIGHT7};
  for (byte i = 0; i < 9; i++)
    if (airBlocks[i] == id)
      return true;
  return false;
}

byte Block::isTouching(const xcoord_t x, const ycoord_t y, const id_t id) {
  // Scans blocks starting from the top and going clockwise
  id_t count{0};
  if (x >= -xLimit && x <= xLimit && y + 1 >= 0 && y + 1 <= yLimit && get(x, y + 1) == id)
    count++;
  if (x + 1 >= -xLimit && x + 1 <= xLimit && y >= 0 && y <= yLimit && get(x + 1, y) == id)
    count++;
  if (x >= -xLimit && x <= xLimit && y - 1 >= 0 && y - 1 <= yLimit && get(x, y - 1) == id)
    count++;
  if (x - 1 >= -xLimit && x - 1 <= xLimit && y >= 0 && y <= yLimit && get(x - 1, y) == id)
    count++;
  return count;
}
byte Block::isTouching(const CoordPair coords, const id_t id) {
  return isTouching(coords.x, coords.y, id);
}
byte Block::isTouchingWide(const xcoord_t x, const ycoord_t y, const id_t id) {
  // Scans blocks starting from the top and going clockwise
  id_t count{0};
  for (int xOffset = -1; xOffset <= 1; xOffset++) {
    for (int yOffset = -1; yOffset <= 1; yOffset++) {
      if (x + xOffset >= -xLimit && x + xOffset <= xLimit && y + yOffset >= 0 && y + yOffset <= yLimit)
        if (get(x + xOffset, y + yOffset) == id)
          count++;
    }
  }
  return count;
}
byte Block::isTouchingWide(const CoordPair coords, const id_t id) {
  return isTouchingWide(coords.x, coords.y, id);
}
uint16_t Block::isNear(const xcoord_t x, const ycoord_t y, const id_t id, const byte distance, const MeasurementType measurementType) {
  if (x < -xLimit || x > xLimit) {
    com.out.throwError(XCOORD_OOB);
    return 0;
  }
  if (y < 0 || y > yLimit) {
    com.out.throwError(XCOORD_OOB);
    return 0;
  }
#ifdef BLOCK_NEAR_TOUCHING_LOGGING
  com.out.log("Block::isNear Started with args " + String(x) + ", " + String(y) + ", " + String(id) + ", " + String(distance) + ", " + (measurementType == Chebyshev ? "Chebyshev" : "Taxicab"));
#endif

  uint16_t count{0};
  if (distance < 2 || distance > 10) {
    com.out.throwError(PARAM_OOB);
    return 0;
  }
  if (measurementType == Chebyshev) {
#ifdef BLOCK_NEAR_TOUCHING_LOGGING
#endif
    for (int8_t i = -distance; i <= distance; i++) {
      for (int8_t j = -distance; j <= distance; j++) {
        if (x + i >= -xLimit && x + i <= xLimit && y + j >= 0 && y + j <= yLimit && !(i == 0 && j == 0)) { //Prevent checking for out-of-bounds blocks.
          if (block.get(x + i, y + j) == id) {
            count++;
          }
        }
      }
    }
  }
  if (measurementType == Taxicab) {
    for (int8_t i = -distance; i <= distance; i++) {
      for (int8_t j = ((i == 0) ? -distance : ((i < 0) ? (-distance - i) : (i - distance))); j <= ((i == 0) ? distance : ((i < 0) ? (distance + i) : (distance - i))); j++) {
        if (x + i >= -xLimit && x + i <= xLimit && y + j >= 0 && y + j <= yLimit && !(i == 0 && j == 0)) { //Prevent checking for out-of-bounds blocks.
          if (block.get(x + i, y + j) == id) {
            count++;
          }
        }
      }
    }
  }
  return count;
}
uint16_t Block::isNear(const CoordPair coordPair, const id_t id, const byte distance, const MeasurementType measurementType) {
  return isNear(coordPair.x, coordPair.y, id, distance, measurementType);
}

void Block::createBlockDB (const worldWidth_t width, const worldHeight_t height) {
  uint16_t totalSize = width * height;
  blockDB = new id_t[totalSize];
#ifdef BLOCK_DB_CREATION_LOGGING
  com.out.log("Created blockDB with dimensions: " + String(width) + ", " + String(height));
#endif
}

blockDBAddress_t Block::coordsToAddress (const xcoord_t x, const ycoord_t y) {
  return (worldWidth * y) + x + ((worldWidth - 1) / 2);
}

bool Block::scanToolTable (const id_t table[TT_ARRAY_SIZE], const id_t& tool) {
  for (byte i = 0; i < TT_ARRAY_SIZE; i++)
    if (table[i] == tool)
      return true;
  return false;
}

Block block;
