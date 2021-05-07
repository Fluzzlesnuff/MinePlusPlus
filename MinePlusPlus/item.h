#ifndef item_h
#define item_h
#include "datatypes.h"

class Item {
    static constexpr uint8_t numItems = 32;
    static Item *items[numItems]; //A list of pointers to ItemObjects.
    xcoord_t x;
    ycoord_t y;
    id_t id;
    unsigned long timeOfCreation; //The time in milliseconds, from system start, that the item was dropped.
  public:
    Item (xcoord_t xParam, ycoord_t yParam, id_t idParam);
    static void spawn (xcoord_t x, ycoord_t y, id_t id); //Creates an item at the given coords.
    static void pickUp (xcoord_t x, ycoord_t y); //Deletes an item at the given coords, and puts it into the player's inventory.
    static bool despawn (); //Removes the oldest item. Used when the maximum number of items is reached.
    static void list (); //Lists the items currently in existence via serialLog.
};

#endif
