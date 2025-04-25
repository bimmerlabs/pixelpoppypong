#include <jo/jo.h>
#include "physics.h"
#include "audio.h"
#include "math.h"
#include "gameplay.h"
#include "highscores.h"
#include "screen_transition.h"

extern PLAYER g_Players[MAX_PLAYERS];

BallTouchTracker touchedBy[MAX_PLAYERS];
unsigned int ballTtouchTimer = 0;

void initTouchCounter(Uint8 resetTouchCount) {
    for(unsigned int i = 0; i < MAX_PLAYERS; i++)
    {
        touchedBy[i].onLeftSide = false;
        touchedBy[i].hasTouched = false;
        if (resetTouchCount == 1) {
            touchedBy[i].touchCount = 0;
        }
        touchedBy[i].teamChoice = 0;
    }
    ballTtouchTimer = 0;
}

void stopBallMovement(Sprite *ball) {
    ball->vel.x = toFIXED(0);
    ball->vel.y = toFIXED(0);
}

// Function to initialize the ball's movement
void start_ball_movement(Sprite *ball) {    
    switch(g_Game.gameDifficulty)
    {
        case GAME_DIFFICULTY_EASY:
            g_Game.maxBallVelocity = EASY_MAX_VELOCITY;
            ball->vel.x = my_random_range(toFIXED(5), toFIXED(9));
            ball->vel.y = my_random_range(toFIXED(2), toFIXED(8));
            ball->vel.z = my_random_range(0, 5);
            break;
        case GAME_DIFFICULTY_MEDIUM:
            g_Game.maxBallVelocity = MEDIUM_MAX_VELOCITY;
            ball->vel.x = my_random_range(toFIXED(6), toFIXED(10));
            ball->vel.y = my_random_range(toFIXED(3), toFIXED(9));
            ball->vel.z = my_random_range(0, 10);
            break;
        case GAME_DIFFICULTY_HARD:
            g_Game.maxBallVelocity = HARD_MAX_VELOCITY;
            ball->vel.x = my_random_range(toFIXED(7), toFIXED(11));
            ball->vel.y = my_random_range(toFIXED(4), toFIXED(10));
            ball->vel.z = my_random_range(0, 20);
            break;
        default:
            g_Game.maxBallVelocity = MEDIUM_MAX_VELOCITY;
            ball->vel.x = my_random_range(toFIXED(6), toFIXED(10));
            ball->vel.y = my_random_range(toFIXED(3), toFIXED(9));
            ball->vel.z = my_random_range(0, 10);
            break;
    }
               
    // Randomize direction
    if (JO_MOD_POW2(jo_random(999 * ball->rot.z), 2)) ball->vel.x = -ball->vel.x; // modulus
    if (JO_MOD_POW2(jo_random(999 * ball->rot.z), 2)) ball->vel.y = -ball->vel.y; // modulus
    if (JO_MOD_POW2(jo_random(99999), 2)) ball->vel.z = -ball->vel.z; // modulus
    
    // game is only active if ball is moving
    g_Game.isActive = true;
    explode_ball = false;
}

// inline?
void adjust_xy_velocity_based_on_spin(Sprite *ball) {
    // Calculate tangential velocity
    FIXED tangential_velocity = jo_fixed_mult(ball->pos.r, ball->vel.z);

    // Scale tangential velocity by friction
    FIXED friction_effect = jo_fixed_mult(tangential_velocity, FRICTION_COEFFICIENT);

    // Adjust x and y velocities
    ball->vel.x += friction_effect; // Modify as per direction of spin
    ball->vel.y -= friction_effect; // Modify as per direction of spin
}

