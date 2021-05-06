#ifndef block_h
#define block_h
#include "datatypes.h"
#define TT_ARRAY_SIZE 5
extern id_t* blockDB;
class Block {
    friend class World;
    bool scanToolTable (const id_t table[TT_ARRAY_SIZE], const id_t& tool) const;
    blockDBAddress_t coordsToAddress (xcoord_t x, ycoord_t y) const;
  public:
    id_t get (xcoord_t x, ycoord_t y) const;
    id_t get (const CoordPair& coords) const;
    void set (xcoord_t x, ycoord_t y, id_t id);
    void set (const CoordPair& coords, id_t id);
    bool place (xcoord_t x, ycoord_t y, id_t blockType); //Attempts to place a block at the given coords. Returns true if the block was replaceable.
    bool place (const CoordPair& coords, id_t blockType);
    //These functions return whether a block is a certain type. They take an ID, most often from block.get().
    bool isOpaque(id_t id) const;
    bool isSolid(id_t id) const;
    bool isFlammable(id_t id) const;
    bool isBrokenByFluid(id_t id) const;
    bool isBrokenByFallingBlocks(id_t id) const;
    bool isMineable (id_t id) const;
    bool isAnimated(id_t id) const;
    bool isAir(id_t id) const;
    bool isLight(id_t id) const;
    bool isWater(id_t id) const;
    bool isDeletedWater(id_t id) const;
    bool isLava(id_t id) const;
    bool isDeletedLava(id_t id) const;
    bool isFarmland(id_t id) const;
    bool isCrop(id_t id) const;
    bool isWheat(id_t id) const;
    bool isCarrot(id_t id) const;
    bool isPotato(id_t id) const;
    //These functions return the number of blocks of the given type that are touching the block at the given coordinate pair.
    byte isTouching(xcoord_t x, ycoord_t y, id_t id) const; //Only counts blocks directly adjacent (max 4).
    byte isTouching(const CoordPair& coords, id_t id) const;
    byte isTouchingWide(xcoord_t x, ycoord_t y, id_t id) const; //Counts blocks diagonally, too (max 8).
    byte isTouchingWide(const CoordPair& coords, id_t id) const;
    uint16_t isNear(xcoord_t x, ycoord_t y, id_t id, byte distance, MeasurementType measurementType) const; //Counts nearby blocks of the given type.
    uint16_t isNear(const CoordPair& coords, id_t id, byte distance, MeasurementType measurementType) const;
    //Other block functions
    bool isOpenToSky(xcoord_t x, ycoord_t y, id_t ignoreBlock1 = Blocks::air, id_t ignoreBlock2 = Blocks::air) const;
    bool dropsItem (id_t id, id_t toolUsed) const; //Returns true if the block drops items when mined with the given tool.
    id_t convertToDeleted (id_t id) const; //Convert from Blocks::water0 to Blocks::Update::deletedWater0, for example
    id_t convertFromDeleted (id_t id) const; //Convert from Blocks::Update::deletedWater0 to Blocks::water0, for example
    void createBlockDB (worldWidth_t width, worldHeight_t height); //Initializes the blockDB (block database)
};
extern Block block;

#endif
