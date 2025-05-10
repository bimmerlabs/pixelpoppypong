#include <jo/jo.h>
#include "goal.h"
#include "../main.h"
#include "../BG_DEF/sprite_colors.h"

GOAL g_Goal[MAX_PLAYERS] = {0};
Uint8 g_ActiveGoals = 0;

void initGoalColors(void)
{
    for(unsigned int i = 0; i < MAX_PLAYERS; i++)
    {
        // set initial player colors (player 0 uses the default)
        if (i == 1) {
            hsl_incSprites.h -= 90;
        }
        else if (i == 2) {
            hsl_incSprites.h -= 180;
        }
        else if (i == 3) {
            hsl_incSprites.h -= 270;
        }
        update_palette_Goals[i] = update_sprites_color(&p_rangeGoals[i]);
    }
}

void initGoals(void)
{
    g_ActiveGoals = g_Game.numPlayers; // TODO: remove?
    PGOAL _goal = NULL;
    for(unsigned int i = 0; i < MAX_PLAYERS; i++)
    {
        _goal = &g_Goal[i];
        // _goal->objectState = OBJECT_STATE_INACTIVE;
        _goal->isColliding = false;
        _goal->onLeftSide = false;
        _goal->drawSingleGoal = true;
        _goal->scale = 0;
        _goal->pos.x = 0;
        _goal->pos.top = 0;
        _goal->pos.mid = 0;
        _goal->pos.bot = 0;
        _goal->pos.top_flip = 0;
        _goal->pos.bot_flip = 0;
        _goal->pos.top_zmode = 0;
        _goal->pos.mid_zmode = 0;
        _goal->pos.bot_zmode = 0;
        _goal->id = -1; // is this needed?
        _goal->sprite = &goal[i];
    }
}

