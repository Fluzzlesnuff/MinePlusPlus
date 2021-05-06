#ifndef item_h
#define item_h
#include "datatypes.h"

class ItemObject {
    friend class Item;
    xcoord_t x;
    ycoord_t y;
    id_t id;
    unsigned long timeOfCreation; //The time in milliseconds, from system start, that the item was dropped.
    ItemObject (xcoord_t xParam, ycoord_t yParam, id_t idParam);
};
extern ItemObject *items[]; //A list of pointers to ItemObjects.

class Item { //A set of functions for interacting with the items array.
  public:
    void spawn (xcoord_t x, ycoord_t y, id_t id); //Creates an item at the given coords.
    void pickUp (xcoord_t x, ycoord_t y); //Deletes an item at the given coords, and puts it into the player's inventory.
    bool despawn (); //Removes the oldest item. Used when the maximum number of items is reached.
    void list () const; //Lists the items currently in existence via serialLog.
};
extern Item item; //The item object for the item.xxx functions.

#endif
