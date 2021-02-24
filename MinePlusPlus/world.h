#ifndef world_h
#define world_h

#include "datatypes.h"

#define UPDATE_DISTANCE 10 //The horizontal distance from the player in which updates occur. Inclusive, so
                            //a value of 1 updates the player's block and those to the left and right

extern worldWidth_t worldWidth;
extern worldHeight_t worldHeight;

extern xcoord_t xLimit; //Inclusive. Make negative to use negative x limit.
extern ycoord_t yLimit; //Inclusive. Bottom limit is 0.

class World { //One instance only, but used as a proper object.
    WorldSize size;
    double ticksPerSecond;
    uint16_t msPerTick;
    void setWorldDimensions(WorldSize sizeParam);
    void start (); //Sets starts the necessary world services *after* a world has been either loaded or generated.
    
    bool update (WorldUpdateType updateType); //Flows water, grows crops, drops gravel, etc.
    bool updateConstant();
    bool updateTick();
    bool update2Tick();
    bool update4Tick();
    bool update5Tick();
    bool update8Tick();
    
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
    void generateLakes ();
    void endUnderGroundGeneration ();
    void generateDeserts ();
    void generateTrees ();
    void generateLeaves ();
    void generatePlants ();
  public:
    bool isRunning = false;
    bool tryUpdate();
    void setTickRate (double tickRateParam);
    void generate (WorldSize sizeParam); //Creates a new world in RAM.
    void load (); //Loads a world from EEPROM into ram.
    void save (); //Saves the world in RAM to EEPROM, overwriting any older saves.
};

extern World world; //The world object for world.xx functions.

#endif
