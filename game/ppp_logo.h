#pragma once

extern unsigned int g_LogoTimer;
extern int transparency_rate;
#define PPP_LOGO_TIMER (17 * 60)
#define PPP_MOSAIC_TIMER (9 * 60)
#define PPP_NBG0_INC 1
#define PPP_NBG1_INC 1
#define PPP_NBG1_MIN 0

#define TRANSPARENCY_MIN    CLRate31_1
#define TRANSPARENCY_MAX    CLRate0_32

#define LIGHT_SPEED toFIXED(3)

void pppLogo_init(void);
void pppLogo_input(void);
void pppLogo_update(void);
void pppLogo_draw(void);
void update_light_position(void);