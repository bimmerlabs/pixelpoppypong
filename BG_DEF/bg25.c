#include "bg25.h"

// palette for normal map calculations
RgbPalette rgbBg25 = {
    { {191, 3, 51}, {117, 23, 53}, {133, 57, 83}, {77, 3, 61}, {3, 7, 79}, {3, 57, 101}, {47, 115, 155}, {79, 153, 185}, 
      {3, 7, 41}, {3, 29, 49}, {3, 109, 111}, {3, 75, 73}, {3, 57, 49}, {3, 45, 37}, {5, 65, 45}, {255, 255, 255} }
};

// buffer palette for writing to CRAM (populated at runtime)
RgbBuff bufferBg25 = {
    { 0, 0, 0, 0, 0, 0, 0, 0, 
      0, 0, 0, 0, 0, 0, 0, 0 }
};

// palette for calculating color and lighting - hue, saturation, luminance (populated at runtime)
HslPalette hslBg25 = {
    { {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 
      {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0} }
};

// palette ranges - used with the palette group collection.
PaletteRange p_rangeBg25  = { 0, 15 }; // for running the demo
PaletteRange p_rangeBg250 = { 0, 15 }; // default range - includes all unique palette entries
PaletteRange p_rangeBg251 = { 0, 3 };
PaletteRange p_rangeBg252 = { 4, 7 };
PaletteRange p_rangeBg253 = { 8, 14 };
PaletteRange p_rangeBg254 = { 15, 15 };

// tracks the accumulation of changes for the HSL color model
GlobalHSL hsl_incBg250 = {0, 0, 0};

// initial image setup: hue, saturation, luminance, x_pos, y_pos, x_scale, y_scale, x_scroll (rate), y_scroll (rate), min_sat_id, max_sat_id, min_lum_id, max_lum_id
ImageAttr attrBg250 = { 0, 0, 0, toFIXED(0.0), toFIXED(0.0), toFIXED(0.25), toFIXED(0.0), 0, 0, 0, 0}; // default attributes (for single color image)

PaletteRange *paletteRangeBg25[] = {
    &p_rangeBg250,  // Index 0
    &p_rangeBg251,  // Index 1
    &p_rangeBg252,  // Index 2
    &p_rangeBg253,  // Index 3
    &p_rangeBg254   // Index 4
};

Uint8 bg25_paletteGroups = 5;

Uint8 bg25_spr_id = 0;

static jo_palette bg25_palette;

// Helper Functions

jo_palette	*my_bg25_palette_handling(void)
{
    jo_create_palette(&bg25_palette);
    return (&bg25_palette);
}

void init_bg25_img(void) {
    jo_img_8bits    img;
    jo_set_tga_palette_handling(my_bg25_palette_handling);
    img.data = JO_NULL;
    jo_tga_8bits_loader(&img, JO_ROOT_DIR, "BG25.TGA", 0);
    jo_vdp2_set_nbg1_8bits_image(&img, bg25_palette.id, false);
    jo_free_img(&img);
    jo_set_tga_palette_handling(JO_NULL);

    MultiRgbToHsl(&hslBg25, &rgbBg25, &p_rangeBg250);
    min_max_sl_id(&hslBg25, &p_rangeBg250, &attrBg250);
    
    slZoomNbg1(toFIXED(0.50), toFIXED(1.00));
}

void update_bg25_color(void) {
    MultiPalette2Buffer(&bufferBg25, &hslBg25, &hsl_incBg250, &p_rangeBg25);
}

void update_bg25_palette(void) {
    UpdatePaletteFromBuffer(&bufferBg25, &bg25_palette, &p_rangeBg25);
}

void reset_bg25 (void) {
    MultiRgbToHsl(&hslBg25, &rgbBg25, &p_rangeBg250);
    min_max_sl_id(&hslBg25, &p_rangeBg250, &attrBg250);
    MultiPalette2Buffer(&bufferBg25, &hslBg25, &hsl_incBg250, &p_rangeBg250);
}

