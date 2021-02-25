#include "includes.h"

void CommunicationChannel::Output::throwError (Error error) {
  cout << prefix;
  Serial.print(F("Error thrown: "));
  switch (error) {
    case Error::UNKNOWN_ERROR:
      Serial.println(F("Unknown Error"));
      break;
    case Error::XCOORD_OOB:
      Serial.println(F("X Coordinate Out-Of-Bounds"));
      break;
    case Error::YCOORD_OOB:
      Serial.println(F("Y Coordinate Out-Of-Bounds"));
      break;
    case Error::PARAM_OOB:
      Serial.println(F("Other Parameter Out-Of-Bounds"));
      break;
    case Error::PROB_SUM_ERR:
      Serial.println(F("Sum of Probabilities is more than 1"));
      break;
  }
  exit(0);
}

CommunicationChannel com;

OStream& operator<< (OStream& out, int input) {
  Serial.print(input);
  return out;
}
OStream& operator<< (OStream& out, const String& input) {
  Serial.print(input);
  return out;
}
OStream& operator<< (OStream& out, char input) {
  Serial.print(input);
  return out;
}
OStream& operator<< (OStream& out, const __FlashStringHelper *input) {
  Serial.print(input);
  return out;
}
OStream& operator<< (OStream& out, const CoordPair& coordPair) {
  Serial.print(coordPair.x);
  Serial.print(F(", "));
  Serial.print(coordPair.y);
  return out;
}
OStream& operator<< (OStream& out, const ExactCoordPair& coordPair) {
  Serial.print(coordPair.x);
  Serial.print(F(", "));
  Serial.print(coordPair.y);
  return out;
}
OStream& operator<< (OStream& out, IOStreamFlag flag) {
  switch (flag) {
    case endl:
      Serial.print('\n');
      break;
    case prefix:
      Serial.print(F("[At"));
      for (byte i = log10(millis()); i < 7; i++)
        Serial.print(F(" "));
      Serial.print(millis());
      Serial.print(F(" ms]:  "));
      break;
  }
  return out;
}
OStream cout;
