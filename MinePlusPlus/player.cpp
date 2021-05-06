#include "includes.h"

void Player::move (xcoord_t xParam, ycoord_t yParam) {
  move(double(xParam), double(yParam));
}
void Player::move (double xParam, double yParam) {
#ifdef PLAYER_MOVE_LOGGING
  cout << F("Moving player to (") << xParam << F(", ") << yParam << F(")") << endl;
#endif
  if (x < -xLimit || x > xLimit)
    cout << XCOORD_OOB;
  if (y < 0 || y > yLimit)
    cout << YCOORD_OOB;
  x = xParam;
  y = yParam;
}
void Player::move (const ExactCoordPair& coords) {
  move(coords.x, coords.y);
}
void Player::move (const CoordPair& coords) {
  move(coords.x, coords.y);
}
bool Player::walk (Direction dir, double distance) {
  switch (dir) {
    case left:
      if (block.isAir(block.get(getCoords(-1))) && block.isAir(block.get(getCoords(-1, -1)))) {
        move(x - distance, y);
        return true;
      }
      return false;
    case right:
      if (block.isAir(block.get(getCoords(1))) && block.isAir(block.get(getCoords(1, -1)))) {
        move(x + distance, y);
        return true;
      }
      return false;
    case up:
      if (block.isAir(block.get(getCoords(0, 1)))) {
        move(x, y + distance);
        return true;
      }
      return false;
     default: return false;
  }
}
CoordPair Player::getCoords(xcoord_t xOffset, ycoord_t yOffset) const {
  return CoordPair{xcoord_t(round(x + xOffset)), ycoord_t(round(y + yOffset))};
}
ExactCoordPair Player::getExactCoords(double xOffset, double yOffset) const {
  return ExactCoordPair{x + xOffset, y + yOffset};
}
Player player;
