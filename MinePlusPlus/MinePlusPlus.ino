//VERSION: development version

#include "includes.h"

#define GENERATE_ON_START

#define PRESET_SEED 189

const int16_t playerSpeed = 200;
bool selectedGenerateButton = false;

void setup() {
  com.init();
  Serial.println(freeMemory());
  leftButton.setSampleSize(10);
  jumpButton.setSampleSize(10);
  rightButton.setSampleSize(10);
  leftMouseButton.setSampleSize(10);
  rightMouseButton.setSampleSize(10);
  leftMouseButton.setThreshold(990);
  rightMouseButton.setThreshold(990);
#ifdef PRESET_SEED
  randomSeed(PRESET_SEED);
#endif
#ifndef PRESET_SEED
  randomSeed(analogRead(A15));
#endif
  com.out.print(F("\n\n\n\n"));
  com.out.log(F("Initializing Display"));
  GLCD.Init();
  com.out.log(F("\tComplete"));
  screen.renderBitmap(Bitmaps::UI::loadIcon, 16, 2, 31, 23);
  screen.renderBitmap(Bitmaps::UI::generateIcon, 16, 2, 79, 23);
  screen.renderBitmap(Bitmaps::UI::upArrow, 8, 1, 35, 40);
  Serial.println(freeMemory());
}

void loop() {
  if (world.isRunning)
    worldLoop();
  else {
#ifdef GENERATE_ON_START
    GLCD.ClearScreen();
    world.isRunning = true;
    world.generate(Default);
#endif
    if (leftButton.read()) {
      selectedGenerateButton = false;
      GLCD.ClearScreen();
      screen.renderBitmap(Bitmaps::UI::loadIcon, 16, 2, 31, 23);
      screen.renderBitmap(Bitmaps::UI::generateIcon, 16, 2, 79, 23);
      screen.renderBitmap(Bitmaps::UI::upArrow, 8, 1, 35, 40);
    }
    if (rightButton.read()) {
      selectedGenerateButton = true;
      GLCD.ClearScreen();
      screen.renderBitmap(Bitmaps::UI::loadIcon, 16, 2, 31, 23);
      screen.renderBitmap(Bitmaps::UI::generateIcon, 16, 2, 79, 23);
      screen.renderBitmap(Bitmaps::UI::upArrow, 8, 1, 83, 40);
    }
    if (jumpButton.read()) {
      GLCD.ClearScreen();
      if (selectedGenerateButton) {
        world.isRunning = true;
        world.generate(Default);
      } else {
        world.isRunning = true;
        world.load();
      }
    }
  }
}
void worldLoop() {
  if (com.in.runCommands())
    screen.renderWorld();
  if (leftButton.read(Normal, playerSpeed) && !rightButton.read()) {
    CoordPair newCoords = player.getCoords(-1);
    if (newCoords.x >= -xLimit) {
      player.move(newCoords);
      screen.renderWorld();
    }
  }
  if (rightButton.read(Normal, playerSpeed) && !leftButton.read()) {
    CoordPair newCoords = player.getCoords(1);
    if (newCoords.x <= xLimit) {
      player.move(newCoords);
      screen.renderWorld();
    }
  }
  if (jumpButton.read(Normal, playerSpeed)) {
    CoordPair newCoords = player.getCoords(0, 1);
    if (newCoords.y <= yLimit) {
      player.move(newCoords);
      screen.renderWorld();
    }
  }
  if (leftMouseButton.read(Normal, playerSpeed)) {
    CoordPair newCoords = player.getCoords(0, -1);
    if (newCoords.y >= 0) {
      player.move(newCoords);
      screen.renderWorld();
    }
  }
  if (rightMouseButton.read()) {
    world.save();
    exit(0);
  }
  screen.updateAnimations();
}

#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__
 
int freeMemory() {
  char top;
#ifdef __arm__
  return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
#else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}
