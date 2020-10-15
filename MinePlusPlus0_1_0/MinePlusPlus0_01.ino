#include "includes.h"

const int16_t playerSpeed = 200;
bool runningWorld = false;
bool selectedGenerateButton = false;

void setup() {
  com.init();
  leftButton.setSampleSize(10);
  jumpButton.setSampleSize(10);
  rightButton.setSampleSize(10);
  leftMouseButton.setSampleSize(10);
  rightMouseButton.setSampleSize(10);
  leftMouseButton.setThreshold(990);
  rightMouseButton.setThreshold(990);
  randomSeed(analogRead(A15));
  com.out.print("\n\n\n");
  com.out.log(F("Initializing Display"));
  GLCD.Init();
  com.out.log(F("\tComplete"));
  screen.renderBitmap(Bitmaps::UI::loadIcon, 16, 2, 31, 23);
  screen.renderBitmap(Bitmaps::UI::generateIcon, 16, 2, 79, 23);
  screen.renderBitmap(Bitmaps::UI::upArrow, 8, 1, 35, 40);
}

void loop() {
  if (runningWorld)
    worldLoop();
  else {
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
        runningWorld = true;
        world.generate(Default);
      } else {
        runningWorld = true;
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
}
