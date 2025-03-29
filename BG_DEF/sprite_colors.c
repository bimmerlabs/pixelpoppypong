#include "sprite_colors.h"
#include "../main.h"

#include "../palettetools.h"
#include "../lighting.h"
#include "../font.h"

bool do_update_All = false;
bool do_update_font2All = false;
bool do_update_ppplogo = false;
bool do_update_logo1 = false;
bool do_update_Goals[MAX_PLAYERS];
bool do_update_Pmenu[MAX_PLAYERS];
bool do_update_PmenuAll = false;
bool do_update_menu1 = false;
bool do_update_menu2 = false;
bool do_update_menu3 = false;
bool do_update_menu4 = false;
bool do_update_fish = false;
bool do_update_bomb = false;
bool do_update_shroom = false;

bool update_palette_All = false;
bool update_palette_font2All = false;
bool update_palette_ppplogo = false;
bool update_palette_logo1 = false;
bool update_palette_Goals[MAX_PLAYERS];
bool update_palette_Pmenu[MAX_PLAYERS];
bool update_palette_PmenuAll = false;
bool update_palette_menu1 = false;
bool update_palette_menu2 = false;
bool update_palette_menu3 = false;
bool update_palette_menu4 = false;
bool update_palette_fish = false;
bool update_palette_bomb = false;
bool update_palette_shroom = false;

LightSource light = {toFIXED(10), toFIXED(255), toFIXED(255), 0, 0, 0};  // scene light

