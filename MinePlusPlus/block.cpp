#include "includes.h"

id_t* blockDB;

id_t Block::get (xcoord_t x, ycoord_t y) {
#ifdef BLOCK_GET_LOGGING
  cout << prefix << F("Getting block at (") << x << F(", ") << y << ')' << endl;
#endif
  if (x < -xLimit || x > xLimit || y < 0 || y > yLimit)
    return Blocks::Runtime::empty;
  return blockDB[coordsToAddress(x, y)];
}
id_t Block::get (const CoordPair& coords) {
  return get(coords.x, coords.y);
}
void Block::set (xcoord_t x, ycoord_t y, id_t id) {
  blockDB[coordsToAddress(x, y)] = id;
  world.lightingUpdateNeeded = true;
#ifdef BLOCK_SET_LOGGING
  cout << prefix << F("Set block at (") << x << F(", ") << y << F(") with ID: ") << id << endl;
#endif
}
void Block::set (const CoordPair& coords, id_t id) {
  set(coords.x, coords.y, id);
}

bool Block::isMineable (id_t id) {
  if (block.isAir(id))
    return false;
  if (block.isWater(id))
    return false;
  if (block.isLava(id))
    return false;
  if (id == Blocks::tnt)
    return false;
  return true;
}
bool Block::dropsItem (id_t id, id_t toolUsed) {
  const id_t cobblestone[TT_ARRAY_SIZE] {Items::woodPickaxe, Items::stonePickaxe, Items::goldPickaxe, Items::ironPickaxe, Items::diamondPickaxe};
  const id_t coalOre[TT_ARRAY_SIZE] {Items::woodPickaxe, Items::stonePickaxe, Items::goldPickaxe, Items::ironPickaxe, Items::diamondPickaxe};
  const id_t ironOre[TT_ARRAY_SIZE] {Items::stonePickaxe, Items::goldPickaxe, Items::ironPickaxe, Items::diamondPickaxe};
  const id_t goldOre[TT_ARRAY_SIZE] {Items::ironPickaxe, Items::diamondPickaxe};
  const id_t diamondOre[TT_ARRAY_SIZE] {Items::ironPickaxe, Items::diamondPickaxe};
  const id_t stone[TT_ARRAY_SIZE] {Items::woodPickaxe, Items::stonePickaxe, Items::goldPickaxe, Items::ironPickaxe, Items::diamondPickaxe};
  const id_t sandstone[TT_ARRAY_SIZE] {Items::woodPickaxe, Items::stonePickaxe, Items::goldPickaxe, Items::ironPickaxe, Items::diamondPickaxe};
  const id_t obsidian[TT_ARRAY_SIZE] {Items::diamondPickaxe};
  const id_t stoneBricks[TT_ARRAY_SIZE] {Items::woodPickaxe, Items::stonePickaxe, Items::goldPickaxe, Items::ironPickaxe, Items::diamondPickaxe};
  const id_t goldBlock[TT_ARRAY_SIZE] {Items::ironPickaxe, Items::diamondPickaxe};
  const id_t furnace[TT_ARRAY_SIZE] {Items::woodPickaxe, Items::stonePickaxe, Items::goldPickaxe, Items::ironPickaxe, Items::diamondPickaxe};
  if (!isMineable(id))
    return false;
  switch (id) {
    case Blocks::fire:
      return false;
    case Blocks::cobblestone:
      return scanToolTable(cobblestone, toolUsed);
    case Blocks::coalOre:
      return scanToolTable(coalOre, toolUsed);
    case Blocks::ironOre:
      return scanToolTable(ironOre, toolUsed);
    case Blocks::goldOre:
      return scanToolTable(goldOre, toolUsed);
    case Blocks::diamondOre:
      return scanToolTable(diamondOre, toolUsed);
    case Blocks::stone:
      return scanToolTable(stone, toolUsed);
    case Blocks::sandstone:
      return scanToolTable(sandstone, toolUsed);
    case Blocks::obsidian:
      return scanToolTable(obsidian, toolUsed);
    case Blocks::stoneBricks:
      return scanToolTable(stoneBricks, toolUsed);
    case Blocks::goldBlock:
      return scanToolTable(goldBlock, toolUsed);
    case Blocks::furnace:
      return scanToolTable(furnace, toolUsed);
    default:
      return true;
  }
}
bool Block::isAnimated(id_t id) {
  return id == Blocks::fire;
}

