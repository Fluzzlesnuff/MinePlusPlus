#ifndef com_h
#define com_h

class CommunicationChannel {
  public:
    void init(long bitrate = 1000000);
    class Output {
      public:
        void prefix ();
        void print (const String& string);
        void print (const __FlashStringHelper *string);
        void println (const String& string);
        void println (const __FlashStringHelper *string);
        void log (const String& string);
        template<size_t length>
        void logChars (const char (&string)[length]);
        void logMultiple (const StringSumHelper &string);
        void log (const __FlashStringHelper *string);
        void log (const ExactCoordPair& coordPair); //Print a ExactCoordPair to the serial monitor.
        void log (const CoordPair& coordPair); //Print a CoordPair to the serial monitor.
        void throwError (Error error);
    };
    class Input {
        String incomingLine;
      public:
        String read ();
        CommandData parseCommand (const String& string);
        bool runCommands(); //reads com.in, parses, and runs commands. Should be called as often as possible.
        //Returns true if a command was recieved and run without errors.

        class Command {
            CommandType thisCommandType;
            String communicationName; //The name used in messages relating to the command, before the colon.
            int8_t numberOfArguments; //The required number of arguments, not including the command name.
            ArgumentType::ArgumentType argumentTypes[8];
          public:
            Command (CommandType commandTypeParam, const String& nameParam, index_t numArgsParam = 0, const ArgTypeList &argTypeListParam = ArgTypeList {
                       {}
                     });
            CommandData parsePerCommand (const String *args, const index_t inputArgsCount);
        };
        const Command setblock{CommandType::SetBlock, "Setblock", 3, ArgTypeList{{ArgumentType::XCoord, ArgumentType::YCoord, ArgumentType::ID}}}; //All that is to pass an array to the constructor.
        const Command getblock {
          CommandType::GetBlock, "Getblock", 2, ArgTypeList{{ArgumentType::XCoord, ArgumentType::YCoord}}
        };
        const Command teleport{CommandType::Teleport, "Teleport", 2, ArgTypeList{{ArgumentType::XCoord, ArgumentType::YCoord}}};
        const Command fill{CommandType::Fill, "Fill", 5, ArgTypeList{{ArgumentType::XCoord, ArgumentType::YCoord, ArgumentType::XCoord, ArgumentType::YCoord, ArgumentType::ID}}};
        const Command save{CommandType::Save, "Save"};
        const Command load{CommandType::Load, "Load"};
        const Command mem{CommandType::GetMemory, "Memory"};
        const Command map{CommandType::ShowOverview, "Map"};

    };
    Input in;
    Output out;
};


extern CommunicationChannel com;

class IOStream {

};

class IStream : public IOStream {

};

class OStream : public IOStream {
    void prefix ();
  public:
    OStream& operator<< (const OStream& out, int input);
    OStream& operator<< (const OStream& out, const String& input);
    OStream& operator<< (const OStream& out, char input);
};

extern OStream cout;
extern const char endl;

#include "comT.h"
#endif