// base palette for color map calculations
RgbPalette rgbSprites = {
    { {1, 1, 1}, {1, 255, 1}, {255, 255, 255}, {87, 87, 87}, {145, 145, 145}, {223, 125, 27}, {235, 189, 21}, {249, 163, 27}, 
      {255, 245, 141}, {249, 179, 183}, {33, 191, 255}, {33, 191, 255}, {33, 191, 255}, {33, 191, 255}, {237, 29, 37}, {241, 255, 1}, 
      {255, 10, 10}, {255, 194, 10}, {133, 255, 10}, {10, 255, 71}, {10, 255, 255}, {10, 71, 255}, {133, 10, 255}, {255, 10, 194}, 
      {255, 255, 255}, {1, 1, 1}, {27, 27, 27}, {27, 27, 27}, {247, 247, 1}, {255, 255, 255}, {63, 73, 205}, {247, 247, 1}, 
      {1, 1, 1}, {161, 25, 99}, {213, 77, 153}, {235, 121, 185}, {255, 175, 201}, {40, 130, 200}, {153, 217, 234}, {199, 235, 245}, 
      {205, 205, 205}, {215, 206, 203}, {231, 227, 223}, {255, 255, 255}, {143, 211, 251}, {47, 157, 223}, {39, 123, 173}, {255, 251, 177}, 
      {255, 243, 1}, {243, 231, 1}, {243, 119, 167}, {223, 47, 91}, {173, 39, 107}, {225, 163, 177}, {195, 75, 99}, {149, 51, 71}, 
      {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {85, 217, 255}, {85, 217, 255}, {85, 217, 255}, {85, 217, 255}, 
      {1, 1, 1}, {255, 125, 169}, {255, 175, 201}, {127, 127, 127}, {137, 125, 119}, {195, 195, 195}, {201, 197, 193}, {217, 215, 213}, 
      {255, 255, 255}, {0, 0, 0}, {1, 1, 1}, {255, 125, 169}, {255, 175, 201}, {127, 127, 127}, {195, 195, 195}, {255, 255, 255}, 
      {1, 1, 1}, {255, 125, 169}, {255, 175, 201}, {127, 127, 127}, {137, 125, 119}, {195, 195, 195}, {241, 217, 175}, {243, 209, 159}, 
      {255, 255, 255}, {1, 203, 255}, {63, 73, 205}, {153, 217, 235}, {253, 179, 1}, {255, 243, 1}, {237, 29, 37}, {255, 201, 15}, 
      {1, 1, 1}, {255, 125, 169}, {255, 175, 201}, {127, 127, 127}, {43, 35, 35}, {195, 195, 195}, {255, 175, 43}, {249, 163, 27}, 
      {255, 255, 255}, {41, 35, 30}, {253, 150, 57}, {118, 78, 50}, {144, 133, 132}, {72, 22, 79}, {252, 245, 163}, {211, 1, 1}, 
      {1, 1, 1}, {255, 125, 169}, {255, 175, 201}, {127, 127, 127}, {79, 55, 25}, {195, 195, 195}, {129, 89, 39}, {199, 145, 79}, 
      {255, 255, 255}, {244, 246, 59}, {240, 101, 35}, {169, 39, 100}, {252, 252, 229}, {245, 181, 150}, {249, 197, 59}, {196, 45, 52}, 
      {1, 1, 1}, {237, 137, 171}, {241, 189, 207}, {127, 127, 127}, {19, 19, 19}, {195, 195, 195}, {65, 65, 65}, {0, 0, 0}, 
      {71, 65, 63}, {1, 1, 1}, {255, 255, 255}, {63, 73, 205}, {1, 163, 233}, {5, 183, 255}, {1, 203, 255}, {153, 217, 235}, 
      {1, 1, 1}, {255, 125, 169}, {255, 175, 201}, {127, 127, 127}, {137, 125, 119}, {195, 195, 195}, {253, 245, 239}, {201, 197, 193}, 
      {255, 255, 255}, {255, 103, 53}, {253, 179, 1}, {255, 201, 15}, {255, 205, 1}, {255, 255, 255}, {255, 46, 40}, {205, 31, 27}, 
      {1, 1, 1}, {255, 125, 169}, {255, 175, 201}, {127, 127, 127}, {79, 55, 25}, {195, 195, 195}, {175, 141, 103}, {129, 89, 39}, 
      {255, 255, 255}, {255, 241, 187}, {255, 233, 157}, {255, 205, 121}, {225, 155, 85}, {167, 99, 31}, {0, 0, 0}, {0, 0, 0}, 
      {1, 1, 1}, {255, 125, 169}, {255, 175, 201}, {127, 127, 127}, {85, 121, 129}, {195, 195, 195}, {177, 201, 217}, {127, 175, 191}, 
      {255, 255, 255}, {1, 1, 1}, {29, 81, 129}, {27, 105, 185}, {53, 133, 237}, {33, 191, 255}, {83, 191, 249}, {133, 209, 251}, 
      {1, 1, 1}, {81, 55, 23}, {175, 141, 103}, {127, 127, 127}, {79, 55, 25}, {195, 195, 195}, {129, 89, 39}, {175, 141, 103}, 
      {255, 255, 255}, {141, 231, 255}, {157, 251, 253}, {191, 253, 255}, {207, 249, 253}, {253, 255, 255}, {0, 0, 0}, {255, 255, 255}, 
      {1, 1, 1}, {39, 26, 24}, {73, 46, 42}, {107, 67, 53}, {123, 78, 58}, {134, 93, 72}, {157, 107, 81}, {163, 111, 90}, 
      {255, 255, 255}, {161, 120, 96}, {171, 133, 105}, {178, 138, 115}, {189, 148, 120}, {201, 159, 134}, {217, 182, 150}, {233, 201, 161}, 
      {11, 11, 11}, {255, 125, 169}, {249, 179, 183}, {87, 87, 87}, {255, 245, 141}, {255, 255, 255}, {255, 175, 43}, {249, 163, 27}, 
      {223, 125, 27}, {189, 103, 17}, {1, 1, 129}, {1, 129, 129}, {0, 255, 127}, {17, 65, 129}, {191, 17, 129}, {129, 255, 129}, 
      {129, 129, 249}, {65, 17, 129}, {239, 191, 129}, {239, 65, 129}, {191, 239, 129}, {17, 191, 129}, {65, 239, 129}, {255, 3, 135}, 
      {127, 1, 129}, {255, 127, 129}, {211, 255, 127}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0} }
};

