#include "includes.h"
bool World::tryUpdate() {
  static uint32_t lastTickTime;
  static uint32_t ticksDone;

  leftmostXCoordinate = max(player.getCoords().x - UPDATE_DISTANCE, -xLimit);
  rightmostXCoordinate = min(player.getCoords().x + UPDATE_DISTANCE, xLimit);
  updateMadeChanges = false;

  update(Constant);
  if (millis() - lastTickTime >= msPerTick) {
    lastTickTime += msPerTick;
    ticksDone++;
    update(Tick);
    if (ticksDone % 2 == 0)
      update(Two_Tick);
    if (ticksDone % 4 == 0) //Nested because a number divisible by 4 will always also be divisible by 2
      update(Four_Tick);
    if (ticksDone % 8 == 0) //Nested because a number divisible by 8 will always also be divisible by 4
      update(Eight_Tick);
    if (ticksDone % 5 == 0)
      update(Five_Tick);
  }
  if (updateMadeChanges)
    updateLighting();
  return updateMadeChanges;
}
void World::update (WorldUpdateType updateType) {
  switch (updateType) {
    case WorldUpdateType::Constant:   updateConstant(); break;
    case WorldUpdateType::Tick:       updateTick();     break;
    case WorldUpdateType::Two_Tick:   update2Tick();    break;
    case WorldUpdateType::Four_Tick:  update4Tick();    break;
    case WorldUpdateType::Five_Tick:  update5Tick();    break;
    case WorldUpdateType::Eight_Tick: update8Tick();    break;
  }
}
uint8_t World::updateAll () {
  leftmostXCoordinate = -xLimit;
  rightmostXCoordinate = xLimit;

  uint8_t passes;
  do {
    updateMadeChanges = false;
    updateTick();
    update2Tick();
    update4Tick();
    update5Tick();
    update8Tick();
    ++passes;
  } while (updateMadeChanges);
  updateLighting();
  return passes;
}
void World::updateConstant() {
  if (lightingUpdateNeeded) {
    updateLighting();
    lightingUpdateNeeded = false;
    updateMadeChanges = true;
  }
}
void World::updateTick() {
  updateFallingBlocks();
}
void World::update2Tick() {
}
void World::update4Tick() {
}
void World::update5Tick() {
  updateWater();
  updateLava();
}
void World::World::update8Tick() {

}

