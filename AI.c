#include "AI.h"
#include "main.h"
#include "assets.h"
#include "physics.h"
#include "gameplay.h"
#include "objects/player.h"

extern PLAYER g_Players[MAX_PLAYERS];

void playerAI(Sprite *ball) {    
    // for debug
    // unsigned int text_y = 5;
    // unsigned int text_x = 2;
    for(unsigned int i = 0; i <  COUNTOF(g_Players); i++)
    {
        // if (i == 2) {
            // text_y = 5;
            // text_x = 20;
        // }
        PPLAYER player = &g_Players[i];
        if(player->objectState != OBJECT_STATE_ACTIVE || player->isAI == false)
        // if(player->objectState != OBJECT_STATE_ACTIVE)
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
            // if (game_options.debug_display) {
                // jo_nbg0_printf(2, 14, "CENTER_DIFF:%i", toINT(center_diff));
            // }
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
        }
        
        speedLimitPlayer(player);

        // move the player
        player->_sprite->pos.y += player->curPos.dy;
        player->_sprite->pos.x += player->curPos.dx;
        player->_sprite->vel.x += player->curPos.dx;
        player->_sprite->vel.y += player->curPos.dy;
        // update_bounding_box(player->_sprite);
        
        // if (game_options.debug_mode) {
            // jo_nbg0_printf(text_x, text_y, "PLAYER ID: %i", player->playerID);
            // text_y++;
            // jo_nbg0_printf(text_x, text_y, "TEAM: %i", player->teamChoice);
            // text_y++;
            
            // // Check inputs
            // jo_nbg0_printf(text_x, text_y, "DX: %i", player->curPos.dx);
            // text_y++;
            // jo_nbg0_printf(text_x, text_y, "DY: %i", player->curPos.dy);
            // text_y++;
            // // jo_nbg0_printf(text_x, text_y, "INPUT ID: %i", player->input->id);
            // // text_y++;
            // // jo_nbg0_printf(text_x, text_y, "AXIS X: %i", player->input->axis_x);
            // // text_y++;
            // // jo_nbg0_printf(text_x, text_y, "AXIS Y: %i", player->input->axis_y);
            // // text_y++;
            // // jo_nbg0_printf(text_x, text_y, "SENS: %i", player->input->sensitivity);
            // // text_y++;

            // // Player states
            // jo_nbg0_printf(text_x, text_y, "LIVES: %i", player->numLives);
            // text_y++;
            // jo_nbg0_printf(text_x, text_y, "IS READY: %i", player->isReady);
            // text_y++;
            // jo_nbg0_printf(text_x, text_y, "IS PLAYING: %i", player->isPlaying);
            // text_y++;
            // jo_nbg0_printf(text_x, text_y, "SCORED: %i", player->scored);
            // text_y++;

            // // Character attributes
            // jo_nbg0_printf(text_x, text_y, "CHAR: %i", player->character.choice);
            // text_y++;
            
            // // Movement attributes
            // jo_nbg0_printf(text_x, text_y, "MAX SPD: %i", player->maxSpeed);
            // text_y++;
            // jo_nbg0_printf(text_x, text_y, "ACCEL: %i", player->acceleration);
            // text_y++;
            // jo_nbg0_printf(text_x, text_y, "POWER: %i", player->power);
            // text_y++;
        // }
        
        detect_player_ball_collision(&pixel_poppy, player);
        boundAiPlayer(player);
    }
}
