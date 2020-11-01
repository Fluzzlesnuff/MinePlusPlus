#ifndef miscT_h
#define miscT_h

#include "misc.h"

template<typename T1, typename T2>
T1 floor (T1 value1, T2 value2) {
  return value1 < value2 ? value1 : value2; //If they're the same, this will return value1, but that doesn't matter, because they're the same.
}
template<typename T1, typename T2>
T1 ceil (T1 value1, T2 value2) {
  return value1 > value2 ? value1 : value2; //If they're the same, this will return value2, but that doesn't matter, because they're the same.
}

#endif
