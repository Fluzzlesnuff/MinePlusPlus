#ifndef command_h
#define command_h
#include "includes.h"

class CommandManager {
  public:
    CommandData parseCommand (const String& string);
    bool runCommands(); //reads com.in, parses, and runs commands. Should be called as often as possible.
    //Returns true if a command was recieved and run without errors.
    class Command {
        CommandType thisCommandType;
        String communicationName; //The name used in messages relating to the command, before the colon.
        int8_t numberOfArguments; //The required number of arguments, not including the command name.
        ArgumentType argumentTypes[8];
      public:
        Command (CommandType commandTypeParam, const String& nameParam, uint8_t numArgsParam = 0, const ArgTypeList &argTypeListParam = ArgTypeList {{}});
        CommandData parsePerCommand (const String *args, const uint8_t inputArgsCount) const;
    };
    const Command setblock{CommandType::SetBlock, "Setblock", 3, ArgTypeList{{ArgumentType::XCoord, ArgumentType::YCoord, ArgumentType::ID}}}; //All that is to pass an array to the constructor.
    const Command getblock {CommandType::GetBlock, "Getblock", 2, ArgTypeList{{ArgumentType::XCoord, ArgumentType::YCoord}}};
    const Command teleport{CommandType::Teleport, "Teleport", 2, ArgTypeList{{ArgumentType::XCoord, ArgumentType::YCoord}}};
    const Command fill{CommandType::Fill, "Fill", 5, ArgTypeList{{ArgumentType::XCoord, ArgumentType::YCoord, ArgumentType::XCoord, ArgumentType::YCoord, ArgumentType::ID}}};
    const Command save{CommandType::Save, "Save"};
    const Command load{CommandType::Load, "Load"};
    const Command mem{CommandType::GetMemory, "Memory"};
    const Command map{CommandType::ShowOverview, "Map"};
};
extern CommandManager command;

#endif
