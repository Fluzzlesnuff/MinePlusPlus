#ifndef command_h
#define command_h
#include "includes.h"

using ParseFunction = bool(*)(int16_t*);

class Command {
    const String commandName; //The name used in messages relating to the command, before the colon.
    const ParseFunction function; //The function used to parse the command
    const uint8_t numArgs; //The required number of arguments, not including the command name.
    const ArgumentType* argTypes;
    static const String* splitString (const String& string); //Split human-input command string by spaces
    const int16_t* parseArgs (const String* args) const; //Convert string arguments into numeric arguments. Returns nullptr on error.
  public:
    Command (const String& nameParam, const ParseFunction functionParam, uint8_t numArgsParam = 0, const ArgumentType* argTypesParam = nullptr);
    static bool runCommands(); //reads cin, parses, and runs commands. Should be called as often as possible.
};

extern Command setblock;
extern Command getblock;
extern Command teleport;
extern Command fill;
extern Command save;
extern Command load;
extern Command mem;
extern Command showMap;

#endif
