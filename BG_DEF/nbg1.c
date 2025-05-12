#include "nbg1.h"
#include "../main.h"

// palette for normal map calculations
RgbPalette rgbBg0 = {
    { {255, 203, 87}, {255, 113, 81}, {255, 165, 115}, {251, 31, 71}, {163, 191, 239}, {211, 217, 219}, {255, 237, 204}, {255, 255, 241}, 
      {33, 77, 65}, {61, 95, 71}, {237, 205, 159}, {199, 177, 115}, {135, 137, 73}, {91, 121, 65}, {153, 151, 99}, {255, 255, 241} }
};

// buffer palette for writing to CRAM (populated at runtime)
RgbBuff bufferBg0 = {
    { 0, 0, 0, 0, 0, 0, 0, 0, 
      0, 0, 0, 0, 0, 0, 0, 0 }
};

// palette for calculating color and lighting - hue, saturation, luminance (populated at runtime)
HslPalette hslBg0 = {
    { {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 
      {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0} }
};

// palette ranges - used with the palette group collection.
PaletteRange p_rangeBg0  = { 0, 15 }; // for running the demo

// tracks the accumulation of changes for the HSL color model
GlobalHSL hsl_incBg0 = {0, 0, 0};

// 15 bit palettes (FIXED)
// dawn
static RgbBuff rgbBg01 = {
    { 	43839,43487,47775,41087,63220,61306,59327,64511,
	41252,41319,53053,47832,42544,41451,45651,64511,
	65535 }
};
// day
static RgbBuff rgbBg02 = {
    { 	42431,38107,41311,35956,65265,65501,65432,65296,
	32930,35075,44659,33359,33225,35174,33289,65432,
	65296 }
};
// dusk
static RgbBuff rgbBg03 = {
    { 	32863,39026,40250,37901,44271,43351,43519,49983,
	35940,39078,43579,40374,39178,38119,39279,58303,
	57343 }
};
// night
static RgbBuff rgbBg04 = {
    { 	38935,38990,43248,39945,41984,45280,52677,56937,
	37888,39008,46496,42272,39136,37024,38144,65535,
	64511 }
};

static PaletteRange p_rangeNbg1_1  = { 0, 16 }; // indexes of palette entries to update

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
}

// TODO: need hsl inits and color functions from bg0.h

void update_nbg1_palette(void) {
    switch (current_background) {
    	case 1: {
		UpdatePaletteFromBuffer(&rgbBg01, &nbg1_palette, &p_rangeNbg1_1);
		break;
	}
    	case 2: {
		UpdatePaletteFromBuffer(&rgbBg02, &nbg1_palette, &p_rangeNbg1_1);
		break;
	}
    	case 3: {
		UpdatePaletteFromBuffer(&rgbBg03, &nbg1_palette, &p_rangeNbg1_1);
		break;
	}
    	case 4: {
		UpdatePaletteFromBuffer(&rgbBg04, &nbg1_palette, &p_rangeNbg1_1);
		break;
	}
    	default: {
		break;
	}
    }
}


