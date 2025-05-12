#pragma once
#include "player.h"
#include "../assets.h"

#define GOAL_X_POS 330
#define GOAL_X_BOUNDS toFIXED(GOAL_X_POS - 40) // goal width is 64px
#define GOAL_LEFT_BOUNDS -toFIXED(GOAL_X_POS - 40) // goal width is 64px
#define GOAL_RIGHT_BOUNDS toFIXED(GOAL_X_POS - 40) // goal width is 64px
#define GOAL_CENTER_POS 0
#define GOAL_MARGIN toFIXED(20)

typedef enum _GOAL_SCALE
{
    GOAL_SCALE_EASY = 20,
    GOAL_SCALE_MEDIUM = 20, //16
    GOAL_SCALE_HARD = 20, //13
    GOAL_SCALE_VS_MODE = 10,
} GOAL_SCALE;

typedef enum _GOAL_Y_POS
{
    GOAL_Y_POS_EASY = 140,
    GOAL_Y_POS_MEDIUM = 140, // 120
    GOAL_Y_POS_HARD = 140, // 100
    GOAL_Y_POS_MID = 0,
    GOAL_Y_POS_TOP_VS_MODE = 160,
    GOAL_Y_POS_MID_VS_MODE = 79,
    GOAL_Y_POS_BOT_VS_MODE = 2,
} GOAL_Y_POS;

typedef struct _GOAL_POSITION
{
    int x;
    int top;
    int bot;
    int mid;
    int top_flip;
    int bot_flip;
    int top_zmode;
    int mid_zmode;
    int bot_zmode;

} GOAL_POSITION, *PGOAL_POSITION;

typedef struct _GOAL_OBJECT
{
    bool isColliding;
    bool onLeftSide; // is this needed? (use player as proxy)
    int scale;
    GOAL_POSITION pos;
    Uint8 id;
    Sprite *sprite;
    PPLAYER player;
    bool drawSingleGoal;
} GOAL, *PGOAL;

extern GOAL g_Goal[MAX_PLAYERS];

void initGoalColors(void);
void initGoals(void);
void setGoalSize(void);

void drawGoals(void);

void checkRightGoalCollision(Sprite *ball);
void checkLeftGoalCollision(Sprite *ball);