#include "includes.h"
#include "storageLocations.h"

StorageSystem::StorageSystem () {
  sortTable();
  for (int i = 0; i < 4; ++i)
    if (getWorldStart(i) == 0xffff) {
      numWorlds = i;
      break;
    }
  numWorlds = 4;
}
void StorageSystem::load (uint8_t worldIndex) const {
#ifdef SAVE_LOAD_LOGGING
  cout << prefix << F("Beginning World Loading") << endl;
#endif
  world.setWorldDimensions(WorldSize(EEPROM.read(EEL::World::worldSize) >> 4));
  xLimit = (worldWidth - 1) / 2;
  yLimit = worldHeight - 1;
  block.createBlockDB(worldWidth, worldHeight);
  uint16_t EEPROMAddress = EEL::World::dataStart;
  uint16_t blockDBAddress = 0;
  while (true) {
    byte byteRead = EEPROM.read(EEPROMAddress);
    if (byteRead == Storage::threeByteFlag) {
      byte numberToLoad = EEPROM.read(EEPROMAddress + 1);
      byte blockToLoad = EEPROM.read(EEPROMAddress + 2);
#ifdef SAVE_LOAD_BYTE_LOGGING
      cout << F("F 3\t\t# ") << numberToLoad << F("\t\tI ") << blockToLoad << F("\tB ") << blockDBAddress << F("-") << (blockDBAddress + numberToLoad - 1) << F("\t\tE ") << EEPROMAddress << F("-") << (EEPROMAddress + 2) << endl;
#endif
      EEPROMAddress += 3;
      for (uint8_t i = 0; i < numberToLoad; ++i)
        blockDB[blockDBAddress + i] = blockToLoad;
      blockDBAddress += numberToLoad - 1;
    } else if (byteRead == Storage::fourByteFlag) {
      uint16_t numberToLoad = (EEPROM.read(EEPROMAddress + 1) * 256) + EEPROM.read(EEPROMAddress + 2);
      byte blockToLoad = EEPROM.read(EEPROMAddress + 3);
#ifdef SAVE_LOAD_BYTE_LOGGING
      cout << F("F 4\t\t# ") << numberToLoad << F("\t\tI ") << blockToLoad << F("\tB ") << blockDBAddress << F("-") << (blockDBAddress + numberToLoad - 1) << F("\t\tE ") << EEPROMAddress << F("-") << (EEPROMAddress + 3) << endl;
#endif
      EEPROMAddress += 4;
      for (uint16_t i = 0; i < numberToLoad; ++i)
        blockDB[blockDBAddress + i] = blockToLoad;
      blockDBAddress += numberToLoad - 1;
    } else if (byteRead == Storage::end) {
#ifdef SAVE_LOAD_BYTE_LOGGING
      cout << prefix << F("Found COMP_END") << endl;
#endif
      break;
    }
    else {
#ifdef SAVE_LOAD_BYTE_LOGGING
      cout << F("\t\t\t\tI ") << byteRead << F("\tB ") << blockDBAddress << F("\t\t\tE ") << EEPROMAddress << endl;
#endif
      blockDB[blockDBAddress] = byteRead;
      ++EEPROMAddress;
    }
    ++blockDBAddress;
  }
  player.move(EEPROM.read(EEL::World::playerX), EEPROM.read(EEL::World::playerY));
  world.start();
#ifdef SAVE_LOAD_LOGGING
  cout << prefix << F("\tFinished") << endl;
#endif
}
void StorageSystem::save () const {
  uint16_t compressedSize = getCompressedSize();
  cout << F("Size: ") << compressedSize << endl;
  uint8_t openSpace = findFreeSpace(compressedSize);
  if (openSpace == 255) {
    cout << F("Could not save; not enough available space.\n");
    return;
  }

  //Actually save world
  EEPROM.update(EEL::World::worldSize, world.size << 4);
  EEPROM.update(EEL::World::playerX, lowByte(player.getCoords().x));
  EEPROM.update(EEL::World::playerY, player.getCoords().y);
  uint16_t numberFound = 0;
  uint16_t EEPROMAddress = EEL::World::dataStart;
  id_t currentBlockID = blockDB[0];
  uint16_t i;
  for (i = 0; i < worldWidth * worldHeight; ++i) {
    if (currentBlockID == blockDB[i]) {
      ++numberFound;
    } else {
      if (numberFound == 1) {
        EEPROM.update(EEPROMAddress, currentBlockID);
#ifdef SAVE_LOAD_BYTE_LOGGING
        cout << F("\t\t\t\tI ") << currentBlockID << F("\tB ") << i - 1 << F("\t\t\tE ") << EEPROMAddress << endl;
#endif
        EEPROMAddress += 1;
      } else if (numberFound < 256) {
        EEPROM.update(EEPROMAddress, Storage::threeByteFlag);
        EEPROM.update(EEPROMAddress + 1, lowByte(numberFound));
        EEPROM.update(EEPROMAddress + 2, currentBlockID);
#ifdef SAVE_LOAD_BYTE_LOGGING
        cout << F("F 3\t\t# ") << numberFound << F("\t\tI ") << currentBlockID << F("\tB ") << i - numberFound << F("-") << i - 1 << F("\t\tE ") << EEPROMAddress << F("-") << EEPROMAddress + 2 << endl;
#endif
        EEPROMAddress += 3;
      } else {
        EEPROM.update(EEPROMAddress, Storage::fourByteFlag);
        EEPROM.update(EEPROMAddress + 1, highByte(numberFound));
        EEPROM.update(EEPROMAddress + 2, lowByte(numberFound));
        EEPROM.update(EEPROMAddress + 3, currentBlockID);
#ifdef SAVE_LOAD_BYTE_LOGGING
        cout << F("F 4\t\t# ") << numberFound << F("\t\tI ") << currentBlockID << F("\tB ") << i - numberFound << F("-") << i - 1 << F("\t\tE ") << EEPROMAddress << F("-") << EEPROMAddress + 3 << endl;
#endif
        EEPROMAddress += 4;
      }
      currentBlockID = blockDB[i];
      numberFound = 1;
    }
  }
  if (numberFound == 1) {
    EEPROM.update(EEPROMAddress, currentBlockID);
#ifdef SAVE_LOAD_BYTE_LOGGING
    cout << F("\t\t\t\tI ") << currentBlockID << F("\tB ") << i - 1 << F("\t\t\tE ") << EEPROMAddress << endl;
#endif
    ++EEPROMAddress;
  } else if (numberFound < 256) {
    EEPROM.update(EEPROMAddress, Storage::threeByteFlag);
    EEPROM.update(EEPROMAddress + 1, lowByte(numberFound));
    EEPROM.update(EEPROMAddress + 2, currentBlockID);
#ifdef SAVE_LOAD_BYTE_LOGGING
    cout << F("F 3\t\t# ") << numberFound << F("\t\tI ") << currentBlockID << F("\tB ") << i - numberFound << F("-") << i - 1 << F("\t\tE ") << EEPROMAddress << F("-") << EEPROMAddress + 2 << endl;
#endif
    EEPROMAddress += 3;
  } else {
    EEPROM.update(EEPROMAddress, Storage::fourByteFlag);
    EEPROM.update(EEPROMAddress + 1, highByte(numberFound));
    EEPROM.update(EEPROMAddress + 2, lowByte(numberFound));
    EEPROM.update(EEPROMAddress + 3, currentBlockID);
#ifdef SAVE_LOAD_BYTE_LOGGING
    cout << F("F 4\t\t# ") << numberFound << F("\t\tI ") << currentBlockID << F("\tB ") << i - numberFound << F("-") << i - 1 << F("\t\tE ") << EEPROMAddress << F("-") << EEPROMAddress + 3 << endl;
#endif
    EEPROMAddress += 4;
  }
  EEPROM.update(EEPROMAddress, Storage::end);
  EEPROM.update(EEPROMAddress + 1, Storage::end);
}

