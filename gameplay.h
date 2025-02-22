#pragma once

#include <jo/jo.h>

// #define ROUND_OVER_TIME (4 * 60)

#define GAME_BEGIN_TIME (3 * 60)
#define DEMO_TIME (30 * 60)
#define TIMEOUT 99 // seconds - MAYBE MAKE AN OPTION?

#define GOAL_TOP toFIXED(-100.0)
#define GOAL_BOTTOM toFIXED(100.0)

#define INITIALS_LENGTH 3  // Three-letter initials like "ABC"
#define SCORE_ENTRIES 10
extern ANGLE ball_rotation;
extern unsigned int g_DemoTimer;
extern bool start_timer;
extern bool round_start;
extern bool g_RoundOver;
extern Uint16 g_GameBeginTimer;
extern Uint16 g_RoundOverTimer;

typedef struct {
    int score;
    char initials[INITIALS_LENGTH + 1];  // +1 for null terminator
} HighScoreEntry;

extern HighScoreEntry highScores[SCORE_ENTRIES];

// SCORE LEVELS TO UNLOCK NEW CHARACTERS
typedef enum _CHARACTER_UNLOCK_SCORES
{
    UNLOCK_SPARTA = 2500,
    UNLOCK_POPPY = 5000,
    UNLOCK_TJ = 10000,
    UNLOCK_GEORGE = 25000,
    UNLOCK_WUPPY = 50000,
    UNLOCK_CRAIG = 100000,
    UNLOCK_GARFIELD = 250000,
} UNLOCK_SCORES;void highScore_init(void);
void gameplay_init(void);
void demo_init(void);
void demo_update(void);
void game_timer(void);
void gameplay_draw(void);
void gameplay_update(void);
void gameplay_input(void);
void demo_input(void);