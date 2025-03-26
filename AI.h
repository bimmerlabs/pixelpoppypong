#ifndef AI_H
#define AI_H

#include "sprites.h"
#include "objects/player.h"

#define FIELD_THRESHOLD toFIXED(5) // how far the ball is on the other side (makes AI move ealier / easier)
#define LARGE_GOAL_THRESHOLD_MAX toFIXED(20)
#define LARGE_GOAL_THRESHOLD_MIN toFIXED(10)
#define SMALL_GOAL_THRESHOLD_MAX toFIXED(10)
#define SMALL_GOAL_THRESHOLD_MIN toFIXED(5)
#define MEDIUM_MOVEMENT_FACTOR toFIXED(0.001) // this is really the difficulty factor (higher = harder)
#define SLOW_MOVEMENT_FACTOR toFIXED(0.0005) // this is really the difficulty factor (higher = harder)

#define AI_GOAL_CENTER toFIXED(80.0)

#define HARD_AI_MOVEMENT_SPEED   toFIXED(0.0020)
#define MEDIUM_AI_MOVEMENT_SPEED toFIXED(0.0012)
#define EASY_AI_MOVEMENT_SPEED   toFIXED(0.0004)

void playerAI(Sprite *ball);
void centerAiPlayer(PPLAYER player);
void boundAiPlayer(PPLAYER player);

#endif