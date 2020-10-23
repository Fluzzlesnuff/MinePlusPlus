#ifndef player_h
#define player_h

#include "datatypes.h"

class Player { //One instance only, but used as a proper object.
    double x, y;
  public:
    void move (const xcoord_t xParam, const ycoord_t yParam); // Forcefully change the player's location to the given coordinates.
    void move (const double xParam, const double yParam); //Alterate function to change the players' location to the given decimal coordinates.
    void move (const ExactCoordPair coords);
    void move (const CoordPair coords);
    bool walk (const Direction dir, const double distance = 1); //Attempt to move the player 1 block in the given direction. Returns true if the move is not obstructed by a solid block.
    CoordPair getCoords(const xcoord_t xOffset = 0, const ycoord_t yOffset = 0); // By default, returns the player's coordinates. If offsets are supplied, returns the offset coordinates.
    /*(e.g. getCoords(1, 0) returns the coordinates of the block to the right of the player's head.*/
    ExactCoordPair getExactCoords(const double xOffset = 0, const double yOffset = 0);
};

extern Player player;

#endif
