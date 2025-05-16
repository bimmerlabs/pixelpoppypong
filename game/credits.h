#pragma once

#define CREDITS_BEGIN (1 * 60)
#define CREDITS_DISPLAY_TIME (2.5 * 60)
#define CREDITS_DELAY_TIME (1 * 60)

#define CREDITS_X1 8
#define CREDITS_X2 12

typedef struct _CREDITS
{
    unsigned int timer;
    bool fade_in;
    bool display;
    bool fade_out;
    Uint8 id;
    int display_timer;
    int delay_timer;
    Uint8 bg_spr_id;
} CREDITS;

void init_credits(void);
void display_credits(void);
void credits_input(void);