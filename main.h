#pragma once

#include <jo/jo.h>
#include "state.h"
#include "util.h"
#include "audio.h"
#include "audio.h"

#define VERSION "0.53"
#define MAX_PLAYERS 4
#define MAX_TEAMS 4

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
// extern Uint16 g_GameTimer; // put in game struct?
extern Uint16 cursor_angle; // for title & pawsed menus

typedef struct {
    bool debug_mode;
    bool debug_display;
    bool mesh_display;
    bool mosaic_display;
    bool use_rtc;
} GameOptions, *PGameOptions;

extern GameOptions game_options;

// 1-4 players
typedef enum _NUMBER_OF_PLAYERS
{
    ONE_PLAYER = 0,
    TWO_PLAYER = 1,
    THREE_PLAYER = 2,
    FOUR_PLAYER = 3,
    GAME_PLAYERS_MAX,
} NUMBER_OF_PLAYERS;

// supported game types
typedef enum _GAME_MODE
{
    GAME_MODE_STORY = 0, // STORY MODE?
    GAME_MODE_BATTLE = 1, // BATTLE MODE (2 OR 4 PLAYER MODE ONLY)
    GAME_MODE_CLASSIC = 2, // JUST PONG
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

#define GOAL_X_POS 330
#define GOAL_CENTER_POS 0

typedef enum _GOAL_SCALE
{
    GOAL_SCALE_EASY = 20,
    GOAL_SCALE_MEDIUM = 16,
    GOAL_SCALE_HARD = 13,
    GOAL_SCALE_VS_MODE = 10,
} GOAL_SCALE;

typedef enum _GOAL_Y_POS
{
    GOAL_Y_POS_EASY = 140,
    GOAL_Y_POS_MEDIUM = 120,
    GOAL_Y_POS_HARD = 100,
    GOAL_Y_POS_MID = 0,
    GOAL_Y_POS_TOP_VS_MODE = 160,
    GOAL_Y_POS_MID_VS_MODE = 79,
    GOAL_Y_POS_BOT_VS_MODE = 2,
} GOAL_Y_POS;

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
    Uint8 currentNumPlayers;

    // number of teams
    Uint8 minTeams; // MIN AND MAX ARE ALWAYS THE SAME - REMOVE ONE
    Uint8 maxTeams;
    Uint8 numTeams;
    
    // classic, story, battle
    GAME_MODE gameMode;

    // easy, medium, hard
    GAME_DIFFICULTY gameDifficulty;

    // easy, medium, hard, vs_mode
    GOAL_SCALE goalScale;
    GOAL_Y_POS goalYPosTop;
    GOAL_Y_POS goalYPosBot;
    Uint8 goalYPosMid;
    
    // debug level
    int debug;

    // number of lives (1, 3, 6, 9)
    Uint8 numLives;
    // number of stars (1, 2, 3)
    Uint8 numStars;
    
    // TIMERS
    Uint16 BeginTimer;
    Uint16 GameTimer;
    Uint16 BombTimer;
    Uint16 RoundOverTimer;
    Uint16 DemoTimer;

    // is the game loading?
    bool isLoading;

    // is the game is paused?
    bool isPaused;

    // did somebody score a goal?
    bool isGoalScored;

    // is the game finished?
    bool isRoundOver;
    Uint8 countofRounds;
    Uint8 numContinues;

    // is the game finished?
    bool isGameOver;
    
    // is the game playing?
    bool isActive;
    
    // is the game playing?
    bool isBallActive;

    // how many frames since all players were dead
    int gameOverFrames;

} GAME, *PGAME;

// globals
extern GAME g_Game;
