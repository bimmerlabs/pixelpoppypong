#pragma once

#include "assets.h"
#include "BG_DEF/sprite_colors.h"

#define CHARACTER_ENTRY_TIMER 99

extern unsigned int g_CharacterEntryTimer;

// typedef enum _INITIALS
// {
    // FIRST_INITIAL = 0,
    // SECOND_INITIAL,
    // THIRD_INITIAL,
    // MAX_INITIAL,
// } INITIALS;

void characterSelectInit(void);
void characterEntryInput(void);
void characterEntryUpdate(void);
void characterEntryDraw(void);
void characterEntryPositionUpdate(int angle, int sprAngle);

static inline int snap_to_nearest_angle(int angle, bool pressedLeft, bool pressedRight, bool *isAngleSnapped) {
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

// static inline void reset_character_entry_settings(void) {
    // set_spr_scale(&pixel_poppy, 6, 6);
    // pixel_poppy.rot.z = 0;
    // set_spr_position(&pixel_poppy, 0, 0, 100);
    // sprite_frame_reset(&pixel_poppy);
// }