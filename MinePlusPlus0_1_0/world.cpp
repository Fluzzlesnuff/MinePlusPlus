#include "includes.h"

worldWidth_t worldWidth;
worldHeight_t worldHeight;

xcoord_t xLimit; //Inclusive. Make negative to use negative x limit.
ycoord_t yLimit; //Inclusive. Bottom limit is 0.

void World::update (WorldUpdateType updateType) {

}

void World::setTickRate (double tickRateParam) {

}
void World::setWorldDimensions(WorldSize sizeParam) {
  size = sizeParam;
  switch (size) {
    case Default:
      worldWidth = 201;
      worldHeight = 33;
      break;
    case Tall:
      worldWidth = 7;
      worldHeight = 7;
      break;
    case Wide:
      worldWidth = 512;
      worldHeight = 16;
      break;
  }
}

void World::start() {
  player.move(0, safeDivide(yLimit, 2) + 4);
  screen.renderWorld();
}
void World::generate (WorldSize sizeParam) {
  setWorldDimensions(sizeParam);
  com.out.log(F("Beginning World Generation"));
  xLimit = (worldWidth - 1) / 2;
  yLimit = worldHeight - 1;
  block.createBlockDB(worldWidth, worldHeight);
  { //Air
    com.out.log(F("Generation Stage 0: Air"));
    for (xcoord_t x = -xLimit; x <= xLimit; x++) {
      for (ycoord_t y = 0; y <= yLimit; y++)
        block.set(x, y, B_AIR);
    }
    com.out.log(F("\tFinished"));
  }
  { //Stone
    com.out.log(F("Generation Stage 1: Solid Stone Layer"));
    for (xcoord_t x = -xLimit; x <= xLimit; x++) {
      for (ycoord_t y = 0; y <= safeDivide(yLimit, 2) - 2; y++)
        block.set(x, y, B_STONE);
    }
    for (int i = 0; i < 2; i++) { //Stone Hills
      ycoord_t currentHillHeight = safeDivide(yLimit, 2) + 2;
      int8_t lastDeviation = 0;
      int8_t lastLastDeviation = 0;
      for (xcoord_t x = 0; (i ? (x <= xLimit) : (x >= -xLimit)); (i ? (x++) : (x--))) {
        int8_t deviation = randomNumber(0, 0.7, 1, 0.275, 2, 0.02, 3);
        bool upOrDown = randomNumber(0, 0.5, 1, 0.5); //true is up, false is down
        if (currentHillHeight >= yLimit - 4) { //if the height is near the build limit, the  height can't increase further
          currentHillHeight -= deviation;
          lastDeviation = -deviation;
        } else if (currentHillHeight <= safeDivide(yLimit, 2) - 1) { //if the height is near the world halfway point, the hill height can't decrease further
          currentHillHeight += deviation;
          lastDeviation = deviation;
        } else if (lastLastDeviation <= -2 && deviation >= 2 && upOrDown) { //if a 2x2 pitfall if forming, prevent it
          currentHillHeight += 1;
          lastDeviation = 1;
        } else if (lastDeviation > 0) { //if the last deviation was positive, the new deviation can't be negtive
          currentHillHeight += deviation;
          lastDeviation = deviation;
        } else if (lastDeviation < 0) { //if the last deviation was negative, the new deviation can't be positive
          currentHillHeight -= deviation;
          lastDeviation = -deviation;
        } else {
          if (upOrDown) { //if no special rules have been used, just choose whether to move the ground up or down.
            currentHillHeight += deviation;
            lastDeviation = deviation;
          } else {
            currentHillHeight -= deviation;
            lastDeviation = -deviation;
          }
        }
        lastLastDeviation = lastDeviation;
        for (ycoord_t y = safeDivide(yLimit, 2) - 5; y <= currentHillHeight; y++) //finally fill in the stone at and below  the currentHillHeight
          block.set(x, y, B_STONE);
      }
    }
    com.out.log(F("\tFinished"));
  }
  { //Dirt
    com.out.log(F("Generation Stage 2: Dirt Top Layer"));
    for (xcoord_t x = -xLimit; x <= xLimit; x++) {
      for (ycoord_t y = 0; y <= yLimit; y++) {
        if (block.get(x, y) == B_STONE && (y == yLimit || block.get(x, y + 1) == B_AIR || y == yLimit - 1 || block.get(x, y + 2) == B_AIR || ((y == yLimit - 2 || block.get(x, y + 3) == B_AIR) && randomNumber(1, 0.95, 0)) || ((y == yLimit - 3 || block.get(x, y + 4) == B_AIR) && randomNumber(0, 0.98, 1))))
          /* Explanation:
             This is a mess. It reads as: if the block in question is stone, and one of the following conditions is true, it becomes dirt.
             The conditions: either it's 2 blocks below air 2 blocks of air, or (large chance) within 3 blocks of air, or (small chance) it's within 4 blocks of air.
          */
          block.set(x, y, B_DIRT);
      }
    }
    start();
    com.out.log(F("\tFinished"));
  }
  /*
    serialLog(F("Generation Stage 2: Random Iron Ore"));
    for (xcoord_t x = -xLimit; x <= xLimit; x++) {
    for (ycoord_t y = 0; y <= yLimit; y++)
      if (random() % 100 < 10 && block.get(x, y) == B_STONE)
        block.set(x, y, B_IRON_ORE);
    }
    serialLog(F("\tFinished"));
    serialLog(F("Finished Generating World"));
    }*/
}
void World::load () {
  /*for (int j = 0; j < 16; j++) {
  for (int i = 0; i < 256; i++) {
    byte testByte = EEPROM.read(i + (j * 256));
    com.out.log(String(testByte == i ? testByte : String(testByte) + "  !!!"));
  }
  }*/
  com.out.log(F("Beginning World Loading"));
  setWorldDimensions(WorldSize(EEPROM.read(0)));
  xLimit = (worldWidth - 1) / 2;
  yLimit = worldHeight - 1;
  block.createBlockDB(worldWidth, worldHeight);
  com.out.log(F("created new blockDB"));
  largeindex_t EEPROMAddress = 4;
  largeindex_t blockDBAddress = 0;
  while (true) {
    byte byteRead = EEPROM.read(EEPROMAddress);
    if (byteRead == FLAG_3_BYTE) {
      byte numberToLoad = EEPROM.read(EEPROMAddress + 1);
      largeindex_t endAddress = blockDBAddress + numberToLoad - 1;
      byte blockToLoad = EEPROM.read(EEPROMAddress + 2);
      EEPROMAddress += 3;
      com.out.log("F 3\t\t# " + String(numberToLoad) + "\t\tI " + String(blockToLoad) + "\tB " + String(blockDBAddress) + "-" + String(blockDBAddress + numberToLoad - 1) + "\t\tE " + String(EEPROMAddress) + "-" + String(EEPROMAddress + 3));
      for (blockDBAddress; blockDBAddress < endAddress; blockDBAddress++)
        blockDB[blockDBAddress] = blockToLoad;
    } else if (byteRead == FLAG_4_BYTE) {
      largeindex_t numberToLoad = (EEPROM.read(EEPROMAddress + 1) * 256) + EEPROM.read(EEPROMAddress + 2);
      largeindex_t endAddress = blockDBAddress + numberToLoad - 1;
      byte blockToLoad = EEPROM.read(EEPROMAddress + 3);
      EEPROMAddress += 4;
      com.out.log("F 4\t\t# " + String(numberToLoad) + "\t\tI " + String(blockToLoad) + "\tB " + String(blockDBAddress) + "-" + String(blockDBAddress + numberToLoad - 1) + "\t\tE " + String(EEPROMAddress) + "-" + String(EEPROMAddress + 3));
      for (blockDBAddress; blockDBAddress < endAddress; blockDBAddress++)
        blockDB[blockDBAddress] = blockToLoad;
    } else if (byteRead == COMP_END) {
      com.out.log("Found COMP_END");
      break;
    }
    else {
      com.out.log("\t\t\t\tI " + String(byteRead) + "\tB " + String(blockDBAddress) + "\t\tE " + String(EEPROMAddress));
      blockDB[blockDBAddress] = byteRead;
      EEPROMAddress++;
    }
    blockDBAddress++;
  }
  start();
  com.out.log(F("\tFinished"));
}
void World::save () {
  /*for (int j = 0; j < 16; j++) {
  for (int i = 0; i < 256; i++)
    EEPROM.update(i + (j * 256), i);
  }*/
  
  EEPROM.update(0, lowByte(size));
  EEPROM.update(1, highByte(player.getCoords().x));
  EEPROM.update(2, lowByte(player.getCoords().x));
  EEPROM.update(3, player.getCoords().y);
  largeindex_t numberFound = 0;
  largeindex_t EEPROMAddress = 4;
  id_t currentBlockID = blockDB[0];
  for (largeindex_t i = 0; i < worldWidth * worldHeight; i++) {
    if (currentBlockID == blockDB[i]) {
      numberFound++;
    } else {
      if (numberFound == 1) {
        EEPROM.update(EEPROMAddress, currentBlockID);
        com.out.log("\t\t\t\tI " + String(currentBlockID) + "\tB " + String(i - 1) + "\t\tE " + String(EEPROMAddress));
        EEPROMAddress++;
      } else if (numberFound < 256) {
        EEPROM.update(EEPROMAddress, FLAG_3_BYTE);
        EEPROM.update(EEPROMAddress + 1, lowByte(numberFound));
        EEPROM.update(EEPROMAddress + 2, currentBlockID);
        com.out.log("F 3\t\t# " + String(numberFound) + "\t\tI " + String(currentBlockID) + "\tB " + String(i - numberFound) + "-" + String(i - 1) + "\t\tE " + String(EEPROMAddress) + "-" + String(EEPROMAddress + 3));
        EEPROMAddress += 3;
      } else {
        EEPROM.update(EEPROMAddress, FLAG_4_BYTE);
        EEPROM.update(EEPROMAddress + 1, highByte(numberFound));
        EEPROM.update(EEPROMAddress + 2, lowByte(numberFound));
        EEPROM.update(EEPROMAddress + 3, currentBlockID);
        com.out.log("F 4\t\t# " + String(numberFound) + "\t\tI " + String(currentBlockID) + "\tB " + String(i - numberFound) + "-" + String(i - 1) + "\t\tE " + String(EEPROMAddress) + "-" + String(EEPROMAddress + 3));
        EEPROMAddress += 4;
      }
      currentBlockID = blockDB[i];
      numberFound = 1;
    }
  }
  if (numberFound == 1) {
    EEPROM.update(EEPROMAddress, currentBlockID);
    EEPROMAddress++;
  } else if (numberFound < 256) {
    EEPROM.update(EEPROMAddress, FLAG_3_BYTE);
    EEPROM.update(EEPROMAddress + 1, lowByte(numberFound));
    EEPROM.update(EEPROMAddress + 2, currentBlockID);
    EEPROMAddress += 3;
  } else {
    EEPROM.update(EEPROMAddress, FLAG_4_BYTE);
    EEPROM.update(EEPROMAddress + 1, highByte(numberFound));
    EEPROM.update(EEPROMAddress + 2, lowByte(numberFound));
    EEPROM.update(EEPROMAddress + 3, currentBlockID);
    EEPROMAddress += 4;
  }
  EEPROM.update(EEPROMAddress, COMP_END);
  EEPROM.update(EEPROMAddress + 1, COMP_END);
  
}

World world;
