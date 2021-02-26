#ifndef comT_h
#define comT_h

#include "com.h"
template <typename T>
OStream& operator<< (OStream& out, T input) {
  Serial.print(input);
  return out;
}

#endif
