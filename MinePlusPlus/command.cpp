#include "includes.h"
#ifdef COMMANDS_ENABLED
Command::Command (const String& nameParam, const ParseFunction functionParam, uint8_t numArgsParam, const ArgumentType* argTypesParam) :
  commandName {nameParam}, function {functionParam}, numArgs {numArgsParam}, argTypes {argTypesParam}
{}
const String* Command::splitString (const String& string) {
  String out[8];
  uint8_t wordIndex = 0;
  String currentWord;
  for (uint8_t i = 0; i < string.length(); ++i) {
    char c = string.charAt(i);
    if (c == ' ') {
      out[wordIndex] = currentWord;
      currentWord = "";
      ++wordIndex;
    } else
      currentWord += c;
  }
  out[wordIndex] = currentWord;
  for (uint8_t i = wordIndex + 1; i < 8; ++i) //Zero all remaining strings
    out[i] = "";
  return out;
}
const int16_t*  Command::parseArgs (const String* args) const {
  int16_t out[7];
  uint8_t numArgsFound = 0;
  for (uint8_t i = 1; i < 8; ++i) { //Starts at 1 because args[0] is command name
    if (args[i] == "")
      break;
    ++numArgsFound;
  }
  if (numArgsFound < numArgs) {
    cout << prefix << commandName << F(": Not enough arguments.") << endl;
    return nullptr;
  } else if (numArgsFound > numArgs) {
    cout << prefix << commandName << F(": Too many arguments.") << endl;
    return nullptr;
  }
  for (uint8_t i = 1; i <= numArgs; ++i) {
    if (args[i] == "0") { //to differentiate between toInt() returning an error and an intentional 0, parsing of 0 is done here.
      out[i - 1] = 0;
    } else if ((args[i].charAt(0) == 'B' || args[i].charAt(0) == 'b') && (args[i].charAt(1) == '0' || args[i].charAt(1) == '1')) { //binary to decimal parsing
      int16_t parsedDecimal = 0;
      for (uint8_t binaryParseIndex = args[i].length() - 1; binaryParseIndex > 0; binaryParseIndex--) {
        if (args[i].charAt(binaryParseIndex) == '1')
          parsedDecimal += bit(args[i].length() - binaryParseIndex - 1);
        else if (args[i].charAt(binaryParseIndex) != '0')
          cout << prefix << commandName << F(": Argument ") << i << F(" is invalid.") << endl;
      }
      out[i - 1] = parsedDecimal;
    } else if (args[i].charAt(0) == '~') { //relative coordinates to exact coordinates parsing
      if (!(argTypes[i] == ArgumentType::XCoord || argTypes[i] == ArgumentType::YCoord)) {
        cout << prefix << commandName << F(": Argument ") << i << F(" is invalid.") << endl;
        return nullptr;
      }
      int16_t parsedCoordinate = 0;
      int16_t numberPart = 0;
      if (args[i].charAt(1) == '0' || args[i].length() == 1)
        numberPart = 0;
      else {
        numberPart = (args[i].substring(1)).toInt();
        if (numberPart == 0) {
          cout << prefix << commandName << F(": Argument ") << i << F(" is invalid.") << endl;
          return nullptr;
        }
      }
      if (argTypes[i] == ArgumentType::XCoord)
        out[i - 1] = player.getCoords().x + numberPart;
      else if (argTypes[i] == ArgumentType::YCoord)
        out[i - 1] = player.getCoords().y + numberPart;
    } else if (args[i].toInt() != 0) {//regular parsing
      out[i - 1] = args[i].toInt();
    } else {
      cout << prefix << commandName << F(": Argument ") << i << F(" is invalid.") << endl;
      return nullptr;
    }
    if ((argTypes[i] == ArgumentType::XCoord && (out[i - 1] < -xLimit || out[i - 1] > xLimit)) || (argTypes[i] == ArgumentType::YCoord && (out[i - 1] < 0 || out[i - 1] > yLimit))) { //Check bounds
      cout << prefix << commandName << F(": Argument ") << i << F(" is outside of the world.") << endl;
      return nullptr;
    }
  }
  return out;
}
bool Command::runCommands() {
  String string;
  cin >> string;
  if (string == "")
    return false;
  const String* args = splitString(string);
  Command* commandObject;
  if (args[0] == F("setblock"))
    commandObject = &setblock;
  else if (args[0] == F("getblock"))
    commandObject = &getblock;
  else if (args[0] == F("tp"))
    commandObject = &teleport;
  else if (args[0] == F("fill"))
    commandObject = &fill;
  else if (args[0] == F("save"))
    commandObject = &save;
  else if (args[0] == F("load"))
    commandObject = &load;
  else if (args[0] == F("mem"))
    commandObject = &mem;
  else if (args[0] == F("map"))
    commandObject = &showMap;
  else {
    cout << prefix << F("Invalid Command") << endl;
    return false;
  }
  int16_t* parsedArgs = commandObject->parseArgs(args);
  if (parsedArgs == nullptr)
    return false;
  return commandObject->function(parsedArgs);
}

