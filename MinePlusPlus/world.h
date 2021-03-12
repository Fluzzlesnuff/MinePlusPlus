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
    friend class Block;
    WorldSize size;
    double ticksPerSecond;
    uint16_t msPerTick;
    void setWorldDimensions(WorldSize sizeParam);
    void start (); //Sets starts the necessary world services *after* a world has been either loaded or generated.

    xcoord_t leftmostXCoordinate; //Valid area for updates
    xcoord_t rightmostXCoordinate; //Valid area for updates
    bool updateMadeChanges;
    bool lightingUpdateNeeded;
    
    void update (WorldUpdateType updateType); //Flows water, grows crops, drops gravel, etc.
    uint8_t updateAll ();
    void updateConstant();
    void updateTick();
    void update2Tick();
    void update4Tick();
    void update5Tick();
    void update8Tick();

    void updateLighting ();
    void updateWater ();
    void updateLava ();
    void updateFallingBlocks ();
    void updateFloatingItems ();
    
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
