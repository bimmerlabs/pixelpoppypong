#pragma once

extern unsigned int g_LogoTimer;
extern int transparency_rate;
#define PPP_LOGO_TIMER (15 * 60)
#define PPP_MOSAIC_TIMER (9 * 60)
#define PPP_NBG0_INC 1
#define PPP_NBG1_INC 1
#define PPP_NBG1_MIN 0

#define T_DAWN 5
#define T_DAY 11
#define T_DUSK 17
#define T_NIGHT 23

#define TRANSPARENCY_MIN    CLRate31_1
#define TRANSPARENCY_MAX    CLRate0_32

// time of day
typedef enum _BG_TIME
{
    BG_DAWN  = 1,
    BG_DAY   = 2,
    BG_DUSK  = 3,
    BG_NIGHT = 4,
    BG_CNTRL = 5,
} BG_TIME;

void pppLogo_init(void);
void pppLogo_input(void);
void pppLogo_update(void);
void pppLogo_draw(void);
