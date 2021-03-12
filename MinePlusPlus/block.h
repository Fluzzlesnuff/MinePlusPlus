#ifndef block_h
#define block_h
#include "datatypes.h"
#define TT_ARRAY_SIZE 5
extern id_t* blockDB;
class Block { //Not used in the object sense. Instead, this is just a set of functions.
    friend class World;
    bool scanToolTable (const id_t table[TT_ARRAY_SIZE], const id_t& tool);
    blockDBAddress_t coordsToAddress (xcoord_t x, ycoord_t y);
  public:
    id_t get (xcoord_t x, ycoord_t y); //Gets the ID of the block at a certain coordinate pair.
    id_t get (const CoordPair& coords); //Like above, but takes a CoordPair, most often from player.getCoords or mob.getCoords.
    void set (xcoord_t x, ycoord_t y, id_t id); //Changes the block at a given coordinate to the given ID.
    void set (const CoordPair& coords, id_t id); //Like above, but takes a CoordPair, most often from player.getCoords or mob.getCoords.
    bool place (xcoord_t x, ycoord_t y, id_t blockType); //Attempts to place a block at the given coords. Returns true if the block was replaceable.
    bool place (const CoordPair& coords, id_t blockType);
    //These functions return whether a block is a certain type. They take an ID, most often from block.get().
    bool isOpaque(id_t id);
    bool isSolid(id_t id);
    bool isWater(id_t id);
    bool isDeletedWater(id_t id);
    bool isLava(id_t id);
    bool isDeletedLava(id_t id);
    bool isFlammable(id_t id);
    bool isFarmland(id_t id);
    bool isAir(id_t id);
    bool isLight(id_t id);
    bool isBrokenByFluid(id_t id);
    bool isBrokenByFallingBlocks(id_t id);
    bool isMineable (id_t id); //Returns true if the block is mineable.
    bool dropsItem (id_t id, id_t toolUsed); //Returns true if the block drops items when mined with the given tool.
    bool isAnimated(id_t id);
    //These functions return the number of blocks of the given type that are touching the block at the given coordinate pair.
    byte isTouching(xcoord_t x, ycoord_t y, id_t id); //Only counts blocks directly adjacent (max 4).
    byte isTouching(const CoordPair& coords, id_t id);
    byte isTouchingWide(xcoord_t x, ycoord_t y, id_t id); //Counts blocks diagonally, too (max 8).
    byte isTouchingWide(const CoordPair& coords, id_t id);
    uint16_t isNear(xcoord_t x, ycoord_t y, id_t id, byte distance, MeasurementType measurementType); //Counts nearby blocks of the given type.
    uint16_t isNear(const CoordPair& coords, id_t id, byte distance, MeasurementType measurementType);
    bool isOpenToSky(xcoord_t x, ycoord_t y, id_t ignoreBlock1 = Blocks::air, id_t ignoreBlock2 = Blocks::air);
    id_t convertToDeleted (id_t id);
    id_t convertFromDeleted (id_t id);
    void createBlockDB (worldWidth_t width, worldHeight_t height);
};
extern Block block;

#endif
