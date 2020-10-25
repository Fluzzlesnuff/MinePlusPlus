#include "includes.h"

void Player::move (const xcoord_t xParam, const ycoord_t yParam) {
  move(double(xParam), double(yParam));
}

void Player::move (const double xParam, const double yParam) {
#ifdef PLAYER_MOVE_LOGGING
  com.out.logMultiple("Moving player to (" + String(xParam) + ", " + String(yParam) + ").");
#endif
  if (x < -xLimit || x > xLimit)
    com.out.throwError(XCOORD_OOB);
  if (y < 0 || y > yLimit)
    com.out.throwError(YCOORD_OOB);
  x = xParam;
  y = yParam;
}

void Player::move (const ExactCoordPair coords) {
  move(coords.x, coords.y);
}

void Player::move (const CoordPair coords) {
  move(coords.x, coords.y);
}

bool Player::walk (const Direction dir, const double distance = 1) {
  switch (dir) {
    case left:
      if (block.isAir(block.get(player.getCoords(-1))) && block.isAir(block.get(player.getCoords(-1, -1)))) {

        return true;
      }
  }
}

CoordPair Player::getCoords(const xcoord_t xOffset = 0, const ycoord_t yOffset = 0) {
  return CoordPair{xcoord_t(round(x + xOffset)), ycoord_t(round(y + yOffset))};
}

ExactCoordPair Player::getExactCoords(const double xOffset = 0, const double yOffset = 0) {
  return ExactCoordPair{x + xOffset, y + yOffset};
}

Player player;
