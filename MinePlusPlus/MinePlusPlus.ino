//VERSION: development version

#include "includes.h"
bool selectedGenerateButton = false;

void setup() {
  com.init();
  cout << prefix << F("Communication Channel(s) Initialized") << endl;
  leftButton.setSampleSize(10);
  jumpButton.setSampleSize(10);
  rightButton.setSampleSize(10);
  leftMouseButton.setSampleSize(10);
  rightMouseButton.setSampleSize(10);
  leftMouseButton.setThreshold(990);
  rightMouseButton.setThreshold(990);
  world.setTickRate(10);
#ifdef PRESET_SEED
  randomSeed(PRESET_SEED);
#endif
#ifndef PRESET_SEED
  randomSeed(analogRead(A15));
#endif
  cout << prefix << F("Initializing Display") << endl;
  GLCD.Init();
  cout << prefix << F("\tComplete") << endl;
#ifndef GENERATE_ON_START
  screen.renderBitmap(Bitmaps::UI::loadIcon, 16, 2, 31, 23);
  screen.renderBitmap(Bitmaps::UI::generateIcon, 16, 2, 79, 23);
  screen.renderBitmap(Bitmaps::UI::upArrow, 8, 1, 35, 40);
#endif
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
  if(world.tryUpdate())
    screen.renderWorld();
  if (leftButton.read(Normal, PLAYER_SPEED) && !rightButton.read()) {
    CoordPair newCoords = player.getCoords(-1);
    if (newCoords.x >= -xLimit) {
      player.move(newCoords);
      screen.renderWorld();
    }
  }
  if (rightButton.read(Normal, PLAYER_SPEED) && !leftButton.read()) {
    CoordPair newCoords = player.getCoords(1);
    if (newCoords.x <= xLimit) {
      player.move(newCoords);
      screen.renderWorld();
    }
  }
  if (jumpButton.read(Normal, PLAYER_SPEED)) {
    CoordPair newCoords = player.getCoords(0, 1);
    if (newCoords.y <= yLimit) {
      player.move(newCoords);
      screen.renderWorld();
    }
  }
  if (leftMouseButton.read(Normal, PLAYER_SPEED)) {
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