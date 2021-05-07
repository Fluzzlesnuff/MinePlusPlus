namespace EEPROMLocations {
  const uint16_t shutdown =         0x0000;
  const uint16_t worldTableStart =  0x0001;
  const uint16_t worldTableEnd =    0x0010;
  const uint16_t versionHigh =      0x0011;
  const uint16_t versionLow =       0x0012;
  const uint16_t prefsStart =       0x0013;
  const uint16_t prefsEnd =         0x001f;
  const uint16_t dataStart =        0x0020;

  const uint16_t eepromEnd =        0x1000;

namespace World {
  const uint16_t worldSize =      0x0000;
  const uint16_t playerX =        0x0001;
  const uint16_t playerY =        0x0002;
  const uint16_t playerHealth =   0x0003;
  const uint16_t playerHunger =   0x0004;
  const uint16_t playerEffects =  0x0005;
  const uint16_t spawnpointX =    0x0006;
  const uint16_t spawnpointY =    0x0007;
  const uint16_t dataStart =      0x0008;
}
}
