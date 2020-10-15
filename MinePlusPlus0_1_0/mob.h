#ifndef mob_h
#define mob_h

#include "datatypes.h"

class Mob { //A proper object. Used in arrays to control each mob.
  public:
    xcoord_t x; //x coordinate of the mob
    ycoord_t y; //y coordinate of the mob's HEAD
    id_t health; //current health score
    id_t maxHealth; //maximum possible health
    void damage (id_t hearts); //decrease the number of hearts of the mob
    void kill (); //kill the mob, without inducing an item drop. Overridden per-mob to drop item(s).
    bool walk (Direction dir); //Attempt to move the mob 1 block in the given direction. Returns true if the move is not obstructed by a solid block.
    void follow (bool safety = true); //walk()s the mob one block towards the player, avoiding lava, damaging drops, and fire. follow(false) overrides the safety behavior.
    int drop (id_t id1, id_t num1 = 1); //Summon items with the given number at the mob's FEET. Use multiple calls of drop() for multiple types of items
    int dropRand (id_t id1, id_t num1, float chance1, id_t num2 = 0, float chance2 = 0, id_t num3 = 0, float chance3 = 0 );
    /*  Summon items with the given numbers and probabilities at the mob's FEET.
        dropRand(I_STRING, 3, 0.5); has a 50% chance to drop 3 string, and no chance of any other number. Use multiple triplets to potentially drop different amounts
        i.e dropRand(I_STRING, 1, 1.0, 1, 0.5); to drop 1 string with a 50% chance of an additional string.
        Multiple calls of dropRand() can be used to drop more than 1 type of item */
};

class Hostile : public Mob {
    void attackMelee (id_t hearts); //attempt to damage the player by touch
    void attackRanged (id_t hearts, id_t projectileId); //fires a projectile of the given type at the player
};
class Passive : public Mob {

};
class Zombie : public Hostile {
    void kill ();
};
class Skeleton : public Hostile {

};
class Chicken : public Passive {

};
class Cow : public Passive {

};
class Pig : public Passive {

};
class Sheep : public Passive {

};

#endif
