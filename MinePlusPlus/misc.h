#ifndef misc_h
#define misc_h

int safeDivide (const int input, const int divisor);
int randomNumber(const int num1, double prob1, const int num2 = 0, double prob2 = -1, const int num3 = 0, double prob3 = -1, const int num4 = 0);
bool doubleIsNear(const double a, const double b, const double threshold);
int freeMemory();
template<typename T1, typename T2>
T1 floor (T1 value1, T2 value2);
template<typename T1, typename T2>
T1 ceil (T1 value1, T2 value2);

#include "miscT.h"
#endif
