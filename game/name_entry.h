#pragma once

#include "../core/assets.h"
#include "../palettefx/sprite_colors.h"

#define NAME_ENTRY_TIMER 99
#define NAME_X_RADIUS toFIXED(140)
#define NAME_Y_RADIUS toFIXED(50)
#define NAME_Y_POS 170
#define END_RADIUS toFIXED(-1400)
#define GAP_ANGLE 12

typedef struct _NAME_ENTRY
{
    unsigned int timer;
    unsigned int selection;
    bool rotateLeft;
    bool rotateRight;
    char initials[4];  // 3 letters + null terminator
    int char_id[3];
    
    FIXED xRadius;
    FIXED yRadius;
    FIXED yPos;
    int angle;
    int gapAngle;
    bool pressedLeft;
    bool pressedRight;
    bool isAngleSnapped;
    bool finished;
    bool end;
} NAME_ENTRY;

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
