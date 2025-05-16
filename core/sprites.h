#pragma once

#include <jo/jo.h>
#include "main.h"
#include "math.h"

#define FRAMERATE1 4 // for animation speed (power of 2)
#define FRAMERATE2 6 // for animation speed
#define FRAMERATE3 8 // for animation speed (power of 2)
#define EASY_BALL_RADIUS toFIXED(56)
#define NORMAL_BALL_RADIUS toFIXED(36)

typedef struct {
    FIXED x, y, z;
    FIXED r;
} Position;

typedef struct {
    FIXED x, y;
} Scale;

typedef struct {
    int x, y, z;
} Rotation;

typedef struct {
    FIXED x, y;
} Vector;

typedef struct {
    FIXED x, y;
    int z;
} Velocity;

typedef struct _BoundingBox
{
    FIXED min_x, min_y;
    FIXED max_x, max_y;
    FIXED width, height;
} BoundingBox;

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
    BoundingBox bbox;
    FIXED mass;
    bool isColliding;
    Uint8    spr_id;
    bool   visible;
    int    pal_id;
    int    flip;
    int    mesh;
    int    zmode;
    Animation anim1;
    Animation anim2;
} Sprite;

extern Sprite font; // VDP1 font
extern Sprite ppplogo;
extern Sprite pppshadow;

extern Sprite logo1;
extern Sprite logo2;
extern Sprite cursor;
extern Sprite menu_text;
extern Sprite menu_choices;
extern Sprite menu_bg1;
extern Sprite menu_bg2;
extern Sprite character_portrait;
extern Sprite player_bg;
extern Sprite player_cursor;

// game UI
extern Sprite timer_num1;
extern Sprite timer_num10;
extern Sprite timer_num100;
extern Sprite meter;
extern Sprite heart;
extern Sprite star;
extern Sprite goal[MAX_PLAYERS];

extern Sprite shield[MAX_PLAYERS];

// characters
extern Sprite pixel_poppy;
extern Sprite pixel_poppy_shadow;
extern Sprite macchi;
extern Sprite jelly;
extern Sprite penny;
extern Sprite potter;
extern Sprite sparta;
extern Sprite poppy;
extern Sprite tj;
extern Sprite george;
extern Sprite wuppy;
extern Sprite stadler;
extern Sprite garfield;
extern Sprite paw_blank;

// items
extern Sprite bomb_item;
extern Sprite fishtank_item;
extern Sprite shroom_item;
extern Sprite craig_item;
extern Sprite garfield_item;

static inline void set_item_position(Sprite *sprite) {
    sprite->pos.x = toFIXED(my_random_range(-240, 240));
    sprite->pos.y = toFIXED(my_random_range(-160, 160));
    if (JO_MOD_POW2(jo_random(9999), 2)) { // sprite flip
        sprite->flip = sprHflip;
    }
    else {
        sprite->flip = sprNoflip;
    }
}

static inline void set_spr_position(Sprite *sprite, int px, int py, int pz) {
    sprite->pos.x = toFIXED(px);
    sprite->pos.y = toFIXED(py);
    sprite->pos.z = toFIXED(pz);
}

static inline void set_shield_position(Sprite *_player, Sprite *_shield, FIXED shield_pos) {
    _shield->pos.x = _player->pos.x + shield_pos;
    _shield->pos.y = _player->pos.y;
    _shield->pos.z = _player->pos.z;
}

static __jo_force_inline void set_spr_scale(Sprite *sprite, float sx, float sy) {
    sprite->scl.x = toFIXED(sx);
    sprite->scl.y = toFIXED(sy);
}


// states

// left side (flipped controls)
// sprNoflip : Normal display.
// sprVflip : Display upside down.
// right side (flipped controls)
// sprHflip : Horizontally flipped display.
// sprHVflip : Flip the display vertically and horizontally.

static __jo_force_inline void	my_sprite_draw(Sprite *sprite) { // note: switched to using 256bnk mode, so all palette indexes are now 0
	FIXED pos[XYZSS] = { sprite->pos.x, sprite->pos.y, sprite->pos.z, sprite->scl.x, sprite->scl.y };
	SPR_ATTR attr = SPR_ATTRIBUTE( sprite->spr_id, 0, No_Gouraud, sprite->mesh | ECdis | CL256Bnk, sprite->flip | sprite->zmode );
	slDispSpriteHV(pos, &attr, DEGtoANG(sprite->rot.z));
}

// increments 1 frame
static __jo_force_inline bool static_animation(Sprite *sprite) {
    if (JO_MOD_POW2(g_Game.frame, FRAMERATE1) == 0) { // modulus
        sprite->anim1.frame++;
        sprite->spr_id = sprite->anim1.asset[sprite->anim1.frame];
    }
    if (sprite->anim1.frame > sprite->anim1.max) {
        sprite->anim1.frame = 0;
        sprite->spr_id = sprite->anim1.asset[sprite->anim1.frame];
        return false;
    }
    return true;
}

// loops through all frames based on powers of 2
static __jo_force_inline void looped_animation_pow(Sprite *sprite, unsigned int framerate) {
        // move to an animation module
        if (JO_MOD_POW2(g_Game.frame, framerate) == 0) { // modulus
            sprite->anim1.frame++;
            if (sprite->anim1.frame > sprite->anim1.max) {
                sprite->anim1.frame = 0;
            }
            sprite->spr_id = sprite->anim1.asset[sprite->anim1.frame];
        }
}

// loops through second animation based on modulus
static __jo_force_inline void looped_animation_mod(Sprite *sprite, unsigned int framerate) {
        // move to an animation module
        if (g_Game.frame % framerate == 0) { // modulus
            sprite->anim2.frame++;
            if (sprite->anim2.frame > sprite->anim2.max) {
                sprite->anim2.frame = 0;
            }
            sprite->spr_id = sprite->anim2.asset[sprite->anim2.frame];
        }
}

static __jo_force_inline bool explode_animation(Sprite *sprite) {
    if (g_Game.frame % 6 == 0) { // modulus
        sprite->anim2.frame++;
    }
    if (sprite->anim2.frame > sprite->anim2.max) {
        sprite->spr_id = sprite->anim2.asset[sprite->anim2.max];
        return false;
    }
    else {
        sprite->spr_id = sprite->anim2.asset[sprite->anim2.frame];
    }
    return true;
}

static __jo_force_inline void sprite_frame_reset(Sprite *sprite) {
    sprite->anim1.frame = 0;
    sprite->spr_id = sprite->anim1.asset[sprite->anim1.frame];
    sprite->anim2.frame = 0;
}

void ball_animation_reset(Sprite *ball);

void drawGoalSprites(Sprite *sprite, int sprite_id, int shadow_id, int zmode, int flip, int x, int y, int scale_y);

void resetSpriteColors(void);

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