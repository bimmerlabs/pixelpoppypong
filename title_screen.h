#pragma once

#define TITLE_TIMER (30 * 60)
#define MODE_ID 1       // sprite index offset for mode text
#define MODE_OFFSET 2   // sprite index offset for mode select
#define PLAYER_ID 2     // sprite index offset for player text
#define PLAYER_OFFSET 5 // sprite index offset for player select
#define DIFF_ID 2       // sprite index offset for difficulty text
#define DIFF_OFFSET 9   // sprite index offset for difficulty select
#define OPTIONS_ID 1    // sprite index offset for options text
#define START_ID 0      // sprite index offset for menu text

#define LOGO1_ONSCREEN_Y -200
#define LOGO2_ONSCREEN_Y 200
#define LOGO1_OFFSCREEN_Y -400
#define LOGO2_OFFSCREEN_Y 400
#define LOGO_VELOCITY 12

#define MENU_Y 150

#define POPPY_MAX_SCALE 6

extern FIXED pos_x;
extern FIXED pos_y;
extern unsigned int g_TitleTimer;
extern int g_TitleScreenChoice;
extern int g_OptionScreenChoice;

extern int logo1_pos;
extern int logo2_pos;
extern int logo_velocity;
extern bool logo_falling;
extern bool logo_bounce;

typedef enum _MENU_OPTIONS
{
    TITLE_OPTION_GAME_MODE = 0,
    TITLE_OPTION_GAME_PLAYERS = 1,
    TITLE_OPTION_GAME_DIFFICULTY = 2,
    TITLE_OPTION_GAME_START = 3,
    TITLE_OPTION_GAME_OPTIONS = 4,
    TITLE_OPTION_MAX,
} MENU_OPTIONS;

typedef enum _OPTIONS
{
    OPTION_DEBUG_MODE = 0,
    OPTION_DEBUG_TEXT,
    OPTION_DRAWMESH,
    OPTION_DRAWMOSAIC,
    OPTION_USE_RTC,
    // OPTION_BUP_DEV,
    // OPTION_ANALOG,
    // OPTION_WIDESCREEN,
    OPTION_EXIT,
    OPTION_MAX,
} OPTIONS;

void titleScreen_init(void);
void titleMenu_init(void);
void optionsScreen_init(void);
void titleScreen_input(void);
void menuScreen_input(void);
void optionsScreen_input(void);
void startScreen_update(void);
void titleScreen_update(void);
void optionsScreen_update(void);
void drawTitle(void);
void drawMenu(void);
void drawOptions(void);
void drawMenuCursor(void);
void drawOptionsCursor(void);