// Function to update the ball's position and check for collisions
void update_ball(Sprite *ball) {
       
    // Update position based on velocity
    ball->pos.x += ball->vel.x;
    ball->pos.y += ball->vel.y;
    ball->rot.z += ball->vel.z;
    
    if (!explode_ball) {
        Uint8 spin = ABS(ball->vel.z);
        if (spin > 50) {
            ball->spr_id = ball->anim1.asset[6];
        }
        else if (spin > 40 && spin <= 50) {
            ball->spr_id = ball->anim1.asset[5];
        }
        else if (spin > 30 && spin <= 40) {
            ball->spr_id = ball->anim1.asset[4];
        }
        else if (spin > 20 && spin <= 30) {
            ball->spr_id = ball->anim1.asset[3];
        }
        else if (spin > 10 && spin <= 20) {
            ball->spr_id = ball->anim1.asset[1];
        }
        else if (spin > 0 && spin <= 10) {
            ball->spr_id = ball->anim1.asset[0];
        }
    }
    
    // Check for collisions with goals
    if (ball->pos.x >= GOAL_X_BOUNDS) {
        checkRightWallScore(ball);
    }
    else if (ball->pos.x <= -GOAL_X_BOUNDS) {
        checkLeftWallScore(ball);
    }
    
    // Check for collisions with walls
    if (ball->pos.x >= SCREEN_RIGHT) {
        ball->pos.x = SCREEN_RIGHT;
        pcm_play(g_Assets.bumpPcm16, PCM_VOLATILE, 6);
        ball->vel.x = -ball->vel.x; // Reverse X velocity
        // moving right
        if (ball->vel.x > MIN_VELOCITY_X) {
            ball->vel.x -= BALL_FRICTION_X;
            if (ball->vel.x < MIN_VELOCITY_X)
                ball->vel.x = MIN_VELOCITY_X;
        }
        // moving left
        else if (ball->vel.x < -MIN_VELOCITY_X) {
            ball->vel.x += BALL_FRICTION_X;
            if (ball->vel.x > MIN_VELOCITY_X)
                ball->vel.x = -MIN_VELOCITY_X;
        }

        // Adjust Z velocity for horizontal collision
        ball->vel.z = calculate_z_velocity(ball->vel.x, ball->vel.y, true);        
    }
    if (ball->pos.x <= SCREEN_LEFT) {
        ball->pos.x = SCREEN_LEFT;
        pcm_play(g_Assets.bumpPcm16, PCM_VOLATILE, 6);
        ball->vel.x = -ball->vel.x; // Reverse X velocity
        // moving right
        if (ball->vel.x > MIN_VELOCITY_X) {
            ball->vel.x -= BALL_FRICTION_X;
            if (ball->vel.x < MIN_VELOCITY_X)
                ball->vel.x = MIN_VELOCITY_X;
        }
        // moving left
        else if (ball->vel.x < -MIN_VELOCITY_X) {
            ball->vel.x += BALL_FRICTION_X;
            if (ball->vel.x > MIN_VELOCITY_X)
                ball->vel.x = -MIN_VELOCITY_X;
        }

        // Adjust Z velocity for horizontal collision
        ball->vel.z = calculate_z_velocity(ball->vel.x, ball->vel.y, true);        
    }
    if (ball->pos.y >= SCREEN_BOTTOM - ball->pos.r) {
        ball->pos.y = SCREEN_BOTTOM - ball->pos.r;
        pcm_play(g_Assets.bumpPcm16, PCM_VOLATILE, 6);
        ball->vel.y = -ball->vel.y; // Reverse Y velocity
        // moving up
        if (ball->vel.y < -MIN_VELOCITY_Y) {
            ball->vel.y += BALL_FRICTION_Y;
            if (ball->vel.y > MIN_VELOCITY_Y)
                ball->vel.y = -MIN_VELOCITY_Y;
        }
        // Adjust Z velocity for vertical collision
        ball->vel.z = calculate_z_velocity(ball->vel.x, ball->vel.y, false);
    }
    if (ball->pos.y <= SCREEN_TOP + ball->pos.r) {
        ball->pos.y = SCREEN_TOP + ball->pos.r;
        pcm_play(g_Assets.bumpPcm16, PCM_VOLATILE, 6);
        ball->vel.y = -ball->vel.y; // Reverse Y velocity
        // moving down
        if (ball->vel.y > MIN_VELOCITY_Y) {
            ball->vel.y -= BALL_FRICTION_Y;
            if (ball->vel.y < MIN_VELOCITY_Y)
                ball->vel.y = MIN_VELOCITY_Y;
        }
        // Adjust Z velocity for vertical collision
        ball->vel.z = calculate_z_velocity(ball->vel.x, ball->vel.y, false);
    }

    // control ball speed
    if (ball->vel.x > g_Game.maxBallVelocity) {
        ball->vel.x = g_Game.maxBallVelocity;
    }
    else if (ball->vel.x < -g_Game.maxBallVelocity) {
        ball->vel.x = -g_Game.maxBallVelocity;
    }
    if (ball->vel.y > g_Game.maxBallVelocity) {
        ball->vel.y = g_Game.maxBallVelocity;
    }
    else if (ball->vel.y < -g_Game.maxBallVelocity) {
        ball->vel.y = -g_Game.maxBallVelocity;
    }
    
    adjust_xy_velocity_based_on_spin(ball);
    
    // Keep rotation within 0-359 degrees
    if (ABS(ball->rot.z) == 360) {
        ball->rot.z = 0;
    }
}

