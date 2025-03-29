#include <jo/jo.h>
#include "physics.h"
#include "audio.h"
#include "math.h"
#include "gameplay.h"
#include "highscores.h"
#include "screen_transition.h"

extern PLAYER g_Players[MAX_PLAYERS];

BallTouchTracker touchedBy[MAX_PLAYERS];

void initTouchCounter(void) {
    for(unsigned int i = 0; i < MAX_PLAYERS; i++)
    {
        touchedBy[i].onLeftSide = false;
        touchedBy[i].hasTouched = false;
        touchedBy[i].touchCount = 0;
        touchedBy[i].teamChoice = 0;
    }
}

// Function to initialize the ball's movement
void start_ball_movement(Sprite *ball) {    
    // this works as is because I can set a minimum in the range    
    // TODO: set based on difficulty / game mode
    ball->vel.x = toFIXED(my_random_range(5, 10));
    ball->vel.y = toFIXED(my_random_range(4, 9));
    ball->vel.z = my_random_range(1, 5);
           
    // Randomize direction
    if (JO_MOD_POW2(jo_random(999), 2)) ball->vel.x = -ball->vel.x; // modulus
    if (JO_MOD_POW2(jo_random(999), 2)) ball->vel.y = -ball->vel.y; // modulus
    if (JO_MOD_POW2(jo_random(999), 2)) ball->vel.z = -ball->vel.z; // modulus
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
        
    
    switch (ball->vel.z) {
        case 1:
            ball->spr_id = ball->anim1.asset[0];
            break;
        case 2:
            ball->spr_id = ball->anim1.asset[1];
            break;
        case 3:
            ball->spr_id = ball->anim1.asset[2];
            break;
        case 4:
            ball->spr_id = ball->anim1.asset[3];
            break;
        case 5:
            ball->spr_id = ball->anim1.asset[4];
            break;
        case 6:
            ball->spr_id = ball->anim1.asset[5];
            break;
        default:
            ball->spr_id = ball->anim1.asset[0];
            break;
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
        
        // switch for number of teams
        checkLeftWallScore(ball);
        
    }
    else if (ball->pos.x <= SCREEN_LEFT) {
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
        
        checkRightWallScore(ball);
        
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
        
    // physics init?
    FIXED max_velocity;
    switch(g_Game.gameDifficulty)
    {
        case GAME_DIFFICULTY_EASY:
            max_velocity = EASY_MAX_VELOCITY;
            break;
        case GAME_DIFFICULTY_MEDIUM:
            max_velocity = MEDIUM_MAX_VELOCITY;
            break;
        case GAME_DIFFICULTY_HARD:
            max_velocity = HARD_MAX_VELOCITY;
            break;
        default:
            max_velocity = MEDIUM_MAX_VELOCITY;
            break;
    }
    // control ball speed
    if (ball->vel.x > max_velocity) {
        ball->vel.x = max_velocity;
    }
    else if (ball->vel.x < -max_velocity) {
        ball->vel.x = -max_velocity;
    }
    if (ball->vel.y > max_velocity) {
        ball->vel.y = max_velocity;
    }
    else if (ball->vel.y < -max_velocity) {
        ball->vel.y = -max_velocity;
    }
    
    adjust_xy_velocity_based_on_spin(ball);
    
    // Keep rotation within 0-359 degrees
    if (ball->rot.z == 360) {
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

    // Relative velocity vector
    FIXED dvx = ball->vel.x - player->_sprite->vel.x;
    FIXED dvy = ball->vel.y - player->_sprite->vel.y;

    // Calculate the dot product of the relative velocity and the collision normal
    FIXED dot_product = jo_fixed_mult(dvx, dx) + jo_fixed_mult(dvy, dy);
    
    // If the dot product is positive, the ball is moving away
    if (dot_product > toFIXED(0)) {
        player->_sprite->isColliding = false;
        return false; // No need to check for collision
    }

    // Radius of the player (used for the semicircle)
    int player_radius = toINT(player->_sprite->pos.r);
    
    // **Step 1: Check collision with the rectangle (depends on player side)**
    FIXED player_left, player_right, player_top, player_bottom;
    switch(g_Game.gameMode)
    {
        case GAME_MODE_CLASSIC:
            if (player->onLeftSide) {
                // Rectangle extends to the **left** of the semicircle
                player_left   = player->_sprite->pos.x - player_radius;
                player_right  = player->_sprite->pos.x;
            } else {
                // Rectangle extends to the **right** of the semicircle
                player_left   = player->_sprite->pos.x;
                player_right  = player->_sprite->pos.x + player_radius;
            }
            player_top    = player->_sprite->pos.y - 2*player_radius;
            player_bottom = player->_sprite->pos.y + 2*player_radius;
            break;
        default:
            if (player->onLeftSide) {
                // Rectangle extends to the **left** of the semicircle
                player_left   = player->_sprite->pos.x - player_radius;
                player_right  = player->_sprite->pos.x + player_radius;
            } else {
                // Rectangle extends to the **right** of the semicircle
                player_left   = player->_sprite->pos.x - player_radius;
                player_right  = player->_sprite->pos.x + player_radius;
            }
            player_top    = player->_sprite->pos.y - player_radius;
            player_bottom = player->_sprite->pos.y + player_radius;
            break;
    }


    // Check if the ball is inside the rectangle
    if (ball->pos.x >= player_left && ball->pos.x <= player_right &&
        ball->pos.y >= player_top && ball->pos.y <= player_bottom) {
        handle_ball_player_reaction(ball, player);
        updateBallTouch(player);
        return true;
    }    
    
    // **Step 2: Check collision with the semicircle**
    int ball_radius = toINT(ball->pos.r);
    int radius_sum = player_radius + ball_radius;
    int radius_squared = radius_sum * radius_sum;

    // Distance squared between ball and player center
    int distance_squared = (toINT(dx) * toINT(dx)) + (toINT(dy) * toINT(dy));

    if (distance_squared <= radius_squared) {
        handle_ball_player_reaction(ball, player);
        updateBallTouch(player);
        return true;
    }

    // No collision
    player->_sprite->isColliding = false;
    return false;
}

// SIMPLER / BETTER? (ball/circle)
void handle_ball_player_reaction(Sprite *ball, PPLAYER player) {
    // Calculate relative position vector
    FIXED dx = ball->pos.x - player->_sprite->pos.x;
    FIXED dy = ball->pos.y - player->_sprite->pos.y;

    // Calculate squared distance to avoid sqrt unless necessary
    FIXED distance_squared = jo_fixed_mult(dx, dx) + jo_fixed_mult(dy, dy);

    // Compute collision normal (unit vector)
    FIXED collision_normal_x = jo_fixed_div(dx, jo_fixed_sqrt(distance_squared));
    FIXED collision_normal_y = jo_fixed_div(dy, jo_fixed_sqrt(distance_squared));

    // Compute the dot product of relative velocity and collision normal
    FIXED dot_product = jo_fixed_mult(ball->vel.x, collision_normal_x) +
                        jo_fixed_mult(ball->vel.y, collision_normal_y);

    if (dot_product > toFIXED(0)) { // remove?
        // Ball is moving away from the player; no need to adjust velocity
        return;
    }

    player->_sprite->isColliding = true;
    
    // Reflect the ball's velocity along the collision normal, factoring in player's movement    
    switch(g_Game.gameMode)
    {
        case GAME_MODE_CLASSIC:
            if (player->onLeftSide) {
                ball->vel.x = jo_fixed_mult(jo_fixed_mult(-ball->vel.x, collision_normal_x), player->power);
            }
            else {
                ball->vel.x = jo_fixed_mult(jo_fixed_mult(ball->vel.x, collision_normal_x), player->power);
            }
            ball->vel.y = jo_fixed_mult(ball->vel.y, player->power);
            break;
        default:
            ball->vel.x -= jo_fixed_mult(jo_fixed_mult(dot_product, collision_normal_x), player->power);
            ball->vel.y -= jo_fixed_mult(jo_fixed_mult(dot_product, collision_normal_y), player->power);
            break;
    }
    
    // Apply player's movement influence
    FIXED rel_vel_x = jo_fixed_mult(player->_sprite->vel.x, toFIXED(0.35));
    if ((player->onLeftSide && player->_sprite->vel.x > 0) || 
        (!player->onLeftSide && player->_sprite->vel.x < 0)) {
        ball->vel.x += rel_vel_x;
    }
}