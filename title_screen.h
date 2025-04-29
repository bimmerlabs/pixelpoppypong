#pragma once
#include "BG_DEF/sprite_colors.h"

#define TITLE_TIMER (30 * 60)
#define LOGO_TIMER (25 * 60)
#define LOGO_TIMER2 (24 * 60)
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

extern unsigned int g_TitleTimer;
extern int g_TitleScreenChoice;
extern int g_OptionScreenChoice;

extern int logo1_pos;
extern int logo2_pos;
extern int logo_velocity;
extern bool logo_falling;
extern bool logo_bounce;

extern bool increasing;
extern int h_value;

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
    OPTION_DEBUG_COLLISION,
    OPTION_DRAWMESH,
    OPTION_DRAWMOSAIC,
    OPTION_USE_RTC,
    OPTION_BIG_HEAD,
    OPTION_ITEMS,
    OPTION_MEOW,
    OPTION_ANALOG,
    OPTION_EXIT,
    OPTION_MAX,
} OPTIONS;

static inline void animateMenuColor(bool *_do_update) {
    if (increasing) {
        if (h_value < 27) {
            h_value += 1;
            hsl_incSprites.h = -2;
            hsl_incSprites.l = 3;
            hsl_incSprites.s = -3;
        } 
        else {
            increasing = false;
        }
    } 
    else {
        if (h_value > 0) {
            h_value -= 1;
            hsl_incSprites.h = 2;
            hsl_incSprites.l = -3;
            hsl_incSprites.s = +3;
        } 
        else {
            increasing = true;
        }
    }
    *_do_update = true;
}

static inline void selectGameMode(void) {
    if (g_Game.gameMode == GAME_MODE_STORY) {
        g_Game.minPlayers = ONE_PLAYER;
        g_Game.maxPlayers = ONE_PLAYER;
        g_Game.numPlayers = g_Game.minPlayers;
        g_Team.minTeams = ONE_TEAM;
        g_Team.maxTeams = ONE_TEAM;
    }
    else if (g_Game.gameMode == GAME_MODE_CLASSIC) {
        g_Game.minPlayers = TWO_PLAYER;
        g_Game.maxPlayers = TWO_PLAYER;
        g_Game.numPlayers = g_Game.minPlayers;
        g_Team.minTeams = TWO_TEAMS;
        g_Team.maxTeams = TWO_TEAMS;
        // g_Team.numTeams = TEAM_1; // why is this here?
    }
    else {
        g_Game.minPlayers = TWO_PLAYER;
        if (!releaseCanidate) {
            g_Game.maxPlayers = FOUR_PLAYER;
        }
        else {
            g_Game.maxPlayers = TWO_PLAYER;
        }
        g_Game.numPlayers = g_Game.minPlayers;
        g_Team.minTeams = TWO_TEAMS;
        g_Team.maxTeams = TWO_TEAMS;
    }
}

static inline void selectNumPlayers(void) {
    if (g_Game.numPlayers == ONE_PLAYER) {
        g_Team.minTeams = ONE_TEAM;
        g_Team.maxTeams = ONE_TEAM;
    }
    else if (g_Game.numPlayers == TWO_PLAYER) {
        g_Team.minTeams = TWO_TEAMS;
        g_Team.maxTeams = TWO_TEAMS;
    }
    else if (g_Game.numPlayers == THREE_PLAYER) {
        g_Team.minTeams = THREE_TEAMS;
        g_Team.maxTeams = THREE_TEAMS;
    }
    else if (g_Game.numPlayers == FOUR_PLAYER) {
        g_Team.minTeams = FOUR_TEAMS;
        g_Team.maxTeams = FOUR_TEAMS;
    }
}

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
