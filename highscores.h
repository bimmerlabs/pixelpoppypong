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

static __jo_force_inline bool updatePlayerLives(Uint8 scoredOnPlayerID)
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
            return true;
        }
    }
    return false;
}

static __jo_force_inline void updateLeftPlayerStars(int lastTouchPlayerID)
{
    if (lastTouchPlayerID != -1 && g_Players[lastTouchPlayerID].onLeftSide) {
        g_Players[lastTouchPlayerID].score.stars++;       
    }
    else {
        if (g_Game.numTeams > 2) {
            g_Players[g_goalPlayerId[TEAM_1-1]].score.stars++; 
            g_Players[g_goalPlayerId[TEAM_3-1]].score.stars++;
        }
        else {
            g_Players[g_goalPlayerId[TEAM_3-1]].score.stars++;
        }
    }
}

static __jo_force_inline void updateRightPlayerStars(int lastTouchPlayerID)
{
    if (lastTouchPlayerID != -1 && !g_Players[lastTouchPlayerID].onLeftSide) {
        g_Players[lastTouchPlayerID].score.stars++;       
    }
    else {
        if (g_Game.numTeams == 4) {
            g_Players[g_goalPlayerId[TEAM_2-1]].score.stars++; 
            g_Players[g_goalPlayerId[TEAM_4-1]].score.stars++;
        }
        else {
            g_Players[g_goalPlayerId[TEAM_2-1]].score.stars++;
        }
    }
}

static __jo_force_inline void calculateScore(Sprite *ball, Uint8 playerID) {
    unsigned int points = (JO_ABS(toINT(ball->vel.x)) * 50) 
                         + (JO_ABS(toINT(ball->vel.y)) * 500) 
                         + (JO_ABS(toINT(ball->vel.z)) * 1000);
    g_Players[playerID].score.points += points;
    g_Game.isGoalScored = true;
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
        calculateScore(ball, lastTouchPlayerID);        
    }
    else {
        // todo: assign score if nobody on other side touched the ball
        if (g_Game.numTeams > 2) {
            calculateScore(ball, g_goalPlayerId[TEAM_1-1]);
            calculateScore(ball, g_goalPlayerId[TEAM_3-1]);
        }
        else {
            calculateScore(ball, g_goalPlayerId[TEAM_1-1]);
        }
    }
    
    if (g_Game.numTeams == 4 && ball->pos.y > SCREEN_MIDDLE) {
        scoredOnPlayerID = g_goalPlayerId[TEAM_4-1];
    }
    else if (g_Game.numTeams == 4 && ball->pos.y < SCREEN_MIDDLE) {
        scoredOnPlayerID = g_goalPlayerId[TEAM_2-1];
    }
    else {
        scoredOnPlayerID = g_goalPlayerId[TEAM_2-1];
    }
    
    if (updatePlayerLives(scoredOnPlayerID)) {
        // scored on player lost all their lives
        updateLeftPlayerStars(lastTouchPlayerID);
    }
    
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
        calculateScore(ball, lastTouchPlayerID);  
    }
    else {
        // todo: assign score if nobody on other side touched the ball
        if (g_Game.numTeams == 4) {
            calculateScore(ball, g_goalPlayerId[TEAM_2-1]);
            calculateScore(ball, g_goalPlayerId[TEAM_4-1]);
        }
        else {
            calculateScore(ball, g_goalPlayerId[TEAM_2-1]);
        }
    }
    
    if (g_Game.numTeams > 2 && ball->pos.y > SCREEN_MIDDLE) {
        scoredOnPlayerID = g_goalPlayerId[TEAM_3-1];
    }
    else if (g_Game.numTeams > 2 && ball->pos.y < SCREEN_MIDDLE) {
        scoredOnPlayerID = g_goalPlayerId[TEAM_1-1];
    }
    else {
        scoredOnPlayerID = g_goalPlayerId[TEAM_1-1];
    }
    
    if (updatePlayerLives(scoredOnPlayerID)) {
        // scored on player lost all their lives
        updateRightPlayerStars(lastTouchPlayerID);
    }
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