bool Block::place (const CoordPair& coords, id_t blockType) {
  return true;
}

bool Block::isOpaque(id_t id) {
  return true;
}
bool Block::isSolid(id_t id) {
  using namespace Blocks;
  if (block.isWater(id) || block.isLava(id) || block.isAir(id))
    return false;
  if (block.isFarmland(id))
    return true;
  id_t otherSolidBlocks[] {dirt, cobblestone, coalOre, ironOre, goldOre, diamondOre, closedDoor, closedTrapdoor, stone, sandstone, gravel, sand, wood, planks, leaves, obsidian, glass, stoneBricks, wool, goldBlock, sponge, tnt};
  for (byte i = 0; i < 22; i++)
    if (otherSolidBlocks[i] == id)
      return true;
  return false;
}
bool Block::isWater(id_t id) {
  return (id >= Blocks::water0 && id <= Blocks::water7) || id == Blocks::waterSource || isDeletedWater(id);
}
bool Block::isDeletedWater(id_t id) {
  return id >= Blocks::Update::deletedWater0 && id <= Blocks::Update::deletedWater7;
}
bool Block::isLava(id_t id) {
  return (id >= Blocks::lava0 && id <= Blocks::lava3) || id == Blocks::lavaSource || isDeletedLava(id);
}
bool Block::isDeletedLava(id_t id) {
  return id >= Blocks::Update::deletedLava0 && id <= Blocks::Update::deletedLava3;
}
bool Block::isFlammable(id_t id) {
  using namespace Blocks;
  id_t flammableBlocks[] {closedDoor, openDoor, closedTrapdoor, openTrapdoor, wood, planks, leaves, wool, ladder, grass, sapling, flower};
  for (byte i = 0; i < 12; i++)
    if (flammableBlocks[i] == id)
      return true;
  return false;
}
bool Block::isFarmland(id_t id) {
  return id >= Blocks::farmland0 && id <= Blocks::farmland3;
}
bool Block::isAir(id_t id) {
  return (id >= Blocks::Runtime::light0 && id <= Blocks::Runtime::light7) || id == Blocks::air;
}
bool Block::isLight(id_t id) {
  return (id >= Blocks::Runtime::light0 && id <= Blocks::Runtime::light7);
}
bool Block::isBrokenByFluid(id_t id) {
  using namespace Blocks;
  const id_t fluidBreakableBlocks[] {torch, grass, flower};
  if (isAir(id))
    return true;
  for (byte i = 0; i < 3; i++)
    if (fluidBreakableBlocks[i] == id)
      return true;
  return (id >= Blocks::water0 && id <= Blocks::water6); //Check for water(0-6) if array scan returns false
}
bool Block::isBrokenByFallingBlocks(id_t id) {
  using namespace Blocks;
  const id_t fallingBlockBreakableBlocks[] {torch, grass, flower};
  if (isAir(id))
    return true;
  for (byte i = 0; i < 3; i++)
    if (fallingBlockBreakableBlocks[i] == id)
      return true;
  return false;
}

