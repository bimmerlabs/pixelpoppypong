#pragma once

extern unsigned int g_LogoTimer;
#define PPP_LOGO_TIMER (10 * 60)
#define PPP_FADE_TIMER (6 * 60)

void pppLogo_init(void);
void pppLogo_input(void);
void pppLogo_update(void);
void pppLogo_draw(void);
