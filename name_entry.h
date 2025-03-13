#pragma once

#include "assets.h"

#define NAME_ENTRY_TIMER 99

extern unsigned int g_NameEntryTimer;

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

static inline int snap_to_nearest_12(int angle, bool pressedLeft, bool pressedRight, bool *isAngleSnapped) {
    // If already snapped, do nothing
    if (*isAngleSnapped) {
        return angle;
    }

    // Adjust angle only when a button is pressed
    if (pressedRight) {
        angle--;  // Move counterclockwise
        if (angle < 0)
            angle += 360;
    } else if (pressedLeft) {
        angle++;  // Move clockwise
        if (angle > 360)
            angle -= 360;
    }

    // If angle is now a multiple of 12, stop adjusting
    if (angle % 12 == 0) {
        *isAngleSnapped = true;  // Mark snapping complete
    }

    return angle;
}

static bool rotateLeft = false;
static bool rotateRight = false;
static inline int snap_to_end(int angle, bool *isAngleSnapped) {
    // If already snapped, do nothing
    if (*isAngleSnapped) {
        return angle;
    }
    if (angle == 0)
        angle = 360;
    if (!rotateLeft && !rotateRight) {
        if (angle - 180 < 0) {
            rotateRight = true;
        }
        else {
            rotateLeft = true;
        }
    }
    // Adjust angle only when a button is pressed
    if (rotateRight) { // need to compare to original angle value and set a boolean?
        angle -= 6;  // Move counterclockwise
        if (angle < 0)
            angle += 360;
    }
    if (rotateLeft) {
        angle += 6;  // Move clockwise
        if (angle > 360)
            angle -= 360;
    }

    // jo_nbg0_printf(20, 13, "ROTATERIGHT:%i", rotateRight);
    // jo_nbg0_printf(20, 14, "ROTATELEFT:%i", rotateLeft);
    // If angle is now a multiple of 12, stop adjusting
    if (angle == 12) {
        *isAngleSnapped = true;  // Mark snapping complete
    }

    return angle;
}