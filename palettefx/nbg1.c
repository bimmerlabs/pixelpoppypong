#include "nbg1.h"
#include "../main.h"
#include "../core/math.h"

// bool g_AnimateStars = false;

static unsigned char current_background = BG_NIGHT;
// static char star1_luminance = 0;
// static char star2_luminance = 0;
// static bool star1_increasing = false;
// static bool star2_increasing = false;
// static int star1_cycles = 0;
// static int star2_cycles = 0;
// static int star1_cycle_threshold = 0;
// static int star2_cycle_threshold = 0;
// static char cycle_max = 0;
// static char luminance_max = 0;
static PaletteRange p_rangeNbg1[NBG1_MAX];

bool do_update_nbg1_all = false;
bool update_nbg1_palette_all = false;
// bool do_update_nbg1_stars = false;
// bool update_nbg1_palette_stars = false;

// palette for normal map calculations
RgbPalette rgbNbg1 = {
    { {255, 203, 87}, {255, 113, 81}, {255, 165, 115}, {251, 31, 71}, {163, 191, 239}, {211, 217, 219}, {255, 237, 204}, {255, 255, 241}, 
      {33, 77, 65}, {61, 95, 71}, {237, 205, 159}, {199, 177, 115}, {135, 137, 73}, {91, 121, 65}, {153, 151, 99}, {240, 254, 255}, 
      // {240, 241, 255}, {79, 153, 185}, {79, 153, 185}, {79, 153, 185}, {79, 153, 185}, {79, 153, 185}, {79, 153, 185}, {79, 153, 185} }
      {240, 241, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255} }
};

// buffer palette for writing to CRAM (populated at runtime)
RgbBuff bufferNbg1 = {
    { 0, 0, 0, 0, 0, 0, 0, 0, 
      0, 0, 0, 0, 0, 0, 0, 0, 
      0, 0, 0, 0, 0, 0, 0, 0 }
};

