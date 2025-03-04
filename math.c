#include "math.h"

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

int my_random_range(int min, int max) {
    int number = jo_random(99999);
    return min + (number % ABS(max - min) + 1); // modulus
}

// Precomputed sine values
// probably has too many entries?
static const FIXED sineTable[99] = {
    0, 1143, 2287, 3429, 4571, 5711, 6850, 7986, 9120,
    10252, 11380, 12504, 13625, 14742, 15854, 16961, 18064, 19160,
    20251, 21336, 22414, 23486, 24550, 25606, 26655, 27696, 28729,
    29752, 30767, 31772, 32767, 33753, 34728, 35693, 36647, 37589,
    38521, 39440, 40347, 41243, 42125, 42995, 43852, 44695, 45525,
    46340, 47142, 47929, 48702, 49460, 50203, 50931, 51643, 52339,
    53019, 53683, 54331, 54963, 55577, 56175, 56755, 57319, 57864,
    58393, 58903, 59395, 59870, 60326, 60763, 61183, 61583, 61965,
    62328, 62672, 62997, 63302, 63589, 63856, 64103, 64331, 64540,
    64729, 64898, 65047, 65176, 65286, 65376, 65446, 65496, 65526,
    65536, 65526, 65496, 65446, 65376, 65286, 65176, 65047, 64898,
};

// Function to get sine from the precomputed table
FIXED my_fixed_sine(int angle) {
    if (angle <= 90) 
        return sineTable[angle];
    else if (angle <= 180) 
        return sineTable[180 - angle];  // Mirror for 90°-180°
    else if (angle <= 270) 
        return -sineTable[angle - 180]; // Negative for 180°-270°
    else 
        return -sineTable[360 - angle]; // Negative mirror for 270°-360°
}