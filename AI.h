#ifndef AI_H
#define AI_H

#include "sprites.h"

#define FIELD_THRESHOLD toFIXED(5) // how far the ball is on the other side (makes AI move ealier / easier)
#define GOAL_THRESHOLD toFIXED(40) // how far the ball is on the other side (makes AI move ealier / easier)
#define MOVEMENT_FACTOR toFIXED(0.10) // 0.04 = easy, 0.5 = medium, 0.08 = hard? // might need to be based on the ball max velocity
#define SLOW_MOVEMENT_FACTOR toFIXED(0.001) // this is really the difficulty factor (higher = harder)

void playerAI(Sprite *ball);

#endif