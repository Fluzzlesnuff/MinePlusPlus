#ifndef misc_h
#define misc_h

int safeDivide (const int input, const int divisor);
int randomNumber(const int num1, double prob1, const int num2 = 0, double prob2 = -1, const int num3 = 0, double prob3 = -1, const int num4 = 0);
bool doubleIsNear(const double a, const double b, const double threshold);
int freeMemory();

#endif
