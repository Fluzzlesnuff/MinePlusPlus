#include "includes.h"

String CommunicationChannel::Input::read () {
  while (Serial.available()) {
    char c = char(Serial.read());
    if (c == '\n') {
      String returnData = incomingLine;
      incomingLine = "";
      return returnData;
    }
    else
      incomingLine += c;
  }
  return "";
}
