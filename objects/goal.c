#include <jo/jo.h>
#include "goal.h"
#include "../main.h"
#include "../game/highscores.h"
#include "../palettefx/sprite_colors.h"

GOAL g_Goal[MAX_PLAYERS] = {0};
bool g_AnimateGoal = false;
bool g_ExplodeGoal = false;

static bool goal_increasing = false;
static int goal_h_value = 0;
static short goal_animation_cycles = 0;

void initGoalColors(void)
{
    g_AnimateGoal = false;
    g_ExplodeGoal = false;
    goal_increasing = false;
    goal_h_value = 0;
    goal_animation_cycles = 0;
    for(unsigned int i = 0; i < MAX_PLAYERS; i++)
    {
        // set initial player colors (player 0 uses the default)
        if (i == 1) {
            hsl_incSprites[HSL_GOAL].h -= 90;
        }
        else if (i == 2) {
            hsl_incSprites[HSL_GOAL].h -= 180;
        }
        else if (i == 3) {
            hsl_incSprites[HSL_GOAL].h -= 270;
        }
        update_palette_Goals[i] = update_sprites_color(&p_rangeGoals[i], HSL_GOAL);
    }
}

void animateGoalColor(bool *_do_update) {
    if (!g_AnimateGoal) {
        return;
    }
    if (goal_animation_cycles < 9) {
        if (goal_increasing) {
            if (goal_h_value < 6) {
                goal_h_value += 1;
                hsl_incSprites[HSL_GOAL].h = -5;
            } 
            else {
                goal_increasing = false;
            }
        } 
        else {
            if (goal_h_value > -6) {
                goal_h_value -= 1;
                hsl_incSprites[HSL_GOAL].h = +5;
            } 
            else {
                goal_increasing = true;
            }
            if (goal_h_value == 0) {
                goal_animation_cycles++;
            }
        }
        *_do_update = true;
    }
    else {
        g_AnimateGoal = false;
        goal_animation_cycles = 0;
    }
}

void initGoals(void)
{
    PGOAL _goal = NULL;
    for(unsigned int i = 0; i < MAX_PLAYERS; i++)
    {
        _goal = &g_Goal[i];
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
            continue;
        }
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

void explodeGoals(void) {
    if (!g_ExplodeGoal) {
        return;
    }
    PGOAL _goal = NULL;
    for(unsigned int i = 0; i < MAX_PLAYERS; i++)
    {
        _goal = &g_Goal[i];
        if (_goal->player->isAI && g_Game.gameMode == GAME_MODE_STORY) {
            continue;
        }
        if (g_Team.objectState[_goal->player->teamChoice] == OBJECT_STATE_ACTIVE && _goal->player->subState == PLAYER_STATE_DEAD) {
            _goal->sprite->rot.z += 1;
            if (g_Transition.explosion_flash) {
                g_Transition.explosion_flash = explosionEffect();
                set_spr_scale(_goal->sprite, 3, 3);
                _goal->sprite->pos.y = toFIXED(_goal->pos.mid);
                _goal->sprite->zmode = _ZmCC;
            }
            else {
                my_sprite_draw(_goal->sprite);
                g_ExplodeGoal = explode_animation(_goal->sprite);
                if (!g_ExplodeGoal) {
                    _goal->sprite->rot.z = 0;
                    if (g_Game.gameMode != GAME_MODE_STORY && g_Game.currentNumPlayers > 1) {
                        g_Team.objectState[_goal->player->teamChoice] = OBJECT_STATE_INACTIVE;
                        setGoalSize(); // not in story mode
                    }
                }
            }
        }
    }
}

void checkRightGoalCollision(Sprite *ball) {
    // iterate through goals on right side, check bounds
    PGOAL _goal;
    for(unsigned int i = 0; i < MAX_PLAYERS; i++)
    {
        _goal = &g_Goal[i];
        if (_goal->player->objectState == OBJECT_STATE_INACTIVE || _goal->player->subState == PLAYER_STATE_DEAD || _goal->onLeftSide) {
            continue;
        }
        if (ball->pos.y - GOAL_MARGIN > toFIXED(_goal->pos.top) && ball->pos.y + GOAL_MARGIN < toFIXED(_goal->pos.bot)) { // removed ball radius - need another solution
            g_Game.goalID = _goal->id;
            g_AnimateGoal = true;
            if (g_Game.gameMode == GAME_MODE_STORY) {
                g_Game.isGoalScored = true;
                calculateScore(ball, 0);
                updatePlayerLives(1);
                playCDTrack(g_Audio.goalScoredTrack, false);
                nextGoalScoredTrack();
                break;
            }
            else if (!g_Game.isGoalScored) { // scored on TEAM_2 or TEAM_4
                updateScore(ball, _goal->id);
                break;
            }
        }
    }
}

void checkLeftGoalCollision(Sprite *ball) {
    // iterate through goals on left side, check bounds
    PGOAL _goal;
    for(unsigned int i = 0; i < MAX_PLAYERS; i++)
    {
        _goal = &g_Goal[i];
        if (_goal->player->objectState == OBJECT_STATE_INACTIVE || _goal->player->subState == PLAYER_STATE_DEAD || !_goal->onLeftSide) {
            continue;
        }
        if (ball->pos.y - GOAL_MARGIN > toFIXED(_goal->pos.top) && ball->pos.y + GOAL_MARGIN < toFIXED(_goal->pos.bot)) { // removed ball radius - need another solution
            g_Game.goalID = _goal->id;
            g_AnimateGoal = true;
            if (g_Game.gameMode == GAME_MODE_STORY) {
                g_Game.isGoalScored = true;
                ballTtouchTimer = 0;
                updatePlayerLives(0);
                playCDTrack(g_Audio.goalScoredTrack, false);
                nextGoalScoredTrack();
                break;
            }
            else if (!g_Game.isGoalScored) { // scored on TEAM_2 or TEAM_4
                updateScore(ball, _goal->id);
                break;
            }
        }
    }
}
