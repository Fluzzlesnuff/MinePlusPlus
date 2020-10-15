#ifndef datatypes_h
#define datatypes_h

//For all enums, structs, and typedefs. #include in *header files* that need them. Already included in .cpp files due to includes.h

typedef int16_t   ycoord_t;
typedef int8_t    xcoord_t;
typedef uint16_t  blockDBAddress_t;
typedef uint16_t  worldWidth_t;
typedef uint8_t   worldHeight_t;
typedef uint8_t   index_t;
typedef int8_t    nindex_t;
typedef uint16_t  largeindex_t;
typedef int16_t   nlargeindex_t;

typedef byte id_t;

enum Direction {
  left,
  right,
  up
};

enum WorldSize {
  Default,   //Default 256*32
  Tall,      //Tall    128*64
  Wide,      //Wide    512*16
};

enum WorldUpdateType {
  Constant,
  Tick,
  Two_Tick,
  Four_Tick,
  Eight_Tick
};

enum Error {
  UNKNOWN_ERROR = -1,
  XCOORD_OOB    = -2,
  YCOORD_OOB    = -3,
  PARAM_OOB     = -4,
  PROB_SUM_ERR  = -5
};

enum MeasurementType {
  Chebyshev = 1,
  Taxicab = 2
};

enum ButtonReadMode {
  Normal,
  Push,
  Press,
  Release,
  Change
};

enum CommandType {
  NoCommand,
  CommandError,
  Fill,
  SetBlock,
  Summon,
  SummonItem,
  Give,
  Teleport,
  Save,
  Load
};

namespace ArgumentType {
enum ArgumentType {
  Other,
  XCoord,
  YCoord,
  TargetSelector,
  ID
};
}

struct ArgTypeList {
  ArgumentType::ArgumentType argTypes[8];
};

struct CoordPair {
  xcoord_t x;
  ycoord_t y;
};

struct ExactCoordPair {
  double x, y;
};

struct CommandData {
  CommandType type;
  int16_t args[8];
};

#endif
