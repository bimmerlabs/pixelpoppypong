#pragma once

#include "assets.h"
#include "objects/player.h"

#define MIN_VELOCITY_X toFIXED(7)
#define MIN_VELOCITY_Y toFIXED(3)
#define EASY_MAX_VELOCITY toFIXED(8.5) // 8 = easy, 10 = medium, 13 = hard?
#define MEDIUM_MAX_VELOCITY toFIXED(10) // 8 = easy, 10 = medium, 13 = hard?
#define HARD_MAX_VELOCITY toFIXED(13) // 8 = easy, 10 = medium, 13 = hard?
#define BALL_FRICTION_Y toFIXED(1.75)
#define BALL_FRICTION_X toFIXED(1.15)
#define BALL_ROTATION 1.7 // IDEA:  PICK UP A BOOMERANG ITEM - MAKE THIS NUMBER HIGHER?
#define REBOUND toFIXED(0.1) // the lower this is, the bigger the rebound?
#define FRICTION_COEFFICIENT toFIXED(2.5) // Adjust this value to alter the ball curve

typedef struct {
    bool onLeftSide;
    bool hasTouched;
    Uint16 touchCount;
    Uint8 teamChoice;
} BallTouchTracker;

extern BallTouchTracker touchedBy[MAX_PLAYERS];

extern Uint16 ballTtouchTimer;

void initTouchCounter(void);

void stopBallMovement(Sprite *ball);
void start_ball_movement(Sprite *ball);

void adjust_xy_velocity_based_on_spin(Sprite *ball);

// Function to calculate Z velocity based on X and Y velocity
// tried to convert to fixed - doesn't work - maybe it overflows?
static inline int calculate_z_velocity(FIXED vx, FIXED vy, bool horizontal_collision) {
    // Convert velocity from fixed-point to float
    // comment out to use fixed
    float fx = (float)(vx) / (1 << 16);
    float fy = (float)(vy) / (1 << 16);

    // Calculate magnitude of velocity vector (speed)
    float speed = jo_sqrtf((fx * fx) + (fy * fy));
    // FIXED speed = jo_fixed_sqrt((vx * vx) + (vy * vy));

    // Scale rotation velocity based on speed
    int z_velocity = (int)(speed * BALL_ROTATION); // Scale factor can be adjusted to match gameplay

    // Determine rotation direction based on the type of collision and vector
    if (horizontal_collision) {
        // Horizontal wall collision: reverse direction if Y velocity is positive
        z_velocity *= (vy > 0) ? 1 : -1;
    } else {
        // Vertical wall collision: reverse direction if X velocity is positive
        z_velocity *= (vx > 0) ? -1 : 1;
    }

    // Ensure z_velocity is at least 1 in magnitude
    if (z_velocity == 0) {
        z_velocity = 1;
    }
    // speed limit z_velocity
    else if (z_velocity > 10) {
        z_velocity = 1;
    }

    return z_velocity;
}

static inline void updateBallTouch(PPLAYER player) {
    // Reset hasTouched for all players
    for(unsigned int i = 0; i <= g_Game.numPlayers; i++) {
        touchedBy[i].hasTouched = false;
    }

    // Update the player who last touched the ball
    touchedBy[player->playerID].hasTouched = true;
    touchedBy[player->playerID].touchCount++;
    touchedBy[player->playerID].teamChoice = player->teamChoice; // needs to be initialized, not here
    ballTtouchTimer = 0;
}

static inline FIXED my_fixed_clamp(FIXED value, FIXED min, FIXED max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

// Function to update the ball's position and check for collisions
void update_ball(Sprite *ball);

// // Function to detect and handle ball-player collision
bool detect_player_ball_collision(Sprite *ball, PPLAYER player);

void handle_ball_player_reaction(Sprite *ball, PPLAYER player);