uint16_t StorageSystem::getCompressedSize () const {
  uint16_t output = 0;
  uint16_t numberFound = 0;
  id_t currentBlockID = blockDB[0];
  for (uint16_t i = 0; i <= worldWidth * worldHeight; ++i) {
    if (currentBlockID == blockDB[i] && i != worldWidth * worldHeight) {
      ++numberFound;
    } else {
      if (numberFound == 1) {
        ++output;
      } else if (numberFound < 256) {
        output += 3;
      } else {
        output += 4;
      }
      currentBlockID = blockDB[i];
      numberFound = 1;
    }
  }
  return output;
}
void StorageSystem::reshuffleWorlds (uint8_t index) const {

}
uint16_t StorageSystem::getFreeSpace () const {
  uint16_t freeSpace = getWorldStart(0) - EEL::dataStart - 1; //Space before first world
  for (int index = 0; index < numWorlds - 1; ++index) {//Spaces between worlds
    freeSpace += getWorldStart(index + 1) - getWorldEnd(index) - 1;
  }
  freeSpace += EEL::eepromEnd - getWorldEnd(numWorlds - 1); //Space between last world and end.
  return freeSpace;
}
uint8_t StorageSystem::findFreeSpace (uint16_t compressedSize) const {
  using namespace EEL;
  sortTable();
  if (readWord(worldTableStart) - dataStart >= compressedSize)
    return 0;
  for (uint8_t i = 1; i <= 3; i++)
    if ((readWord(worldTableStart + (4 * i)) - readWord(worldTableStart + (4 * i) - 2)) > compressedSize)
      return i;
  if (eepromEnd - readWord(worldTableEnd - 1) >= compressedSize)
    return 4;
  return 255;
}

