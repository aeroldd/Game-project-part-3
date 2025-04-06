#ifndef UTIL_H
#define UTIL_H

void delay(int noMilliseconds);
void fancyPrint(const char* format, ...);
void pressAnyKey();

// Rolls a die of size max
int roll(int max);

// Rolls a d20 with checking for critical success or failure
int rolld20();

#endif