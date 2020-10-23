#ifndef world_h
#define world_h

#include "datatypes.h"

extern worldWidth_t worldWidth;
extern worldHeight_t worldHeight;

extern xcoord_t xLimit; //Inclusive. Make negative to use negative x limit.
extern ycoord_t yLimit; //Inclusive. Bottom limit is 0.

class World { //One instance only, but used as a proper object.
    WorldSize size;
    void setWorldDimensions(const WorldSize sizeParam);
    void start (); //Sets starts the necessary world services *after* a world has been either loaded or generated.
    void generateAir ();
    void generateStone ();
    void generateDirtLayer ();
    void generateCaves ();
    void generateGravelVeins ();
    void generateDirtVeins ();
    void generateDiamond ();
    void generateIron ();
    void generateGold ();
    void generateCoal ();
    void generateLavaPools ();
    void generateWaterPools ();
    void endUnderGroundGeneration ();
  public:
    bool isRunning = false;
    void update (const WorldUpdateType updateType); //Flows water, grows crops, drops gravel, etc.
    void setTickRate (const double tickRateParam);
    void generate (const WorldSize sizeParam); //Creates a new world in RAM.
    void load (); //Loads a world from EEPROM into ram.
    void save (); //Saves the world in RAM to EEPROM, overwriting any older saves.
};

extern World world; //The world object for world.xxx functions.

#endif
