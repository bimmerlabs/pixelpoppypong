// nbg1.h
#pragma once

#include "../palettetools.h"

extern RgbPalette rgbBg0;
extern RgbBuff bufferBg0;
extern HslPalette hslBg0;

extern Uint8 current_background;

extern ImageAttr attrNbg1;

void init_nbg1_img(void);

void update_nbg1_palette(void);
