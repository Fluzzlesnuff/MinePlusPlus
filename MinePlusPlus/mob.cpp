#include "includes.h"

void Mob::damage (const id_t hearts) {
  if (hearts >= health) {
    kill();
  } else {
    health -= hearts;
  }
}

void Mob::kill () {

}

bool Mob::walk (const Direction dir) {
  if (dir == right) {
    if (!block.isSolid(block.get(x + 1, y)) && !block.isSolid(block.get(x + 1, y - 1))) {
      x++;
      return true;
    } else {
      return false;
    }
  } else if (dir == left) {
    if (!block.isSolid(block.get(x - 1, y)) && !block.isSolid(block.get(x - 1, y - 1))) {
      x--;
      return true;
    } else {
      return false;
    }
  } else if (dir == up) {
    if (!block.isSolid(block.get(x, y + 1))) {
      y++;
      return true;
    } else {
      return false;
    }
  }
}

int Mob::drop (const id_t id, const id_t num = 1) {

}

int Mob::dropRand (const id_t id1, const id_t num1, const float chance1, const id_t num2 = 0, const float chance2 = 0, const id_t num3 = 0, const float chance3 = 0 ) {

}

void Mob::follow (const bool safety = true) {

}

void Hostile::attackMelee (const id_t hearts) {

}

void Hostile::attackRanged (const id_t hearts, const id_t projectileId) {

}

void Zombie::kill () {
  dropRand(I_FLESH, 1, 1.0, 1, 0.5, 1, 0.1);
  dropRand(I_POTATO, 1, 0.1);
  dropRand(I_CARROT, 1, 0.1);
}
