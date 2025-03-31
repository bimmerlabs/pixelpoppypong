#include "AI.h"
#include "main.h"
#include "assets.h"
#include "physics.h"
#include "gameplay.h"

extern PLAYER g_Players[MAX_PLAYERS];

void playerAI(Sprite *ball) {    
    // for debug
    // unsigned int text_y = 5;
    // unsigned int text_x = 2;
    for(unsigned int i = 0; i <= g_Game.numPlayers; i++)
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
        if (!g_Game.isBallActive) {
            centerAiPlayer(player);
            // continue;
        }
        else if (player->onLeftSide == true) {
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
                    player->curPos.dy += jo_fixed_mult(player->acceleration, y_diff);
                }
            }
            else {
                centerAiPlayer(player);
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
                    player->curPos.dy += jo_fixed_mult(player->acceleration, y_diff);
                }
            } 
            else {
                centerAiPlayer(player);
            }
        }
        
        speedLimitPlayer(player);

        // move the player
        player->_sprite->pos.y += player->curPos.dy;
        player->_sprite->pos.x += player->curPos.dx;
        player->_sprite->vel.x += player->curPos.dx;
        player->_sprite->vel.y += player->curPos.dy;
        // update_bounding_box(player->_sprite);
        
        // if (g_GameOptions.debug_mode) {
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

void centerAiPlayer(PPLAYER player)
{
    FIXED middle;
    // account for game modes (1-4 players)
    switch(player->teamChoice)
    {
        case TEAM_1: {
            middle = -AI_GOAL_CENTER;
            if (g_Game.numTeams < 3) {
                middle = SCREEN_MIDDLE;
            }
            break;
        }
        case TEAM_2: {
            middle = -AI_GOAL_CENTER;
            if (g_Game.numTeams <= 3) {
                middle = SCREEN_MIDDLE;
            }
            break;
        }
        default:
            middle = AI_GOAL_CENTER;
            break;
    }
    FIXED center_diff = middle - player->_sprite->pos.y;
    if (center_diff > player->goalCenterThresholdMax || center_diff < -player->goalCenterThresholdMax) {
        player->curPos.dy += jo_fixed_mult(MEDIUM_MOVEMENT_FACTOR, center_diff);
    }
    else if (center_diff > player->goalCenterThresholdMin || center_diff < -player->goalCenterThresholdMin) {
        player->curPos.dy -= jo_fixed_mult(SLOW_MOVEMENT_FACTOR, center_diff);
    }
    else {
        player->curPos.dy = toFIXED(0);
    }
    // if (player->teamChoice == TEAM_2) {
        // jo_nbg0_printf(20, 16, "POS.Y:%i", JO_FIXED_TO_INT(player->_sprite->pos.y));
        // jo_nbg0_printf(20, 17, "CENTER_DIFF:%i", JO_FIXED_TO_INT(center_diff));
        // jo_nbg0_printf(20, 18, "MIDDLE:%i",  JO_FIXED_TO_INT(middle));
        // jo_nbg0_printf(20, 19, "CURPOS.DY:%i",  JO_FIXED_TO_INT(player->curPos.dy));
    // } 
    // if (player->teamChoice == TEAM_3) {
        // jo_nbg0_printf(2, 16, "POS.Y:%i", JO_FIXED_TO_INT(player->_sprite->pos.y));
        // jo_nbg0_printf(2, 17, "CENTER_DIFF:%i", JO_FIXED_TO_INT(center_diff));
        // jo_nbg0_printf(2, 18, "MIDDLE:%i",  JO_FIXED_TO_INT(middle));
        // jo_nbg0_printf(2, 19, "CURPOS.DY:%i",  JO_FIXED_TO_INT(player->curPos.dy));
    // }
    player->_sprite->pos.y += player->curPos.dy;
    player->_sprite->vel.y += player->curPos.dy;
    boundAiPlayer(player);
}

void boundAiPlayer(PPLAYER player)
{
    if (player->onLeftSide == true) {
        if(player->_sprite->pos.x > -PLAYER_BOUNDARY_MIDDLE - PLAYER_WIDTH)
        {
            player->_sprite->pos.x = -PLAYER_BOUNDARY_MIDDLE - PLAYER_WIDTH;
        }
        if(player->_sprite->pos.x < PLAYER_BOUNDARY_LEFT + PLAYER_WIDTH)
        {
            player->_sprite->pos.x = PLAYER_BOUNDARY_LEFT + PLAYER_WIDTH;
        }
    }
    else {
        if(player->_sprite->pos.x > PLAYER_BOUNDARY_RIGHT - PLAYER_WIDTH)
        {
            player->_sprite->pos.x = PLAYER_BOUNDARY_RIGHT - PLAYER_WIDTH;
        }
        if(player->_sprite->pos.x < PLAYER_BOUNDARY_MIDDLE + PLAYER_WIDTH)
        {
            player->_sprite->pos.x = PLAYER_BOUNDARY_MIDDLE + PLAYER_WIDTH;
        }
    }

    // account for game modes (1-4 players)
    FIXED bottom = SCREEN_BOTTOM;
    FIXED top = SCREEN_MIDDLE;
    switch(player->teamChoice)
    {
        case TEAM_1: {
            bottom = SCREEN_MIDDLE;
            top = SCREEN_TOP;
            if (g_Game.numTeams < 3) {
                bottom = SCREEN_BOTTOM;
            }
            break;
        }
        case TEAM_2: {
            bottom = SCREEN_MIDDLE;
            top = SCREEN_TOP;
            if (g_Game.numTeams <= 3) {
                bottom = SCREEN_BOTTOM;
            }
            break;
        }
        default:
            break;
    }
    if(player->_sprite->pos.y > bottom)
    {
        player->_sprite->pos.y = bottom;
        player->curPos.dy = toFIXED(0);
    }

    if(player->_sprite->pos.y < top)
    {
        player->_sprite->pos.y = top;
        player->curPos.dy = toFIXED(0);
    }
}