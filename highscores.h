#pragma once

#include "physics.h"
#include "gameplay.h"
#include "objects/player.h"

#define SCORE_DISPLAY_TIME (30 * 60)

#define INITIALS_LENGTH 3  // Three-letter initials like "ABC"
#define SCORE_ENTRIES 10

extern unsigned int g_ScoreTimer;

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
} UNLOCK_SCORES;

void highScore_init(void);
void init_scores(void);
void display_scores(void);
void score_input(void);

void sortHighScores(HighScoreEntry scores[]);
void addHighScore(Uint16 newScore, const char *initials);
void update_bg_position(void);

extern PLAYER g_Players[MAX_PLAYERS];

static inline void updateScoreLeft(Sprite *ball) {
    int lastTouchPlayerID = -1;
    // Find the last player who touched the ball
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (touchedBy[i].hasTouched) {
            lastTouchPlayerID = i;
            break; // Exit the loop early since only one player should have hasTouched == true
        }
    }
    // Assign score if a valid player touched it
    if (lastTouchPlayerID != -1 && g_Players[lastTouchPlayerID].onLeftSide) {
        unsigned int points = (JO_ABS(toINT(ball->vel.x)) * 10) 
                            + (JO_ABS(toINT(ball->vel.y)) * 100) 
                            + (JO_ABS(toINT(ball->vel.z)) * 500);
        g_Players[lastTouchPlayerID].score.points += points;
    }    
}

static inline void updateScoreRight(Sprite *ball) {
    int lastTouchPlayerID = -1;
    // Find the last player who touched the ball
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (touchedBy[i].hasTouched) {
            lastTouchPlayerID = i;
            break; // Exit the loop early since only one player should have hasTouched == true
        }
    }
    // Assign score if a valid player touched it
    if (lastTouchPlayerID != -1 && !g_Players[lastTouchPlayerID].onLeftSide) {
        unsigned int points = (JO_ABS(toINT(ball->vel.x)) * 10) 
                            + (JO_ABS(toINT(ball->vel.y)) * 100) 
                            + (JO_ABS(toINT(ball->vel.z)) * 500);
        g_Players[lastTouchPlayerID].score.points += points;
    }     
}