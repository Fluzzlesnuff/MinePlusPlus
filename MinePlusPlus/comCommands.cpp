#include "includes.h"

CommunicationChannel::Input::Command::Command (CommandType commandTypeParam, const String& nameParam, uint8_t numArgsParam, const ArgTypeList& argTypeListParam) {
  thisCommandType = commandTypeParam;
  numberOfArguments = numArgsParam;
  communicationName = nameParam;
  for (uint8_t i = 0; i < 8; i++)
    argumentTypes[i] = argTypeListParam.argTypes[i];
}

CommandData CommunicationChannel::Input::parseCommand (const String& string) {
  if (string == "") {
    CommandData failCommand;
    failCommand.type = CommandType::NoCommand;
    return failCommand;
  }
  cout << prefix << "\"" + string << "\"" << endl;
  CommandData outputCommand;
  String commandArgs[8];
  uint8_t partIndex = 0;
  String currentPart;
  uint8_t numArgs = 0;
  for (uint8_t i = 0; i < string.length(); i++) {
    char c = string.charAt(i);
    if (c == ' ') {
      commandArgs[partIndex] = currentPart;
      currentPart = "";
      partIndex++;
      numArgs++;
    } else
      currentPart += c;
  }
  commandArgs[partIndex] = currentPart;
  if (commandArgs[0] == "setblock") {
    outputCommand = com.in.setblock.parsePerCommand(commandArgs, numArgs);
  } else if (commandArgs[0] == "getblock") {
    outputCommand = com.in.getblock.parsePerCommand(commandArgs, numArgs);
  } else if (commandArgs[0] == "tp") {
    outputCommand = com.in.teleport.parsePerCommand(commandArgs, numArgs);
  } else if (commandArgs[0] == "fill") {
    outputCommand = com.in.fill.parsePerCommand(commandArgs, numArgs);
  } else if (commandArgs[0] == "save") {
    outputCommand = com.in.save.parsePerCommand(commandArgs, numArgs);
  } else if (commandArgs[0] == "load") {
    outputCommand = com.in.load.parsePerCommand(commandArgs, numArgs);
  } else if (commandArgs[0] == "mem") {
    outputCommand = com.in.mem.parsePerCommand(commandArgs, numArgs);
  } else if (commandArgs[0] == "map") {
    outputCommand = com.in.map.parsePerCommand(commandArgs, numArgs);
  } else {
    cout << prefix << "Invalid Command" << endl;
    outputCommand.type = CommandType::CommandError;
  }
  return outputCommand;
}

CommandData CommunicationChannel::Input::Command::parsePerCommand (const String* args, uint8_t inputArgsCount) const {
  CommandData outputCommand;
  outputCommand.type = thisCommandType;
  if (inputArgsCount < numberOfArguments) {
    cout << prefix << communicationName << ": Not enough arguments." << endl;
    outputCommand.type = CommandType::CommandError;
  } else if (inputArgsCount > numberOfArguments) {
    cout << prefix << communicationName << ": Too many arguments." << endl;
    outputCommand.type = CommandType::CommandError;
  } else {
    for (uint8_t i = 0; i < numberOfArguments; i++) {
      if (args[i + 1] == "0") { //to differentiate between toInt() returning an error and an intentional 0, parsing of 0 is done here.
        outputCommand.args[i] = 0;
      }

      else if ((args[i + 1].charAt(0) == 'B' || args[i + 1].charAt(0) == 'b') && (args[i + 1].charAt(1) == '0' || args[i + 1].charAt(1) == '1')) { //binary to decimal parsing
        int16_t parsedDecimal = 0;
        for (uint8_t binaryParseIndex = args[i + 1].length() - 1; binaryParseIndex > 0; binaryParseIndex--) {
          if (args[i + 1].charAt(binaryParseIndex) == '1')
            parsedDecimal += bit(args[i + 1].length() - binaryParseIndex - 1);
          else if (args[i + 1].charAt(binaryParseIndex) != '0')
            cout << prefix << communicationName << ": Argument " << i + 1 << " is invalid." << endl;
        }
        outputCommand.args[i] = parsedDecimal;
      }

      else if (args[i + 1].charAt(0) == '~') { //relative coordinates to exact coordinates parsing
        if (!(argumentTypes[i] == ArgumentType::XCoord || argumentTypes[i] == ArgumentType::YCoord)) {
          cout << prefix << communicationName << ": Argument " << i + 1 << " is invalid." << endl;
          outputCommand.type = CommandType::CommandError;
        }

        int16_t parsedCoordinate = 0;
        int16_t numberPart = 0;
        if (args[i + 1].charAt(1) == '0' || args[i + 1].length() == 1)
          numberPart = 0;
        else {
          numberPart = (args[i + 1].substring(1)).toInt();
          if (numberPart == 0) {
            cout << prefix << communicationName << ": Argument " << i + 1 << " is invalid." << endl;
            outputCommand.type = CommandType::CommandError;
          }

        }
        if (argumentTypes[i] == ArgumentType::XCoord)
          outputCommand.args[i] = player.getCoords().x + numberPart;
        else if (argumentTypes[i] == ArgumentType::YCoord)
          outputCommand.args[i] = player.getCoords().y + numberPart;
      }

      else if (args[i + 1].toInt() != 0) {//regular parsing
        outputCommand.args[i] = args[i + 1].toInt();
      }

      else {
        cout << prefix << communicationName << ": Argument " << i + 1 << " is invalid." << endl;
        outputCommand.type = CommandType::CommandError;
      }

      if ((argumentTypes[i] == ArgumentType::XCoord) && (outputCommand.args[i] < -xLimit || outputCommand.args[i] > xLimit)) {
        cout << prefix << communicationName << ": Argument " << i + 1 << " is outside of the world." << endl;
        outputCommand.type = CommandType::CommandError;
      } else if ((argumentTypes[i] == ArgumentType::YCoord) && (outputCommand.args[i] < 0 || outputCommand.args[i] > yLimit)) {
        cout << prefix << communicationName << ": Argument " << i + 1 << " is outside of the world." << endl;
        outputCommand.type = CommandType::CommandError;
      }
    }
  }
  return outputCommand;
}