// buffer palette for writing to CRAM (populated at runtime)
RgbBuff bufferSprites = {
    { 0, 0, 0, 0, 0, 0, 0, 0, 
      0, 0, 0, 0, 0, 0, 0, 0, 
      0, 0, 0, 0, 0, 0, 0, 0, 
      0, 0, 0, 0, 0, 0, 0, 0, 
      0, 0, 0, 0, 0, 0, 0, 0, 
      0, 0, 0, 0, 0, 0, 0, 0, 
      0, 0, 0, 0, 0, 0, 0, 0, 
      0, 0, 0, 0, 0, 0, 0, 0, 
      0, 0, 0, 0, 0, 0, 0, 0, 
      0, 0, 0, 0, 0, 0, 0, 0, 
      0, 0, 0, 0, 0, 0, 0, 0, 
      0, 0, 0, 0, 0, 0, 0, 0, 
      0, 0, 0, 0, 0, 0, 0, 0, 
      0, 0, 0, 0, 0, 0, 0, 0, 
      0, 0, 0, 0, 0, 0, 0, 0, 
      0, 0, 0, 0, 0, 0, 0, 0, 
      0, 0, 0, 0, 0, 0, 0, 0, 
      0, 0, 0, 0, 0, 0, 0, 0, 
      0, 0, 0, 0, 0, 0, 0, 0, 
      0, 0, 0, 0, 0, 0, 0, 0, 
      0, 0, 0, 0, 0, 0, 0, 0, 
      0, 0, 0, 0, 0, 0, 0, 0, 
      0, 0, 0, 0, 0, 0, 0, 0, 
      0, 0, 0, 0, 0, 0, 0, 0, 
      0, 0, 0, 0, 0, 0, 0, 0, 
      0, 0, 0, 0, 0, 0, 0, 0, 
      0, 0, 0, 0, 0, 0, 0, 0, 
      0, 0, 0, 0, 0, 0, 0, 0, 
      0, 0, 0, 0, 0, 0, 0, 0, 
      0, 0, 0, 0, 0, 0, 0, 0, 
      0, 0, 0, 0, 0, 0, 0, 0, 
      0, 0, 0, 0, 0, 0, 0 }
};

