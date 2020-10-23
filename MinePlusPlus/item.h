#ifndef item_h
#define item_h

#include "datatypes.h"

class ItemObject { //A proper object. Used in arrays. This is under the-hood functionality, and is not usable outside of the Item class.
    friend class Item; //Only the Item class (and so the item.xxx functions) can access this class.
    xcoord_t x;
    ycoord_t y;
    id_t id;
    unsigned long timeOfCreation; //The time in milliseconds, from system start, that the item was dropped.
    ItemObject (const xcoord_t xParam, const ycoord_t yParam, const id_t idParam);
};

extern ItemObject *items[]; //A list of pointers to ItemObjects.

class Item { //A set of functions for interacting with the items array.
  public:
    void spawn (const xcoord_t x, const ycoord_t y, const id_t id); //Creates an item at the given coords.
    void pickUp (const xcoord_t x, const ycoord_t y); //Deletes an item at the given coords, and puts it into the player's inventory.
    bool despawn (); //Removes the oldest item. Used when the maximum number of items is reached.
    void list (); //Lists the items currently in existence via serialLog.
};

extern Item item; //The item object for the item.xxx functions.

#endif
