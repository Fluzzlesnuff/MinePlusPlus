#ifndef storage_h
#define storage_h
#include "datatypes.h"

class StorageSystem {
  void writeWord (uint16_t address, word value);
  word readWord (uint16_t address);
  uint16_t getCompressedSize ();
  void reshuffleWorlds (uint8_t index); //Moves worlds 'left' to fill in gaps. Only moves worlds after the given gap index (0 moves all, 4 moves none)
  uint8_t findOpenSpace (uint16_t compressedSize); //Returns the gap index where the current world can be saved. 0 is before all others. 4 is at the end. 255 is not enough space. 
  void sortTable ();
  public:
    void printTable ();
    void clearTable ();
    void load (); //Loads a world from EEPROM into ram.
    void save (); //Saves the world in RAM to EEPROM, overwriting any older saves.
};
extern StorageSystem storage;

#endif
