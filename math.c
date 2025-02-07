#include "math.h"

FIXED FIXED_100 = toFIXED(100);
FIXED FIXED_127 = toFIXED(127);
FIXED FIXED_255 = toFIXED(255);
FIXED FIXED_510 = toFIXED(510);
FIXED FIXED_765 = toFIXED(765);
FIXED FIXED_1024 = toFIXED(1024);

// from danny duarte
Uint32 ApproximateIntegerSqrt(Uint32 value)
{
    // Initial variables for approximation
    Uint32 baseValue = 1;                // Base value used to scale the approximation
    Uint32 estimatedValue = value >> 2; // Initial approximation, roughly value / 4

    // Refine the approximation using bit shifts
    while (baseValue < estimatedValue)
    {
        estimatedValue >>= 1;  // Halve the estimated value
        baseValue <<= 1;      // Double the base value
    }

    // Return the refined approximation
    return baseValue + estimatedValue;
}

Uint8 state;

Uint8 FastRandom()
{
  state ^= state << 13;
  state ^= state << 17;
  state ^= state << 5;
  return state;
}

Uint8 RandomInRange(Uint8 from, Uint8 to)
{
  Uint8 number = FastRandom();
  return from + (number % ABS(to - from)); // modulus
}

int my_random_range(int min, int max) {
    int number = jo_random(99999);
    return min + (number % ABS(max - min) + 1); // modulus
}