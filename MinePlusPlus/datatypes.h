#ifndef datatypes_h
#define datatypes_h

typedef int16_t   ycoord_t;
typedef int8_t    xcoord_t;
typedef uint16_t  blockDBAddress_t;
typedef uint16_t  worldWidth_t;
typedef uint8_t   worldHeight_t;
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
  Five_Tick,
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
  GetBlock,
  Summon,
  SummonItem,
  Give,
  Teleport,
  Save,
  Load,
  GetMemory,
  ShowOverview
};
enum class ArgumentType {
  Other,
  XCoord,
  YCoord,
  TargetSelector,
  ID
};
enum class FunctionCallContext {
  Generic,
  Update,
  Generation
};
enum IOStreamFlag {
  endl,
  prefix
};

struct ArgTypeList {
  ArgumentType argTypes[8];
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
