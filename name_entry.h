#pragma once

#include "assets.h"
// #include "character_select.h"
#include "BG_DEF/sprite_colors.h"

#define NAME_ENTRY_TIMER 99

extern unsigned int g_NameEntryTimer;

extern bool rotateLeft;
extern bool rotateRight;

typedef enum _INITIALS
{
    FIRST_INITIAL = 0,
    SECOND_INITIAL,
    THIRD_INITIAL,
    MAX_INITIAL,
} INITIALS;

void nameEntryInit(void);
void nameEntryInput(void);
void nameEntryUpdate(void);
void nameEntryDraw(void);
void nameEntryPositionUpdate(int angle, int sprAngle);

int snap_to_nearest_12(int angle);

int snap_to_end(int angle);