// palette for calculating color and lighting - hue, saturation, luminance (populated at runtime)
HslPalette hslNbg1 = {
    { {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 
      {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 
      {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0} }
};

// tracks the accumulation of changes for the HSL color model
GlobalHSL hsl_incNbg1[HSL_INC_NBG1_MAX] = {0};

// 15 bit palettes (FIXED)
// dawn
static RgbBuff rgbNbg11 = {
    { 	43839,43487,47775,41087,63220,61306,59327,64511,
	41252,41319,53053,47832,42544,41451,45651,64511,
	64511,65535,65535,65535,65535,65535,65535,65535 }
};
// day
static RgbBuff rgbNbg12 = {
    { 	42431,38107,41311,35956,65265,65501,65432,65296,
	32930,35075,44659,33359,33225,35174,33289,65265,
	65265,57343,57343,57343,57343,57343,57343,57343 }
};
// dusk
static RgbBuff rgbNbg13 = {
    { 	32863,39026,40250,37901,44271,43351,43519,49983,
	35940,39078,43579,40374,39178,38119,39279,58303,
	58303,57343,57343,57343,57343,57343,57343,57343 }
};
// night
static RgbBuff rgbNbg14 = {
    { 	38935,38990,43248,39945,41984,45280,52677,56937,
	37888,39008,46496,42272,39136,37024,38144,65535,
	65535,64511,64511,64511,64511,64511,64511,64511 }
};

static PaletteRange p_rangeNbg1[NBG1_MAX] = {
	{ 17, 23 }, // SUN
	{ 17, 17 }, { 18, 18 }, { 19, 19 }, { 20, 20 }, { 21, 21 }, { 22, 22 }, { 23, 23 }, // MOON
	{ 15, 16 }, { 15, 15 }, { 16, 16 }, // STARS
	{ 0, 23 }, // ALL
};

// initial image setup: hue, saturation, luminance, x_pos, y_pos, x_scale, y_scale, x_scroll (rate), y_scroll (rate), min_sat_id, max_sat_id, min_lum_id, max_lum_id
ImageAttr attrNbg1 = { 0, 0, 0, toFIXED(0.0), toFIXED(0.0), toFIXED(0.25), toFIXED(0.0), 0, 0, 0, 0}; // default attributes (for single color image)

static jo_palette nbg1_palette;

void init_nbg1_img(void) {
    jo_img_8bits    img;
    jo_create_palette(&nbg1_palette);

    jo_set_tga_default_palette(&nbg1_palette);
    img.data = JO_NULL;
    jo_tga_8bits_loader(&img, JO_ROOT_DIR, "BG0.TGA", 0);
    jo_vdp2_set_nbg1_8bits_image(&img, nbg1_palette.id, false);
    jo_free_img(&img);
    jo_set_tga_default_palette(JO_NULL);
    
    slZoomNbg1(toFIXED(0.50), toFIXED(1.00));
    
    MultiRgbToHsl(&hslNbg1, &rgbNbg1, &p_rangeNbg1[NBG1_ALL]);
    min_max_sl_id(&hslNbg1, &p_rangeNbg1[NBG1_ALL], &attrNbg1);

    // cycle_max = 4;
    // luminance_max = 9;
    // star1_luminance = my_random_range(0, cycle_max);
    // star2_luminance = my_random_range(0, cycle_max);
    // star1_cycle_threshold = my_random_range(1, luminance_max);
    // star2_cycle_threshold = my_random_range(1, luminance_max);
}

// directly set color based on time slot
void update_nbg1_time_slot(void) {
    // initStarAnimation();
    // this way only until I implement palette adjustments
    if (!g_GameOptions.use_rtc) {
        current_background++;
        if (current_background > 4) {
            current_background = 1;
        }
    }
    else {
        // update based on time of day
        jo_getdate(&g_Game.now);
        // dawn
        if (g_Game.now.hour >= T_DAWN && g_Game.now.hour < T_DAY) {
            current_background = BG_DAWN;
        }
        // day
        if (g_Game.now.hour >= T_DAY && g_Game.now.hour < T_DUSK) {
            current_background = BG_DAY;
        }
        // dusk
        if (g_Game.now.hour >= T_DUSK && g_Game.now.hour < T_NIGHT) {
            current_background = BG_DUSK;
        }
        // night
        if (g_Game.now.hour >= T_NIGHT && g_Game.now.hour < T_DAWN) {
            current_background = BG_NIGHT;
        }
    }
    
    switch (current_background) {
    	case 1: {
		// dawn
		UpdatePaletteFromBuffer(&rgbNbg11, &nbg1_palette, &p_rangeNbg1[NBG1_ALL]);
		// if (g_GameOptions.use_rtc) {
		    // cycle_max = 5;
		    // luminance_max = 7;
		    // g_AnimateStars = true;
		// }
		break;
	}
    	case 2: {
		// day
                // reset_nbg1(HSL_INC_NBG1_ALL, NBG1_ALL);
		UpdatePaletteFromBuffer(&rgbNbg12, &nbg1_palette, &p_rangeNbg1[NBG1_ALL]);
		// g_AnimateStars = false;
		break;
	}
    	case 3: {
		// dusk
		UpdatePaletteFromBuffer(&rgbNbg13, &nbg1_palette, &p_rangeNbg1[NBG1_ALL]);
		// if (g_GameOptions.use_rtc) {
		    // cycle_max = 6;
		    // luminance_max = 7;
		    // g_AnimateStars = true;
		// }
		break;
	}
    	case 4: {
		// night
		UpdatePaletteFromBuffer(&rgbNbg14, &nbg1_palette, &p_rangeNbg1[NBG1_ALL]);
		// if (g_GameOptions.use_rtc) {
		    // cycle_max = 6;
		    // luminance_max = 7;
		    // g_AnimateStars = true;
		// }
		break;
	}
    	default: {
		break;
	}
    }
}

// set the HSL color by hue, saturation, and luminance increments
bool update_nbg1_color(unsigned char hsl_inc, unsigned char range) {
    MultiPalette2Buffer(&bufferNbg1, &hslNbg1, &hsl_incNbg1[hsl_inc], &p_rangeNbg1[range]);
    return true;
}

// update CRAM from buffer palette
bool update_nbg1_palette(unsigned char range) {
    UpdatePaletteFromBuffer(&bufferNbg1, &nbg1_palette, &p_rangeNbg1[range]);
    return false;
}

void reset_nbg1(unsigned char hsl_inc, unsigned char range) {
    MultiRgbToHsl(&hslNbg1, &rgbNbg1, &p_rangeNbg1[range]);
    min_max_sl_id(&hslNbg1, &p_rangeNbg1[range], &attrNbg1);
    MultiPalette2Buffer(&bufferNbg1, &hslNbg1, &hsl_incNbg1[hsl_inc], &p_rangeNbg1[range]);
    update_nbg1_palette_all = true;
}

// directly set the HSL color
bool calculate_nbg1_color(PaletteRange *range) {
    MultiHslTorRgb2Buffer(&hslNbg1, &bufferNbg1, range);
    return true;
}

// void initStarAnimation(void) {
    // reset_nbg1(HSL_INC_NBG1_ALL, NBG1_ALL);
    // hsl_incNbg1[HSL_INC_STARS1].l = 0;
    // hsl_incNbg1[HSL_INC_STARS1].s = 0;
    // hsl_incNbg1[HSL_INC_STARS2].l = 0;
    // hsl_incNbg1[HSL_INC_STARS2].s = 0;
    // do_update_nbg1_stars = false;
    // update_nbg1_palette_stars = false;
    // star1_increasing = false;
    // star2_increasing = false;
    // star1_cycles = 0;
    // star2_cycles = 0;
    // g_AnimateStars = false;
// }

// void animateStars(void) {
    // if (!g_AnimateStars) {
        // return;
    // }
    // jo_nbg0_printf(2, 10, "STAR1 INC:%i", star1_increasing);
    // jo_nbg0_printf(2, 11, "STAR1 CYCLE:%i", star1_cycles);
    // jo_nbg0_printf(2, 12, "STAR1 CYCLE THRESH:%i", star1_cycle_threshold);
    // jo_nbg0_printf(2, 13, "STAR1 LUMINANCE:%i", star1_luminance);
    // jo_nbg0_printf(2, 14, "STAR1 L:%i", hsl_incNbg1[HSL_INC_STARS1].l);
    // jo_nbg0_printf(2, 15, "STAR1 S:%i", hsl_incNbg1[HSL_INC_STARS1].s);
        // if (star1_increasing) {
            // if (star1_cycles < star1_cycle_threshold) {
                // star1_cycles += 1;
                // hsl_incNbg1[HSL_INC_STARS1].l = star1_luminance;
                // hsl_incNbg1[HSL_INC_STARS1].s = -1;
            // } 
            // else {
                // star1_increasing = false;
                // star1_cycle_threshold = my_random_range(0, cycle_max);
            // }
        // } 
        // else {
            // if (star1_cycles > -star1_cycle_threshold) {
                // star1_cycles -= 1;
                // hsl_incNbg1[HSL_INC_STARS1].l = -star1_luminance;
                // hsl_incNbg1[HSL_INC_STARS1].s = 1;
            // } 
            // else {
                // star1_increasing = true;
                // star1_luminance = my_random_range(0, luminance_max);
            // }
        // }
    // jo_nbg0_printf(22, 10, "STAR2 INC:%i", star2_increasing);
    // jo_nbg0_printf(22, 11, "STAR2 CYCLE:%i", star2_cycles);
    // jo_nbg0_printf(22, 12, "STAR2 CYCLE THRESH:%i", star2_cycle_threshold);
    // jo_nbg0_printf(22, 13, "STAR2 LUMINANCE:%i", star2_luminance);
    // jo_nbg0_printf(22, 14, "STAR2 L:%i", hsl_incNbg1[HSL_INC_STARS2].l);
    // jo_nbg0_printf(22, 15, "STAR2 S:%i", hsl_incNbg1[HSL_INC_STARS2].s);
        // if (star2_increasing) {
            // if (star2_cycles < star2_cycle_threshold) {
                // star2_cycles += 1;
                // hsl_incNbg1[HSL_INC_STARS2].l = star2_luminance;
                // hsl_incNbg1[HSL_INC_STARS2].s = -1;
            // } 
            // else {
                // star2_increasing = false;
                // star2_cycle_threshold = my_random_range(0, cycle_max);
            // }
        // } 
        // else {
            // if (star2_cycles > -star2_cycle_threshold) {
                // star2_cycles -= 1;
                // hsl_incNbg1[HSL_INC_STARS2].l = -star2_luminance;
                // hsl_incNbg1[HSL_INC_STARS2].s = 1;
            // } 
            // else {
                // star2_increasing = true;
                // star2_luminance = my_random_range(0, luminance_max);
            // }
        // }
        // do_update_nbg1_stars = true;
// }

bool update_nbg1_range(PaletteRange *range) {
    UpdatePaletteFromBuffer(&bufferNbg1, &nbg1_palette, range);
    return false;
}