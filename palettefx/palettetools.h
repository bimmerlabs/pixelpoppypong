#ifndef PALETTETOOLS_H
#define PALETTETOOLS_H
#include <jo/jo.h>

#define MAX_PALETTE_ENTRIES 256

// Struct definitions
typedef struct {
    Uint8 r;
    Uint8 g;
    Uint8 b;
} ObjectColor;

typedef struct {
    Sint16 h;
    Sint16 s;
    Sint16 l;
} ObjectHSL;

typedef struct {
    Sint16 h;
    Sint16 s;
    Sint16 l;
} GlobalHSL;

typedef struct {
    Uint8 lower;
    Uint8 upper;
} PaletteRange;

typedef struct
{
    ObjectColor color[MAX_PALETTE_ENTRIES];
} RgbPalette;

typedef struct
{
    FIXED color[MAX_PALETTE_ENTRIES];
} RgbBuff;

typedef struct
{
    ObjectHSL color[MAX_PALETTE_ENTRIES];
} HslPalette;

typedef struct {
    Uint16 hue;
    Uint8 sat;
    Uint8 lum;
    FIXED x_pos;
    FIXED y_pos;
    FIXED x_scroll;
    FIXED y_scroll;
    Uint8 min_sat_id;
    Uint8 max_sat_id;
    Uint8 min_lum_id;
    Uint8 max_lum_id;
} ImageAttr;

#define SET_BG_POSITION(pos, x, y) \
    do {                           \
        (pos).x_pos = (toFIXED(x));             \
        (pos).y_pos = (toFIXED(y));             \
    } while (0)

// Function prototypes
void clamp_hue(ObjectHSL *hsl);
void update_colors(ObjectHSL *hsl, GlobalHSL *hsl_increment, ObjectColor *color);
void reset_palette(RgbPalette *dest, const RgbPalette *src, PaletteRange *range);  // might not be needed
void SinglePaletteUpdate(int index, ObjectColor color, jo_palette palette);

void MultiPaletteUpdate(jo_palette *palette, HslPalette *hslPal, GlobalHSL *hsl_increment, PaletteRange *range);
void MultiPalette2Buffer(RgbBuff *bufferPal, HslPalette *hslPal, GlobalHSL *hsl_increment, PaletteRange *range);
void UpdatePaletteFromBuffer(RgbBuff *bufferPal, jo_palette *palette, PaletteRange *range);

void MultiRgbToHsl(HslPalette *hslPal, RgbPalette *rgbPal, PaletteRange *range);
void MultiHslTorRgb(HslPalette *hslPal, RgbPalette *rgbPal, PaletteRange *range);

void MultiHslTorRgb2Buffer(HslPalette *hslPal, RgbBuff *bufferPal, PaletteRange *range);

void min_max_sl_id(HslPalette *hslPal, PaletteRange *range, ImageAttr *attr);

#endif // PALETTETOOLS_H
