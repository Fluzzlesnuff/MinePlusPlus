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

void World::update (const WorldUpdateType updateType) {

}
void World::setTickRate (const double tickRateParam) {

}
void World::setWorldDimensions(const WorldSize sizeParam) {
  size = sizeParam;
  switch (size) {
    case Default:
      worldWidth = 161;
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
  GLCD.ClearScreen();
  screen.renderWorld();
}
void World::generate (WorldSize sizeParam) {
  setWorldDimensions(sizeParam);
  com.out.logChars("Beginning World Generation");
  xLimit = (worldWidth - 1) / 2;
  yLimit = worldHeight - 1;
  block.createBlockDB(worldWidth, worldHeight);
  player.move(0, safeDivide(yLimit, 2) + 4);
  generateAir();
#ifdef RENDER_WHILE_GENERATING
  screen.renderWorldOverview();
#endif
  Serial.println(freeMemory());
  generateStone();
#ifdef RENDER_WHILE_GENERATING
  screen.renderWorldOverview();
#endif
  Serial.println(freeMemory());
  generateDirtLayer();
#ifdef RENDER_WHILE_GENERATING
  screen.renderWorldOverview();
#endif
  Serial.println(freeMemory());
  generateCaves();
#ifdef RENDER_WHILE_GENERATING
  screen.renderWorldOverview();
#endif
  Serial.println(freeMemory());
  generateGravelVeins();
#ifdef RENDER_WHILE_GENERATING
  screen.renderWorldOverview();
#endif
  Serial.println(freeMemory());
  generateDirtVeins();
#ifdef RENDER_WHILE_GENERATING
  screen.renderWorldOverview();
#endif
  Serial.println(freeMemory());
  generateDiamond();
#ifdef RENDER_WHILE_GENERATING
  screen.renderWorldOverview();
#endif
  Serial.println(freeMemory());
  generateIron();
#ifdef RENDER_WHILE_GENERATING
  screen.renderWorldOverview();
#endif
  Serial.println(freeMemory());
  generateGold();
#ifdef RENDER_WHILE_GENERATING
  screen.renderWorldOverview();
#endif
  Serial.println(freeMemory());
  generateCoal();
#ifdef RENDER_WHILE_GENERATING
  screen.renderWorldOverview();
#endif
  Serial.println(freeMemory());
  generateLavaPools();
#ifdef RENDER_WHILE_GENERATING
  screen.renderWorldOverview();
#endif
  Serial.println(freeMemory());
  generateWaterPools();
#ifdef RENDER_WHILE_GENERATING
  screen.renderWorldOverview();
#endif
  Serial.println(freeMemory());
  generateLakes();
#ifdef RENDER_WHILE_GENERATING
  screen.renderWorldOverview();
#endif
  Serial.println(freeMemory());
  endUnderGroundGeneration();
#ifdef RENDER_WHILE_GENERATING
  screen.renderWorldOverview();
#endif
  Serial.println(freeMemory());
  generateDeserts();
#ifdef RENDER_WHILE_GENERATING
  screen.renderWorldOverview();
#endif
  Serial.println(freeMemory());
  generateTrees();
#ifdef RENDER_WHILE_GENERATING
  screen.renderWorldOverview();
#endif
  Serial.println(freeMemory());
  generateLeaves();
#ifdef RENDER_WHILE_GENERATING
  screen.renderWorldOverview();
#endif
  Serial.println(freeMemory());
  generatePlants();
#ifdef RENDER_WHILE_GENERATING
  screen.renderWorldOverview();
#endif
  Serial.println(freeMemory());
  start();
  com.out.logChars("\tFinished");
}
void World::load () {
  /*for (int j = 0; j < 16; j++) {
    for (int i = 0; i < 256; i++) {
    byte testByte = EEPROM.read(i + (j * 256));
    com.out.logChars(String(testByte == i ? testByte : String(testByte) + "  !!!"));
    }
    }*/
#ifdef SAVE_LOAD_LOGGING
  com.out.logChars("Beginning World Loading");
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
      com.out.logChars("F 3\t\t# " + String(numberToLoad) + "\t\tI " + String(blockToLoad) + "\tB " + String(blockDBAddress) + "-" + String(blockDBAddress + numberToLoad - 1) + "\t\tE " + String(EEPROMAddress) + "-" + String(EEPROMAddress + 2));
#endif
      EEPROMAddress += 3;
      for (index_t i = 0; i < numberToLoad; i++)
        blockDB[blockDBAddress + i] = blockToLoad;
      blockDBAddress += numberToLoad - 1;
    } else if (byteRead == FLAG_4_BYTE) {
      largeindex_t numberToLoad = (EEPROM.read(EEPROMAddress + 1) * 256) + EEPROM.read(EEPROMAddress + 2);
      byte blockToLoad = EEPROM.read(EEPROMAddress + 3);
#ifdef SAVE_LOAD_BYTE_LOGGING
      com.out.logChars("F 4\t\t# " + String(numberToLoad) + "\t\tI " + String(blockToLoad) + "\tB " + String(blockDBAddress) + "-" + String(blockDBAddress + numberToLoad - 1) + "\t\tE " + String(EEPROMAddress) + "-" + String(EEPROMAddress + 3));
#endif
      EEPROMAddress += 4;
      for (largeindex_t i = 0; i < numberToLoad; i++)
        blockDB[blockDBAddress + i] = blockToLoad;
      blockDBAddress += numberToLoad - 1;
    } else if (byteRead == COMP_END) {
#ifdef SAVE_LOAD_BYTE_LOGGING
      com.out.logChars("Found COMP_END");
#endif
      break;
    }
    else {
#ifdef SAVE_LOAD_BYTE_LOGGING
      com.out.logChars("\t\t\t\tI " + String(byteRead) + "\tB " + String(blockDBAddress) + "\t\t\tE " + String(EEPROMAddress));
#endif
      blockDB[blockDBAddress] = byteRead;
      EEPROMAddress++;
    }
    blockDBAddress++;
  }
  player.move(EEPROM.read(EE_PLAYER_X_HIGH_BYTE), EEPROM.read(EE_PLAYER_LOW_X_Y_BYTE));
  start();
