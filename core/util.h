#pragma once

// useful macros
#define COUNTOF(x) sizeof(x)/sizeof(*x)
#define toINT(x) jo_fixed2int(x) // maybe reimplement here instead of relying on jo engine

extern unsigned char hunds;
extern unsigned char tens;
extern unsigned char ones;

void sanitizeValue(int* value, int min, int max);
void shuffleArray(unsigned int* array, unsigned int size);

int minmumValue(int min, int value);

void convertNumberToDigits(int number);