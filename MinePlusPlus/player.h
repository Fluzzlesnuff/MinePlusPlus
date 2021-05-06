#ifndef player_h
#define player_h
#include "datatypes.h"

class Player {
    double x, y;
  public:
    void move (xcoord_t xParam, ycoord_t yParam); // Forcefully change the player's location to the given coordinates.
    void move (double xParam, double yParam); //Alterate function to change the players' location to the given decimal coordinates.
    void move (const ExactCoordPair& coords);
    void move (const CoordPair& coords);
    bool walk (Direction dir, double distance = 1); //Attempt to move the player 1 block in the given direction. Returns true if the move is not obstructed by a solid block.
    CoordPair getCoords(xcoord_t xOffset = 0, ycoord_t yOffset = 0); // By default, returns the player's coordinates. If offsets are supplied, returns the offset coordinates.
    ExactCoordPair getExactCoords(double xOffset = 0, double yOffset = 0);
};
extern Player player;

#endif
