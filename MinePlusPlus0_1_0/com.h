#ifndef com_h
#define com_h

class CommunicationChannel {
  public:
    void init(long bitrate = 1000000);
    class Output {
        void prefix ();
      public:
        void print (String string);
        void println (String string);
        void log (String string);
        void log (ExactCoordPair coordPair); //Print a ExactCoordPair to the serial monitor.
        void log (CoordPair coordPair); //Print a CoordPair to the serial monitor.
        void throwError (Error error);
    };
    class Input {
        String incomingLine;
      public:
        String read ();
        CommandData parseCommand (String string);
        bool runCommands(); //reads com.in, parses, and runs commands. Should be called as often as possible.
        //Returns true if a command was recieved and run without errors.

        class Command {
            CommandType thisCommandType;
            String communicationName; //The name used in messages relating to the command, before the colon.
            int8_t numberOfArguments; //The required number of arguments, not including the command name.
            ArgumentType::ArgumentType argumentTypes[8];
          public:
            Command (CommandType commandTypeParam, String nameParam, index_t numArgsParam = 0, ArgTypeList argTypeListParam = ArgTypeList{{}});
            CommandData parsePerCommand (String *args, index_t inputArgsCount);
        };
        Command setblock{CommandType::SetBlock, "Setblock", 3, ArgTypeList{{ArgumentType::XCoord, ArgumentType::YCoord, ArgumentType::ID}}}; //All that is to pass an array to the constructor.
        Command getblock{CommandType::GetBlock, "Getblock", 2, ArgTypeList{{ArgumentType::XCoord, ArgumentType::YCoord}}};
        Command teleport{CommandType::Teleport, "Teleport", 2, ArgTypeList{{ArgumentType::XCoord, ArgumentType::YCoord}}};
        Command fill{CommandType::Fill, "Fill", 5, ArgTypeList{{ArgumentType::XCoord, ArgumentType::YCoord, ArgumentType::XCoord, ArgumentType::YCoord, ArgumentType::ID}}};
        Command save{CommandType::Save, "Save"};
        Command load{CommandType::Load, "Load"};

    };
    Input in;
    Output out;
};


extern CommunicationChannel com;
#endif
