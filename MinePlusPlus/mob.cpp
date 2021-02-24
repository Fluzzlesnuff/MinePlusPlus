#include "includes.h"

void Mob::damage (id_t hearts) {
  if (hearts >= health) {
    kill();
  } else {
    health -= hearts;
  }
}

void Mob::kill () {

}

bool Mob::walk (Direction dir) {
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

int Mob::drop (id_t id, id_t num) {

}

int Mob::dropRand (id_t id1, id_t num1, float chance1, id_t num2, float chance2, id_t num3, float chance3) {

}

void Mob::follow (bool safety) {

}

void Hostile::attackMelee (id_t hearts) {

}

void Hostile::attackRanged (id_t hearts, id_t projectileId) {

}

void Zombie::kill () {
  dropRand(I_FLESH, 1, 1.0, 1, 0.5, 1, 0.1);
  dropRand(I_POTATO, 1, 0.1);
  dropRand(I_CARROT, 1, 0.1);
}
