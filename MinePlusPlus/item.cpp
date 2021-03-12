#include "includes.h"

ItemObject *items[16];
ItemObject::ItemObject (xcoord_t xParam, ycoord_t yParam, id_t idParam) {
  x = xParam;
  y = yParam;
  id = idParam;
  timeOfCreation = millis();
}
void Item::pickUp (xcoord_t x, ycoord_t y) {
}
void Item::spawn (xcoord_t x, ycoord_t y, id_t id) {
  for (uint8_t i = 0; i < (sizeof(items) / sizeof(items[0])); i++) { //find the first NULL pointer in the array and assign it the value of the new item
    if (items[i] == NULL) {
      items[i] = new ItemObject(x, y, id);
      return;
    }
  }
  despawn(); //If no pointers were NULL (i.e. return; didn't get called), despawn an item and try again.
  spawn(x, y, id); //recurse
}
bool Item::despawn () {
  unsigned long oldestTime = millis();
  id_t oldestItemIndex = 255;
  for (uint8_t i = 0; i < (sizeof(items) / sizeof(items[0])); i++) {
    if (items[i] == NULL) //If the pointer at items[i] is a null pointer, skip over it
      continue;
    if (items[i]->timeOfCreation < oldestTime) { // If a new oldest item is found, set the oldestTime and oldestItemIndex to that new item
      oldestTime = items[i]->timeOfCreation;
      oldestItemIndex = i;
    }
  }
  if (oldestItemIndex == 255) //If no items were found, return false for a failure.
    return false;
  delete items[oldestItemIndex]; //Once the loop has completed and the oldest item has been found, delete it.
  items[oldestItemIndex] = NULL;
  return true;
}
Item item;
