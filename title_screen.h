#pragma once

#define TITLE_TIMER (30 * 60)
#define MODE_ID 0     // sprite index offset for menu text
#define MODE_OFFSET 6 // sprite index offset for mode select
#define DIFF_ID 1     // sprite index offset for menu text
#define DIFF_OFFSET 3 // sprite index offset for difficulty select
#define START_ID 2    // sprite index offset for menu text

extern FIXED pos_x;
extern FIXED pos_y;
extern unsigned int g_TitleTimer;
extern int g_TitleScreenChoice;

void titleScreen_init(void);
void titleScreen_input(void);
void titleScreen_menu(void);
void titleScreen_update(void);
void titleScreen_draw(void);

