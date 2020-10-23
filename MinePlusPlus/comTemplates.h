#ifndef comTemplates_t
#define comTemplates_t

#include "com.h"

template<size_t length>
void CommunicationChannel::Output::logChars (const char (&string)[length]) {
  com.out.prefix();
  Serial.println(string);
}

#endif
