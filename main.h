#pragma once

#include <jo/jo.h>
#include "state.h"
#include "util.h"
#include "audio.h"

#define VERSION "0.44"
#define MAX_DEBUG_LEVEL 1
#define MAX_PLAYERS 4
#define MAX_TEAMS 4
#define MIN_TEAMS 2
#define MAX_TEAM_MEMBERS 2

// Screen position
#define SCREEN_RIGHT  toFIXED(352.0)
#define SCREEN_LEFT  toFIXED(-352.0)
#define SCREEN_MIDDLE  toFIXED(0)
#define SCREEN_QUARTER  toFIXED(100.0)
#define SCREEN_WIDTH  toFIXED(704.0)
#define SCREEN_HEIGHT  toFIXED(480.0)
#define SCREEN_TOP toFIXED(-240.0)
#define SCREEN_BOTTOM toFIXED(240.0)

extern jo_datetime now;
extern Uint8 frame;
extern Uint8 g_GameTimer;
extern Uint16 cursor_angle; // for title & pawsed menus

// temporary - to be removed
extern int player1_score;
extern int player2_score;

typedef struct {
    bool debug_mode;
    bool debug_display;
    bool mesh_display;
    bool mosaic_display;
    bool use_rtc;
} GameOptions;

extern GameOptions game_options;

// 1-4 players
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

// game resolutions (for testing)
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
    GAME_STATE nextState;
    GAME_STATE lastState;

    // number of players
    NUMBER_OF_PLAYERS minPlayers;
    NUMBER_OF_PLAYERS maxPlayers;
    NUMBER_OF_PLAYERS numPlayers;

    // number of teams
    int minTeams;
    int maxTeams;
    int numTeams;
    
    // classic, story, battle
    GAME_MODE gameMode;

    // easy, medium, hard
    GAME_DIFFICULTY gameDifficulty;
    
    // debug level
    int debug;

    // number of lives (1, 3, 6, 9)
    int numLives;

    // is the game loading?
    bool isLoading;

    // is the game is paused?
    bool isPaused;

    // is the game finished?
    bool isGameOver;
    
    // is the game playing?
    bool isActive;

    // how many frames since all players were dead
    int gameOverFrames;

} GAME, *PGAME;

// globals
extern GAME g_Game;
