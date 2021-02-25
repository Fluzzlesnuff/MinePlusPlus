#include "includes.h"

void World::generate (WorldSize sizeParam) {
  setWorldDimensions(sizeParam);
  cout << prefix << F("Beginning World Generation") << endl;
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
  cout << prefix << F("\tFinished") << endl;
}

//########################
//# GENERATION FUNCTIONS #
//########################
void World::generateAir () {
  cout << prefix << F("Generation Stage 0: Air") << endl;
  for (xcoord_t x = -xLimit; x <= xLimit; x++)
    for (ycoord_t y = 0; y <= yLimit; y++)
      block.set(x, y, B_AIR);
  cout << prefix << F("\tFinished") << endl;
}
void World::generateStone () {
  cout << prefix << F("Generation Stage 1: Solid Stone Layer") << endl;
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
  cout << prefix << F("\tFinished") << endl;
}
void World::generateDirtLayer () {
  cout << prefix << F("Generation Stage 2: Dirt Top Layer") << endl;
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
  cout << prefix << F("Generation Stage 3: Caves") << endl;
  cout << prefix << F("\tSeeding Caves") << endl;
  for (xcoord_t x = -xLimit; x <= xLimit; x++) {
    for (ycoord_t y = 0; y <= yLimit; y++)
      if ((random() % 100) == 0 && block.get(x, y) == B_STONE && !block.isNear(x, y, B_AIR, 4, Taxicab) && (!block.isNear(x, y, G_AIR, 7, Chebyshev) || (random() % 10) == 0)) {
        //    1% chance           and      is stone              and              not near the surface     and       not near another cave seed      unless     small chance
        block.set(x, y, G_AIR);
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
    cout << prefix << F("\tGrowing Caves:") << F(" Pass ") << i + 1 << endl;
    Serial.println(freeMemory());
    for (xcoord_t x = -xLimit; x <= xLimit; x++) {
      for (ycoord_t y = 0; y <= yLimit; y++) {
        byte numTouching = block.isTouchingWide(x, y, G_AIR);
        if (numTouching == 0)
          continue;
        byte chance = chances[i][numTouching - 1];
        if (chance == 0)
          continue;
        if (block.isTouching(x, y, G_AIR) && (random() % chance) == 0)
          block.set(x, y, G_T_AIR);
      }
    }
    for (xcoord_t x = -xLimit; x <= xLimit; x++) {
      for (ycoord_t y = 0; y <= yLimit; y++)
        if (block.get(x, y) == G_T_AIR)
          block.set(x, y, G_AIR);
    }
#ifdef RENDER_WHILE_GENERATING
    screen.renderWorldOverview();
#endif
  }
  cout << prefix << F("\tFinished") << endl;
}
void World::generateGravelVeins () {
  cout << prefix << F("Generation Stage 4: Gravel") << endl;
  cout << prefix << F("\tSeeding Gravel Veins") << endl;
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
    cout << prefix << "\tGrowing Gravel Veins: Pass " << i + 1 << endl;
    for (xcoord_t x = -xLimit; x <= xLimit; x++) {
      for (ycoord_t y = 0; y <= yLimit; y++) {
        byte numTouching = block.isTouchingWide(x, y, B_GRAVEL);
        if (numTouching == 0)
          continue;
        byte chance = chances[i][numTouching - 1];
        if (chance == 0)
          continue;
        if ((block.isTouching(x, y, B_GRAVEL) && (random() % chance) == 0) || block.isTouching(x, y, B_GRAVEL) >= 3)
          block.set(x, y, G_GRAVEL);
      }
    }
    for (xcoord_t x = -xLimit; x <= xLimit; x++) {
      for (ycoord_t y = 0; y <= yLimit; y++)
        if (block.get(x, y) == G_GRAVEL)
          block.set(x, y, B_GRAVEL);
    }
  }
  cout << prefix << F("\tFinished") << endl;
}
void World::generateDirtVeins () {
  cout << prefix << F("Generation Stage 5: Dirt Veins") << endl;
  cout << prefix << F("\tSeeding Dirt Veins") << endl;
  for (xcoord_t x = -xLimit; x <= xLimit; x++) {
    for (ycoord_t y = safeDivide(worldHeight, 4); y <= yLimit; y++)
      if ((random() % 250) == 0 && block.get(x, y) == B_STONE)
        block.set(x, y, G_DIRT);
  }
  for (int i = 0; i < 3; i++) {
    const byte chances[3][8] = {
      {1,  1,  1,  1,  1,  1,  1,  1},
      {3,  2,  2,  2,  1,  1,  1,  1},
      {0,  0,  0,  0,  1,  1,  1,  1}
    };
    cout << prefix << "\tGrowing Dirt Veins: Pass " << i + 1 << endl;
    for (xcoord_t x = -xLimit; x <= xLimit; x++) {
      for (ycoord_t y = 0; y <= yLimit; y++) {
        byte numTouching = block.isTouchingWide(x, y, G_DIRT);
        if (numTouching == 0)
          continue;
        byte chance = chances[i][numTouching - 1];
        if (chance == 0)
          continue;
        if ((block.isTouching(x, y, G_DIRT) && (random() % chance) == 0) || block.isTouching(x, y, G_DIRT) >= 3)
          block.set(x, y, G_T_DIRT);
      }
    }
    for (xcoord_t x = -xLimit; x <= xLimit; x++) {
      for (ycoord_t y = 0; y <= yLimit; y++)
        if (block.get(x, y) == G_T_DIRT)
          block.set(x, y, G_DIRT);
    }
  }
  for (xcoord_t x = -xLimit; x <= xLimit; x++) {
    for (ycoord_t y = 0; y <= yLimit; y++)
      if (block.get(x, y) == G_DIRT)
        block.set(x, y, B_DIRT);
  }
  cout << prefix << F("\tFinished") << endl;
}
void World::generateDiamond () {
  cout << prefix << F("Generation Stage 6: Diamond Ore") << endl;
  cout << prefix << F("\tSeeding Diamond Veins") << endl;
  for (xcoord_t x = -xLimit; x <= xLimit; x++) {
    for (ycoord_t y = 0; y <= 4; y++)
      if (((random() % 300) == 0 || ((random() % 100) == 0 && block.isTouching(x, y, G_AIR))) && block.get(x, y) == B_STONE)
        block.set(x, y, B_DIA_ORE);
  }
  cout << prefix << F("\tGrowing Diamond Veins") << endl;
  for (xcoord_t x = -xLimit; x <= xLimit; x++) {
    for (ycoord_t y = 0; y <= 5; y++) {
      if ((block.isTouching(x, y, B_DIA_ORE) && (random() % 10) != 0) && block.get(x, y) == B_STONE)
        block.set(x, y, G_T_DIA);
    }
  }
  for (xcoord_t x = -xLimit; x <= xLimit; x++) {
    for (ycoord_t y = 0; y <= yLimit; y++)
      if (block.get(x, y) == G_T_DIA)
        block.set(x, y, B_DIA_ORE);
  }
  cout << prefix << F("\tFinished") << endl;
}
void World::generateIron () {
  cout << prefix << F("Generation Stage 7: Iron Ore") << endl;
  cout << prefix << F("\tSeeding Iron Veins") << endl;
  for (xcoord_t x = -xLimit; x <= xLimit; x++) {
    for (ycoord_t y = 0; y <= safeDivide(worldHeight, 3); y++)
      if (((random() % 100) == 0 || ((random() % 400) == 0 && block.isTouching(x, y, G_AIR))) && block.get(x, y) == B_STONE)
        block.set(x, y, B_IRON_ORE);
  }
  cout << prefix << F("\tGrowing Iron Veins") << endl;
  for (xcoord_t x = -xLimit; x <= xLimit; x++) {
    for (ycoord_t y = 0; y <= yLimit; y++) {
      if ((block.isTouchingWide(x, y, B_IRON_ORE) && (random() % 4) != 0) && block.get(x, y) == B_STONE)
        block.set(x, y, G_T_IRON);
    }
  }
  for (xcoord_t x = -xLimit; x <= xLimit; x++) {
    for (ycoord_t y = 0; y <= yLimit; y++)
      if (block.get(x, y) == G_T_IRON)
        block.set(x, y, B_IRON_ORE);
  }
  cout << prefix << F("\tFinished") << endl;
}
void World::generateGold () {
  cout << prefix << F("Generation Stage 8: Gold Ore") << endl;
  cout << prefix << F("\tSeeding Gold Veins") << endl;
  for (xcoord_t x = -xLimit; x <= xLimit; x++) {
    for (ycoord_t y = 0; y <= yLimit; y++)
      if (((random() % 150) == 0 || ((random() % 800) == 0 && block.isTouching(x, y, G_AIR))) && block.get(x, y) == B_STONE)
        block.set(x, y, B_GOLD_ORE);
  }
  cout << prefix << F("\tGrowing Gold Veins") << endl;
  for (xcoord_t x = -xLimit; x <= xLimit; x++) {
    for (ycoord_t y = 0; y <= safeDivide(worldHeight, 3); y++) {
      if ((block.isTouchingWide(x, y, B_GOLD_ORE) && (random() % 3) != 0) && block.get(x, y) == B_STONE)
        block.set(x, y, G_T_GOLD);
    }
  }
  for (xcoord_t x = -xLimit; x <= xLimit; x++) {
    for (ycoord_t y = 0; y <= yLimit; y++)
      if (block.get(x, y) == G_T_GOLD)
        block.set(x, y, B_GOLD_ORE);
  }
  cout << prefix << F("\tFinished") << endl;
}
void World::generateCoal () {
  cout << prefix << F("Generation Stage 9: Coal Ore") << endl;
  cout << prefix << F("\tSeeding Coal Veins") << endl;
  for (xcoord_t x = -xLimit; x <= xLimit; x++) {
    for (ycoord_t y = 0; y <= safeDivide(worldHeight, 2); y++)
      if (((random() % 100) == 0 || ((random() % 70) == 0 && block.isTouching(x, y, G_AIR))) && block.get(x, y) == B_STONE)
        block.set(x, y, B_COAL_ORE);
  }
  cout << prefix << F("\tGrowing Coal Veins: Pass 1") << endl;
  for (xcoord_t x = -xLimit; x <= xLimit; x++) {
    for (ycoord_t y = 0; y <= yLimit; y++) {
      if ((block.isTouchingWide(x, y, B_COAL_ORE) && (random() % 2) == 0) && block.get(x, y) == B_STONE)
        block.set(x, y, G_T_COAL);
    }
  }
  for (xcoord_t x = -xLimit; x <= xLimit; x++) {
    for (ycoord_t y = 0; y <= yLimit; y++)
      if (block.get(x, y) == G_T_COAL)
        block.set(x, y, B_COAL_ORE);
  }
  cout << prefix << F("\tGrowing Coal Veins: Pass 2") << endl;
  for (xcoord_t x = -xLimit; x <= xLimit; x++) {
    for (ycoord_t y = 0; y <= yLimit; y++) {
      if ((block.isTouching(x, y, B_COAL_ORE) && (random() % 4) == 0) && block.get(x, y) == B_STONE)
        block.set(x, y, G_T_COAL);
    }
  }
  for (xcoord_t x = -xLimit; x <= xLimit; x++) {
    for (ycoord_t y = 0; y <= yLimit; y++)
      if (block.get(x, y) == G_T_COAL)
        block.set(x, y, B_COAL_ORE);
  }
  cout << prefix << F("\tFinished") << endl;
}
void World::generateLavaPools () {
  cout << prefix << F("Generation Stage 10: Lava Pools") << endl;
  cout << prefix << F("\tSeeding Lava Pools") << endl;
  for (xcoord_t x = -xLimit; x <= xLimit; x++) {
    for (ycoord_t y = 0; y <= 5; y++)
      if ((random() % 60) == 0 && block.get(x, y) == G_AIR)
        block.set(x, y, B_LAVA3);
  }
  uint8_t i = 1;
  while (true) {
    cout << prefix << F("\tGrowing Lava Pools: Pass ") << i << endl;
    bool changesMade = false;
    for (xcoord_t x = -xLimit; x <= xLimit; x++) {
      for (ycoord_t y = 0; y <= 5; y++)
        if (block.isTouching(x, y, B_LAVA3) && block.get(x, y) == G_AIR) {
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
  cout << prefix << F("Generation Stage 11: Water Pools") << endl << F("\tSeeding Water Pools");
  for (xcoord_t x = -xLimit; x <= xLimit; x++) {
    for (ycoord_t y = 0; y <= safeDivide(worldHeight, 3); y++)
      if ((random() % 50) == 0 && block.get(x, y) == G_AIR && (y == 0 ? true : (block.get(x, y - 1) == B_STONE)))
        block.set(x, y, B_WATER_SOURCE);
  }
  uint8_t i = 1;
  while (true) {
    cout << prefix << F("\tGrowing Water Pools: Pass ") << i << endl;
    bool changesMade = false;
    for (xcoord_t x = -xLimit; x <= xLimit; x++) {
      for (ycoord_t y = 0; y <= 5; y++)
        if (block.isTouching(x, y, B_WATER_SOURCE) && block.get(x, y) == G_AIR) {
          block.set(x, y, B_WATER_SOURCE);
          changesMade = true;
        }
    }
    if (!changesMade)
      break;
    i++;
  }
}
void World::generateLakes () {
  cout << prefix << F("Generation Stage 12: Lakes") << endl;
  cout << prefix << F("\tSeeding Lakes") << endl;
  for (xcoord_t x = -xLimit; x <= xLimit; x++) {
    for (ycoord_t y = safeDivide(worldHeight, 2); y <= safeDivide(worldHeight, 2) + 2; y++)
      if ((random() % 40) == 0 && block.get(x, y) == B_AIR)
        block.set(x, y, B_WATER_SOURCE);
  }
#ifdef RENDER_WHILE_GENERATING
  screen.renderWorldOverview();
#endif
  uint8_t i = 1;
  while (true) {
    cout << prefix << F("\tGrowing Lakes: Pass ") << i << endl;
    bool changesMade = false;
    for (xcoord_t x = -xLimit; x <= xLimit; x++) {
      for (ycoord_t y = safeDivide(worldHeight, 2) - 1; y <= safeDivide(worldHeight, 2) + 2; y++)
        if (block.isTouching(x, y, B_WATER_SOURCE) && block.get(x, y) == B_AIR) {
          block.set(x, y, B_WATER_SOURCE);
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
  cout << prefix << F("\tGenerating Lake Beds") << endl;
  for (xcoord_t x = -xLimit; x <= xLimit; x++)
    for (ycoord_t y = safeDivide(worldHeight, 2) - 3; y <= safeDivide(worldHeight, 2) + 4; y++)
      if (block.isNear(x, y, B_WATER_SOURCE, 2, Taxicab) && (block.get(x, y) == B_DIRT || block.get(x, y) == B_STONE))
        block.set(x, y, B_SAND);
}
void World::endUnderGroundGeneration () {
  cout << prefix << F("\tGeneration Stage 13: Clean up Underground Generation") << endl;
  Serial.println(freeMemory());
  for (xcoord_t x = -xLimit; x <= xLimit; x++) {
    for (ycoord_t y = 0; y <= yLimit; y++)
      if (block.get(x, y) == G_AIR)
        block.set(x, y, B_AIR);
  }
}
void World::generateDeserts () {
  cout << prefix << F("Generation Stage 14: Deserts") << endl;
  cout << prefix << F("\tSeeding Deserts") << endl;
  for (xcoord_t x = -xLimit; x <= xLimit; x++) {
    for (ycoord_t y = safeDivide(worldHeight, 2) - 3; y <= yLimit; y++)
      if ((random() % 200) == 0 && block.get(x, y) == B_DIRT && (y == yLimit ? true : (block.get(x, y + 1) == B_AIR)) && !block.isNear(x, y, B_WATER_SOURCE, 5, Chebyshev))
        block.set(x, y, G_SAND);
  }
#ifdef RENDER_WHILE_GENERATING
  screen.renderWorldOverview();
#endif
  uint8_t i = 1;
  for (int i = 0; i < 3; i++) {
    cout << prefix << F("\tGrowing Deserts: Pass ") << i << endl;
    for (xcoord_t x = -xLimit; x <= xLimit; x++)
      for (ycoord_t y = 0; y <= yLimit; y++)
        if ((block.isTouching(x, y, G_SAND) || block.isTouching(x, y, B_SNDSTN)) && block.get(x, y) == B_DIRT)
          block.set(x, y, G_T_SAND);
    for (xcoord_t x = -xLimit; x <= xLimit; x++)
      for (ycoord_t y = safeDivide(worldHeight, 2) - 5; y <= yLimit; y++)
        if ((block.isTouching(x, y, G_SAND) || block.isTouching(x, y, B_SNDSTN)) && block.get(x, y) == B_STONE)
          block.set(x, y, G_T_SNDSTN);
    for (xcoord_t x = -xLimit; x <= xLimit; x++)
      for (ycoord_t y = 0; y <= yLimit; y++) {
        if (block.get(x, y) == G_T_SAND)
          block.set(x, y, G_SAND);
        else if (block.get(x, y) == G_T_SNDSTN)
          block.set(x, y, B_SNDSTN);
      }
  }
  for (xcoord_t x = -xLimit; x <= xLimit; x++)
    for (ycoord_t y = 0; y <= yLimit; y++)
      if (block.get(x, y) == G_SAND)
        block.set(x, y, B_SAND);
}
void World::generateTrees () {
  cout << prefix << F("Generation Stage 15: Trees") << endl;
  cout << prefix << F("\tPlanting Trees") << endl;
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
    cout << prefix << F("\tGrowing Trees: Pass ") << i << endl;
    for (xcoord_t x = -xLimit; x <= xLimit; x++)
      for (ycoord_t y = safeDivide(worldHeight, 2) - 4; y <= yLimit; y++)
        if (block.get(x, y) == B_AIR && block.get(x, y - 1) == B_WOOD && (random() % 10 < chances[i]))
          block.set(x, y, G_T_WOOD);
    for (xcoord_t x = -xLimit; x <= xLimit; x++)
      for (ycoord_t y = 0; y <= yLimit; y++)
        if (block.get(x, y) == G_T_WOOD)
          block.set(x, y, B_WOOD);
  }
  cout << prefix << F("\tGrowing Trees: Adding Branches") << endl;
  for (xcoord_t x = -xLimit; x <= xLimit; x++)
    for (ycoord_t y = safeDivide(worldHeight, 2) - 4; y <= yLimit; y++)
      if (block.get(x, y) == B_AIR && block.isTouching(x, y, B_WOOD) && (random() % 10) == 0 && !block.isTouching(x, y, B_DIRT) && !block.isTouching(x, y, B_SAND) && !block.isTouching(x, y, B_STONE) && !block.isTouching(x, y, B_WATER_SOURCE))
        block.set(x, y, G_T_WOOD);
  for (xcoord_t x = -xLimit; x <= xLimit; x++)
    for (ycoord_t y = 0; y <= yLimit; y++)
      if (block.get(x, y) == G_T_WOOD)
        block.set(x, y, B_WOOD);
}
void World::generateLeaves () {
  cout << prefix << F("Generation Stage 16: Leaves") << endl << F("\tPass 1") << endl;
  for (xcoord_t x = -xLimit; x <= xLimit; x++)
    for (ycoord_t y = 0; y <= yLimit; y++)
      if (block.get(x, y) == B_AIR && block.isTouchingWide(x, y, B_WOOD) && ((!block.isTouchingWide(x, y, B_DIRT) && !block.isTouchingWide(x, y, B_STONE) && !block.isTouchingWide(x, y, B_SAND) && !block.isTouchingWide(x, y, B_WATER_SOURCE)) || (x <= -xLimit + 1 ? false : block.get(x - 2, y) == G_T_LEAVES) || (x >= xLimit - 1 ? false : block.get(x + 2, y) == G_T_LEAVES)))
        block.set(x, y, G_T_LEAVES);
  for (xcoord_t x = -xLimit; x <= xLimit; x++)
    for (ycoord_t y = 0; y <= yLimit; y++)
      if (block.get(x, y) == G_T_LEAVES)
        block.set(x, y, B_LEAVES);
#ifdef RENDER_WHILE_GENERATING
  screen.renderWorldOverview();
#endif
  cout << prefix << F("\tPass 2") << endl;
  for (xcoord_t x = -xLimit; x <= xLimit; x++)
    for (ycoord_t y = 0; y <= yLimit; y++)
      if ((block.get(x, y) == B_AIR && block.isTouching(x, y, B_LEAVES) && ((x == -xLimit ? false : block.get(x - 1, y) == B_LEAVES) || (x == xLimit ? false : block.get(x + 1, y) == B_LEAVES)) && ((x <= -xLimit + 1 ? false : block.get(x - 2, y) == B_WOOD) || (x >= xLimit - 1 ? false : block.get(x + 2, y) == B_WOOD))) && (random() % 5) != 0)
        block.set(x, y, G_T_LEAVES);
  for (xcoord_t x = -xLimit; x <= xLimit; x++)
    for (ycoord_t y = 0; y <= yLimit; y++)
      if (block.get(x, y) == G_T_LEAVES)
        block.set(x, y, B_LEAVES);
#ifdef RENDER_WHILE_GENERATING
  screen.renderWorldOverview();
#endif
}
void World::generatePlants () {
  cout << prefix << F("Generation Stage 17: Grass") << endl;
  cout << prefix << F("\tSeeding Grass Patches") << endl;
  for (xcoord_t x = -xLimit; x <= xLimit; x++)
    for (ycoord_t y = safeDivide(worldHeight, 2) - 2; y <= yLimit; y++)
      if (block.get(x, y) == B_AIR && block.get(x, y - 1) == B_DIRT && (random() % 20) == 0 && block.isOpenToSky(x, y, B_LEAVES))
        block.set(x, y, B_GRASS);
  for (int i = 0; i < 4; i++) {
    cout << prefix << F("\tGrowing Grass Patches:") << F(" Pass ") << i + 1 << endl;
    for (xcoord_t x = -xLimit; x <= xLimit; x++)
      for (ycoord_t y = safeDivide(worldHeight, 2) - 2; y <= yLimit; y++)
        if (block.get(x, y) == B_AIR && block.get(x, y - 1) == B_DIRT && (random() % 2) == 0 && block.isTouchingWide(x, y, B_GRASS) && block.isOpenToSky(x, y, B_LEAVES))
          block.set(x, y, G_T_GRASS);
    for (xcoord_t x = -xLimit; x <= xLimit; x++)
      for (ycoord_t y = safeDivide(worldHeight, 2) - 2; y <= yLimit; y++)
        if (block.get(x, y) == G_T_GRASS)
          block.set(x, y, B_GRASS);
  }
  cout << prefix << F("\tAdding Flowers") << endl;
  for (xcoord_t x = -xLimit; x <= xLimit; x++)
    for (ycoord_t y = safeDivide(worldHeight, 2) - 2; y <= yLimit; y++)
      if (block.get(x, y) == B_GRASS && (random() % 5) == 0)
        block.set(x, y, B_FLOWER);
  cout << prefix << F("\tRemoving Some Grass") << endl; //Makes patches have holes in them, as opposed to being full carpets of grass
  for (xcoord_t x = -xLimit; x <= xLimit; x++)
    for (ycoord_t y = safeDivide(worldHeight, 2) - 2; y <= yLimit; y++)
      if (block.get(x, y) == B_GRASS && (random() % 5) == 0)
        block.set(x, y, B_AIR);
}
