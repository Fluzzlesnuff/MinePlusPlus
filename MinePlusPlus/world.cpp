#include "includes.h"

#define EE_SHUTDOWN_BYTE            0
#define EE_WORLDMAP_START_BYTE      1
#define EE_WORLDMAP_END_BYTE        24
#define EE_VERSION_HIGH_BYTE        25
#define EE_VERSION_LOW_BYTE         26
#define EE_DEFAULT_ICON_BYTE        27

#define EE_WORLD_SIZE_BYTE          32
#define EE_PLAYER_X_HIGH_BYTE       33
#define EE_PLAYER_LOW_X_Y_BYTE      34
#define EE_PLAYER_HEALTH_BYTE       35
#define EE_PLAYER_HUNGER_BYTE       36
#define EE_PLAYER_SPAWNPOINTH_BYTE  37
#define EE_PLAYER_SPAWNPOINTL_BYTE  38
#define EE_PLAYER_RESERVED_BYTE     39
#define EE_WORLD_DATA_START_BYTE    40

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
      worldWidth = 11;
      worldHeight = 11;
      break;
    case Wide:
      worldWidth = 51;
      worldHeight = 11;
      break;
  }
}
void World::start() {
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
  }
  { //Caves
    com.out.log(F("Generation Stage 3: Caves"));
    com.out.log(F("\tSeeding Caves"));
    for (xcoord_t x = -xLimit; x <= xLimit; x++) {
      for (ycoord_t y = 0; y <= safeDivide(worldHeight, 3); y++)
        if ((random() % 100) == 0 && block.get(x, y) == B_STONE)
          block.set(x, y, GEN_AIR);
    }
    for (int i = 0; i < 5; i++) {
      const byte chances[5][8] = {
        {1,  1,  1,  1,  1,  1,  1,  1},  // An array of chance lists, one for each value of i.
        {2,  2,  1,  1,  1,  1,  1,  1},  // Chance list: An array of values of x.
        {5,  3,  2,  1,  1,  1,  1,  1},  // For each number of isTouchingWide blocks, there is a value of x,
        {0, 0,  3,  2,  1,  1,  1,  1},  // where the odds of changing the block in question to air are 1/x.
        {0,  0,  0,  0,  0,  0,  1,  1}   // 0 means no chance.
      };
      com.out.log("\tGrowing Caves: Pass " + String(i + 1));
      for (xcoord_t x = -xLimit; x <= xLimit; x++) {
        for (ycoord_t y = 0; y <= yLimit; y++) {
          byte numTouching = block.isTouchingWide(x, y, GEN_AIR);
          if (numTouching == 0)
            continue;
          byte chance = chances[i][numTouching - 1];
          if (chance == 0)
            continue;
          if (block.isTouching(x, y, GEN_AIR) && (random() % chance) == 0)
            block.set(x, y, GEN_T_AIR);
        }
      }
      for (xcoord_t x = -xLimit; x <= xLimit; x++) {
        for (ycoord_t y = 0; y <= yLimit; y++)
          if (block.get(x, y) == GEN_T_AIR)
            block.set(x, y, GEN_AIR);
      }
    }
    com.out.log(F("\tFinished"));
  }
  { //Gravel
    com.out.log(F("Generation Stage 4: Gravel"));
    com.out.log(F("\tSeeding Gravel Veins"));
    for (xcoord_t x = -xLimit; x <= xLimit; x++) {
      for (ycoord_t y = 0; y <= yLimit; y++)
        if ((random() % 150) == 0 && block.get(x, y) == B_STONE)
          block.set(x, y, B_GRAVEL);
    }
    for (int i = 0; i < 4; i++) {
      const byte chances[4][8] = {
        {1,  1,  1,  1,  1,  1,  1,  1},  
        {3,  2,  2,  2,  1,  1,  1,  1},
        {0, 0,  5,  3,  3,  3,  2,  1},
        {0,  0,  0,  0,  1,  1,  1,  1}
      };
      com.out.log("\tGrowing Gravel Veins: Pass " + String(i + 1));
      for (xcoord_t x = -xLimit; x <= xLimit; x++) {
        for (ycoord_t y = 0; y <= yLimit; y++) {
          byte numTouching = block.isTouchingWide(x, y, B_GRAVEL);
          if (numTouching == 0)
            continue;
          byte chance = chances[i][numTouching - 1];
          if (chance == 0)
            continue;
          if ((block.isTouching(x, y, B_GRAVEL) && (random() % chance) == 0) || block.isTouching(x, y, B_GRAVEL) >= 3)
            block.set(x, y, GEN_GRAVEL);
        }
      }
      for (xcoord_t x = -xLimit; x <= xLimit; x++) {
        for (ycoord_t y = 0; y <= yLimit; y++)
          if (block.get(x, y) == GEN_GRAVEL)
            block.set(x, y, B_GRAVEL);
      }
    }
    com.out.log(F("\tFinished"));
  }
  { //Dirt Veins
    com.out.log(F("Generation Stage 5: Dirt Veins"));
    com.out.log(F("\tSeeding Dirt Veins"));
    for (xcoord_t x = -xLimit; x <= xLimit; x++) {
      for (ycoord_t y = safeDivide(worldHeight, 4); y <= yLimit; y++)
        if ((random() % 200) == 0 && block.get(x, y) == B_STONE)
          block.set(x, y, GEN_DIRT);
    }
    for (int i = 0; i < 3; i++) {
      const byte chances[3][8] = {
        {1,  1,  1,  1,  1,  1,  1,  1},  
        {3,  2,  2,  2,  1,  1,  1,  1},
        {0,  0,  0,  0,  1,  1,  1,  1}
      };
      com.out.log("\tGrowing Dirt Veins: Pass " + String(i + 1));
      for (xcoord_t x = -xLimit; x <= xLimit; x++) {
        for (ycoord_t y = 0; y <= yLimit; y++) {
          byte numTouching = block.isTouchingWide(x, y, GEN_DIRT);
          if (numTouching == 0)
            continue;
          byte chance = chances[i][numTouching - 1];
          if (chance == 0)
            continue;
          if ((block.isTouching(x, y, GEN_DIRT) && (random() % chance) == 0) || block.isTouching(x, y, GEN_DIRT) >= 3)
            block.set(x, y, GEN_T_DIRT);
        }
      }
      for (xcoord_t x = -xLimit; x <= xLimit; x++) {
        for (ycoord_t y = 0; y <= yLimit; y++)
          if (block.get(x, y) == GEN_T_DIRT)
            block.set(x, y, GEN_DIRT);
      }
    }
    for (xcoord_t x = -xLimit; x <= xLimit; x++) {
    for (ycoord_t y = 0; y <= yLimit; y++)
      if (block.get(x, y) == GEN_DIRT)
        block.set(x, y, B_DIRT);
    }
    com.out.log(F("\tFinished"));
  }
  com.out.log(F("\tGeneration Stage 13: Clean up Underground Generation"));
  for (xcoord_t x = -xLimit; x <= xLimit; x++) {
    for (ycoord_t y = 0; y <= yLimit; y++)
      if (block.get(x, y) == GEN_AIR)
        block.set(x, y, B_AIR);
  }
  player.move(0, safeDivide(yLimit, 2) + 4);
  start();
  com.out.log(F("\tFinished"));
}
void World::load () {
  /*for (int j = 0; j < 16; j++) {
    for (int i = 0; i < 256; i++) {
    byte testByte = EEPROM.read(i + (j * 256));
    com.out.log(String(testByte == i ? testByte : String(testByte) + "  !!!"));
    }
    }*/
#ifdef SAVE_LOAD_LOGGING
  com.out.log(F("Beginning World Loading"));
#endif
  byte worldSizeByte = EEPROM.read(EE_WORLD_SIZE_BYTE);
  byte worldSize = (bitRead(worldSizeByte, 7) * 8) + (bitRead(worldSizeByte, 6) * 4) + (bitRead(worldSizeByte, 5) * 2) + bitRead(worldSizeByte, 4); //shift the bits
  setWorldDimensions(WorldSize(worldSize));
  xLimit = (worldWidth - 1) / 2;
  yLimit = worldHeight - 1;
  block.createBlockDB(worldWidth, worldHeight);
  largeindex_t EEPROMAddress = EE_WORLD_DATA_START_BYTE;
  largeindex_t blockDBAddress = 0;
  while (true) {
    byte byteRead = EEPROM.read(EEPROMAddress);
    if (byteRead == FLAG_3_BYTE) {
      byte numberToLoad = EEPROM.read(EEPROMAddress + 1);
      byte blockToLoad = EEPROM.read(EEPROMAddress + 2);
#ifdef SAVE_LOAD_BYTE_LOGGING
      com.out.log("F 3\t\t# " + String(numberToLoad) + "\t\tI " + String(blockToLoad) + "\tB " + String(blockDBAddress) + "-" + String(blockDBAddress + numberToLoad - 1) + "\t\tE " + String(EEPROMAddress) + "-" + String(EEPROMAddress + 2));
#endif
      EEPROMAddress += 3;
      for (index_t i = 0; i < numberToLoad; i++)
        blockDB[blockDBAddress + i] = blockToLoad;
      blockDBAddress += numberToLoad - 1;
    } else if (byteRead == FLAG_4_BYTE) {
      largeindex_t numberToLoad = (EEPROM.read(EEPROMAddress + 1) * 256) + EEPROM.read(EEPROMAddress + 2);
      byte blockToLoad = EEPROM.read(EEPROMAddress + 3);
#ifdef SAVE_LOAD_BYTE_LOGGING
      com.out.log("F 4\t\t# " + String(numberToLoad) + "\t\tI " + String(blockToLoad) + "\tB " + String(blockDBAddress) + "-" + String(blockDBAddress + numberToLoad - 1) + "\t\tE " + String(EEPROMAddress) + "-" + String(EEPROMAddress + 3));
#endif
      EEPROMAddress += 4;
      for (largeindex_t i = 0; i < numberToLoad; i++)
        blockDB[blockDBAddress + i] = blockToLoad;
      blockDBAddress += numberToLoad - 1;
    } else if (byteRead == COMP_END) {
#ifdef SAVE_LOAD_BYTE_LOGGING
      com.out.log("Found COMP_END");
#endif
      break;
    }
    else {
#ifdef SAVE_LOAD_BYTE_LOGGING
      com.out.log("\t\t\t\tI " + String(byteRead) + "\tB " + String(blockDBAddress) + "\t\t\tE " + String(EEPROMAddress));
#endif
      blockDB[blockDBAddress] = byteRead;
      EEPROMAddress++;
    }
    blockDBAddress++;
  }
  player.move(EEPROM.read(EE_PLAYER_X_HIGH_BYTE), EEPROM.read(EE_PLAYER_LOW_X_Y_BYTE));
  start();
#ifdef SAVE_LOAD_LOGGING
  com.out.log(F("\tFinished"));
#endif
}
void World::save () {
  /*for (int j = 0; j < 16; j++) {
    for (int i = 0; i < 256; i++)
    EEPROM.update(i + (j * 256), i);
    }*/
  byte worldSizeByte = 0x00;
  bitWrite(worldSizeByte, 7, bitRead(size, 3));
  bitWrite(worldSizeByte, 6, bitRead(size, 2));
  bitWrite(worldSizeByte, 5, bitRead(size, 1));
  bitWrite(worldSizeByte, 4, bitRead(size, 0));
  EEPROM.update(EE_WORLD_SIZE_BYTE, worldSizeByte);
  EEPROM.update(EE_PLAYER_X_HIGH_BYTE, lowByte(player.getCoords().x));
  EEPROM.update(EE_PLAYER_LOW_X_Y_BYTE, player.getCoords().y);
  largeindex_t numberFound = 0;
  largeindex_t EEPROMAddress = EE_WORLD_DATA_START_BYTE;
  id_t currentBlockID = blockDB[0];
  largeindex_t i;
  for (i = 0; i < worldWidth * worldHeight; i++) {
    if (currentBlockID == blockDB[i]) {
      numberFound++;
    } else {
      if (numberFound == 1) {
        EEPROM.update(EEPROMAddress, currentBlockID);
#ifdef SAVE_LOAD_BYTE_LOGGING
        com.out.log("\t\t\t\tI " + String(currentBlockID) + "\tB " + String(i - 1) + "\t\t\tE " + String(EEPROMAddress));
#endif
        EEPROMAddress += 1;
      } else if (numberFound < 256) {
        EEPROM.update(EEPROMAddress, FLAG_3_BYTE);
        EEPROM.update(EEPROMAddress + 1, lowByte(numberFound));
        EEPROM.update(EEPROMAddress + 2, currentBlockID);
#ifdef SAVE_LOAD_BYTE_LOGGING
        com.out.log("F 3\t\t# " + String(numberFound) + "\t\tI " + String(currentBlockID) + "\tB " + String(i - numberFound) + "-" + String(i - 1) + "\t\tE " + String(EEPROMAddress) + "-" + String(EEPROMAddress + 2));
#endif
        EEPROMAddress += 3;
      } else {
        EEPROM.update(EEPROMAddress, FLAG_4_BYTE);
        EEPROM.update(EEPROMAddress + 1, highByte(numberFound));
        EEPROM.update(EEPROMAddress + 2, lowByte(numberFound));
        EEPROM.update(EEPROMAddress + 3, currentBlockID);
#ifdef SAVE_LOAD_BYTE_LOGGING
        com.out.log("F 4\t\t# " + String(numberFound) + "\t\tI " + String(currentBlockID) + "\tB " + String(i - numberFound) + "-" + String(i - 1) + "\t\tE " + String(EEPROMAddress) + "-" + String(EEPROMAddress + 3));
#endif
        EEPROMAddress += 4;
      }
      currentBlockID = blockDB[i];
      numberFound = 1;
    }
  }
  if (numberFound == 1) {
    EEPROM.update(EEPROMAddress, currentBlockID);
#ifdef SAVE_LOAD_BYTE_LOGGING
    com.out.log("\t\t\t\tI " + String(currentBlockID) + "\tB " + String(i - 1) + "\t\t\tE " + String(EEPROMAddress));
#endif
    EEPROMAddress++;
  } else if (numberFound < 256) {
    EEPROM.update(EEPROMAddress, FLAG_3_BYTE);
    EEPROM.update(EEPROMAddress + 1, lowByte(numberFound));
    EEPROM.update(EEPROMAddress + 2, currentBlockID);
#ifdef SAVE_LOAD_BYTE_LOGGING
    com.out.log("F 3\t\t# " + String(numberFound) + "\t\tI " + String(currentBlockID) + "\tB " + String(i - numberFound) + "-" + String(i - 1) + "\t\tE " + String(EEPROMAddress) + "-" + String(EEPROMAddress + 2));
#endif
    EEPROMAddress += 3;
  } else {
    EEPROM.update(EEPROMAddress, FLAG_4_BYTE);
    EEPROM.update(EEPROMAddress + 1, highByte(numberFound));
    EEPROM.update(EEPROMAddress + 2, lowByte(numberFound));
    EEPROM.update(EEPROMAddress + 3, currentBlockID);
#ifdef SAVE_LOAD_BYTE_LOGGING
    com.out.log("F 4\t\t# " + String(numberFound) + "\t\tI " + String(currentBlockID) + "\tB " + String(i - numberFound) + "-" + String(i - 1) + "\t\tE " + String(EEPROMAddress) + "-" + String(EEPROMAddress + 3));
#endif
    EEPROMAddress += 4;
  }
  EEPROM.update(EEPROMAddress, COMP_END);
  EEPROM.update(EEPROMAddress + 1, COMP_END);

}

World world;
