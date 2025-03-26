#pragma once

#include "physics.h"
#include "gameplay.h"
#include "physics.h"
#include "objects/player.h"

#define SCORE_DISPLAY_TIME (30 * 60)

#define INITIALS_LENGTH 3  // Three-letter initials like "ABC"
#define SCORE_ENTRIES 10

typedef enum _TEAMS_FOR_SCORING
{
    ONE_TEAM = 1,
    TWO_TEAMS = 2,
    THREE_TEAMS = 3,
    FOUR_TEAMS = 4,
} TEAMS_FOR_SCORING;
    

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

static __jo_force_inline void updatePlayerLives(int lastTouchPlayerID, int scoredOnPlayerID)
{
    if (g_Players[scoredOnPlayerID].numLives > 0) {
        g_Players[scoredOnPlayerID].numLives--;
        if (g_Players[scoredOnPlayerID].numLives == 0) {
            // kill player?
            g_Players[scoredOnPlayerID].score.deaths++;
            if (g_Players[scoredOnPlayerID].score.deaths < g_Game.numContinues) {
                g_Players[scoredOnPlayerID].numLives = getLives();
            }
            else {
                g_Players[scoredOnPlayerID].subState = PLAYER_STATE_DEAD;
            }
            g_Players[lastTouchPlayerID].score.stars++;
        }
    }
}

// these don't work - need to assign a goal id for the player, instead of trying to calculate it since the player id isn't the same thing as the goal
static __jo_force_inline void updateScoreLeft(Sprite *ball) {
    int lastTouchPlayerID = -1;
    int scoredOnPlayerID = -1;
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
        g_Game.isGoalScored = true;
        
    }
    if (g_Game.numTeams == 4 && ball->pos.y > SCREEN_MIDDLE) {
        scoredOnPlayerID = 3;
    }
    else if (g_Game.numTeams == 4 && ball->pos.y < SCREEN_MIDDLE) {
        scoredOnPlayerID = 1;
    }
    else {
        scoredOnPlayerID = 1;
    }
    updatePlayerLives(lastTouchPlayerID, scoredOnPlayerID);
}

static __jo_force_inline void updateScoreRight(Sprite *ball) {
    int lastTouchPlayerID = -1;
    int scoredOnPlayerID = -1;
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
        g_Game.isGoalScored = true;
    }
    if (g_Game.numTeams > 2 && ball->pos.y > SCREEN_MIDDLE) {
        scoredOnPlayerID = 2;
    }
    else if (g_Game.numTeams > 2 && ball->pos.y < SCREEN_MIDDLE) {
        scoredOnPlayerID = 0;
    }
    else {
        scoredOnPlayerID = 0;
    }
    updatePlayerLives(lastTouchPlayerID, scoredOnPlayerID);
}

static __jo_force_inline void checkLeftWallScore(Sprite *ball) {
    // switch for number of teams
    // possible cases: 1 player, 3 players
    switch (g_Game.numTeams) {
        case ONE_TEAM:
        case TWO_TEAMS: {
            if (ball->pos.y > -toFIXED(g_Game.goalYPosTop) && ball->pos.y < toFIXED(g_Game.goalYPosTop)) {
                updateScoreLeft(ball); // scored on TEAM_1
            }
            break;
        }
        case THREE_TEAMS:
        case FOUR_TEAMS: {
            // if there are 3 teams, then we need to check the bounds of both the upper & lower goals
            if (ball->pos.y > -toFIXED(GOAL_Y_POS_TOP_VS_MODE) && ball->pos.y < -toFIXED(GOAL_Y_POS_BOT_VS_MODE)) {
                updateScoreLeft(ball); // scored on TEAM_1
            }
            else if (ball->pos.y > toFIXED(GOAL_Y_POS_BOT_VS_MODE) && ball->pos.y < toFIXED(GOAL_Y_POS_TOP_VS_MODE)) {
                updateScoreLeft(ball); // scored on TEAM_3
            }
            break;
        }
        default:
            break;
    }
}

static __jo_force_inline void checkRightWallScore(Sprite *ball) {
    // switch for number of teams
    // possible cases: 2 player, 4 players
    switch (g_Game.numTeams) {
        case ONE_TEAM:
        case TWO_TEAMS: {
            if (ball->pos.y > -toFIXED(g_Game.goalYPosTop) && ball->pos.y < toFIXED(g_Game.goalYPosTop)) {
                updateScoreRight(ball); // scored on TEAM_2
            }
            break;
        }
        case THREE_TEAMS:
        case FOUR_TEAMS: {
            // if there are 4 teams, then we need to check the bounds of both the upper & lower goals
            if (ball->pos.y > -toFIXED(GOAL_Y_POS_TOP_VS_MODE) && ball->pos.y < -toFIXED(GOAL_Y_POS_BOT_VS_MODE)) {
                updateScoreRight(ball); // scored on TEAM_2
            }
            else if (ball->pos.y > toFIXED(GOAL_Y_POS_BOT_VS_MODE) && ball->pos.y < toFIXED(GOAL_Y_POS_TOP_VS_MODE)) {
                updateScoreRight(ball); // scored on TEAM_4
            }
            break;
        }
        default:
            break;
    }
}