bool detect_player_ball_collision(Sprite *ball, PPLAYER player) {
    // don't check collision behind player
    if (player->onLeftSide && player->_sprite->pos.x > ball->pos.x) {
        player->_sprite->isColliding = false;
        return false;
    }
    if (!player->onLeftSide && player->_sprite->pos.x < ball->pos.x) {
        player->_sprite->isColliding = false;
        return false;
    }
    
    // Relative position vector
    FIXED dx = ball->pos.x - player->_sprite->pos.x;
    FIXED dy = ball->pos.y - player->_sprite->pos.y;
    // Distance squared between ball and player center
    int distance_squared = (toINT(dx) * toINT(dx)) + (toINT(dy) * toINT(dy));
    
    // Radius of the player (used for the semicircle)
    int player_radius = toINT(player->_sprite->pos.r);
    
    // **Step 1: Check collision with the rectangle (depends on player side)**
    FIXED player_left, player_right, player_top, player_bottom;
    if (player->onLeftSide) {
        // Rectangle extends to the **left** of the semicircle
        player_left   = player->_sprite->pos.x - player_radius;
        player_right  = player->_sprite->pos.x;
    } else {
        // Rectangle extends to the **right** of the semicircle
        player_left   = player->_sprite->pos.x;
        player_right  = player->_sprite->pos.x + player_radius;
    }
    player_top    = player->_sprite->pos.y - player_radius;
    player_bottom = player->_sprite->pos.y + player_radius;

    // Check if the ball is inside the rectangle
    if (ball->pos.x >= player_left && ball->pos.x <= player_right &&
        ball->pos.y >= player_top && ball->pos.y <= player_bottom) {
        if (!explode_ball) {
            updateBallTouch(player);
        }
        if (g_GameOptions.enableMeows && !player->_sprite->isColliding) {
            pcm_play(g_Assets.meowPcm8[g_Assets.meowID], PCM_PROTECTED, 7);
            g_Assets.meowID++;
            if (g_Assets.meowID > MEOW9)
                g_Assets.meowID = MEOW1;
        }
        handle_ball_player_reaction(ball, player, distance_squared, dx, dy);
        return true;
    }    
    
    // **Step 2: Check collision with the semicircle**
    int ball_radius = toINT(ball->pos.r);
    int radius_sum = player_radius + ball_radius;
    int radius_squared = radius_sum * radius_sum;

    if (distance_squared <= radius_squared) {
        if (!explode_ball) {
            updateBallTouch(player);
        }
        if (g_GameOptions.enableMeows && !player->_sprite->isColliding) {
            pcm_play(g_Assets.meowPcm8[g_Assets.meowID], PCM_PROTECTED, 7);
            g_Assets.meowID++;
            if (g_Assets.meowID > MEOW9)
                g_Assets.meowID = MEOW1;
        }
        handle_ball_player_reaction(ball, player, distance_squared, dx, dy);
        return true;
    }

    // No collision
    player->_sprite->isColliding = false;
    return false;
}

// SIMPLER / BETTER? (ball/circle)
void handle_ball_player_reaction(Sprite *ball, PPLAYER player, int distance_squared, FIXED dx, FIXED dy) {
    if (explode_ball && !player->isExploded) {
        player->isExploded = explodePLayer(player);
        player->_sprite->isColliding = false;
        return;
    }    
    // Calculate relative position vector    
    FIXED collision_normal_x = jo_fixed_div(dx, toFIXED(ApproximateIntegerSqrt(distance_squared)));
    FIXED collision_normal_y = jo_fixed_div(dy, toFIXED(ApproximateIntegerSqrt(distance_squared)));

    // Compute the dot product of relative velocity and collision normal
    FIXED dot_product = jo_fixed_mult(ball->vel.x, collision_normal_x) +
                        jo_fixed_mult(ball->vel.y, collision_normal_y);

    if (dot_product > toFIXED(0)) { // remove?
        // Ball is moving away from the player; no need to adjust velocity
        return;
    }

    player->_sprite->isColliding = true;
    
  if (!g_GameOptions.testCollision) {        
    // Reflect the ball's velocity along the collision normal, factoring in player's movement    
    ball->vel.x -= jo_fixed_mult(jo_fixed_mult(dot_product, collision_normal_x), player->power);
    ball->vel.y -= jo_fixed_mult(jo_fixed_mult(dot_product, collision_normal_y), player->power);
    
    // Apply player's movement influence
    FIXED rel_vel_x = jo_fixed_mult(player->_sprite->vel.x, toFIXED(0.35));
    if ((player->onLeftSide && player->_sprite->vel.x > 0) || 
        (!player->onLeftSide && player->_sprite->vel.x < 0)) {
        ball->vel.x += rel_vel_x;
    }
  }
}

// distance formula without the square root
bool checkItemDistance(Sprite *player, Sprite *item)
{
    if (!g_Game.isActive) {
        return false;
    }
    
    int total_radius = toINT(player->pos.r) + ITEM_RADIUS;
    int dist = total_radius * total_radius;
    
    int x_dist = toINT(player->pos.x) - toINT(item->pos.x);
    x_dist = x_dist * x_dist;

    int y_dist = toINT(player->pos.y) - toINT(item->pos.y);
    y_dist = y_dist * y_dist;

    if(dist > x_dist + y_dist)
    {
        return true;
    }
    return false;
}