#ifdef SAVE_LOAD_LOGGING
  com.out.logChars("\tFinished");
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
        com.out.logChars("\t\t\t\tI " + String(currentBlockID) + "\tB " + String(i - 1) + "\t\t\tE " + String(EEPROMAddress));
#endif
        EEPROMAddress += 1;
      } else if (numberFound < 256) {
        EEPROM.update(EEPROMAddress, FLAG_3_BYTE);
        EEPROM.update(EEPROMAddress + 1, lowByte(numberFound));
        EEPROM.update(EEPROMAddress + 2, currentBlockID);
#ifdef SAVE_LOAD_BYTE_LOGGING
        com.out.logChars("F 3\t\t# " + String(numberFound) + "\t\tI " + String(currentBlockID) + "\tB " + String(i - numberFound) + "-" + String(i - 1) + "\t\tE " + String(EEPROMAddress) + "-" + String(EEPROMAddress + 2));
#endif
        EEPROMAddress += 3;
      } else {
        EEPROM.update(EEPROMAddress, FLAG_4_BYTE);
        EEPROM.update(EEPROMAddress + 1, highByte(numberFound));
        EEPROM.update(EEPROMAddress + 2, lowByte(numberFound));
        EEPROM.update(EEPROMAddress + 3, currentBlockID);
#ifdef SAVE_LOAD_BYTE_LOGGING
        com.out.logChars("F 4\t\t# " + String(numberFound) + "\t\tI " + String(currentBlockID) + "\tB " + String(i - numberFound) + "-" + String(i - 1) + "\t\tE " + String(EEPROMAddress) + "-" + String(EEPROMAddress + 3));
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
    com.out.logChars("\t\t\t\tI " + String(currentBlockID) + "\tB " + String(i - 1) + "\t\t\tE " + String(EEPROMAddress));
#endif
    EEPROMAddress++;
  } else if (numberFound < 256) {
    EEPROM.update(EEPROMAddress, FLAG_3_BYTE);
    EEPROM.update(EEPROMAddress + 1, lowByte(numberFound));
    EEPROM.update(EEPROMAddress + 2, currentBlockID);
#ifdef SAVE_LOAD_BYTE_LOGGING
    com.out.logChars("F 3\t\t# " + String(numberFound) + "\t\tI " + String(currentBlockID) + "\tB " + String(i - numberFound) + "-" + String(i - 1) + "\t\tE " + String(EEPROMAddress) + "-" + String(EEPROMAddress + 2));
#endif
    EEPROMAddress += 3;
  } else {
    EEPROM.update(EEPROMAddress, FLAG_4_BYTE);
    EEPROM.update(EEPROMAddress + 1, highByte(numberFound));
    EEPROM.update(EEPROMAddress + 2, lowByte(numberFound));
    EEPROM.update(EEPROMAddress + 3, currentBlockID);
#ifdef SAVE_LOAD_BYTE_LOGGING
    com.out.logChars("F 4\t\t# " + String(numberFound) + "\t\tI " + String(currentBlockID) + "\tB " + String(i - numberFound) + "-" + String(i - 1) + "\t\tE " + String(EEPROMAddress) + "-" + String(EEPROMAddress + 3));
#endif
    EEPROMAddress += 4;
  }
  EEPROM.update(EEPROMAddress, COMP_END);
  EEPROM.update(EEPROMAddress + 1, COMP_END);

}