void setGoalSize(void)
{
    PGOAL _goal = NULL;
    for(unsigned int i = 0; i < MAX_PLAYERS; i++)
    {
        _goal = &g_Goal[i];
        _goal->player = &g_Players[i];
        if (_goal->player->objectState == OBJECT_STATE_INACTIVE) {
            // _goal->objectState = OBJECT_STATE_INACTIVE;
            continue;
        }
        // _goal->objectState = OBJECT_STATE_ACTIVE;
        _goal->id = i;
        
        switch (_goal->player->teamChoice) 
        {
            case TEAM_1: {
                if (g_Team.objectState[TEAM_3] == OBJECT_STATE_ACTIVE && (g_Team.objectState[TEAM_2] == OBJECT_STATE_ACTIVE || g_Team.objectState[TEAM_4] == OBJECT_STATE_ACTIVE)) { // 3-4 player mode
                    _goal->pos.x = -GOAL_X_POS;
                    _goal->pos.top = -GOAL_Y_POS_TOP_VS_MODE;
                    _goal->pos.mid = -GOAL_Y_POS_MID_VS_MODE;
                    _goal->pos.bot = -GOAL_Y_POS_BOT_VS_MODE;
                    _goal->scale = GOAL_SCALE_VS_MODE;
                    _goal->drawSingleGoal = false;
                }
                else { // 2-3 player mode
                    _goal->pos.x = -GOAL_X_POS;
                    _goal->pos.top = -GOAL_Y_POS_MEDIUM;
                    _goal->pos.mid = GOAL_CENTER_POS;
                    _goal->pos.bot = GOAL_Y_POS_MEDIUM;
                    _goal->scale = GOAL_SCALE_MEDIUM;
                    _goal->drawSingleGoal = true;
                }
                _goal->pos.top_flip = sprNoflip;
                _goal->pos.bot_flip = sprVflip;
                _goal->pos.top_zmode = _ZmLT;
                _goal->pos.mid_zmode = _ZmLC;
                _goal->pos.bot_zmode = _ZmLB;
                break;
            }
            case TEAM_2: {
                if (g_Team.objectState[TEAM_4] == OBJECT_STATE_ACTIVE && (g_Team.objectState[TEAM_1] == OBJECT_STATE_ACTIVE || g_Team.objectState[TEAM_3] == OBJECT_STATE_ACTIVE)) { // 3-4 player mode
                    _goal->pos.x = GOAL_X_POS;
                    _goal->pos.top = -GOAL_Y_POS_TOP_VS_MODE;
                    _goal->pos.mid = -GOAL_Y_POS_MID_VS_MODE;
                    _goal->pos.bot = -GOAL_Y_POS_BOT_VS_MODE;
                    _goal->scale = GOAL_SCALE_VS_MODE;
                    _goal->drawSingleGoal = false;
                }
                else { // 2-3 player mode
                    _goal->pos.x = GOAL_X_POS;
                    _goal->pos.top = -GOAL_Y_POS_MEDIUM;
                    _goal->pos.mid = GOAL_CENTER_POS;
                    _goal->pos.bot = GOAL_Y_POS_MEDIUM;
                    _goal->scale = GOAL_SCALE_MEDIUM;
                    _goal->drawSingleGoal = true;
                }
                _goal->pos.top_flip = sprHflip;
                _goal->pos.bot_flip = sprHVflip;
                _goal->pos.top_zmode = _ZmRT;
                _goal->pos.mid_zmode = _ZmRC;
                _goal->pos.bot_zmode = _ZmRB;
                break;
            }
            case TEAM_3: {
                _goal->pos.top_flip = sprNoflip;
                _goal->pos.bot_flip = sprVflip;
                _goal->pos.top_zmode = _ZmLT;
                _goal->pos.mid_zmode = _ZmLC;
                _goal->pos.bot_zmode = _ZmLB;
                if (g_Team.objectState[TEAM_1] == OBJECT_STATE_ACTIVE && (g_Team.objectState[TEAM_2] == OBJECT_STATE_ACTIVE || g_Team.objectState[TEAM_4] == OBJECT_STATE_ACTIVE)) { // 3-4 player mode
                    _goal->pos.x = -GOAL_X_POS;
                    _goal->pos.top = GOAL_Y_POS_BOT_VS_MODE;
                    _goal->pos.mid = GOAL_Y_POS_MID_VS_MODE;
                    _goal->pos.bot = GOAL_Y_POS_TOP_VS_MODE;
                    _goal->scale = GOAL_SCALE_VS_MODE;
                    _goal->drawSingleGoal = false;
                }
                else if (g_Team.objectState[TEAM_1] == OBJECT_STATE_ACTIVE) { // 2 player mode
                    switchPlayerPosition(_goal->player); // move to right side
                    _goal->pos.x = GOAL_X_POS;
                    _goal->pos.top = -GOAL_Y_POS_MEDIUM;
                    _goal->pos.mid = GOAL_CENTER_POS;
                    _goal->pos.bot = GOAL_Y_POS_MEDIUM;
                    _goal->scale = GOAL_SCALE_MEDIUM;
                    _goal->pos.top_flip = sprHflip;
                    _goal->pos.bot_flip = sprHVflip;
                    _goal->pos.top_zmode = _ZmRT;
                    _goal->pos.mid_zmode = _ZmRC;
                    _goal->pos.bot_zmode = _ZmRB;
                    _goal->drawSingleGoal = true;
                }
                else { // 2-3 player mode
                    _goal->pos.x = -GOAL_X_POS;
                    _goal->pos.top = -GOAL_Y_POS_MEDIUM;
                    _goal->pos.mid = GOAL_CENTER_POS;
                    _goal->pos.bot = GOAL_Y_POS_MEDIUM;
                    _goal->scale = GOAL_SCALE_MEDIUM;
                    _goal->drawSingleGoal = true;
                }
                break;
            }
            case TEAM_4: {
                _goal->pos.top_flip = sprHflip;
                _goal->pos.bot_flip = sprHVflip;
                _goal->pos.top_zmode = _ZmRT;
                _goal->pos.mid_zmode = _ZmRC;
                _goal->pos.bot_zmode = _ZmRB; 
                if (g_Team.objectState[TEAM_2] == OBJECT_STATE_ACTIVE && (g_Team.objectState[TEAM_1] == OBJECT_STATE_ACTIVE || g_Team.objectState[TEAM_3] == OBJECT_STATE_ACTIVE)) { // 3-4 player mode
                    _goal->pos.x = GOAL_X_POS;
                    _goal->pos.top = GOAL_Y_POS_BOT_VS_MODE;
                    _goal->pos.mid = GOAL_Y_POS_MID_VS_MODE;
                    _goal->pos.bot = GOAL_Y_POS_TOP_VS_MODE;
                    _goal->scale = GOAL_SCALE_VS_MODE; 
                    _goal->drawSingleGoal = false;
                }
                else if (g_Team.objectState[TEAM_2] == OBJECT_STATE_ACTIVE) { // 3-4 player mode
                    switchPlayerPosition(_goal->player); // move to left side
                    _goal->pos.x = -GOAL_X_POS;
                    _goal->pos.top = -GOAL_Y_POS_MEDIUM;
                    _goal->pos.mid = GOAL_CENTER_POS;
                    _goal->pos.bot = GOAL_Y_POS_MEDIUM;
                    _goal->scale = GOAL_SCALE_MEDIUM;
                    _goal->pos.top_flip = sprNoflip;
                    _goal->pos.bot_flip = sprVflip;
                    _goal->pos.top_zmode = _ZmLT;
                    _goal->pos.mid_zmode = _ZmLC;
                    _goal->pos.bot_zmode = _ZmLB;
                    _goal->drawSingleGoal = true;
                }
                else {
                    _goal->pos.x = GOAL_X_POS;
                    _goal->pos.top = -GOAL_Y_POS_MEDIUM;
                    _goal->pos.mid = GOAL_CENTER_POS;
                    _goal->pos.bot = GOAL_Y_POS_MEDIUM;
                    _goal->scale = GOAL_SCALE_MEDIUM;
                    _goal->drawSingleGoal = true;
                } 
                break;
            }
            default:
                break;
        }
        _goal->onLeftSide = _goal->player->onLeftSide;
    }
}

void drawGoals(void) {
    PGOAL _goal = NULL;
    for(unsigned int i = 0; i < MAX_PLAYERS; i++)
    {
        _goal = &g_Goal[i];
        if (_goal->player->objectState == OBJECT_STATE_INACTIVE || _goal->player->subState == PLAYER_STATE_DEAD) {
            continue;
        }
        drawGoalSprites(_goal->sprite, 0, 2, _goal->pos.top_zmode, _goal->pos.top_flip, _goal->pos.x, _goal->pos.top, 2);
        drawGoalSprites(_goal->sprite, 1, 3, _goal->pos.mid_zmode, _goal->pos.top_flip, _goal->pos.x, _goal->pos.mid, _goal->scale);
        drawGoalSprites(_goal->sprite, 0, 2, _goal->pos.bot_zmode, _goal->pos.bot_flip, _goal->pos.x, _goal->pos.bot, 2);
    }
}
