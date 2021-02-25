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

IStream& operator>> (IStream& in, String& var) {
  String out;
  while (Serial.available()) {
    const char c = char(Serial.read());
    if (c == '\n') {
      var = out;
      return in;
    }
    out += c;
  }
}

IStream cin;
