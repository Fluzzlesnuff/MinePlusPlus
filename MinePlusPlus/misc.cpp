#include "includes.h"

int safeDivide (int input, int divisor) {
  return (input - (input % divisor)) / divisor;
}

bool doubleIsNear(double a, double b, double threshold) {
  return abs(a - b) <= threshold;
}
int randomNumber(int num1, double prob1, int num2, double prob2, int num3, double prob3, int num4) {
  double prob4 = 0;
  if (prob1 + prob2 + prob3 + prob4 > 1.01)
    cout << PROB_SUM_ERR;

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

extern char *__brkval;

int freeMemory() {
  char top;
  return &top - __brkval;

}
