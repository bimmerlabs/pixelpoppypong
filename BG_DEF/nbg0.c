#include "nbg0.h"
#include "../main.h"

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

static PaletteRange p_rangeNbg0_1  = { 0, 16 }; // indexes of palette entries to update

// initial image setup: hue, saturation, luminance, x_pos, y_pos, x_scale, y_scale, x_scroll (rate), y_scroll (rate), min_sat_id, max_sat_id, min_lum_id, max_lum_id
ImageAttr attrNbg0 = { 0, 0, 0, toFIXED(0.0), toFIXED(0.0), toFIXED(0.25), toFIXED(0.0), 0, 0, 0, 0}; // default attributes (for single color image)

static jo_palette nbg0_palette;

void init_nbg0_img(void) {
    jo_img_8bits    img;
    jo_create_palette(&nbg0_palette);

    jo_set_tga_default_palette(&nbg0_palette);
    img.data = JO_NULL;
    jo_tga_8bits_loader(&img, JO_ROOT_DIR, "BG01.TGA", 0);
    jo_vdp2_set_nbg1_8bits_image(&img, nbg0_palette.id, false);
    jo_free_img(&img);
    jo_set_tga_default_palette(JO_NULL);
    
    slZoomNbg1(toFIXED(0.50), toFIXED(1.00));
}

void update_nbg0_palette(void) {
    switch (current_background) {
    	case 1: {
		UpdatePaletteFromBuffer(&rgbBg01, &nbg0_palette, &p_rangeNbg0_1);
		break;
	}
    	case 2: {
		UpdatePaletteFromBuffer(&rgbBg02, &nbg0_palette, &p_rangeNbg0_1);
		break;
	}
    	case 3: {
		UpdatePaletteFromBuffer(&rgbBg03, &nbg0_palette, &p_rangeNbg0_1);
		break;
	}
    	case 4: {
		UpdatePaletteFromBuffer(&rgbBg04, &nbg0_palette, &p_rangeNbg0_1);
		break;
	}
    	default: {
		break;
	}
    }
}


