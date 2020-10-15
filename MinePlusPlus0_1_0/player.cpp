#include "includes.h"

void Player::move (xcoord_t xParam, ycoord_t yParam) {
  move(double(xParam), double(yParam));
}

void Player::move (double xParam, double yParam) {
#ifdef PLAYER_MOVE_LOGGING
  com.out.log("Moving player to (" + String(xParam) + ", " + String(yParam) + ").");
#endif
  if (x < -xLimit || x > xLimit)
    com.out.throwError(XCOORD_OOB);
  if (y < 0 || y > yLimit)
    com.out.throwError(YCOORD_OOB);
  x = xParam;
  y = yParam;
}

void Player::move (ExactCoordPair coords) {
  move(coords.x, coords.y);
}

void Player::move (CoordPair coords) {
  move(coords.x, coords.y);
}

bool Player::walk (Direction dir, double distance = 1) {
  switch (dir) {
    case left:
      if (block.isAir(block.get(player.getCoords(-1))) && block.isAir(block.get(player.getCoords(-1, -1)))) {

        return true;
      }
  }
}

CoordPair Player::getCoords(xcoord_t xOffset = 0, ycoord_t yOffset = 0) {
  return CoordPair{xcoord_t(round(x + xOffset)), ycoord_t(round(y + yOffset))};
}

ExactCoordPair Player::getExactCoords(double xOffset = 0, double yOffset = 0) {
  return ExactCoordPair{x + xOffset, y + yOffset};
}

Player player;
