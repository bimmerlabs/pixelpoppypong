#pragma once
// #ifndef SPRITES_H
// #define SPRITES_H

#include <jo/jo.h>

typedef struct {
    FIXED x, y, z;
} Position;

typedef struct {
    FIXED x, y;
} Scale;

typedef struct {
    Uint16 x, y, z;
} Rotation;

typedef struct {
    FIXED x, y;
} Vector;

typedef struct {
    FIXED x, y;
} Velocity;

typedef struct {
    int*  asset;
    Uint8 frame;
    Uint8 max;
} Animation;

typedef struct {
    Position pos;
    Scale    scl;
    Rotation rot;
    Vector   vec2;
    Velocity vel;
    Uint8    spr_id;
    int    pal_id;
    int    flip;
    int    mesh;
    int    zmode;
    Animation anim1;
    Animation anim2;
    Animation anim3;
} Sprite;

extern Sprite cursor;
extern Sprite menu_text;
extern Sprite menu_bg;
extern Sprite timer_num1;
extern Sprite timer_num10;
extern Sprite poppy;
extern Sprite macchi;
extern Sprite jelly;
extern Sprite penny;
extern Sprite potter;
extern Sprite sparta;
extern Sprite player1; // player can make their own pet
extern Sprite boss1;
extern Sprite boss2;

#define SET_SPR_POSITION(pos, px, py, pz) \
    do {                           \
        (pos).x = (toFIXED(px));             \
        (pos).y = (toFIXED(py));             \
        (pos).z = (toFIXED(pz));             \
    } while (0)

#define SET_SPR_SCALE(scl, sx, sy) \
    do {                     \
        (scl).x = (toFIXED(sx));       \
        (scl).y = (toFIXED(sy));       \
    } while (0)

// states

// left side (flipped controls)
// sprNoflip : Normal display.
// sprVflip : Display upside down.
// right side (flipped controls)
// sprHflip : Horizontally flipped display.
// sprHVflip : Flip the display vertically and horizontally.

void	my_sprite_draw(Sprite *sprite);

// #endif // SPRITES_H


// SPR_ATTRIBUTE
// Format
    // #include "sl_def.h"

    // SPR_ATTRIBUTE( tex, col, gouraud, mode, dir );
// argument
    // tex - pattern number (described later)
    // col - Color palette number (described later)
    // gouraud - Gouraud shading table for user Gouraud (described later)
    // mode - sprite attribute (described later)
    // dir - How to draw sprites (described later)
// function
    // #define SPR_ATTRIBUTE(t,c,g,a,d) {t,(a)|(((d)>>24)&0xc0),c,g,(d)&0x0f3f}

    // The argument value list is as follows:

    // tex
      // No_Texture : Single color sprite.

      // Uint16 value: pattern number.

    // col
      // No_Palet : Do not use a palette. (For RGB mode.)
      // Uint16 value: Color code when tex is No_Texture .

                   // Otherwise, the pallet number.

    // gouraud
      // No_Gouraud : Do not use user Gouraud shading.

      // Uint16 value: Gouraud shading table address Expressed as an 8-byte boundary from the beginning of VDP1's VRAM.

    // mode
      // Specify 0 or 1 entry from each group below using "|".

      // group 1
        // MSBoff : Do not write MSB when writing to frame buffer (default).

        // MSBon : Set MSB when writing to frame buffer.

      // group 2
        // No_Window : Not subject to window constraints. (default)
        // Window_In : Display inside the window.

        // Window_Out : Display outside the window.

      // group 3
        // HSSoff : Does not apply high speed shrink. (default)
        // HSson : Apply high speed shrink.

      // group 4
        // MESHoff : Does not apply mesh. (default)
        // MESHon : Apply mesh.

      // group 5
        // ECdis : Disable end code. (default)
        // ECenb : Enable end code.

      // group 6
        // SPdis : Transparent pixel display. (default)
        // SPenb : Transparent pixels disabled.

      // group 7
        // CL16Bnk : 16-color color bank mode. (default)
        // CL16Look : Color lookup mode.

        // CL64Bnk : 64 color color bank mode.

        // CL128Bnk : 128 color color bank mode.

        // CL256Bnk : 256 color color bank mode.

        // CL32KRGB : RGB mode.

      // group 8
        // CL_Replace : Normal drawing mode. (default)
        // CL_Shadow : Shadow mode.

        // CL_Half : Half brightness mode.

        // CL_Trans : Translucent mode.

        // CL_Gouraud : User Gouraud shading mode.

    // dir
      // Select one from each of the following two groups and specify it using "|".

      // group 1
        // sprNoflip : Normal display.

        // sprHflip : Horizontally flipped display.

        // sprVflip : Display upside down.

        // sprHVflip : Flip the display vertically and horizontally.

        // sprPolygon : Single color sprite.

        // sprPolyLine : Polyline display.

      // group 2
        // _ZmLT : Set the center point at the top left when enlarging.

        // _ZmLC : Set the center point when enlarging to the center left.

        // _ZmLB : Set the center point at the bottom left when enlarging.

        // _ZmCT : Set the center point at the top of the center when enlarging.

        // _ZmCC : Center the center point when enlarging.

        // _ZmCB : Set the center point at the bottom of the center when enlarging.

        // _ZmRT : Set the center point at the top right when enlarging.

        // _ZmRC : Set the center point when enlarging to the center right.

        // _ZmRB : Set the center point at the bottom right when enlarging.