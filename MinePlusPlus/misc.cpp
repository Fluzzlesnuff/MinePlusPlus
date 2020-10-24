#include "includes.h"

int safeDivide (const int input, const int divisor) {
  return (input - (input % divisor)) / divisor;
}

bool doubleIsNear(const double a, const double b, const double threshold) {
  return abs(a - b) <= threshold;
}
int randomNumber(const int num1, double prob1, const int num2, double prob2, const int num3, double prob3, const int num4) {
  double prob4 = 0;
  if (prob1 + prob2 + prob3 + prob4 > 1.01)
    com.out.throwError(PROB_SUM_ERR);

  if (doubleIsNear(prob2, -1, 0.01))
    prob2 = 1 - prob1;
  else if (doubleIsNear(prob3, -1, 0.01))
    prob3 = 1 - (prob1 + prob2);
  else
    prob4 = 1 - (prob1 + prob2 + prob3);

  uint16_t randomIndex = random(1, 1001) % 100; //A random number from 1 to 100.
  if (randomIndex <= prob1 * 100)
    return num1;
  else if (randomIndex <= (prob1 * 100) + (prob2 * 100))
    return num2;
  else if (randomIndex <= (prob1 * 100) + (prob2 * 100) + (prob3 * 100))
    return num3;
  else
    return num4;
}

#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__

int freeMemory() {
  char top;
#ifdef __arm__
  return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
#else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}
