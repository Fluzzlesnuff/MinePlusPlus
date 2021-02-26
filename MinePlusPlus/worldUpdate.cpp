#include "includes.h"
bool World::tryUpdate() {
  static uint32_t lastTickTime;
  static uint32_t ticksDone;
  bool madeChanges = false;
  if (update(Constant))
    madeChanges = true;
  if (millis() - lastTickTime >= msPerTick) {
    lastTickTime += msPerTick;
    ticksDone++;
    if (update(Tick))
      madeChanges = true;
    if (ticksDone % 2 == 0) {
      if (update(Two_Tick))
        madeChanges = true;
      if (ticksDone % 4 == 0) { //Nested because a number divisible by 4 will always also be divisible by 2
        if (update(Four_Tick))
          madeChanges = true;
        if (ticksDone % 8 == 0) { //Nested because a number divisible by 8 will always also be divisible by 4
          if (update(Eight_Tick))
            madeChanges = true;
        }
      }
    }
    if (ticksDone % 5 == 0) {
      if (update(Five_Tick))
        madeChanges = true;
    }
  }
  return madeChanges;
}
bool World::update (WorldUpdateType updateType) {
  switch (updateType) {
    case WorldUpdateType::Constant:   return updateConstant();
    case WorldUpdateType::Tick:       return updateTick();
    case WorldUpdateType::Two_Tick:   return update2Tick();
    case WorldUpdateType::Four_Tick:  return update4Tick();
    case WorldUpdateType::Five_Tick:  return update5Tick();
    case WorldUpdateType::Eight_Tick: return update8Tick();
  }
}
bool World::updateConstant() {
  xcoord_t leftmostXCoordinate = max(player.getCoords().x - UPDATE_DISTANCE, -xLimit);
  xcoord_t rightmostXCoordinate = min(player.getCoords().x + UPDATE_DISTANCE, xLimit);
  bool madeChanges = false;
  return madeChanges;
}
bool World::updateTick() {
  xcoord_t leftmostXCoordinate = max(player.getCoords().x - UPDATE_DISTANCE, -xLimit);
  xcoord_t rightmostXCoordinate = min(player.getCoords().x + UPDATE_DISTANCE, xLimit);
  bool madeChanges = false;
  for (xcoord_t x = leftmostXCoordinate; x <= rightmostXCoordinate; x++) //Gravel and sand
    for (ycoord_t y = 0; y <= yLimit - 1; y++)
      if (block.isBrokenByFallingBlocks(block.get(x, y))) {
        if (block.get(x, y + 1) == B_SAND) {
          block.set(x, y, B_SAND);
          block.set(x, y + 1, Blocks::air);
          madeChanges = true;
        } else if (block.get(x, y + 1) == B_GRAVEL) {
          block.set(x, y, B_GRAVEL);
          block.set(x, y + 1, Blocks::air);
          madeChanges = true;
        }
      }
  return madeChanges;
}
bool World::update2Tick() {
  xcoord_t leftmostXCoordinate = max(player.getCoords().x - UPDATE_DISTANCE, -xLimit);
  xcoord_t rightmostXCoordinate = min(player.getCoords().x + UPDATE_DISTANCE, xLimit);
  bool madeChanges = false;
  return madeChanges;
}
bool World::update4Tick() {
  xcoord_t leftmostXCoordinate = max(player.getCoords().x - UPDATE_DISTANCE, -xLimit);
  xcoord_t rightmostXCoordinate = min(player.getCoords().x + UPDATE_DISTANCE, xLimit);
  bool madeChanges = false;
  return madeChanges;
}
bool World::update5Tick() {
  xcoord_t leftmostXCoordinate = max(player.getCoords().x - UPDATE_DISTANCE, -xLimit);
  xcoord_t rightmostXCoordinate = min(player.getCoords().x + UPDATE_DISTANCE, xLimit);
  bool madeChanges = false;
  { //Delete water that shouldn't exist
    for (xcoord_t x = leftmostXCoordinate; x <= rightmostXCoordinate; x++) //Schedule water that doesn't have water to the sides for deletion
      for (ycoord_t y = 0; y <= yLimit; y++) {
        if (block.isWater(block.get(x, y)) && block.get(x, y) != Blocks::waterSource) { //Check whether the block is even water
          bool validWaterToLeft = false;
          bool validWaterToRight = false;
          bool validWaterAbove = false;
          for (uint8_t side = 0; side < 2; side++) {
            if (x == (side ? xLimit : -xLimit)) //There's no water to the side if there's C_VOID to the set.
              (side ? validWaterToRight : validWaterToLeft) = false;
            else { //If it's not void, what is it?
              id_t blockToCheck = block.get(x + (side ? 1 : -1), y);
              if (block.isWater(blockToCheck) && (blockToCheck == Blocks::waterSource ? Blocks::water7 : blockToCheck) > (block.isWater(block.get(x, y)) ? block.get(x, y) : Blocks::water0)) { //If the block is water larger than the block current block, it's valid
                (side ? validWaterToRight : validWaterToLeft) = true;
              }
            }
          }
          if (y != yLimit && block.isWater(block.get(x, y + 1)))
            validWaterAbove = true;
          if (!(validWaterToLeft || validWaterToRight || validWaterAbove))
            block.set(x, y, Blocks::air);
        }
      }
  }
  for (uint8_t side = 0; side < 2; side++) { //Flow water to sides
    for (xcoord_t x = (side ? leftmostXCoordinate : rightmostXCoordinate); side ? (x <= rightmostXCoordinate) : (x >= leftmostXCoordinate); side ? x++ : x--) //Water flowing to the sides
      for (ycoord_t y = 0; y <= yLimit; y++) {
        bool validWaterToSide = false;
        if (block.isBrokenByFluid(block.get(x, y))) { //Check whether the block is even elligible to become water
          if (x == (side ? xLimit : -xLimit)) //There's no water to the side if there's C_VOID to the left.
            validWaterToSide = false;
          else { //If it's not void, what is it?
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
          }
          id_t blockOnSide = 0;
          if (validWaterToSide) {
            blockOnSide = block.get(x +  (side ? 1 : -1), y);
            if (blockOnSide == Blocks::waterSource)
              blockOnSide = Blocks::water7;
          }
          if (validWaterToSide) {
            block.set(x, y, blockOnSide - 1);
            madeChanges = true;
          }
        }
      }
  }
  for (xcoord_t x = leftmostXCoordinate; x <= rightmostXCoordinate; x++) //Flow water down
    for (ycoord_t y = 0; y <= yLimit; y++)
      if (block.isBrokenByFluid(block.get(x, y)) && (y == yLimit ? false : block.isWater(block.get(x, y + 1)))) {
        block.set(x, y, Blocks::water7);
        madeChanges = true;
      }
  for (xcoord_t x = leftmostXCoordinate; x <= rightmostXCoordinate; x++) //Generate source blocks between other source blocks
    for (ycoord_t y = 0; y <= yLimit; y++)
      if (block.isBrokenByFluid(block.get(x, y)) && (x == -xLimit ? false : block.get(x - 1, y) == Blocks::waterSource) && (x == xLimit ? false : block.get(x + 1, y) == Blocks::waterSource)) { //If the block in question has source blocks on both sides
        if (y == 0 || (!block.isBrokenByFluid(block.get(x - 1, y - 1)) && block.isSolid(block.get(x - 1, y - 1)) && !block.isBrokenByFluid(block.get(x + 1, y - 1)) && block.isSolid(block.get(x + 1, y - 1)))) { //If the source blocks have blocks beneath them
          block.set(x, y, Blocks::waterSource);
          madeChanges = true;
        }
      }
  return madeChanges;
}
bool World::update8Tick() {
  xcoord_t leftmostXCoordinate = max(player.getCoords().x - UPDATE_DISTANCE, -xLimit);
  xcoord_t rightmostXCoordinate = min(player.getCoords().x + UPDATE_DISTANCE, xLimit);
  bool madeChanges = false;
  return madeChanges;
}
