#ifndef comT_h
#define comT_h

#include "com.h"

template<size_t length>
void CommunicationChannel::Output::logChars (const char (&string)[length]) {
  com.out.prefix();
  Serial.println(string);
}

#endif