bool CommunicationChannel::Input::runCommands() {
  CommandData parsedCommand = com.in.parseCommand(com.in.read());
  if (parsedCommand.type == CommandType::NoCommand)
    return false;
  if (parsedCommand.type == CommandType::CommandError)
    return false;
  if (parsedCommand.type == CommandType::SetBlock) {
    block.set(parsedCommand.args[0], parsedCommand.args[1], parsedCommand.args[2]);
    cout << prefix << "Set block at (" << parsedCommand.args[0] << ", " << parsedCommand.args[1] << ") with ID " << parsedCommand.args[2];
    return true;
  }
  if (parsedCommand.type == CommandType::GetBlock) {
    cout << prefix << "Block at (" + parsedCommand.args[0] << ", " << parsedCommand.args[1] << ") has ID " << block.get(parsedCommand.args[0], parsedCommand.args[1]) << endl;
    return true;
  }
  if (parsedCommand.type == CommandType::Teleport) {
    player.move(parsedCommand.args[0], parsedCommand.args[1]);
    cout << prefix << "Teleported to (" << parsedCommand.args[0] << ", " << parsedCommand.args[1] << ")" << endl;
    return true;
  }
  if (parsedCommand.type == CommandType::Fill) {
    id_t blockIDToFill = parsedCommand.args[4]; //The name is so long to prevent name conflicts, as blockID, id, etc. may already be used.
    for (int8_t xIndex = parsedCommand.args[0]; xIndex <= parsedCommand.args[2]; xIndex++) {
      for (uint8_t yIndex = parsedCommand.args[1]; yIndex <= parsedCommand.args[3]; yIndex++) {
        block.set(xIndex, yIndex, blockIDToFill);
      }
    }
    cout << prefix << "Filled " << (abs(parsedCommand.args[0] - parsedCommand.args[2]) + 1) * (abs(parsedCommand.args[1] - parsedCommand.args[3]) + 1) << " blocks with ID " << parsedCommand.args[4] << endl;
    return true;
  }
  if (parsedCommand.type == CommandType::Save) {
    world.save();
    cout << prefix << F("Saved World") << endl;
    return true;
  }
  if (parsedCommand.type == CommandType::Load) {
    if (!world.isRunning) {
      world.load();
      cout << prefix << F("Loaded World") << endl;
      return true;
    } else {
      cout << prefix << F("Load: Cannot load a world while a world is running!") << endl;
      return false;
    }
  }
  if (parsedCommand.type == CommandType::GetMemory) {
    cout << prefix << "Free Memory: " << freeMemory() << "B" << endl;
    return true;
  }
  if (parsedCommand.type == CommandType::ShowOverview) {
    cout << prefix << "Showing World Map" << endl;
    GLCD.ClearScreen();
    screen.renderWorldOverview();
    while (!leftButton.read() && !jumpButton.read() && !rightButton.read() && !leftMouseButton.read() && !leftMouseButton.read());
    GLCD.ClearScreen();
    screen.forceRenderWorld();
    return true;
  }

}
