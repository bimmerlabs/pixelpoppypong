#pragma once

#include <jo/jo.h>

#define FRAMERATE 4
#define DEMO_TIMER (30 * 60)

extern ANGLE ball_rotation;
extern unsigned int g_DemoTimer;
extern Bool start_timer;
extern Bool g_RoundOver;
extern Uint16 g_RoundOverTimer;

void demo_update(void);
void game_timer(void);
void gameplay_draw(void);
void my_color_calc(void);void my_palette_update(void);
void load_gameplay_assets(void);
void gameplay_init(void);
void demo_init(void);
void gameplay_update(void);
void gameplay_input(void);