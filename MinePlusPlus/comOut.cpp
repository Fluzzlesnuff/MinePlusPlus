#include "includes.h"

void CommunicationChannel::Output::print (const String string) {
  Serial.print(string);
}
void CommunicationChannel::Output::println (const String string) {
  Serial.println(string);
}
void CommunicationChannel::Output::log (const String string) {
  com.out.prefix();
  Serial.println(string);
}
void CommunicationChannel::Output::logMultiple (const StringSumHelper& string) {
  com.out.prefix();
  Serial.println(string);
}
void CommunicationChannel::Output::log (const ExactCoordPair coordPair) {
  com.out.prefix();
  Serial.print(double(coordPair.x));
  Serial.print(", ");
  Serial.println(double(coordPair.y));
}
void CommunicationChannel::Output::log (const CoordPair coordPair) {
  com.out.prefix();
  Serial.print(coordPair.x);
  Serial.print(F(", "));
  Serial.println(coordPair.y);
}

void CommunicationChannel::Output::throwError (const Error error) {
  com.out.prefix();
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

void CommunicationChannel::Output::prefix () {
  Serial.print(F("[At "));
  Serial.print(millis());
  for (byte i = log10(millis()); i < 7; i++)
    Serial.print(F(" "));
  Serial.print(F("ms]:    "));
}


CommunicationChannel com;
