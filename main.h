#pragma once

#include <jo/jo.h>
#include "state.h"
#include "util.h"
#include "audio.h"

#define VERSION "0.2"
#define MAX_DEBUG_LEVEL 1
#define MAX_PLAYERS 4
#define MAX_TEAMS 4
#define MIN_TEAMS 2
#define MAX_TEAM_MEMBERS 3
#define TIMEOUT 99 // seconds - MAYBE MAKE AN OPTION?

extern Uint8 max_players; // replaces MAX_PLAYERS
extern bool first_load;
extern bool debug_display;
extern bool widescreen; // anamorphic
extern Uint8 frame;
extern Uint8 g_GameTimer;
extern Uint16 cursor_angle; // for title & pawsed menus

// supported game types
typedef enum _GAME_MODE
{
    GAME_MODE_PRACTICE = 0, // unlimited lives
    GAME_MODE_NORMAL = 1, // 3 lives
    GAME_MODE_HARDCORE = 2, // 1 life
    GAME_MODE_TIME_ATTACK = 3, // only one team can move at a time.
    GAME_MODE_MAX,
} GAME_MODE;

// supported game difficulty
typedef enum _GAME_DIFFICULTY
{
    GAME_DIFFICULTY_EASY = 0,
    GAME_DIFFICULTY_MEDIUM = 1,
    GAME_DIFFICULTY_HARD = 2,
    GAME_DIFFICULTY_MAX,
} GAME_DIFFICULTY;

// supported game difficulty
typedef enum _GAME_RESOLUTION
{
    RESOLUTION_NORMAL = TV_352x240,
    RESOLUTION_HIGH = TV_704x480,
    RESOLUTION_WIDE   = TV_704x240,
} GAME_RESOLUTION;

// represents game state
typedef struct _GAME
{
    // current game state
    GAME_STATE gameState;
    
    // current game state
    GAME_STATE nextState;

    // practice, normal, hardcore, or time attack
    GAME_MODE gameMode;

    // easy, medium, hard
    // affects how many mines are in the game
    GAME_DIFFICULTY gameDifficulty;

    // debug level
    int debug;

    // number of lives (1, 3, 6, 9)
    int numLives;

    // is the game is paused
    bool isPaused;

    // is the game finished?
    bool isGameOver;

    // how many frames since all players were dead
    int gameOverFrames;

} GAME, *PGAME;

// globals
extern GAME g_Game;

