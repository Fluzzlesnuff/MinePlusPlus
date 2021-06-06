#ifndef storage_h
#define storage_h
#include "datatypes.h"

class StorageSystem {
  uint8_t numWorlds;
  public:  
  void writeWord (uint16_t address, word value) const;
  word readWord (uint16_t address) const;
  uint16_t getCompressedSize () const; //Get an estimate (should be byte-accurate?) of saved world size
  uint16_t getFreeSpace () const; //Calculate the total free space in EEPROM, in bytes
  uint8_t findFreeSpace (uint16_t compressedSize) const; //Returns the gap index where the current world can be saved. 0 is before all others. 4 is at the end. 255 is not enough space.
  void reshuffleWorlds (uint8_t index) const; //Moves worlds 'left' to fill in gaps. Only moves worlds after the given gap index (0 moves all, 4 moves none)
  void sortTable () const;
  void makeTable (uint16_t* table) const;
  uint16_t getWorldStart (uint8_t index) const;
  uint16_t getWorldEnd (uint8_t index) const;
  //public:
    StorageSystem ();
    void printTable () const;
    void clearTable () const;
    void load (uint8_t worldIndex) const; //Loads a world from EEPROM into ram.
    void save () const; //Saves the world in RAM to EEPROM, overwriting any older saves.
};
extern StorageSystem storage;

#endif
