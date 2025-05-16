// nbg1.h
#pragma once

#include "palettetools.h"

#define T_DAWN 5
#define T_DAY 11
#define T_DUSK 17
#define T_NIGHT 23

// extern bool g_AnimateStars;

// time of day
typedef enum _BG_TIME
{
    BG_DAWN  = 1,
    BG_DAY   = 2,
    BG_DUSK  = 3,
    BG_NIGHT = 4,
    BG_CNTRL = 5,
} BG_TIME;

typedef enum _HSL_INC_NBG1
{
    HSL_INC_STARS1 = 0,
    HSL_INC_STARS2,
    HSL_INC_NBG1_ALL,
    HSL_INC_NBG1_MAX,
} HSL_INC_NBG1;

extern GlobalHSL hsl_incNbg1[HSL_INC_NBG1_MAX];

typedef enum _NBG1_RANGE
{
    NBG1_SUN = 0,
    NBG1_MOON1,
    NBG1_MOON2,
    NBG1_MOON3,
    NBG1_MOON4,
    NBG1_MOON5,
    NBG1_MOON6,
    NBG1_MOON7,
    NBG1_STARS,
    NBG1_STARS1,
    NBG1_STARS2,
    NBG1_ALL,
    NBG1_MAX,
} NBG1_RANGE;

extern bool do_update_nbg1_all;
extern bool update_nbg1_palette_all;
// extern bool do_update_nbg1_stars;
// extern bool update_nbg1_palette_stars;

extern RgbPalette rgbBg0;
extern RgbBuff bufferBg0;
extern HslPalette hslBg0;

// extern Uint8 current_background;
extern ImageAttr attrNbg1;

void init_nbg1_img(void);
void update_nbg1_time_slot(void);

bool update_nbg1_color(unsigned char hsl_inc, unsigned char range);
bool update_nbg1_palette(unsigned char range); // TODO: rename
bool calculate_nbg1_color(PaletteRange *range);
void reset_nbg1(unsigned char hsl_inc, unsigned char range);

// void initStarAnimation(void);
// void animateStars(void);

// bool update_nbg1_range(PaletteRange *range);

// static inline void updateStarsPalette(void) {
    // if (update_Nbg1_palette[HSL_INC_NBG1_STARS]) {
	// update_Nbg1_palette[HSL_INC_NBG1_STARS] = update_nbg1_range(&p_rangeNbg1[NBG1_STARS1]);
    // }
// }


// all palettes
static inline void updateNgb1Colors(void) {
	if (!do_update_nbg1_all)
		return;
	update_nbg1_palette_all = update_nbg1_color(HSL_INC_NBG1_ALL, NBG1_ALL);
	do_update_nbg1_all = false;
}
static inline void updateNgb1Palette(void) {
	if (!update_nbg1_palette_all)
		return;
	update_nbg1_palette_all = update_nbg1_palette(NBG1_ALL);
}
// // STARS
// static inline void updateStarsColors(void) {
	// if (!do_update_nbg1_stars)
		// return;
	// update_nbg1_palette_stars = update_nbg1_color(HSL_INC_STARS1, NBG1_STARS1);
	// update_nbg1_palette_stars = update_nbg1_color(HSL_INC_STARS2, NBG1_STARS2);
	// do_update_nbg1_stars = false;
// }
// static inline void updateStarsPalette(void) {
	// if (!update_nbg1_palette_stars)
		// return;
	// update_nbg1_palette_stars = update_nbg1_palette(NBG1_STARS);
// }