//########################
//# GENERATION FUNCTIONS #
//########################
void World::generateAir () {
  com.out.logChars("Generation Stage 0: Air");
  for (xcoord_t x = -xLimit; x <= xLimit; x++)
    for (ycoord_t y = 0; y <= yLimit; y++)
      block.set(x, y, B_AIR);
  com.out.log("\tFinished");
}
void World::generateStone () {
  com.out.logChars("Generation Stage 1: Solid Stone Layer");
  for (xcoord_t x = -xLimit; x <= xLimit; x++) {
    for (ycoord_t y = 0; y <= safeDivide(yLimit, 2) - 2; y++)
      block.set(x, y, B_STONE);
  }
#ifdef RENDER_WHILE_GENERATING
  screen.renderWorldOverview();
#endif
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
#ifdef RENDER_WHILE_GENERATING
      screen.renderWorldOverview();
#endif
    }
#ifdef RENDER_WHILE_GENERATING
    screen.renderWorldOverview();
#endif
  }
  com.out.logChars("\tFinished");
}
void World::generateDirtLayer () {
  com.out.logChars("Generation Stage 2: Dirt Top Layer");
  Serial.println(freeMemory());
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
void World::generateCaves () {
  com.out.logChars("Generation Stage 3: Caves");
  com.out.logChars("\tSeeding Caves");
  for (xcoord_t x = -xLimit; x <= xLimit; x++) {
    for (ycoord_t y = 0; y <= yLimit; y++)
      if ((random() % 100) == 0 && block.get(x, y) == B_STONE && !block.isNear(x, y, B_AIR, 4, Taxicab) && (!block.isNear(x, y, GEN_AIR, 7, Chebyshev) || (random() % 10) == 0)) {
        //    1% chance           and      is stone              and              not near the surface     and       not near another cave seed      unless     small chance
        block.set(x, y, GEN_AIR);
#ifdef RENDER_WHILE_GENERATING
        screen.renderWorldOverview();
#endif
      }
  }
  for (int i = 0; i < 5; i++) {
    const byte chances[5][8] = {
      {1,  1,  1,  1,  1,  1,  1,  1},  // An array of chance lists, one for each value of i.
      {2,  2,  1,  1,  1,  1,  1,  1},  // Chance list: An array of values of x.
      {5,  3,  2,  1,  1,  1,  1,  1},  // For each number of isTouchingWide blocks, there is a value of x,
      {0, 0,  3,  2,  1,  1,  1,  1},  // where the odds of changing the block in question to air are 1/x.
      {0,  0,  0,  0,  0,  0,  1,  1}   // 0 means no chance.
    };
    com.out.prefix();
    com.out.print("\tGrowing Caves:");
    com.out.print(" Pass ");
    com.out.println(String(i + 1));
    Serial.println(freeMemory());
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
#ifdef RENDER_WHILE_GENERATING
    screen.renderWorldOverview();
#endif
  }
  com.out.logChars("\tFinished");
}
void World::generateGravelVeins () {
  com.out.logChars("Generation Stage 4: Gravel");
  com.out.logChars("\tSeeding Gravel Veins");
  for (xcoord_t x = -xLimit; x <= xLimit; x++) {
    for (ycoord_t y = 0; y <= yLimit; y++)
      if ((random() % 200) == 0 && block.get(x, y) == B_STONE)
        block.set(x, y, B_GRAVEL);
  }
  for (int i = 0; i < 4; i++) {
    const byte chances[4][8] = {
      {1,  1,  1,  1,  1,  1,  1,  1},
      {3,  2,  2,  2,  1,  1,  1,  1},
      {0, 0,  5,  3,  3,  3,  2,  1},
      {0,  0,  0,  0,  1,  1,  1,  1}
    };
    com.out.logMultiple("\tGrowing Gravel Veins: Pass " + String(i + 1));
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
  com.out.logChars("\tFinished");
}
void World::generateDirtVeins () {
  com.out.logChars("Generation Stage 5: Dirt Veins");
  com.out.logChars("\tSeeding Dirt Veins");
  for (xcoord_t x = -xLimit; x <= xLimit; x++) {
    for (ycoord_t y = safeDivide(worldHeight, 4); y <= yLimit; y++)
      if ((random() % 250) == 0 && block.get(x, y) == B_STONE)
        block.set(x, y, GEN_DIRT);
  }
  for (int i = 0; i < 3; i++) {
    const byte chances[3][8] = {
      {1,  1,  1,  1,  1,  1,  1,  1},
      {3,  2,  2,  2,  1,  1,  1,  1},
      {0,  0,  0,  0,  1,  1,  1,  1}
    };
    com.out.logMultiple("\tGrowing Dirt Veins: Pass " + String(i + 1));
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
  com.out.logChars("\tFinished");
}
void World::generateDiamond () {
  com.out.logChars("Generation Stage 6: Diamond Ore");
  com.out.logChars("\tSeeding Diamond Veins");
  for (xcoord_t x = -xLimit; x <= xLimit; x++) {
    for (ycoord_t y = 0; y <= 4; y++)
      if (((random() % 300) == 0 || ((random() % 100) == 0 && block.isTouching(x, y, GEN_AIR))) && block.get(x, y) == B_STONE)
        block.set(x, y, B_DIA_ORE);
  }
  com.out.logChars("\tGrowing Diamond Veins");
  for (xcoord_t x = -xLimit; x <= xLimit; x++) {
    for (ycoord_t y = 0; y <= 5; y++) {
      if ((block.isTouching(x, y, B_DIA_ORE) && (random() % 10) != 0) && block.get(x, y) == B_STONE)
        block.set(x, y, GEN_T_DIA);
    }
  }
  for (xcoord_t x = -xLimit; x <= xLimit; x++) {
    for (ycoord_t y = 0; y <= yLimit; y++)
      if (block.get(x, y) == GEN_T_DIA)
        block.set(x, y, B_DIA_ORE);
  }
  com.out.logChars("\tFinished");
}
void World::generateIron () {
  com.out.logChars("Generation Stage 7: Iron Ore");
  com.out.logChars("\tSeeding Iron Veins");
  for (xcoord_t x = -xLimit; x <= xLimit; x++) {
    for (ycoord_t y = 0; y <= safeDivide(worldHeight, 3); y++)
      if (((random() % 100) == 0 || ((random() % 400) == 0 && block.isTouching(x, y, GEN_AIR))) && block.get(x, y) == B_STONE)
        block.set(x, y, B_IRON_ORE);
  }
  com.out.logChars("\tGrowing Iron Veins");
  for (xcoord_t x = -xLimit; x <= xLimit; x++) {
    for (ycoord_t y = 0; y <= yLimit; y++) {
      if ((block.isTouchingWide(x, y, B_IRON_ORE) && (random() % 4) != 0) && block.get(x, y) == B_STONE)
        block.set(x, y, GEN_T_IRON);
    }
  }
  for (xcoord_t x = -xLimit; x <= xLimit; x++) {
    for (ycoord_t y = 0; y <= yLimit; y++)
      if (block.get(x, y) == GEN_T_IRON)
        block.set(x, y, B_IRON_ORE);
  }
  com.out.logChars("\tFinished");
}
void World::generateGold () {
  com.out.logChars("Generation Stage 8: Gold Ore");
  com.out.logChars("\tSeeding Gold Veins");
  for (xcoord_t x = -xLimit; x <= xLimit; x++) {
    for (ycoord_t y = 0; y <= yLimit; y++)
      if (((random() % 150) == 0 || ((random() % 800) == 0 && block.isTouching(x, y, GEN_AIR))) && block.get(x, y) == B_STONE)
        block.set(x, y, B_GOLD_ORE);
  }
  com.out.logChars("\tGrowing Gold Veins");
  for (xcoord_t x = -xLimit; x <= xLimit; x++) {
    for (ycoord_t y = 0; y <= safeDivide(worldHeight, 3); y++) {
      if ((block.isTouchingWide(x, y, B_GOLD_ORE) && (random() % 3) != 0) && block.get(x, y) == B_STONE)
        block.set(x, y, GEN_T_GOLD);
    }
  }
  for (xcoord_t x = -xLimit; x <= xLimit; x++) {
    for (ycoord_t y = 0; y <= yLimit; y++)
      if (block.get(x, y) == GEN_T_GOLD)
        block.set(x, y, B_GOLD_ORE);
  }
  com.out.logChars("\tFinished");
}
void World::generateCoal () {
  com.out.logChars("Generation Stage 9: Coal Ore");
  com.out.logChars("\tSeeding Coal Veins");
  for (xcoord_t x = -xLimit; x <= xLimit; x++) {
    for (ycoord_t y = 0; y <= safeDivide(worldHeight, 2); y++)
      if (((random() % 100) == 0 || ((random() % 70) == 0 && block.isTouching(x, y, GEN_AIR))) && block.get(x, y) == B_STONE)
        block.set(x, y, B_COAL_ORE);
  }
  com.out.logChars("\tGrowing Coal Veins: Pass 1");
  for (xcoord_t x = -xLimit; x <= xLimit; x++) {
    for (ycoord_t y = 0; y <= yLimit; y++) {
      if ((block.isTouchingWide(x, y, B_COAL_ORE) && (random() % 2) == 0) && block.get(x, y) == B_STONE)
        block.set(x, y, GEN_T_COAL);
    }
  }
  for (xcoord_t x = -xLimit; x <= xLimit; x++) {
    for (ycoord_t y = 0; y <= yLimit; y++)
      if (block.get(x, y) == GEN_T_COAL)
        block.set(x, y, B_COAL_ORE);
  }
  com.out.logChars("\tGrowing Coal Veins: Pass 2");
  for (xcoord_t x = -xLimit; x <= xLimit; x++) {
    for (ycoord_t y = 0; y <= yLimit; y++) {
      if ((block.isTouching(x, y, B_COAL_ORE) && (random() % 4) == 0) && block.get(x, y) == B_STONE)
        block.set(x, y, GEN_T_COAL);
    }
  }
  for (xcoord_t x = -xLimit; x <= xLimit; x++) {
    for (ycoord_t y = 0; y <= yLimit; y++)
      if (block.get(x, y) == GEN_T_COAL)
        block.set(x, y, B_COAL_ORE);
  }
  com.out.logChars("\tFinished");
}
void World::generateLavaPools () {
  com.out.logChars("Generation Stage 10: Lava Pools");
  com.out.logChars("\tSeeding Lava Pools");
  for (xcoord_t x = -xLimit; x <= xLimit; x++) {
    for (ycoord_t y = 0; y <= 5; y++)
      if ((random() % 60) == 0 && block.get(x, y) == GEN_AIR)
        block.set(x, y, B_LAVA3);
  }
  index_t i = 1;
  while (true) {
    com.out.prefix();
    com.out.print("\tGrowing Lava Pools:");
    com.out.print(" Pass ");
    com.out.println(String(i));
    bool changesMade = false;
    for (xcoord_t x = -xLimit; x <= xLimit; x++) {
      for (ycoord_t y = 0; y <= 5; y++)
        if (block.isTouching(x, y, B_LAVA3) && block.get(x, y) == GEN_AIR) {
          block.set(x, y, B_LAVA3);
          changesMade = true;
        }
    }
    if (!changesMade)
      break;
    i++;
  }
}
void World::generateWaterPools () {
  com.out.logChars("Generation Stage 11: Water Pools");
  com.out.logChars("\tSeeding Water Pools");
  for (xcoord_t x = -xLimit; x <= xLimit; x++) {
    for (ycoord_t y = 0; y <= safeDivide(worldHeight, 3); y++)
      if ((random() % 50) == 0 && block.get(x, y) == GEN_AIR && (y == 0 ? true : (block.get(x, y - 1) == B_STONE)))
        block.set(x, y, B_WATER7);
  }
  index_t i = 1;
  while (true) {
    com.out.prefix();
    com.out.print("\tGrowing Water Pools:");
    com.out.print(" Pass ");
    com.out.println(String(i));
    bool changesMade = false;
    for (xcoord_t x = -xLimit; x <= xLimit; x++) {
      for (ycoord_t y = 0; y <= 5; y++)
        if (block.isTouching(x, y, B_WATER7) && block.get(x, y) == GEN_AIR) {
          block.set(x, y, B_WATER7);
          changesMade = true;
        }
    }
    if (!changesMade)
      break;
    i++;
  }
}
void World::generateLakes () {
  com.out.logChars("Generation Stage 12: Lakes");
  com.out.logChars("\tSeeding Lakes");
  for (xcoord_t x = -xLimit; x <= xLimit; x++) {
    for (ycoord_t y = safeDivide(worldHeight, 2); y <= safeDivide(worldHeight, 2) + 2; y++)
      if ((random() % 40) == 0 && block.get(x, y) == B_AIR)
        block.set(x, y, B_WATER7);
  }
#ifdef RENDER_WHILE_GENERATING
  screen.renderWorldOverview();
#endif
  index_t i = 1;
  while (true) {
    com.out.prefix();
    com.out.print("\tGrowing Lakes:");
    com.out.print(" Pass ");
    com.out.println(String(i));
    bool changesMade = false;
    for (xcoord_t x = -xLimit; x <= xLimit; x++) {
      for (ycoord_t y = safeDivide(worldHeight, 2) - 1; y <= safeDivide(worldHeight, 2) + 2; y++)
        if (block.isTouching(x, y, B_WATER7) && block.get(x, y) == B_AIR) {
          block.set(x, y, B_WATER7);
          changesMade = true;
        }
    }
#ifdef RENDER_WHILE_GENERATING
    screen.renderWorldOverview();
#endif
    if (!changesMade)
      break;
    i++;
  }
  com.out.logChars("\tGenerating Lake Beds");
  for (xcoord_t x = -xLimit; x <= xLimit; x++)
    for (ycoord_t y = safeDivide(worldHeight, 2) - 3; y <= safeDivide(worldHeight, 2) + 4; y++)
      if (block.isNear(x, y, B_WATER7, 2, Taxicab) && (block.get(x, y) == B_DIRT || block.get(x, y) == B_STONE))
        block.set(x, y, B_SAND);
}
void World::endUnderGroundGeneration () {
  com.out.logChars("\tGeneration Stage 13: Clean up Underground Generation");
  Serial.println(freeMemory());
  for (xcoord_t x = -xLimit; x <= xLimit; x++) {
    for (ycoord_t y = 0; y <= yLimit; y++)
      if (block.get(x, y) == GEN_AIR)
        block.set(x, y, B_AIR);
  }
}
void World::generateDeserts () {
  com.out.logChars("Generation Stage 14: Deserts");
  com.out.logChars("\tSeeding Deserts");
  for (xcoord_t x = -xLimit; x <= xLimit; x++) {
    for (ycoord_t y = safeDivide(worldHeight, 2) - 3; y <= yLimit; y++)
      if ((random() % 200) == 0 && block.get(x, y) == B_DIRT && (y == yLimit ? true : (block.get(x, y + 1) == B_AIR)) && !block.isNear(x, y, B_WATER7, 5, Chebyshev))
        block.set(x, y, GEN_SAND);
  }
#ifdef RENDER_WHILE_GENERATING
  screen.renderWorldOverview();
#endif
  index_t i = 1;
  for (int i = 0; i < 3; i++) {
    com.out.prefix();
    com.out.print("\tGrowing Deserts:");
    com.out.print(" Pass ");
    com.out.println(String(i + 1));
    for (xcoord_t x = -xLimit; x <= xLimit; x++)
      for (ycoord_t y = 0; y <= yLimit; y++)
        if ((block.isTouching(x, y, GEN_SAND) || block.isTouching(x, y, B_SNDSTN)) && block.get(x, y) == B_DIRT)
          block.set(x, y, GEN_T_SAND);
    for (xcoord_t x = -xLimit; x <= xLimit; x++)
      for (ycoord_t y = safeDivide(worldHeight, 2) - 5; y <= yLimit; y++)
        if ((block.isTouching(x, y, GEN_SAND) || block.isTouching(x, y, B_SNDSTN)) && block.get(x, y) == B_STONE)
          block.set(x, y, GEN_T_SNDSTN);
    for (xcoord_t x = -xLimit; x <= xLimit; x++)
      for (ycoord_t y = 0; y <= yLimit; y++) {
        if (block.get(x, y) == GEN_T_SAND)
          block.set(x, y, GEN_SAND);
        else if (block.get(x, y) == GEN_T_SNDSTN)
          block.set(x, y, B_SNDSTN);
      }
  }
  for (xcoord_t x = -xLimit; x <= xLimit; x++)
    for (ycoord_t y = 0; y <= yLimit; y++)
      if (block.get(x, y) == GEN_SAND)
        block.set(x, y, B_SAND);
}
void World::generateTrees () {
  com.out.logChars("Generation Stage 15: Trees");
  com.out.logChars("\tPlanting Trees");
  for (xcoord_t x = -xLimit; x <= xLimit; x++) {
    for (ycoord_t y = safeDivide(worldHeight, 2) - 4; y <= yLimit; y++)
      if ((random() % 10) == 0 && block.get(x, y) == B_AIR && block.get(x, y - 1) == B_DIRT && block.isOpenToSky(x, y) && !block.isTouchingWide(x, y, B_WOOD))
        block.set(x, y, B_WOOD);
  }
#ifdef RENDER_WHILE_GENERATING
  screen.renderWorldOverview();
#endif
  uint8_t chances[] {10, 10, 10, 2}; //Chances for each pass. Chance of adding wood is chances[i]/10
  for (int i = 0; i < 4; i++) {
    com.out.prefix();
    com.out.print("\tGrowing Trees:");
    com.out.print(" Pass ");
    com.out.println(String(i + 1));
    for (xcoord_t x = -xLimit; x <= xLimit; x++)
      for (ycoord_t y = safeDivide(worldHeight, 2) - 4; y <= yLimit; y++)
        if (block.get(x, y) == B_AIR && block.get(x, y - 1) == B_WOOD && (random() % 10 < chances[i]))
          block.set(x, y, GEN_T_WOOD);
    for (xcoord_t x = -xLimit; x <= xLimit; x++)
      for (ycoord_t y = 0; y <= yLimit; y++)
        if (block.get(x, y) == GEN_T_WOOD)
          block.set(x, y, B_WOOD);
  }
  com.out.prefix();
  com.out.print("\tGrowing Trees:");
  com.out.println(" Adding Branches");
    for (xcoord_t x = -xLimit; x <= xLimit; x++)
      for (ycoord_t y = safeDivide(worldHeight, 2) - 4; y <= yLimit; y++)
        if (block.get(x, y) == B_AIR && block.isTouching(x, y, B_WOOD) && (random() % 10) == 0 && !block.isTouching(x, y, B_DIRT) && !block.isTouching(x, y, B_SAND) && !block.isTouching(x, y, B_STONE) && !block.isTouching(x, y, B_WATER7))
          block.set(x, y, GEN_T_WOOD);
    for (xcoord_t x = -xLimit; x <= xLimit; x++)
      for (ycoord_t y = 0; y <= yLimit; y++)
        if (block.get(x, y) == GEN_T_WOOD)
          block.set(x, y, B_WOOD);
}
void World::generateLeaves () {
  com.out.logChars("Generation Stage 16: Leaves");
  com.out.log("\tPass 1");
  for (xcoord_t x = -xLimit; x <= xLimit; x++)
    for (ycoord_t y = 0; y <= yLimit; y++)
      if (block.get(x, y) == B_AIR && block.isTouchingWide(x, y, B_WOOD) && ((!block.isTouchingWide(x, y, B_DIRT) && !block.isTouchingWide(x, y, B_STONE) && !block.isTouchingWide(x, y, B_SAND) && !block.isTouchingWide(x, y, B_WATER7)) || (x <= -xLimit + 1 ? false : block.get(x - 2, y) == GEN_T_LEAVES) || (x >= xLimit - 1 ? false : block.get(x + 2, y) == GEN_T_LEAVES)))
        block.set(x, y, GEN_T_LEAVES);
  for (xcoord_t x = -xLimit; x <= xLimit; x++)
    for (ycoord_t y = 0; y <= yLimit; y++)
      if (block.get(x, y) == GEN_T_LEAVES)
        block.set(x, y, B_LEAVES);
#ifdef RENDER_WHILE_GENERATING
  screen.renderWorldOverview();
#endif
com.out.log("\tPass 2");
  for (xcoord_t x = -xLimit; x <= xLimit; x++)
    for (ycoord_t y = 0; y <= yLimit; y++)
      if ((block.get(x, y) == B_AIR && block.isTouching(x, y, B_LEAVES) && ((x == -xLimit ? false : block.get(x - 1, y) == B_LEAVES) || (x == xLimit ? false : block.get(x + 1, y) == B_LEAVES)) && ((x <= -xLimit + 1 ? false : block.get(x - 2, y) == B_WOOD) || (x >= xLimit - 1? false : block.get(x + 2, y) == B_WOOD))) && (random() % 5) != 0)
        block.set(x, y, GEN_T_LEAVES);
  for (xcoord_t x = -xLimit; x <= xLimit; x++)
    for (ycoord_t y = 0; y <= yLimit; y++)
      if (block.get(x, y) == GEN_T_LEAVES)
        block.set(x, y, B_LEAVES);
#ifdef RENDER_WHILE_GENERATING
  screen.renderWorldOverview();
#endif
}
void World::generatePlants () {

}
World world;
