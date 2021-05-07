#include "includes.h"

worldWidth_t worldWidth;
worldHeight_t worldHeight;
xcoord_t xLimit; //Inclusive. Make negative to use negative x limit.
ycoord_t yLimit; //Inclusive. Bottom limit is 0.

void World::setTickRate (double tickRateParam) {
  ticksPerSecond = tickRateParam;
  msPerTick = round(1000 / ticksPerSecond);
  cout << prefix << F("Set Tick Rate: ") << ticksPerSecond << F(" TPS (") << msPerTick << F("ms)") << endl;
}
void World::setWorldDimensions(WorldSize sizeParam) {
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
World world;