// ########################
// # COMMAND DECLARATIONS #
// ########################
namespace Functions {
bool setblock (const int16_t* args) {
  block.set(args[0], args[1], args[2]);
  cout << prefix << F("Set block at (") << args[0] << F(", ") << args[1] << F(") with ID ") << args[2] << endl;
  return true;
}
bool getblock (const int16_t* args) {
  cout << prefix << F("Block at (") << args[0] << F(", ") << args[1] << F(") has ID ") << format16 << block.get(args[0], args[1]) << format10 << endl;
  return true;
}
bool teleport (const int16_t* args) {
  player.move(args[0], args[1]);
  cout << prefix << F("Teleported to (") << args[0] << F(", ") << args[1] << ')' << endl;
  return true;
}
bool fill (const int16_t* args) {
  for (int8_t xIndex = args[0]; xIndex <= args[2]; ++xIndex) {
    for (uint8_t yIndex = args[1]; yIndex <= args[3]; ++yIndex) {
      block.set(xIndex, yIndex, args[4]);
    }
  }
  cout << prefix << F("Filled ") << (abs(args[0] - args[2]) + 1) * (abs(args[1] - args[3]) + 1) << F(" blocks with ID ") << format16 << args[4] << format10 << endl;
  return true;
}
bool save (const int16_t* args) {
  world.save();
  cout << prefix << F("Saved World") << endl;
  return true;
}
bool load (const int16_t* args) {
  if (!world.isRunning) {
    world.load();
    cout << prefix << F("Loaded World") << endl;
    return true;
  } else {
    cout << prefix << F("Load: Cannot load a world while a world is running!") << endl;
    return false;
  }
}
bool mem (const int16_t* args) {
  cout << prefix << F("Free Memory: ") << freeMemory() << 'B' << endl;
  return true;
}
bool showMap (const int16_t* args) {
  cout << prefix << F("Showing World Map") << endl;
  GLCD.ClearScreen();
  xcoord_t center = 0;
  do {
    screen.renderWorldOverview(center);
    while (!leftButton.read() && !rightButton.read() && !jumpButton.read());
    if (leftButton.read())
      center = max(-xLimit + 64, center - 32);
    else if (rightButton.read())
      center = min(xLimit - 63, center + 32);
  } while (!jumpButton.read());
  GLCD.ClearScreen();
  screen.forceRenderWorld();
  return true;
}
}
namespace ArgLists {
const ArgumentType setblock [] {ArgumentType::XCoord, ArgumentType::YCoord, ArgumentType::ID};
const ArgumentType getblock [] {ArgumentType::XCoord, ArgumentType::YCoord};
const ArgumentType teleport [] {ArgumentType::XCoord, ArgumentType::YCoord};
const ArgumentType fill     [] {ArgumentType::XCoord, ArgumentType::YCoord, ArgumentType::XCoord, ArgumentType::YCoord, ArgumentType::ID};
}

Command setblock ("Setblock", Functions::setblock, 3, ArgLists::setblock);
Command getblock ("Getblock", Functions::getblock, 2, ArgLists::getblock);
Command teleport ("Teleport", Functions::teleport, 2, ArgLists::teleport);
Command fill ("Fill", Functions::fill, 5, ArgLists::fill);
Command save ("Save", Functions::save);
Command load ("Load", Functions::load);
Command mem ("Memory", Functions::mem);
Command showMap ("Map", Functions::showMap);
#endif
