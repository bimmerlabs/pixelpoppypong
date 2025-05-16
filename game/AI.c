#include "AI.h"
#include "physics.h"
#include "gameplay.h"
#include "../main.h"
#include "../core/assets.h"

extern PLAYER g_Players[MAX_PLAYERS];

void playerAI(Sprite *ball) {    
    for(unsigned int i = 0; i <= g_Game.numPlayers; i++)
    {
        PPLAYER player = &g_Players[i];
        if(player->objectState == OBJECT_STATE_INACTIVE || player->isAI == false  || player->subState == PLAYER_STATE_DEAD)
        {
            continue;
        }
        if (!g_Game.isBallActive) {
            centerAiPlayer(player);
        }
        else if (player->onLeftSide == true) {
            // Check if the ball is moving towards player 2's goal
            if (ball->vel.x < FIXED_0) {

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
            if (ball->vel.x > FIXED_0) {

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
        
        detect_player_ball_collision(&pixel_poppy, player);
        boundAiPlayer(player);
    }
}

void centerAiPlayer(PPLAYER player)
{
    FIXED middle;
    PGOAL _goal = &g_Goal[player->playerID];
    switch(player->teamChoice)
    {
        case TEAM_1: {
            middle = -AI_GOAL_CENTER;
            if (_goal->drawSingleGoal) {
                middle = SCREEN_MIDDLE;
            }
            break;
        }
        case TEAM_2: {
            middle = -AI_GOAL_CENTER;
            if (_goal->drawSingleGoal) {
                middle = SCREEN_MIDDLE;
            }
            break;
        }
        default: {
            middle = AI_GOAL_CENTER;
            if (_goal->drawSingleGoal) {
                middle = SCREEN_MIDDLE;
            }
            break;
        }
    }
    FIXED center_diff = middle - player->_sprite->pos.y;
    if (center_diff > player->goalCenterThresholdMax || center_diff < -player->goalCenterThresholdMax) {
        player->curPos.dy += jo_fixed_mult(MEDIUM_MOVEMENT_FACTOR, center_diff);
    }
    else if (center_diff > player->goalCenterThresholdMin || center_diff < -player->goalCenterThresholdMin) {
        player->curPos.dy -= jo_fixed_mult(SLOW_MOVEMENT_FACTOR, center_diff);
    }
    else {
        player->curPos.dy = FIXED_0;
    }
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

    FIXED bottom = SCREEN_BOTTOM;
    FIXED top = SCREEN_MIDDLE;
    PGOAL _goal = &g_Goal[player->playerID];
    switch(player->teamChoice)
    {
        case TEAM_1: {
            bottom = SCREEN_MIDDLE;
            top = SCREEN_TOP;
            if (_goal->drawSingleGoal) {
                bottom = SCREEN_BOTTOM;
            }
            break;
        }
        case TEAM_2: {
            bottom = SCREEN_MIDDLE;
            top = SCREEN_TOP;
            if (_goal->drawSingleGoal) {
                bottom = SCREEN_BOTTOM;
            }
            break;
        }
        default: {
            if (_goal->drawSingleGoal) {
                top = SCREEN_TOP;
                bottom = SCREEN_BOTTOM;
            }
            break;
        }
    }
    if(player->_sprite->pos.y > bottom)
    {
        player->_sprite->pos.y = bottom;
        player->curPos.dy = FIXED_0;
    }

    if(player->_sprite->pos.y < top)
    {
        player->_sprite->pos.y = top;
        player->curPos.dy = FIXED_0;
    }
}