#include "AI.h"
#include "main.h"
#include "assets.h"
#include "physics.h"
#include "gameplay.h"
#include "objects/player.h"

extern PLAYER g_Players[MAX_PLAYERS];

void playerAI(Sprite *ball) {    
    
    for(unsigned int i = 0; i <  COUNTOF(g_Players); i++)
    {
        PPLAYER player = &g_Players[i];
        if(player->objectState != OBJECT_STATE_ACTIVE)
        {
            continue;
        }
        if (player->onLeftSide == true) {
            FIXED center_diff = toFIXED(0);
            // Check if the ball is moving towards player 2's goal
            if (ball->vel.x < toFIXED(0)) {

                // Check if the ball is close enough to start tracking
                if (ball->pos.x <= toFIXED(128)) {
                    // animate_paw1 = true;
                }
                if (ball->pos.x <= SCREEN_MIDDLE + FIELD_THRESHOLD) {
                    // animate_paw1 = true;
                    // Move the paw towards the ball's y position
                    FIXED y_diff = ball->pos.y - player->_sprite->pos.y;

                    // Gradual movement
                    player->curPos.dy += jo_fixed_mult(MOVEMENT_FACTOR, y_diff);
                    // player->curPos.dy += JO_ABS(ball->vel.y) * y_diff;
                }
            }
            else {
                // Ball is moving away, move back to the center
                center_diff = SCREEN_MIDDLE - player->_sprite->pos.y;
                if (center_diff > GOAL_THRESHOLD || center_diff < -GOAL_THRESHOLD) {
                    // Gradual movement
                    player->curPos.dy += jo_fixed_mult(SLOW_MOVEMENT_FACTOR, center_diff);
                    // animate_paw1 = false;
                }
                else {
                    player->curPos.dy = toFIXED(0);
                }
            }
            if (game_options.debug_display) {
                jo_nbg0_printf(2, 14, "CENTER_DIFF:%i", toINT(center_diff));
            }
        }
        else {
            // Check if the ball is moving towards player 2's goal
            if (ball->vel.x > toFIXED(0)) {

                // Check if the ball is close enough to start tracking
                if (ball->pos.x >= -toFIXED(128)) {
                    // animate_paw1 = true;
                }
                if (ball->pos.x >= SCREEN_MIDDLE - FIELD_THRESHOLD) {
                    // animate_paw1 = true;
                    // Move the paw towards the ball's y position
                    FIXED y_diff = ball->pos.y - player->_sprite->pos.y;

                    // Gradual movement
                    player->curPos.dy += jo_fixed_mult(MOVEMENT_FACTOR, y_diff);
                    // player->curPos.dy += JO_ABS(ball->vel.y) * y_diff;
                }
            } 
            // else if (ball->pos.y < SCREEN_MIDDLE -toFIXED(10) || ball->pos.y > SCREEN_MIDDLE + toFIXED(10)) {
            else {
                // Ball is moving away, move back to the center
                FIXED center_diff = SCREEN_MIDDLE - player->_sprite->pos.y;
                if (center_diff > GOAL_THRESHOLD || center_diff < -GOAL_THRESHOLD) {
                    // Gradual movement
                    player->curPos.dy += jo_fixed_mult(SLOW_MOVEMENT_FACTOR, center_diff);
                    // animate_paw1 = false;
                }
                else {
                    player->curPos.dy = toFIXED(0);
                }
            }
            // else {
                // player->curPos.dy = 0;
            // }
        }
        
        speedLimitPlayer(player);

        // move the player
        player->_sprite->pos.y += player->curPos.dy;
        player->_sprite->pos.x += player->curPos.dx;
        player->_sprite->vel.x += player->curPos.dx;
        player->_sprite->vel.y += player->curPos.dy;
        // update_bounding_box(player->_sprite);
        
        detect_player_ball_collision(&pixel_poppy, player);
        boundPlayer(player);
        
        // player->curPos.dy = 0;
    }
}

