#pragma once

#include <jo/jo.h>
#include "assets.h"
#include "sprites.h"
#include "physics.h"
#include "team_select.h"
#include "objects/player.h"
#include "BG_DEF/sprite_colors.h"

// #define ROUND_OVER_TIME (4 * 60)

#define GAME_BEGIN_TIME (3 * 60)
#define DEMO_TIME (30 * 60)
#define BOMB_TIMER (15 * 60)
#define TIMEOUT 99 // seconds

#define GOAL_TOP toFIXED(-100.0)
#define GOAL_BOTTOM toFIXED(100.0)
#define GAMEPLAY_PORTRAIT_X 300
#define GAMEPLAY_PORTRAIT_Y 190
#define GAMEPLAY_PORTRAIT_Y 190
extern PLAYER g_Players[MAX_PLAYERS];

extern ANGLE ball_rotation;
extern bool start_timer;
extern bool round_start;
static bool draw_bomb;
static bool explode_bomb;
void gameplay_init(void);
void demo_init(void);
void demo_update(void);
void game_timer(void);
void setGoalSize(void);

void gameplay_draw(void);
void gameScore_draw(void);
void gameplayUI_draw(PPLAYER player);
void gameplay_update(void);
void gameplay_input(void);
void demo_input(void);
static inline void initStoryMode(void) {
	g_Game.numPlayers = TWO_PLAYER;
	reset_inputs();
	initPlayers();
	initTeams();
	g_Game.currentNumPlayers = 1;              

	PPLAYER player = &g_Players[0]; // macchi

	player->input = &g_Inputs[0];
	player->input->id = 0;
	player->input->isSelected = true;

	player->_sprite = &macchi;

	player->teamChoice = TEAM_1;
	player->teamSelected = true;
	teamAvailable[player->teamChoice] = false;
	g_Game.numTeams++;

	player->character.selected = true;
	player->character.choice = CHARACTER_MACCHI;
	characterAvailable[player->character.choice] = false;
	player->_portrait->spr_id = player->_portrait->anim1.asset[player->character.choice];

	player->maxSpeed = characterAttributes[player->character.choice].maxSpeed;
	player->acceleration = characterAttributes[player->character.choice].acceleration;
	player->power = characterAttributes[player->character.choice].power;

	player->objectState = OBJECT_STATE_ACTIVE;
	player->isPlaying = PLAYING;
	player->isAI = false;
	boundPlayer(player);
	initStoryCharacters();
	initVsModePlayers();
}

static __jo_force_inline void initPixelPoppy(void) {
    reset_ball_movement(&pixel_poppy);
    sprite_frame_reset(&pixel_poppy);
    pixel_poppy.isColliding = false;
}static inline void draw_ui_element(Sprite *sprite, Uint8 num, int x, int y, int offset) {
    for (int i = 0; i < num; i++) {
        set_spr_position(sprite, x, y, 90);
        my_sprite_draw(sprite);
        x += offset;
    }
}
static inline void draw_heart_element(Sprite *sprite, Uint8 num, int x, int y, int offset) {
    for (int i = 0; i < num; i++) {
        if (num > g_Game.numLives) {
            sprite->spr_id = sprite->anim1.asset[1];
	    num--;
        }
        else {
	    sprite->spr_id = sprite->anim1.asset[0];
	}
        set_spr_position(sprite, x, y, 90);
        my_sprite_draw(sprite);
        x += offset;
    }
    if (num < g_Game.numLives) {
	sprite->spr_id = sprite->anim1.asset[2];
	for (int i = 0; i < (g_Game.numLives - num); i++) {
	    set_spr_position(sprite, x, y, 90);
	    my_sprite_draw(sprite);
	    x += offset;
	}
   }
}

static __jo_force_inline void gameplayScores_draw(PPLAYER player) {
    switch (player->teamChoice) 
    {
        case TEAM_1: {
            jo_nbg0_printf(9, 2, "%09d", player->score.points);
            break;
        }
        case TEAM_2: {
            jo_nbg0_printf(26, 2, "%09d", player->score.points);
            break;
        }
        case TEAM_3: {
            jo_nbg0_printf(9, 26, "%09d", player->score.points);
            break;
        }
        case TEAM_4: {
            jo_nbg0_printf(26, 26, "%09d", player->score.points);
            break;
        }
        default:
            break;
    }
}

