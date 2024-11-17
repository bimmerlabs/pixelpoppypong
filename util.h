#pragma once

// useful macros
#define COUNTOF(x) sizeof(x)/sizeof(*x)
#define toINT(x) jo_fixed2int(x) // maybe reimplement here instead of relying on jo engine

void sanitizeValue(int* value, int min, int max);
void shuffleArray(unsigned int* array, unsigned int size);
