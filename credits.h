#pragma once

#define CREDITS_TIME (100 * 60)
#define CREDITS_BEGIN (1 * 60)
#define CREDITS_DISPLAY_TIME (2 * 60)
#define CREDITS_DELAY_TIME (1 * 60)

#define CREDITS_RATE toFIXED(0.25)
#define CREDITS_START toFIXED(-150)
#define CREDITS_END toFIXED(280)
#define CREDITS_X1 8
#define CREDITS_X2 12

extern unsigned int g_CreditsTimer;

void init_credits(void);
void display_credits(void);
void credits_input(void);