void StorageSystem::printTable () const {
  using namespace EEL;
  cout << F("World Table:") << endl;
  for (uint8_t i = worldTableStart; i < worldTableEnd; i += 4) {
    if (readWord(i) == 0xffff)
      cout << F("Unset") << endl;
    else
      cout << readWord(i) << F("-") << readWord(i + 2) << endl;
  }
}
void StorageSystem::clearTable () const {
  using namespace EEL;
  for (uint8_t i = worldTableStart; i <= worldTableEnd; ++i)
    EEPROM.update(i, 0xff);
}
void StorageSystem::sortTable () const {
  using namespace EEL;
  uint16_t table[10];
  for (uint8_t i = 0; i < 8; i += 2) {
    table[i] = readWord(worldTableStart + (i * 2));
    table[i + 1] = readWord(worldTableStart + (i * 2) + 2);
  }
  table[8] = 0xffff;
  table[9] = 0xffff;
  for (uint8_t firstUnsorted = 0; firstUnsorted < 8; firstUnsorted += 2) {
    uint16_t smallestIndex = 8;
    for (uint8_t i = firstUnsorted; i < 8; i += 2)
      if (table[i] < table[smallestIndex]) {
        smallestIndex = i;
      }
    //swap smallest unsorted into first unsorted index
    cout << F("Swapping entry ") << firstUnsorted << F(" (") << table[firstUnsorted] << F("-") << table[firstUnsorted + 1] << F(") with entry ") << smallestIndex << F(" (") << table[smallestIndex] << F("-") << table[smallestIndex + 1] << endl;
    uint16_t temp = table[smallestIndex];
    table[smallestIndex] = table[firstUnsorted];
    table[firstUnsorted] = temp;
    temp = table[smallestIndex + 1];
    table[smallestIndex + 1] = table[firstUnsorted + 1];
    table[firstUnsorted + 1] = temp;
  }
  for (uint8_t i = 0; i < 8; ++i) {
    cout << F("Writing entry ") << i << F(" (") << table[i] << F(") at addr.") << worldTableStart + (i * 2) << endl;
    writeWord((worldTableStart + (i * 2)), table[i]);
  }
}
void StorageSystem::makeTable (uint16_t* table) const {
  for (uint8_t i = 0; i < 8; ++i)
    writeWord(EEL::worldTableStart + (i * 2), table[i]);  
}
uint16_t StorageSystem::getWorldStart (uint8_t index) const {
  return readWord(EEL::worldTableStart + index * 4);
}
uint16_t StorageSystem::getWorldEnd (uint8_t index) const {
  return readWord(EEL::worldTableStart + index * 4 + 2);
}

void StorageSystem::writeWord (uint16_t address, word value) const {
  EEPROM.update(address, highByte(value));
  EEPROM.update(address + 1, lowByte(value));
}
word StorageSystem::readWord (uint16_t address) const {
  return (EEPROM.read(address) * 256) + EEPROM.read(address + 1);
}

StorageSystem storage;