// void player2AI(Paw* paw, Ball* ball) {    
    // // Check if the ball is moving towards player 2's goal
    // if (ball->vx > 0) {
       // // Adjust the tracking threshold with randomness
        // // int tracking_threshold = jo_random(TRACKING_THRESHOLD);
        // // Check if the ball is close enough to start tracking
        // // if (ball->x >= 0 - tracking_threshold) {
        // if (ball->x >= -128) {
            // animate_paw2 = true;
        // }
        // if (ball->x >= 0) {
            // animate_paw2 = true;
            // // Move the paw towards the ball's y position
            // double y_diff = ball->y - paw->y;

            // // Gradual movement
            // paw->y += MOVEMENT_FACTOR * y_diff;
        // } 
        // else if (!player2_scored) {
            // // Move back to the center
            // double center_diff = 0 - paw->y;

            // // Gradual movement
            // paw->y += MOVEMENT_FACTOR * center_diff;
        // }
    // } 
    // else {
        // // Ball is moving away, move back to the center
        // double center_diff = 0 - paw->y;

        // // Gradual movement
        // paw->y += 0.5*MOVEMENT_FACTOR * center_diff;
        // animate_paw2 = false;
    // }

    // // Ensure the paw stays within the screen bounds
    // paw_boundary(paw); // this might not even be needed, since the AI tracks the ball position anyway
// }

// // if the AI has scored, move the paw to the ball for kick off
// void AI_move_to_ball(Paw* paw, Ball* ball) {
    // // Move the paw towards the ball's y position
    // double y_diff = ball->y - paw->y;
    // // Gradual movement
    // paw->y += MOVEMENT_FACTOR * y_diff;
    
    // // Ensure the paw stays within the screen bounds
    // paw_boundary(paw); // this might not even be needed, since the AI tracks the ball position anyway
// }

// void AI_reset_paws(void) {
    // if (use_player1AI) {
        // AI_move_to_ball(&paw1_pos, &ball_pos);
       // }
    // if (use_player2AI) {
        // AI_move_to_ball(&paw2_pos, &ball_pos);
    // }
// }

// void AI_attack2(void) {
    // // player 1
    // if (touched_by == PLAYER_1) {
        // ball.vx += 3;
        // ball.vy += 2;
        // touched_by = PLAYER_1;
        // kickoff = false;
    // }
    // if (touched_by == PLAYER_2) {
        // ball.vx -= 3;
        // ball.vy -= 2;
        // touched_by = PLAYER_2;
        // kickoff = false;
    // }
// }

// void AI_attack(void) {
    // // player 1
    // if (use_player1AI && touched_by == PLAYER_1) {
        // kickoff = false;
        // game_is_active = true;
        // start_next_round = false;
        // ball = _ball_return(AI_DIFFICULTY, AI_POWERSHOT);
        // ball_pos.vx = ball.vx;
        // ball_pos.vy = ball.vy;
        // touched_by = PLAYER_1;
    // }
    // if (use_player2AI && touched_by == PLAYER_2) {
        // kickoff = false;
        // game_is_active = true;
        // start_next_round = false;
        // ball = _ball_return(AI_DIFFICULTY, AI_POWERSHOT);
        // ball_pos.vx = -ball.vx;
        // ball_pos.vy = -ball.vy;
        // touched_by = PLAYER_2;
    // }
// }

// void AI_kickoff_P1(void) {
            // jo_nbg2_clear();
            // new_game = false;
            // kickoff = false;
            // game_is_active = true;
            // start_next_round = false;
            // start_next_match = false;
            // touched_by = PLAYER_1;
            // ball = _initial_kickoff(AI_DIFFICULTY, AI_POWERSHOT);
            // ball_pos.vx = ball.vx;
            // ball_pos.vy = ball.vy;
// }

// void AI_kickoff_P2(void) {
            // jo_nbg2_clear();
            // new_game = false;
            // kickoff = false;
            // game_is_active = true;
            // start_next_round = false;
            // start_next_match = false;
            // touched_by = PLAYER_2;
            // ball = _initial_kickoff(AI_DIFFICULTY, AI_POWERSHOT);
            // ball_pos.vx = -ball.vx;
            // ball_pos.vy = ball.vy;
// }