byte Block::isTouching(xcoord_t x, ycoord_t y, id_t id) {
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
byte Block::isTouching(const CoordPair& coords, id_t id) {
  return isTouching(coords.x, coords.y, id);
}
byte Block::isTouchingWide(xcoord_t x, ycoord_t y, id_t id) {
  id_t count{0};
  for (int8_t xOffset = -1; xOffset <= 1; xOffset++) {
    for (int8_t yOffset = -1; yOffset <= 1; yOffset++) {
      if (x + xOffset >= -xLimit && x + xOffset <= xLimit && y + yOffset >= 0 && y + yOffset <= yLimit)
        if (get(x + xOffset, y + yOffset) == id)
          count++;
    }
  }
  return count;
}
byte Block::isTouchingWide(const CoordPair& coords, id_t id) {
  return isTouchingWide(coords.x, coords.y, id);
}
uint16_t Block::isNear(xcoord_t x, ycoord_t y, id_t id, byte distance, MeasurementType measurementType) {
  if (x < -xLimit || x > xLimit) {
    cout << XCOORD_OOB;
    return 0;
  }
  if (y < 0 || y > yLimit) {
    cout << XCOORD_OOB;
    return 0;
  }
#ifdef BLOCK_NEAR_TOUCHING_LOGGING
  cout << prefix << F("Block::isNear Started with args ") << x << F(", ") << y << F(", ") << id << F(", ") << distance << F(", ") << (measurementType == Chebyshev ? F("Chebyshev") : F("Taxicab")) << endl;
#endif

  uint16_t count{0};
  if (distance < 2 || distance > 10) {
    cout << PARAM_OOB;
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
uint16_t Block::isNear(const CoordPair& coordPair, id_t id, byte distance, MeasurementType measurementType) {
  return isNear(coordPair.x, coordPair.y, id, distance, measurementType);
}

bool Block::isOpenToSky(xcoord_t x, ycoord_t y, id_t ignoreBlock1, id_t ignoreBlock2) {
  for (ycoord_t yIndex = y + 1; yIndex <= yLimit; yIndex++) {
    id_t idToTest = get(x, yIndex);
    if (!isAir(idToTest) && idToTest != ignoreBlock1 && idToTest != ignoreBlock2)
      return false;
  }
  return true;
}

id_t Block::convertToDeleted (id_t id) {
  using namespace Blocks;
  switch (id) {
    case water0: return Update::deletedWater0;
    case water1: return Update::deletedWater1;
    case water2: return Update::deletedWater2;
    case water3: return Update::deletedWater3;
    case water4: return Update::deletedWater4;
    case water5: return Update::deletedWater5;
    case water6: return Update::deletedWater6;
    case water7: return Update::deletedWater7;

    case lava0: return Update::deletedLava0;
    case lava1: return Update::deletedLava1;
    case lava2: return Update::deletedLava2;
    case lava3: return Update::deletedLava3;

    default: return Runtime::error;
  }
}
id_t Block::convertFromDeleted (id_t id) {
  using namespace Blocks;
  switch (id) {
    case Update::deletedWater0: return water0;
    case Update::deletedWater1: return water1;
    case Update::deletedWater2: return water2;
    case Update::deletedWater3: return water3;
    case Update::deletedWater4: return water4;
    case Update::deletedWater5: return water5;
    case Update::deletedWater6: return water6;
    case Update::deletedWater7: return water7;
    default: return Runtime::error;
  }
}
void Block::createBlockDB (worldWidth_t width, worldHeight_t height) {
  uint16_t totalSize = width * height;
  blockDB = new id_t[totalSize];
#ifdef BLOCK_DB_CREATION_LOGGING
  cout << prefix << F("Created blockDB with dimensions: ") << width << F(", ") << height << endl;
#endif
}

blockDBAddress_t Block::coordsToAddress (xcoord_t x, ycoord_t y) {
  return (worldWidth * y) + x + ((worldWidth - 1) / 2);
}

bool Block::scanToolTable (const id_t table[TT_ARRAY_SIZE], const id_t& tool) {
  for (byte i = 0; i < TT_ARRAY_SIZE; i++)
    if (table[i] == tool)
      return true;
  return false;
}

Block block;