// palette for calculating color and lighting - hue, saturation, luminance (populated at runtime)
HslPalette hslSprites = {
    { {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 
      {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 
      {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 
      {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 
      {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 
      {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 
      {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 
      {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 
      {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 
      {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 
      {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 
      {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 
      {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 
      {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 
      {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 
      {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 
      {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 
      {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 
      {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 
      {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 
      {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 
      {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 
      {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 
      {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 
      {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 
      {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 
      {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 
      {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 
      {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 
      {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 
      {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 
      {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0} }
};

// palette ranges
PaletteRange p_rangeSpritesAll   = { 0, 233 };     // all colors (excluding normal map colors)

// FONT (start index chosen arbitrarily - don't forget to reset palette after exiting screen!
PaletteRange p_rangeFont2All  = {64, 93}; // A - END
PaletteRange p_rangeFont2[FONT_CHARS] = {
    {64, 64}, {65, 65}, {66, 66}, {67, 67}, {68, 68}, {69, 69}, // A B C D E F
    {70, 70}, {71, 71}, {72, 72}, {73, 73}, {74, 74}, {75, 75}, // G H I J K L
    {76, 76}, {77, 77}, {78, 78}, {79, 79}, {80, 80}, {81, 81}, // M N O P Q R
    {82, 82}, {83, 83}, {84, 84}, {85, 85}, {86, 86}, {87, 87}, // S T U V W X
    {88, 88}, {89, 89}, {90, 90}, {91, 91}, {92, 92}, {93, 93}, // Y Z ! . < END
};

PaletteRange p_rangeLogo      = { 16, 23 };     // logo
PaletteRange p_rangeGoals[MAX_PLAYERS] = {      // Goal Colors
    {60, 60}, {61, 61}, {62, 62}, {63, 63}
};
PaletteRange p_rangePmenu[MAX_PLAYERS] = {      // pmenu (players)
    {10, 10}, {11, 11}, {12, 12}, {13, 13}
};
PaletteRange p_rangePmenuAll  = { 10, 12 };

PaletteRange p_rangeMenu1     = { 26, 26 };     // tmenu (title)
PaletteRange p_rangeMenu2     = { 27, 27 };
PaletteRange p_rangeMenu3     = { 28, 28 };
PaletteRange p_rangeMenu4     = { 31, 31 };
PaletteRange p_rangePpoppy1   = { 33, 36 };     // pixel poppy
PaletteRange p_rangePpoppy2   = { 37, 39 };
PaletteRange p_rangePpoppy3   = { 40, 43 };
PaletteRange p_rangeMacchi1   = { 65, 66 };     // macchi
PaletteRange p_rangeMacchi2   = { 68, 71 };
PaletteRange p_rangeJelly1    = { 81, 82 };     // jelly
PaletteRange p_rangeJelly2    = { 84, 87 };
PaletteRange p_rangePenny1    = { 97, 98 };     //penny
PaletteRange p_rangePenny2    = { 100, 103 };
PaletteRange p_rangePotter1   = { 113, 114 };   // potter
PaletteRange p_rangePotter2   = { 116, 119 };
PaletteRange p_rangeSparta1   = { 113, 114 };   // sparta
PaletteRange p_rangeSparta2   = { 116, 119 };
PaletteRange p_rangePoppy1    = { 145, 146 };   // poppy
PaletteRange p_rangePoppy2    = { 148, 151 };
PaletteRange p_rangeTj1       = { 161, 162 };   // tj
PaletteRange p_rangeTj2       = { 164, 167 };
PaletteRange p_rangeGeorge1   = { 177, 178 };   // george
PaletteRange p_rangeGeorge2   = { 180, 183 };
PaletteRange p_rangeWuppy1    = { 193, 194 };   // wuppy
PaletteRange p_rangeWuppy2    = { 196, 199 };
PaletteRange p_rangeStadler1  = { 209, 215 };   // stadler
PaletteRange p_rangeGarfield1 = { 225, 231 };   // garfield
PaletteRange p_rangeBomb      = { 89, 91 };     // bomb
PaletteRange p_rangeFish      = { 153, 156 };   // fish
PaletteRange p_rangeShroom    = { 158, 159 };   // mushroom
PaletteRange p_rangeNormalMap = { 234, 254 };   // normal map

// tracks the accumulation of changes for the HSL color model
GlobalHSL hsl_incSprites = {0, 0, 0};

// initial image setup: hue, saturation, luminance, x_pos, y_pos, x_scale, y_scale, x_scroll (rate), y_scroll (rate), min_sat_id, max_sat_id, min_lum_id, max_lum_id
// TODO: adjust attrSprites hue based on time (normal mapping) original was 288
ImageAttr attrSprites = { 200, 255, 255, toFIXED(0.0), toFIXED(0.0), toFIXED(0.25), toFIXED(0.0), 0, 0, 0, 0};

void init_sprites_img(void) {
    MultiRgbToHsl(&hslSprites, &rgbSprites, &p_rangeSpritesAll);
    min_max_sl_id(&hslSprites, &p_rangeSpritesAll, &attrSprites);
    
    for(unsigned int i = 0; i < (MAX_PLAYERS); i++)
    {
        do_update_Pmenu[i] = false;
        update_palette_Pmenu[i] = false;
    }
    
    // normal maps
    InitNormal2d(&hslSprites, &rgbSprites, &light, &p_rangeNormalMap, &attrSprites);
    MultiPaletteUpdate(&game_palette, &hslSprites, &hsl_incSprites, &p_rangeNormalMap);
}

// directly set the HSL color
bool calculate_sprites_color(PaletteRange *range) {
    MultiHslTorRgb2Buffer(&hslSprites, &bufferSprites, range);
    return true;
}

// set the HSL color by hue, saturation, and luminance increments
bool update_sprites_color(PaletteRange *range) {
    MultiPalette2Buffer(&bufferSprites, &hslSprites, &hsl_incSprites, range);
    return true;
}

// update CRAM from buffer palette
bool update_sprites_palette(PaletteRange *range) {
    UpdatePaletteFromBuffer(&bufferSprites, &game_palette, range);
    return false;
}

void reset_sprites (void) {
    // hsl_incSprites = (GlobalHSL){0, 0, 0}; // is this neccesary?
    MultiRgbToHsl(&hslSprites, &rgbSprites, &p_rangeSpritesAll);
    min_max_sl_id(&hslSprites, &p_rangeSpritesAll, &attrSprites);
    MultiPalette2Buffer(&bufferSprites, &hslSprites, &hsl_incSprites, &p_rangeSpritesAll);
}


void update_ppplogo_color(void) {
    NormalMapLighting2d(&hslSprites, &rgbSprites, &bufferSprites, &light, &p_rangeNormalMap, &hsl_incSprites);
}

void update_ppplogo_palette(void) {
    UpdatePaletteFromBuffer(&bufferSprites, &game_palette, &p_rangeNormalMap);
}