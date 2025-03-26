#include <jo/jo.h>
#include "physics.h"
#include "audio.h"
#include "math.h"
#include "gameplay.h"
#include "highscores.h"
#include "screen_transition.h"

FIXED impulse = toFIXED(0);

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
    // maybe I can slow down the rotation here?
    // if (ball->vel.z > 8) {
        // ball->vel.z--;
    // }
    // else if (ball->vel.z < -8) {
        // ball->vel.z++;
    // }
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
    if (ball->pos.x >= SCREEN_RIGHT - ball->pos.r) {
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
    else if (ball->pos.x <= SCREEN_LEFT + ball->pos.r) {
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
    if (ball->pos.y >= SCREEN_BOTTOM - ball->pos.r || ball->pos.y <= SCREEN_TOP + ball->pos.r) {
        ball->vel.y = -ball->vel.y; // Reverse Y velocity
        // moving down
        if (ball->vel.y > MIN_VELOCITY_Y) {
            ball->vel.y -= BALL_FRICTION_Y;
            if (ball->vel.y < MIN_VELOCITY_Y)
                ball->vel.y = MIN_VELOCITY_Y;
        }
        // moving up
        else if (ball->vel.y < -MIN_VELOCITY_Y) {
            ball->vel.y += BALL_FRICTION_Y;
            if (ball->vel.y > MIN_VELOCITY_Y)
                ball->vel.y = -MIN_VELOCITY_Y;
        }
        // // moving right
        // if (ball->vel.x > MIN_VELOCITY_X) {
            // ball->vel.x -= BALL_FRICTION_X;
        // }
        // // moving left
        // else if (ball->vel.x < -MIN_VELOCITY_X) {
            // ball->vel.x += BALL_FRICTION_X;
        // }

        // Adjust Z velocity for vertical collision
        ball->vel.z = calculate_z_velocity(ball->vel.x, ball->vel.y, false);
    }
    
    // control ball speed
    if (ball->vel.x > MAX_VELOCITY) {
        ball->vel.x = MAX_VELOCITY;
    }
    else if (ball->vel.x < -MAX_VELOCITY) {
        ball->vel.x = -MAX_VELOCITY;
    }
    if (ball->vel.y > MAX_VELOCITY) {
        ball->vel.y = MAX_VELOCITY;
    }
    else if (ball->vel.y < -MAX_VELOCITY) {
        ball->vel.y = -MAX_VELOCITY;
    }
    
    adjust_xy_velocity_based_on_spin(ball);
    
    // BOUND WITHIN SCREEN
    if(ball->pos.x > SCREEN_RIGHT - ball->pos.r)
    {
        #ifndef JO_COMPILE_WITH_AUDIO_SUPPORT
        pcm_parameter_change(g_Assets.bumpPcm16, 6, PCM_PAN_RIGHT);
        pcm_play(g_Assets.bumpPcm16, PCM_VOLATILE, 6);
        #else
        jo_audio_play_sound_on_channel(&g_Assets.bumpPcm16, 6);
        #endif
        ball->pos.x = SCREEN_RIGHT - ball->pos.r;
    }

    if(ball->pos.x < SCREEN_LEFT + ball->pos.r)
    {
        #ifndef JO_COMPILE_WITH_AUDIO_SUPPORT
        pcm_parameter_change(g_Assets.bumpPcm16, 6, PCM_PAN_LEFT);
        pcm_play(g_Assets.bumpPcm16, PCM_VOLATILE, 6);
        #else
        jo_audio_play_sound_on_channel(&g_Assets.bumpPcm16, 6);
        #endif
        ball->pos.x = SCREEN_LEFT + ball->pos.r;
    }

    if(ball->pos.y > SCREEN_BOTTOM - ball->pos.r)
    {
        #ifndef JO_COMPILE_WITH_AUDIO_SUPPORT
        pcm_play(g_Assets.bumpPcm16, PCM_VOLATILE, 6);
        #else
        jo_audio_play_sound_on_channel(&g_Assets.bumpPcm16, 5);
        #endif
        ball->pos.y = SCREEN_BOTTOM - ball->pos.r;
    }

    if(ball->pos.y < SCREEN_TOP + ball->pos.r)
    {
        #ifndef JO_COMPILE_WITH_AUDIO_SUPPORT
        pcm_play(g_Assets.bumpPcm16, PCM_VOLATILE, 6);
        #else
        jo_audio_play_sound_on_channel(&g_Assets.bumpPcm16, 5);
        #endif
        ball->pos.y = SCREEN_TOP + ball->pos.r;
    }
    
    // Keep rotation within 0-359 degrees
    if (ball->rot.z == 360) {
        ball->rot.z = 0;
    }
    
    // update_bounding_box(&pixel_poppy);
}

// inline?
void handle_elastic_collision(Sprite *ball, PPLAYER player, FIXED dx, FIXED dy, FIXED distance_squared) {
    // placeholder values (will be set by difficulty/character selection)
    ball->mass = toFIXED(0.001);
    player->_sprite->mass = toFIXED(0.1); // I don't think I need this?
    
    // Collision normal (unit vector)
    FIXED collision_normal_x = jo_fixed_div(dx, jo_fixed_sqrt(distance_squared));
    FIXED collision_normal_y = jo_fixed_div(dy, jo_fixed_sqrt(distance_squared));
    
    FIXED vel_squared = jo_fixed_mult(ball->vel.x, ball->vel.x) + jo_fixed_mult(ball->vel.y, ball->vel.y);
    FIXED velocity = jo_fixed_sqrt(vel_squared);
    
    impulse = jo_fixed_mult(ball->mass, (jo_fixed_mult(velocity, REBOUND) - velocity));
    FIXED momentumChange = jo_fixed_mult(jo_fixed_mult(impulse, player->_sprite->mass), player->power);
    // Update ball's velocity
    ball->vel.x -= jo_fixed_mult(momentumChange, collision_normal_x);
    ball->vel.y -= jo_fixed_mult(momentumChange, collision_normal_y);


    // control ball speed
    if (ball->vel.x > MAX_VELOCITY) {
        ball->vel.x = MAX_VELOCITY;
    }
    else if (ball->vel.x < -MAX_VELOCITY) {
        ball->vel.x = -MAX_VELOCITY;
    }
    if (ball->vel.y > MAX_VELOCITY) {
        ball->vel.y = MAX_VELOCITY;
    }
    else if (ball->vel.y < -MAX_VELOCITY) {
        ball->vel.y = -MAX_VELOCITY;
    }

    // Update player's velocity
    player->_sprite->vel.x += jo_fixed_mult(jo_fixed_mult((impulse), ball->mass), collision_normal_x);
    player->_sprite->vel.y += jo_fixed_mult(jo_fixed_mult((impulse), ball->mass), collision_normal_y);
}

bool detect_player_ball_collision(Sprite *ball, PPLAYER player) {
    // placeholders (will be set by difficulty/character selection)
    player->_sprite->pos.r = PLAYER_RADIUS;
    // ball->pos.r = BALL_RADIUS;
    
    // don't collide with back side of player
    if (player->onLeftSide && ball->pos.x < player->_sprite->pos.x) {
        ball->isColliding = false;
        player->_sprite->isColliding = false;
        return false;
    }
    if (!player->onLeftSide && ball->pos.x > player->_sprite->pos.x) {
        ball->isColliding = false;
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
    FIXED dot_product = (dvx * dx) + (dvy * dy); // use to tell if object is moving toward or away
    
    // if (game_options.debug_display) {
        // jo_nbg0_printf(2, 16, "DOT_PRODUCT:%i", dot_product);
        // jo_nbg0_printf(2, 17, "DX:%i", dx);
        // jo_nbg0_printf(2, 18, "DY:%i", dy);
        // jo_nbg0_printf(2, 20, "IMPULSE:%i", toINT(impulse));
        // // jo_nbg0_printf(20, 16, "D_SQUARED:%i", distance_squared);
        // jo_nbg0_printf(20, 17, "DVX:%i", dvx);
        // jo_nbg0_printf(20, 18, "DVY:%i", dvy);
    // }
    
    // If the dot product is positive, the ball is moving away
    if (dot_product > toFIXED(0)) {
        ball->isColliding = false;
        player->_sprite->isColliding = false;
        return false; // No need to check for collision
    }

    int distance_squared = (toINT(dx) * toINT(dx)) + (toINT(dy) * toINT(dy));
    // Calculate the squared sum of the radii (FIXED)
    int radius_sum = toINT(ball->pos.r) + toINT(player->_sprite->pos.r);
    int radius_squared = radius_sum * radius_sum; // for ball/player, this could be a fixed value
    
    // Check if the circles overlap
    if (distance_squared <= radius_squared) {
        
        if (distance_squared == 0) {
            // Prevent division by zero
            distance_squared = toFIXED(0.001);
        }

        handle_ball_player_collision(ball, player);
        // handle_collision(ball, player, toFIXED(dx), toFIXED(dy), toFIXED(distance_squared));
       
        ball->isColliding = true;
        player->_sprite->isColliding = true;
        updateBallTouch(player);
        return true;
    }
    else {
        ball->isColliding = false;
        player->_sprite->isColliding = false;
        return false;
    }
}


// SIMPLER / BETTER?
void handle_ball_player_collision(Sprite *ball, PPLAYER player) {
    // Calculate relative position vector
    FIXED dx = ball->pos.x - player->_sprite->pos.x;
    FIXED dy = ball->pos.y - player->_sprite->pos.y;

    // Calculate squared distance to avoid sqrt unless necessary
    FIXED distance_squared = jo_fixed_mult(dx, dx) + jo_fixed_mult(dy, dy);

    if (distance_squared == 0) {
        // Prevent division by zero (objects are on top of each other)
        return;
    }

    // Compute collision normal (unit vector)
    FIXED collision_normal_x = jo_fixed_div(dx, jo_fixed_sqrt(distance_squared));
    FIXED collision_normal_y = jo_fixed_div(dy, jo_fixed_sqrt(distance_squared));

    // Compute the dot product of relative velocity and collision normal
    FIXED dot_product = jo_fixed_mult(ball->vel.x, collision_normal_x) +
                        jo_fixed_mult(ball->vel.y, collision_normal_y);

    if (dot_product > 0) { // remove?
        // Ball is moving away from the player; no need to adjust velocity
        return;
    }

    // Reflect the ball's velocity along the collision normal, factoring in player's movement
    ball->vel.x -= jo_fixed_mult(jo_fixed_mult(dot_product, collision_normal_x), player->power);
    ball->vel.y -= jo_fixed_mult(dot_product, collision_normal_y) * 2;
    
    // Compute relative velocity (ball velocity - player velocity)
    FIXED rel_vel_x = jo_fixed_mult(player->_sprite->vel.x, toFIXED(0.35));
    FIXED rel_vel_y = jo_fixed_mult(player->_sprite->vel.y, toFIXED(0.05));

    // Apply additional influence from player's velocity
    switch (player->onLeftSide) {
        case true:
            if (player->_sprite->vel.x > toFIXED(0)) {
                ball->vel.x += rel_vel_x;
            }
            break;
        default:
            if (player->_sprite->vel.x < toFIXED(0)) {
                ball->vel.x += rel_vel_x;
            }
            break;
    }
    // ball->vel.y += rel_vel_y;
}

// // Returns true if two circles intersect and are moving toward each other
// // uses the distance formula without using sqrt
// bool checkForCircleCollision_int(Sprite *a, Sprite *b)
// {
    // // Calculate the center-to-center vector
    // int dx = toINT(b->pos.x) - toINT(a->pos.x);
    // int dy = toINT(b->pos.y) - toINT(a->pos.y);

    // // Calculate the relative velocity vector
    // int vx_a = toINT(a->vel.x);
    // int vy_a = toINT(a->vel.y);
    // int vx_b = toINT(b->vel.x);
    // int vy_b = toINT(b->vel.y);

    // // Check the dot product of velocity and center-to-center vector
    // int dot_product = (vx_a * dx) + (vy_a * dy) + (vx_b * dx) + (vy_b * dy);
    
    // jo_nbg0_printf(2, 16, "DOT_PRODUCT:%i", dot_product);
    // jo_nbg0_printf(2, 17, "DX:%i", dx);
    // jo_nbg0_printf(2, 18, "DY:%i", dy);
    // jo_nbg0_printf(20, 17, "VX:%i", vx_a);
    // jo_nbg0_printf(20, 18, "VY:%i", vy_a);

    // // If the dot product is positive, the ball is moving away
    // if (dot_product > 0) {
        // return false; // No need to check for collision
    // }
    // // Calculate the distance squared between the centers of the two sprites
    // int xy_distance = (dx * dx) + (dy * dy);

    // // Calculate the squared sum of the radii
    // int r_distance = (toINT(a->pos.r) + toINT(b->pos.r)) * (toINT(a->pos.r) + toINT(b->pos.r));

    // // Check if the circles overlap
    // if (xy_distance <= r_distance)
    // {
        // return true;
    // }
    // else {
        // return false;
    // }
// }

// bool checkForCircleCollision_fixed(Sprite *a, Sprite *b)
// {
    // // Calculate the center-to-center vector (FIXED)
    // FIXED dx = b->pos.x - a->pos.x;
    // FIXED dy = b->pos.y - a->pos.y;

    // // Calculate the relative velocity vector (FIXED)
    // FIXED vx_a = a->vel.x;
    // FIXED vy_a = a->vel.y;
    // FIXED vx_b = b->vel.x;
    // FIXED vy_b = b->vel.y;

    // // Check the dot product of velocity and center-to-center vector (FIXED)
    // FIXED dot_product = jo_fixed_mult(vx_a, dx) + jo_fixed_mult(vy_a, dy) + jo_fixed_mult(vx_b, dx) + jo_fixed_mult(vy_b, dy);
    
    // jo_nbg0_printf(2, 16, "DOT_PRODUCT:%d", dot_product);
    // jo_nbg0_printf(2, 17, "DX:%i", dx);
    // jo_nbg0_printf(2, 18, "DY:%i", dy);
    // jo_nbg0_printf(20, 17, "VX:%i", vx_a);
    // jo_nbg0_printf(20, 18, "VY:%i", vy_a);
    
    // // If the dot product is positive, the ball is moving away
    // if (dot_product > 0) {
        // return false; // No need to check for collision
    // }
    // // Calculate the distance squared between the centers (FIXED)
    // FIXED xy_distance = jo_fixed_mult(dx, dx) + jo_fixed_mult(dy, dy);

    // // Calculate the squared sum of the radii (FIXED)
    // FIXED radius_sum = a->pos.r + b->pos.r;
    // FIXED r_distance = jo_fixed_mult(radius_sum, radius_sum);

    // // Check if the circles overlap
    // if (xy_distance <= r_distance) {
        // return true;
    // } else {
        // return false;
    // }
// }

// // uses the distance formula without using sqrt
// bool checkForCircleCollision(Sprite *a, Sprite *b)
// {
    // int xy_distance = 0;
    // int r_distance = 0;

    // xy_distance = ((toINT(b->pos.x) - toINT(a->pos.x)) * (toINT(b->pos.x) - toINT(a->pos.x))) + 
                // ((toINT(b->pos.y) - toINT(a->pos.y)) * (toINT(b->pos.y) - toINT(a->pos.y)));
    // r_distance = (toINT(a->pos.r) + toINT(b->pos.r)) * (toINT(a->pos.r) + toINT(b->pos.r));

    // if(r_distance > xy_distance)
    // {
        // return true;
    // }
    // else {
        // return false;
    // }
// }

SpriteEntry sprite_list[MAX_SPRITES];
int sprite_id = 0;

// Add a sprite to the sweep and prune list
void add_sprite_to_sweep_and_prune(Sprite *sprite) {
    if (sprite_id >= MAX_SPRITES) return; // Limit reached
    sprite_list[sprite_id].sprite = sprite;
    sprite_list[sprite_id].active = true;
    sprite->isColliding = false;
    sprite_id++;
}

// Remove a sprite from the sweep and prune list
void remove_sprite_from_sweep_and_prune(Sprite *sprite) {
    for (int i = 0; i < sprite_id; i++) {
        if (sprite_list[i].sprite == sprite) {
        sprite_list[sprite_id].active = false;
            // sprite_list[i] = sprite_list[sprite_id - 1]; // Replace with last entry
            sprite_id--;
            return;
        }
    }
}

// Sort sprites by the min_x value of their bounding box
void sort_sprites_by_min_x() {
    for (int i = 0; i < sprite_id - 1; i++) {
        for (int j = i + 1; j < sprite_id; j++) {
            if (sprite_list[i].sprite->bbox.min_x > sprite_list[j].sprite->bbox.min_x) {
                SpriteEntry temp = sprite_list[i];
                sprite_list[i] = sprite_list[j];
                sprite_list[j] = temp;
            }
        }
    }
}

// Perform sweep and prune collision detection
void sweep_and_prune(void) {
    // Sort the sprites along the x-axis
    sort_sprites_by_min_x();

    // Check for overlaps
    for (int i = 0; i < sprite_id; i++) {
        Sprite *a = sprite_list[i].sprite;
        if (!sprite_list[i].active) continue;

        for (int j = i + 1; j < sprite_id; j++) {
            Sprite *b = sprite_list[j].sprite;
            if (!sprite_list[j].active) continue;

            // Stop checking if the next sprite's min_x is beyond the current sprite's max_x
            if (b->bbox.min_x > a->bbox.max_x) break;

            // // Check if the bounding boxes of `a` and `b` overlap
            // if (a->bbox.min_y < b->bbox.max_y &&
                // a->bbox.max_y > b->bbox.min_y &&
                // a->bbox.min_x < b->bbox.max_x &&
                // a->bbox.max_x > b->bbox.min_x) {
                // // Handle collision between `a` and `b`
                // handle_collision(a, b);
            // }
            // else {
                // a->isColliding = false;
                // b->isColliding = false;
            // }
        }
    }
}

// // Example collision handler function
// void handle_collision(Sprite *a, Sprite *b) {
    // a->isColliding = true;
    // b->isColliding = true;
    
    // // handle_elastic_collision(ball, player, toFIXED(dx), toFIXED(dy), toFIXED(distance_squared));
// }

// // Update bounding boxes whenever an object's position changes
// void update_bounding_box(Sprite *sprite) {
    // sprite->bbox.min_x = sprite->pos.x - sprite->bbox.width;
    // sprite->bbox.min_y = sprite->pos.y - sprite->bbox.height;
    // sprite->bbox.max_x = sprite->pos.x + sprite->bbox.width;
    // sprite->bbox.max_y = sprite->pos.y + sprite->bbox.height;
// }