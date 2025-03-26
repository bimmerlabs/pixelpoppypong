#pragma once

#include "assets.h"
#include "objects/player.h"

#define MIN_VELOCITY_X toFIXED(7)
#define MIN_VELOCITY_Y toFIXED(5)
#define MAX_VELOCITY toFIXED(13) // 8 = easy, 10 = medium, 13 = hard?
#define BALL_FRICTION_Y toFIXED(1.75)
#define BALL_FRICTION_X toFIXED(1.25)
#define BALL_ROTATION 1.7 // IDEA:  PICK UP A BOOMERANG ITEM - MAKE THIS NUMBER HIGHER?
#define REBOUND toFIXED(0.1) // the lower this is, the bigger the rebound?


// NOTE: based on float!!!
// 0.05 - easy?
// 0.075 - medium?
// 0.10 - hard?
// NOTE: based on fixed!!!
// 0.075
// 0.2 - too strong
// 0.3 - causes the ball to spin backwards!
#define FRICTION_COEFFICIENT toFIXED(2.5) // Adjust this value to alter the ball curve

typedef struct {
    // int playerID;
    bool onLeftSide;
    bool hasTouched;
    Uint16 touchCount;
    Uint8 teamChoice;
} BallTouchTracker;

extern BallTouchTracker touchedBy[MAX_PLAYERS];

typedef struct {
    Sprite *sprite;
    bool active;
} SpriteEntry;

#define MAX_SPRITES 128

// Array of SpriteEntries to store all sprites for sweep and prune
extern SpriteEntry sprite_list[MAX_SPRITES];
extern int sprite_id;

void initTouchCounter(void);

// Function to initialize the ball's movement
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
    // touchedBy[player->PlayerID].PlayerID = player->PlayerID;
    touchedBy[player->playerID].hasTouched = true;
    touchedBy[player->playerID].touchCount++;
    touchedBy[player->playerID].teamChoice = player->teamChoice; // needs to be initialized, not here
}

// Function to update the ball's position and check for collisions
void update_ball(Sprite *ball);

void handle_elastic_collision(Sprite *ball, PPLAYER player, FIXED dx, FIXED dy, FIXED distance_squared);

void handle_ball_player_collision(Sprite *ball, PPLAYER player);

// // Function to detect and handle ball-player collision
bool detect_player_ball_collision(Sprite *ball, PPLAYER player);

// // COLLISION DETECTION
// bool checkForCircleCollision_int(Sprite *a, Sprite *b);
// bool checkForCircleCollision_fixed(Sprite *a, Sprite *b);

void add_sprite_to_sweep_and_prune(Sprite *sprite);

// Remove a sprite from the sweep and prune list
void remove_sprite_from_sweep_and_prune(Sprite *sprite);

// Sort sprites by the min_x value of their bounding box
void sort_sprites_by_min_x(void);

// Perform sweep and prune collision detection
void sweep_and_prune(void);

// // Example collision handler function
// void handle_collision(Sprite *a, Sprite *b);

// // Update bounding boxes whenever an object's position changes
// void update_bounding_box(Sprite *sprite);