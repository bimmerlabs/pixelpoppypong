#pragma once

#include "assets.h"
#include "objects/player.h"

#define MIN_VELOCITY_X toFIXED(7)
#define MIN_VELOCITY_Y toFIXED(3)
#define MAX_VELOCITY_Z 70
#define EASY_MAX_VELOCITY toFIXED(8) // 8 = easy, 10 = medium, 13 = hard?
#define MEDIUM_MAX_VELOCITY toFIXED(10.5) // 8 = easy, 10 = medium, 13 = hard?
#define HARD_MAX_VELOCITY toFIXED(13) // 8 = easy, 10 = medium, 13 = hard?
#define BALL_FRICTION_Y toFIXED(1.75)
#define BALL_FRICTION_X toFIXED(1.15)
#define BALL_ROTATION 1.7 // IDEA:  PICK UP A BOOMERANG ITEM - MAKE THIS NUMBER HIGHER?
#define REBOUND toFIXED(0.1) // the lower this is, the bigger the rebound?
#define FRICTION_COEFFICIENT toFIXED(2.5) // Adjust this value to alter the ball curve
#define ITEM_RADIUS 32 // Adjust this value to alter the ball curve

typedef struct {
    bool onLeftSide;
    bool hasTouched;
    int touchCount;
    Uint8 teamChoice;
} BallTouchTracker;

extern BallTouchTracker touchedBy[MAX_PLAYERS];
extern Sint8 lastTouchedBy;
extern Sint8 previouslyTouchedBy[3]; // use as a buffer
extern unsigned int ballTtouchTimer;

void initTouchCounter(Uint8 resetTouchCount);
void stopBallMovement(Sprite *ball);
void start_ball_movement(Sprite *ball);

// distance formula without the square root
bool checkItemDistance(Sprite *player, Sprite *item);

// Function to calculate Z velocity based on X and Y velocity
// tried to convert to fixed - doesn't work - maybe it overflows?
static inline int calculate_z_velocity(FIXED vx, FIXED vy, bool horizontal_collision) {
    float fx = (float)(vx) / (1 << 16);
    float fy = (float)(vy) / (1 << 16);

    // Calculate magnitude of velocity vector (speed)
    float speed = jo_sqrtf((fx * fx) + (fy * fy));

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
    else if (z_velocity > MAX_VELOCITY_Z) {
        z_velocity = MAX_VELOCITY_Z;
    }
    // speed limit z_velocity
    else if (z_velocity < -MAX_VELOCITY_Z) {
        z_velocity = -MAX_VELOCITY_Z;
    }

    return z_velocity;
}

static __jo_force_inline void updateBallTouch(PPLAYER player) {
    // Reset hasTouched for all players
    for(unsigned int i = 0; i <= g_Game.numPlayers; i++) {
        touchedBy[i].hasTouched = false;
    }

    // Update the player who last touched the ball
    touchedBy[player->playerID].hasTouched = true;
    if (lastTouchedBy != player->playerID) {
        previouslyTouchedBy[2] = previouslyTouchedBy[1];
        previouslyTouchedBy[1] = previouslyTouchedBy[0];
        previouslyTouchedBy[0] = lastTouchedBy;
    }
    lastTouchedBy = player->playerID;
    if (player->_sprite->isColliding == false && touchedBy[player->playerID].touchCount < 99) {
        touchedBy[player->playerID].touchCount++;
        switch (touchedBy[player->playerID].touchCount) {
            case 20:
                pcm_play(g_Assets.chain0Pcm8, PCM_PROTECTED, 7);
                break;
            case 40:
                pcm_play(g_Assets.chain1Pcm8, PCM_PROTECTED, 7);
                break;
            case 60:
                pcm_play(g_Assets.chain2Pcm8, PCM_PROTECTED, 7);
                break;
            case 80:
                pcm_play(g_Assets.chain3Pcm8, PCM_PROTECTED, 7);
                break;
            case 99:
                pcm_play(g_Assets.chain5Pcm8, PCM_PROTECTED, 7);
                break;
            default:
                break;
        }
    }
    touchedBy[player->playerID].teamChoice = player->teamChoice; // not really needed anymore
    ballTtouchTimer = 0;
}

// Function to update the ball's position and check for collisions
void update_ball(Sprite *ball);

// // Function to detect and handle ball-player collision
bool detect_player_ball_collision(Sprite *ball, PPLAYER player);
