#pragma once

#include "physics.h"
#include "gameplay.h"
#include "physics.h"
#include "audio.h"
#include "objects/player.h"
#include "objects/goal.h"
#include "objects/teams.h"

#define SCORE_DISPLAY_TIME (16 * 60)

#define INITIALS_LENGTH 3  // Three-letter initials like "ABC"
#define SCORE_ENTRIES 10
#define UNLOCK_CRAIG_SCORE 2500000
#define UNLOCK_GARFIELD_SCORE 5000000
#define GOAL_MARGIN toFIXED(20)

extern unsigned int g_ScoreTimer;

typedef struct {
    unsigned int score;
    char initials[INITIALS_LENGTH + 1];  // +1 for null terminator
} HighScoreEntry;

extern HighScoreEntry highScores[SCORE_ENTRIES];

void highScore_init(void);
void init_scores(void);
void display_scores(void);
void score_input(void);

void sortHighScores(HighScoreEntry scores[]);
void addHighScore(unsigned int newScore, const char *initials);
void update_bg_position(void);
bool updatePlayerLives(Uint8 scoredOnPlayerID);

extern PLAYER g_Players[MAX_PLAYERS];

static __jo_force_inline void calculateScore(Sprite *ball, Uint8 playerID) {
    unsigned int points = (JO_ABS(toINT(ball->vel.x)) * 50) 
                         + (JO_ABS(toINT(ball->vel.y)) * 500) 
                         + (JO_ABS(toINT(ball->vel.z)) * 1000);
    g_Players[playerID].score.points += points * touchedBy[playerID].touchCount;
    g_Game.isGoalScored = true;
    ballTtouchTimer = 0;
}

static __jo_force_inline void updatePlayerStars(int lastTouchPlayerID)
{
    g_Players[lastTouchPlayerID].score.stars++;       
}

// 
static __jo_force_inline bool updateScore(Sprite *ball, int playerID) {      
    if (g_Players[playerID].subState == PLAYER_STATE_DEAD) {
        return false;
    }
    
    int lastTouchPlayerID = -1;
    // Find the last player who touched the ball
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (touchedBy[i].hasTouched) {
            lastTouchPlayerID = i;
            break;
        }
    }
    // Assign score if a valid player touched it
    if (lastTouchPlayerID != -1) {
        calculateScore(ball, lastTouchPlayerID);        
    }
    if (updatePlayerLives(playerID) && lastTouchPlayerID != -1) {
        // scored on player lost all their lives
        updatePlayerStars(lastTouchPlayerID);
    }
    return true;
}

// TODO: move to goals.c (change name to "checkgoalcollision" or something)
static __jo_force_inline void checkRightWallScore(Sprite *ball) {
    // iterate through goals on right side, check bounds
    PGOAL _goal;
    for(unsigned int i = 0; i < MAX_PLAYERS; i++)
    {
        _goal = &g_Goal[i];
        if (_goal->player->objectState == OBJECT_STATE_INACTIVE || _goal->player->subState == PLAYER_STATE_DEAD || _goal->onLeftSide) {
            continue;
        }
        if (ball->pos.y - GOAL_MARGIN > toFIXED(_goal->pos.top) && ball->pos.y + GOAL_MARGIN < toFIXED(_goal->pos.bot)) { // removed ball radius - need another solution
            if (g_Game.gameMode == GAME_MODE_STORY) {
                calculateScore(ball, 0);
                updatePlayerLives(1);
                playCDTrack(g_Audio.goalScoredTrack, false);
                nextGoalScoredTrack();
                break;
            }
            else if (updateScore(ball, _goal->id)) { // scored on TEAM_2 or TEAM_4
                playCDTrack(g_Audio.goalScoredTrack, false);
                nextGoalScoredTrack();
                break;
            }
        }
    }
}

static __jo_force_inline void checkLeftWallScore(Sprite *ball) {
    // iterate through goals on left side, check bounds
    PGOAL _goal;
    for(unsigned int i = 0; i < MAX_PLAYERS; i++)
    {
        _goal = &g_Goal[i];
        if (_goal->player->objectState == OBJECT_STATE_INACTIVE || _goal->player->subState == PLAYER_STATE_DEAD || !_goal->onLeftSide) {
            continue;
        }
        if (ball->pos.y - GOAL_MARGIN > toFIXED(_goal->pos.top) && ball->pos.y + GOAL_MARGIN < toFIXED(_goal->pos.bot)) { // removed ball radius - need another solution
            if (g_Game.gameMode == GAME_MODE_STORY) {
                g_Game.isGoalScored = true;
                ballTtouchTimer = 0;
                updatePlayerLives(0);
                playCDTrack(g_Audio.goalScoredTrack, false);
                nextGoalScoredTrack();
                break;
            }
            else if (updateScore(ball, _goal->id)) { // scored on TEAM_2 or TEAM_4
                playCDTrack(g_Audio.goalScoredTrack, false);
                nextGoalScoredTrack();
                break;
            }
        }
    }
}