void World::updateLighting () {
  using namespace Blocks::Runtime;
  for (xcoord_t x = leftmostXCoordinate; x <= rightmostXCoordinate; x++)
      for (ycoord_t y = 0; y <= yLimit; y++) {
        if (block.isAir(block.get(x, y)))
          block.set(x, y, light0);
      }
  for (xcoord_t x = leftmostXCoordinate; x <= rightmostXCoordinate; x++)
      for (ycoord_t y = 0; y <= yLimit; y++) {
        if (block.isAir(block.get(x, y)) && block.isOpenToSky(x, y))
          block.set(x, y, light7);
      }
  for (id_t lightIndex = light6; lightIndex >= light0; --lightIndex)
    for (xcoord_t x = leftmostXCoordinate; x <= rightmostXCoordinate; x++)
      for (ycoord_t y = 0; y <= yLimit; y++) {
        const id_t currentBlock = block.get(x, y);
        if (block.isAir(currentBlock) && block.isTouching(x, y, lightIndex + 1) && ((!block.isLight(currentBlock)) || currentBlock < lightIndex))
          block.set(x, y, lightIndex);
      }
}
void World::updateWater () {
  for (xcoord_t x = leftmostXCoordinate; x <= rightmostXCoordinate; x++) //Schedule water that doesn't have water to the sides for deletion
    for (ycoord_t y = 0; y <= yLimit; y++) {
      const id_t blockToCheck = block.get(x, y);
      if (block.isWater(blockToCheck) && blockToCheck != Blocks::waterSource) { //Check whether the block is even water
        bool validWaterToLeft = false;
        bool validWaterToRight = false;
        bool validWaterAbove = false;
        id_t sideBlock = block.get(x + 1, y);
        if (block.isWater(sideBlock) && (sideBlock > blockToCheck)) { //If the block is water larger than the block current block, it's valid
          validWaterToRight = true;
        }
        sideBlock = block.get(x - 1, y);
        if (block.isWater(sideBlock) && (sideBlock > blockToCheck)) { //If the block is water larger than the block current block, it's valid
          validWaterToLeft = true;
        }
        if (y != yLimit && block.isWater(block.get(x, y + 1)))
          validWaterAbove = true;

        if (!(validWaterToLeft || validWaterToRight || validWaterAbove)) {
          block.set(x, y, block.convertToDeleted(blockToCheck));
        }
      }
    }
  for (xcoord_t x = leftmostXCoordinate; x <= rightmostXCoordinate; x++) //Truly delete water
    for (ycoord_t y = 0; y <= yLimit; y++)
      if (block.isDeletedWater(block.get(x, y)))
        block.set(x, y, Blocks::air);

  for (uint8_t side = 0; side < 2; side++) { //Flow water to sides
    for (xcoord_t x = (side ? leftmostXCoordinate : rightmostXCoordinate); side ? (x <= rightmostXCoordinate) : (x >= leftmostXCoordinate); side ? x++ : x--) //Water flowing to the sides
      for (ycoord_t y = 0; y <= yLimit; y++) {
        bool validWaterToSide = false;
        if (block.isBrokenByFluid(block.get(x, y))) { //Check whether the block is even elligible to become water
          id_t blockToCheck = block.get(x + (side ? 1 : -1), y);
          if (block.isWater(blockToCheck) && (blockToCheck == Blocks::waterSource ? Blocks::water7 : blockToCheck) > (block.isWater(block.get(x, y)) ? block.get(x, y) + 1 : Blocks::water0)) { //If the block is water larger than the block current block, continue
            //Now, to check if there is a block under the water to the side
            if (y == 0) //If it's void, it's good
              validWaterToSide = true;
            else {
              id_t blockToCheckUnderSide = block.get(x + (side ? 1 : -1), y - 1);
              validWaterToSide = (!block.isBrokenByFluid(blockToCheckUnderSide) && !block.isWater(blockToCheckUnderSide));
              //If the block under the block to the left is solid, there's valid water to the left!
            }
          }
          id_t blockOnSide = 0;
          if (validWaterToSide) {
            blockOnSide = block.get(x +  (side ? 1 : -1), y);
            if (blockOnSide == Blocks::waterSource)
              blockOnSide = Blocks::water7;
          }
          if (validWaterToSide) {
            block.set(x, y, blockOnSide - 1);
            updateMadeChanges = true;
          }
        }
      }
  }
  for (xcoord_t x = leftmostXCoordinate; x <= rightmostXCoordinate; x++) //Flow water down
    for (ycoord_t y = 0; y <= yLimit; y++)
      if (block.isBrokenByFluid(block.get(x, y)) && (y == yLimit ? false : block.isWater(block.get(x, y + 1)))) {
        block.set(x, y, Blocks::water7);
        updateMadeChanges = true;
      }
  for (xcoord_t x = leftmostXCoordinate; x <= rightmostXCoordinate; x++) //Generate source blocks between other source blocks
    for (ycoord_t y = 0; y <= yLimit; y++)
      if (block.isBrokenByFluid(block.get(x, y)) && (x == -xLimit ? false : block.get(x - 1, y) == Blocks::waterSource) && (x == xLimit ? false : block.get(x + 1, y) == Blocks::waterSource)) { //If the block in question has source blocks on both sides
        if (y == 0 || (!block.isBrokenByFluid(block.get(x - 1, y - 1)) && block.isSolid(block.get(x - 1, y - 1)) && !block.isBrokenByFluid(block.get(x + 1, y - 1)) && block.isSolid(block.get(x + 1, y - 1)))) { //If the source blocks have blocks beneath them
          block.set(x, y, Blocks::waterSource);
          updateMadeChanges = true;
        }
      }
}
void World::updateLava () {
  for (xcoord_t x = leftmostXCoordinate; x <= rightmostXCoordinate; x++) //Schedule lava that doesn't have lava to the sides for deletion
    for (ycoord_t y = 0; y <= yLimit; y++) {
      const id_t blockToCheck = block.get(x, y);
      if (block.isLava(blockToCheck) && blockToCheck != Blocks::lavaSource) { //Check whether the block is even lava
        bool validLavaToLeft = false;
        bool validLavaToRight = false;
        bool validLavaAbove = false;
        id_t sideBlock = block.get(x + 1, y);
        if (block.isLava(sideBlock) && (sideBlock > blockToCheck)) { //If the block is lava larger than the block current block, it's valid
          validLavaToRight = true;
        }
        sideBlock = block.get(x - 1, y);
        if (block.isLava(sideBlock) && (sideBlock > blockToCheck)) { //If the block is lava larger than the block current block, it's valid
          validLavaToLeft = true;
        }
        if (y != yLimit && block.isLava(block.get(x, y + 1)))
          validLavaAbove = true;

        if (!(validLavaToLeft || validLavaToRight || validLavaAbove)) {
          block.set(x, y, block.convertToDeleted(blockToCheck));
        }
      }
    }
  for (xcoord_t x = leftmostXCoordinate; x <= rightmostXCoordinate; x++) //Truly delete lava
    for (ycoord_t y = 0; y <= yLimit; y++)
      if (block.isDeletedLava(block.get(x, y))) {
        block.set(x, y, Blocks::air);
      }

  for (uint8_t side = 0; side < 2; side++) { //Flow lava to sides
    for (xcoord_t x = (side ? leftmostXCoordinate : rightmostXCoordinate); side ? (x <= rightmostXCoordinate) : (x >= leftmostXCoordinate); side ? x++ : x--) //Lava flowing to the sides
      for (ycoord_t y = 0; y <= yLimit; y++) {
        bool validLavaToSide = false;
        if (block.isBrokenByFluid(block.get(x, y))) { //Check whether the block is even elligible to become lava
          id_t blockToCheck = block.get(x + (side ? 1 : -1), y);
          if (block.isLava(blockToCheck) && (blockToCheck == Blocks::lavaSource ? Blocks::lava3 : blockToCheck) > (block.isLava(block.get(x, y)) ? block.get(x, y) + 1 : Blocks::lava0)) { //If the block is lava larger than the block current block, continue
            //Now, to check if there is a block under the lava to the side
            if (y == 0) //If it's void, it's good
              validLavaToSide = true;
            else {
              id_t blockToCheckUnderSide = block.get(x + (side ? 1 : -1), y - 1);
              validLavaToSide = (!block.isBrokenByFluid(blockToCheckUnderSide) && !block.isLava(blockToCheckUnderSide));
              //If the block under the block to the left is solid, there's valid lava to the left!
            }
          }
          id_t blockOnSide = 0;
          if (validLavaToSide) {
            blockOnSide = block.get(x +  (side ? 1 : -1), y);
            if (blockOnSide == Blocks::lavaSource)
              blockOnSide = Blocks::lava3;
          }
          if (validLavaToSide) {
            block.set(x, y, blockOnSide - 1);
            updateMadeChanges = true;
          }
        }
      }
  }
  for (xcoord_t x = leftmostXCoordinate; x <= rightmostXCoordinate; x++) //Flow lava down
    for (ycoord_t y = 0; y <= yLimit; y++)
      if (block.isBrokenByFluid(block.get(x, y)) && (y == yLimit ? false : block.isLava(block.get(x, y + 1)))) {
        block.set(x, y, Blocks::lava3);
        updateMadeChanges = true;
      }
}
void World::updateFallingBlocks () {
  for (xcoord_t x = leftmostXCoordinate; x <= rightmostXCoordinate; x++)
    for (ycoord_t y = 0; y <= yLimit - 1; y++)
      if (block.isBrokenByFallingBlocks(block.get(x, y))) {
        if (block.get(x, y + 1) == Blocks::sand) {
          block.set(x, y, Blocks::sand);
          block.set(x, y + 1, Blocks::air);
          updateMadeChanges = true;
        } else if (block.get(x, y + 1) == Blocks::gravel) {
          block.set(x, y, Blocks::gravel);
          block.set(x, y + 1, Blocks::air);
          updateMadeChanges = true;
        }
      }
}
