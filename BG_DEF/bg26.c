#include "bg26.h"
#include "../main.h"

// palette for normal map calculations
RgbPalette rgbBg26_1 = {
    { {191, 3, 51}, {117, 23, 53}, {133, 57, 83}, {77, 3, 61}, {3, 7, 79}, {3, 57, 101}, {47, 115, 155}, {79, 153, 185}, 
      {3, 7, 41}, {3, 29, 49}, {3, 109, 111}, {3, 75, 73}, {3, 57, 49}, {3, 45, 37}, {5, 65, 45}, {255, 255, 255}, 
      {255, 255, 241} }
};

// palette for normal map calculations
RgbPalette rgbBg26_2 = {
    { {191, 3, 51}, {117, 23, 53}, {133, 57, 83}, {77, 3, 61}, {3, 7, 79}, {3, 57, 101}, {47, 115, 155}, {79, 153, 185}, 
      {3, 7, 41}, {3, 29, 49}, {3, 109, 111}, {3, 75, 73}, {3, 57, 49}, {3, 45, 37}, {5, 65, 45}, {255, 255, 255}, 
      {255, 255, 241} }
};

// palette for normal map calculations
RgbPalette rgbBg26_3 = {
    { {191, 3, 51}, {117, 23, 53}, {133, 57, 83}, {77, 3, 61}, {3, 7, 79}, {3, 57, 101}, {47, 115, 155}, {79, 153, 185}, 
      {3, 7, 41}, {3, 29, 49}, {3, 109, 111}, {3, 75, 73}, {3, 57, 49}, {3, 45, 37}, {5, 65, 45}, {255, 255, 255}, 
      {255, 255, 241} }
};

// palette for normal map calculations
RgbPalette rgbBg26_4 = {
    { {191, 3, 51}, {117, 23, 53}, {133, 57, 83}, {77, 3, 61}, {3, 7, 79}, {3, 57, 101}, {47, 115, 155}, {79, 153, 185}, 
      {3, 7, 41}, {3, 29, 49}, {3, 109, 111}, {3, 75, 73}, {3, 57, 49}, {3, 45, 37}, {5, 65, 45}, {255, 255, 255}, 
      {255, 255, 241} }
};

// buffer palette for writing to CRAM (populated at runtime)
RgbBuff bufferBg26 = {
    { 0, 0, 0, 0, 0, 0, 0, 0, 
      0, 0, 0, 0, 0, 0, 0, 0, 
      0 }
};

// palette for calculating color and lighting - hue, saturation, luminance (populated at runtime)
HslPalette hslBg26 = {
    { {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 
      {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 
      {0, 0, 0} }
};

// palette ranges - used with the palette group collection.
PaletteRange p_rangeBg26  = { 0, 15 }; // for running the demo
PaletteRange p_rangeBg260 = { 0, 15 }; // default range - includes all unique palette entries
PaletteRange p_rangeBg261 = { 0, 3 };
PaletteRange p_rangeBg262 = { 4, 7 };
PaletteRange p_rangeBg263 = { 8, 14 };
PaletteRange p_rangeBg264 = { 15, 15 };
PaletteRange p_rangeBg265 = { 16, 16 };

// tracks the accumulation of changes for the HSL color model
GlobalHSL hsl_incBg260 = {0, 0, 0};

// initial image setup: hue, saturation, luminance, x_pos, y_pos, x_scale, y_scale, x_scroll (rate), y_scroll (rate), min_sat_id, max_sat_id, min_lum_id, max_lum_id
ImageAttr attrBg260 = { 0, 0, 0, toFIXED(0.0), toFIXED(0.0), toFIXED(0.25), toFIXED(0.0), 0, 0, 0, 0}; // default attributes (for single color image)

PaletteRange *paletteRangeBg26[] = {
    &p_rangeBg260,  // Index 0
    &p_rangeBg261,  // Index 1
    &p_rangeBg262,  // Index 2
    &p_rangeBg263,  // Index 3
    &p_rangeBg264,  // Index 4
    &p_rangeBg265   // Index 5
};

Uint8 bg26_paletteGroups = 6;

Uint8 bg26_spr_id = 0;

static jo_palette bg26_palette;

// Helper Functions

// jo_palette	*my_bg26_palette_handling(void)
// {
    // jo_create_palette(&bg26_palette);
    // return (&bg26_palette);
// }

void init_bg26_img(void) {
    jo_img_8bits    img;
    // jo_set_tga_palette_handling(my_bg26_palette_handling);
    if (first_load) {
        jo_create_palette(&bg26_palette);
        first_load = false;
    }
    jo_set_tga_default_palette(&bg26_palette);
    img.data = JO_NULL;
    
    switch (current_background) {
    	case 1: {
		jo_tga_8bits_loader(&img, JO_ROOT_DIR, "BG01.TGA", 0);
		break;
	}
    	case 2: {
		jo_tga_8bits_loader(&img, JO_ROOT_DIR, "BG02.TGA", 0);
		break;
	}
    	case 3: {
		jo_tga_8bits_loader(&img, JO_ROOT_DIR, "BG03.TGA", 0);
		break;
	}
    	case 4: {
		jo_tga_8bits_loader(&img, JO_ROOT_DIR, "BG04.TGA", 0);
		break;
	}
    }
    // jo_tga_8bits_loader(&img, JO_ROOT_DIR, "BG04.TGA", 0);
    jo_vdp2_set_nbg1_8bits_image(&img, bg26_palette.id, false);
    jo_free_img(&img);
    // jo_set_tga_palette_handling(JO_NULL);
    jo_set_tga_default_palette(JO_NULL);

    // MultiRgbToHsl(&hslBg26, &rgbBg26, &p_rangeBg260);
    min_max_sl_id(&hslBg26, &p_rangeBg260, &attrBg260);
    
    slZoomNbg1(toFIXED(0.50), toFIXED(1.00));
}

void update_bg26_color(void) {
    MultiPalette2Buffer(&bufferBg26, &hslBg26, &hsl_incBg260, &p_rangeBg26);
}

void update_bg26_palette(void) {
    UpdatePaletteFromBuffer(&bufferBg26, &bg26_palette, &p_rangeBg26);
}

void reset_bg26 (void) {
    // MultiRgbToHsl(&hslBg26, &rgbBg26, &p_rangeBg260);
    min_max_sl_id(&hslBg26, &p_rangeBg260, &attrBg260);
    MultiPalette2Buffer(&bufferBg26, &hslBg26, &hsl_incBg260, &p_rangeBg260);
}

