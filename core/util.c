#include <jo/jo.h>
#include "util.h"

unsigned char hunds = 0;
unsigned char tens = 0;
unsigned char ones = 0;

TIME g_Timer = {
    .min_ones = 0,
    .sec_tens = 0,
    .sec_ones = 0,
};

// shuffles an array of integers
void shuffleArray(unsigned int* array, unsigned int size)
{
    unsigned int i = 0;

    for (i = 0; i < size - 1; i++)
    {
        unsigned int j = i + jo_random(0xFFFF) / (0xFFFF / (size - i) + 1);
        int t = array[j];
        array[j] = array[i];
        array[i] = t;
    }
}

// bound integer value between min - max-1;
void sanitizeValue(int* value, int min, int max)
{
    if(*value < min)
    {
        *value = max - 1;
    }
    else if(*value >= max)
    {
        *value = min;
    }
}

int minmumValue(int min, int value)
{
    if(value < min)
    {
        value = min;
    }
    return value;
}

void convertNumberToDigits(int number)
{
    if(number == 0)
    {
        hunds = 0;
        tens = 0;
        ones = 0;
        return;
    }

    hunds = (number / 100);
    tens = ((number - ((number/100) * 100))/10);
    ones = (number % 10); // modulus
}

void convertSecondsToTime(int totalSeconds)
{
    if(totalSeconds == 0)
    {
        g_Timer.min_ones = 0;
        g_Timer.sec_tens = 0;
        g_Timer.sec_ones = 0;
        return;
    }
    
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;

    g_Timer.min_ones = minutes % 10;

    g_Timer.sec_tens = seconds / 10;
    g_Timer.sec_ones = seconds % 10;
}
