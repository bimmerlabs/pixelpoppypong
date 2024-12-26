#pragma once

#include <jo/jo.h>
#include "state.h"
#include "util.h"
#include "audio.h"

#define VERSION "0.35"
#define MAX_DEBUG_LEVEL 1
#define MAX_PLAYERS 4
#define MAX_TEAMS 4
#define MIN_TEAMS 2
#define MAX_TEAM_MEMBERS 3
#define TIMEOUT 99 // seconds - MAYBE MAKE AN OPTION?

extern Uint8 max_players; // replaces MAX_PLAYERS
extern bool first_load;
extern Uint8 frame;
extern Uint8 g_GameTimer;
extern Uint16 cursor_angle; // for title & pawsed menus

typedef struct {
    bool debug_mode;
    bool debug_display;
    bool mesh_display;
    bool mosaic_display;
    bool widescreen;
} GameOptions;

extern GameOptions game_options;

// supported game types
typedef enum _NUMBER_OF_PLAYERS
{
    ONE_PLAYER = 0,
    TWO_PLAYER = 1,
    THREE_PLAYER = 2,
    FOUR_PLAYERS = 3,
    GAME_PLAYERS_MAX,
} NUMBER_OF_PLAYERS;

// supported game types
typedef enum _GAME_MODE
{
    GAME_MODE_CLASSIC = 0, // JUST PONG
    GAME_MODE_STORY = 1, // STORY MODE?
    GAME_MODE_BATTLE = 2, // BATTLE MODE (2 OR 4 PLAYER MODE ONLY)
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
    RESOLUTION_HIGH   = TV_704x480,
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
    NUMBER_OF_PLAYERS numPlayers;
    
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