static __jo_force_inline void drawGoals(PPLAYER player) {
    Uint8 i = player->playerID;
    int x_position = 0;
    int y_position_top = 0;
    int y_position_mid = 0;
    int y_position_bot = 0;
    int top_flip = sprNoflip;
    int bot_flip = sprVflip;
    int top_zmode = _ZmRT;
    int mid_zmode = _ZmRC;
    int bot_zmode = _ZmRB;
    Uint8 goal_scale = 0;
        
    switch (player->teamChoice) 
    {
        case TEAM_1: {
            top_flip = sprNoflip;
            bot_flip = sprVflip;
            top_zmode = _ZmLT;
            mid_zmode = _ZmLC;
            bot_zmode = _ZmLB;
            if (g_Assets.drawSingleGoal[i]) {
                x_position = -GOAL_X_POS;
                y_position_top = -g_Game.goalYPosTop;
                y_position_mid = g_Game.goalYPosMid;
                y_position_bot = g_Game.goalYPosBot;
                goal_scale = g_Game.goalScale; // adjust for difficulty
            }
            else {
                x_position = -GOAL_X_POS;
                y_position_top = -GOAL_Y_POS_TOP_VS_MODE;
                y_position_mid = -GOAL_Y_POS_MID_VS_MODE;
                y_position_bot = -GOAL_Y_POS_BOT_VS_MODE;
                goal_scale = GOAL_SCALE_VS_MODE;                
            }
            break;
        }
        case TEAM_2: {
            top_flip = sprHflip;
            bot_flip = sprHVflip;
            top_zmode = _ZmRT;
            mid_zmode = _ZmRC;
            bot_zmode = _ZmRB;
            if (g_Assets.drawSingleGoal[i]) {
                x_position = GOAL_X_POS;
                y_position_top = -g_Game.goalYPosTop;
                y_position_mid = g_Game.goalYPosMid;
                y_position_bot = g_Game.goalYPosBot;
                goal_scale = g_Game.goalScale;
            }
            else {
                x_position = GOAL_X_POS;
                y_position_top = -GOAL_Y_POS_TOP_VS_MODE;
                y_position_mid = -GOAL_Y_POS_MID_VS_MODE;
                y_position_bot = -GOAL_Y_POS_BOT_VS_MODE;
                goal_scale = GOAL_SCALE_VS_MODE; // adjust for difficulty              
            }
            break;
        }
        case TEAM_3: {
            top_flip = sprNoflip;
            bot_flip = sprVflip;
            top_zmode = _ZmLT;
            mid_zmode = _ZmLC;
            bot_zmode = _ZmLB;
            x_position = -GOAL_X_POS;
            y_position_top = GOAL_Y_POS_BOT_VS_MODE;
            y_position_mid = GOAL_Y_POS_MID_VS_MODE;
            y_position_bot = GOAL_Y_POS_TOP_VS_MODE;
            goal_scale = GOAL_SCALE_VS_MODE;
            break;
        }
        case TEAM_4: {
            top_flip = sprHflip;
            bot_flip = sprHVflip;
            top_zmode = _ZmRT;
            mid_zmode = _ZmRC;
            bot_zmode = _ZmRB;
            x_position = GOAL_X_POS;
            y_position_top = GOAL_Y_POS_BOT_VS_MODE;
            y_position_mid = GOAL_Y_POS_MID_VS_MODE;
            y_position_bot = GOAL_Y_POS_TOP_VS_MODE;
            goal_scale = GOAL_SCALE_VS_MODE;  
            break;
        }
        default:
            break;
    }
    goal[i].spr_id = goal[i].anim1.asset[0];
    goal[i].zmode = top_zmode;
    goal[i].flip = top_flip;
    set_spr_position(&goal[i], x_position, y_position_top, 120);
    set_spr_scale(&goal[i], 2, 2);
    my_sprite_draw(&goal[i]);
    // shadow
    goal[i].spr_id = goal[i].anim1.asset[2];
    set_spr_position(&goal[i], x_position+2, y_position_top+2, 125);
    my_sprite_draw(&goal[i]);

    goal[i].spr_id = goal[i].anim1.asset[1];
    goal[i].zmode = mid_zmode;
    set_spr_position(&goal[i], x_position, y_position_mid, 120);
    set_spr_scale(&goal[i], 2, goal_scale);
    my_sprite_draw(&goal[i]);
    // shadow
    goal[i].spr_id = goal[i].anim1.asset[3];
    set_spr_position(&goal[i], x_position+2, y_position_mid+2, 125);
    my_sprite_draw(&goal[i]);

    goal[i].spr_id = goal[i].anim1.asset[0];
    goal[i].zmode = bot_zmode;
    goal[i].flip = bot_flip;
    set_spr_position(&goal[i], x_position, y_position_bot, 120);
    set_spr_scale(&goal[i], 2, 2);
    my_sprite_draw(&goal[i]);
    // shadow
    goal[i].spr_id = goal[i].anim1.asset[2];
    set_spr_position(&goal[i], x_position+2, y_position_bot+2, 125);
    my_sprite_draw(&goal[i]);
}

