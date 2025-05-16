#pragma once
#include "../palettefx/sprite_colors.h"
#include "../objects/teams.h"

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
    #if ENABLE_DEBUG_MODE == 1
    OPTION_DEBUG_MODE = 0,
    OPTION_DEBUG_TEXT,
    OPTION_DEBUG_COLLISION,
    OPTION_DRAWMESH,
    #else
    OPTION_DRAWMESH = 0,
    #endif
    OPTION_DRAWMOSAIC,
    OPTION_USE_RTC,
    OPTION_BIG_HEAD,
    OPTION_ITEMS,
    OPTION_MEOW,
    OPTION_ANALOG,
    OPTION_EXIT,
    OPTION_MAX,
} OPTIONS;

typedef struct _TITLESCREEN
{
    unsigned int timer;
    int menuChoice;
    int menuLastChoice;
    int optionChoice;
    int logo1_pos;
    int logo2_pos;
    int logo_velocity;
    bool logo_falling;
    bool logo_bounce;
    bool increasing;
    int h_value;
    
    bool draw_start_text;
    bool draw_option_mode;
    bool draw_option_players;
    bool draw_option_difficulty;
    bool draw_option_start;
    bool draw_option_options;
    bool poppy_animation;
    int poppy_animation_frame;
    int poppy_animation_id;
    float poppy_scale;
    float poppy_velocity;
} TITLESCREEN;

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
    }
    else {
        g_Game.minPlayers = TWO_PLAYER;
        g_Game.maxPlayers = FOUR_PLAYER;
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
void animateMenuColor(bool *_do_update);