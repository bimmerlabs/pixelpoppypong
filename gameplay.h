#pragma once

#include <jo/jo.h>

// #define ROUND_OVER_TIME (4 * 60)

#define GAME_BEGIN_TIME (3 * 60)
#define DEMO_TIME (30 * 60)
#define TIMEOUT 99 // seconds - MAYBE MAKE AN OPTION?

#define GOAL_TOP toFIXED(-100.0)
#define GOAL_BOTTOM toFIXED(100.0)

extern ANGLE ball_rotation;
extern unsigned int g_DemoTimer;
extern bool start_timer;
extern bool round_start;
extern bool g_RoundOver;
extern Uint16 g_GameBeginTimer;
extern Uint16 g_RoundOverTimer;

void demo_update(void);
void game_timer(void);
void gameplay_draw(void);
void my_color_calc(void);void my_palette_update(void);
void gameplay_init(void);
void demo_init(void);
void gameplay_update(void);
void gameplay_input(void);
void demo_input(void);