static __jo_force_inline void drawGameItems(void) {
    if (draw_bomb) {
        my_sprite_draw(&bomb_item);
    }
    if (fishtank_item.visible)
        my_sprite_draw(&fishtank_item);
    if (shroom_item.visible)
        my_sprite_draw(&shroom_item);
    if (garfield_item.visible)
        my_sprite_draw(&garfield_item);
    if (craig_item.visible)
        my_sprite_draw(&craig_item);

    //ANIMATIONS
    if (!explode_bomb) {
        looped_animation_pow(&bomb_item, 4);
    }
    else {
        explode_bomb = explode_animation(&bomb_item);
        if (explode_bomb == false) {
            draw_bomb = false;
        }
    }
    looped_animation_pow(&fishtank_item, 8);
    looped_animation_pow(&shroom_item, 4);
    hsl_incSprites.h += 2;
    do_update_shroom = true;
}
static __jo_force_inline void drawVsMode(void) {
        // SPRITES
        my_sprite_draw(&timer_num100); // tens
        my_sprite_draw(&timer_num10); // tens
        my_sprite_draw(&timer_num1);  // ones
        
        drawPlayers();
        drawGameItems();
        
        update_ball(&pixel_poppy);
        my_sprite_draw(&pixel_poppy);
}
static __jo_force_inline void drawStoryMode(void) {
        // SPRITES
        my_sprite_draw(&timer_num100); // tens
        my_sprite_draw(&timer_num10); // tens
        my_sprite_draw(&timer_num1);  // ones
        
        drawPlayers();
        drawGameItems();
        
        update_ball(&pixel_poppy);
        my_sprite_draw(&pixel_poppy);
}
static __jo_force_inline void drawClassicMode(void) {
        // SPRITES
        my_sprite_draw(&timer_num10); // tens
        my_sprite_draw(&timer_num1);  // ones
        
        drawPlayers();
        
        // don't draw until poppy is reset
        if (!g_Game.isGoalScored && !g_Game.isRoundOver) {
            update_ball(&pixel_poppy);
            my_sprite_draw(&pixel_poppy);
            return;
        }
        if (g_Game.isGoalScored) {
            initPixelPoppy();
        }
}

static __jo_force_inline void drawGameUI(void) {
    for(unsigned int i = 0; i <= g_Game.numPlayers; i++)
    {
        PPLAYER player = &g_Players[i];
        if (player->objectState == OBJECT_STATE_INACTIVE) {
            continue;
        }
        looped_animation_pow(player->_sprite, 4); // shield?
        player->_portrait->spr_id = player->_portrait->anim1.asset[player->character.choice];
        gameplayUI_draw(player);
        drawGoals(player);
    }
}

static __jo_force_inline void startGameplay(void) {
        start_ball_movement(&pixel_poppy);
        g_Game.isRoundOver = false;
        g_Game.isGoalScored = false;
}
