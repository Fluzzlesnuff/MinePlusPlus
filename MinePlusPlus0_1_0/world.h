#ifndef world_h
#define world_h

#include "datatypes.h"

extern worldWidth_t worldWidth;
extern worldHeight_t worldHeight;

extern xcoord_t xLimit; //Inclusive. Make negative to use negative x limit.
extern ycoord_t yLimit; //Inclusive. Bottom limit is 0.

class World { //One instance only, but used as a proper object.
  WorldSize size;
  void setWorldDimensions(WorldSize sizeParam);
  void start (); //Sets starts the necessary world services *after* a world has been either loaded or generated.
  public:
    void update (WorldUpdateType updateType); //Flows water, grows crops, drops gravel, etc.
    void setTickRate (double tickRateParam);
    void generate (WorldSize sizeParam); //Creates a new world in RAM.
    void load (); //Loads a world from EEPROM into ram.
    void save (); //Saves the world in RAM to EEPROM, overwriting any older saves.
};

extern World world; //The world object for world.xxx functions.

#endif
