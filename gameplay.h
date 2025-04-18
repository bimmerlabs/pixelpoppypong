#pragma once

#include <jo/jo.h>
#include "assets.h"
#include "sprites.h"
#include "physics.h"
#include "team_select.h"
#include "objects/player.h"
#include "BG_DEF/sprite_colors.h"

// #define ROUND_OVER_TIME (4 * 60)

#define ROUND_BEGIN_TIME_NORMAL (6 * 60)
#define DROP_BALL_TIME_NORMAL (4 * 60) // MAYBE THIS VARIES BASED ON SCORE OR EXPLODING?
#define ROUND_BEGIN_TIME_FAST (3.5 * 60)
#define DROP_BALL_TIME_FAST (1.5 * 60) // MAYBE THIS VARIES BASED ON SCORE OR EXPLODING?
#define DEMO_TIME (30 * 60)
#define BOMB_TIMER (15 * 60)
#define TIMEOUT_BATTLE 999 // seconds
#define TIMEOUT_CLASSIC 299
#define TIMEOUT_STORY_EASY 599
#define TIMEOUT_STORY_MEDIUM 299
#define TIMEOUT_STORY_HARD 259

#define BALL_TOUCH_TIMEOUT (5 * 60)
#define GAME_END_DELAY_TIMEOUT (5 * 60)

#define MAX_ROUNDS 8

#define GAMEPLAY_PORTRAIT_X 300
#define GAMEPLAY_PORTRAIT_Y 190
#define GAMEPLAY_PORTRAIT_Y 190
extern PLAYER g_Players[MAX_PLAYERS];

extern Uint8 g_goalPlayerId[MAX_PLAYERS];

extern ANGLE ball_rotation;
extern bool start_gameplay_timer;
extern bool round_start;
extern bool explode_ball;
extern bool play_continue_track;

static bool draw_bomb;
static bool explode_bomb;

void gameplay_init(void);
void demo_init(void);
void demo_update(void);
void setGoalSize(void);

void gameplay_timer(void);
void gameplay_draw(void);
void tallyScore(void);
int determineWinner(void);
void gameScore_draw(void);
void gameplayUI_draw(PPLAYER player);
void gameplay_update(void);
void reset_ball_movement(Sprite *ball);
void gameplay_input(void);
void demo_input(void);
void setGameTimer(void);

#define BALL_VELOCITY toFIXED(10)
extern int ball_velocity;
extern bool ball_falling;
extern bool ball_bounce;
bool drop_ball_animation(Sprite *ball);

static __jo_force_inline void initPixelPoppy(void) {
    reset_ball_movement(&pixel_poppy);
    sprite_frame_reset(&pixel_poppy);
    pixel_poppy.isColliding = false;
    g_Game.isGoalScored = false;
}
static __jo_force_inline void setItemPositions(void) {
    set_item_position(&bomb_item);
    set_item_position(&fishtank_item);
    set_item_position(&shroom_item);
    set_item_position(&garfield_item);
    set_item_position(&craig_item);
}
static inline void draw_star_element(Sprite *sprite, Uint8 num, int x, int y, int offset) {
    for (int i = 0; i < num; i++) {
        set_spr_position(sprite, x, y, 90);
        my_sprite_draw(sprite);
        x += offset;
    }
}
static inline void draw_heart_element(Sprite *sprite, PPLAYER player, int x, int y, int offset) {
    for (int i = 0; i < player->numLives; i++) {
        if (player->numLives > player->totalLives) {
            sprite->spr_id = sprite->anim1.asset[1];
	    player->numLives--;
        }
        else {
	    sprite->spr_id = sprite->anim1.asset[0];
	}
        set_spr_position(sprite, x, y, 90);
        my_sprite_draw(sprite);
        x += offset;
    }
    if (player->numLives < player->totalLives) {
	sprite->spr_id = sprite->anim1.asset[2];
	for (int i = 0; i < (player->totalLives - player->numLives); i++) {
	    set_spr_position(sprite, x, y, 90);
	    my_sprite_draw(sprite);
	    x += offset;
	}
   }
}

static __jo_force_inline void storymodeScore_draw(PPLAYER player) {
    switch (player->teamChoice) 
    {
        case TEAM_1: {
            jo_nbg0_printf(6, 2, "%09d", player->score.points);
            jo_nbg0_printf(15, 2, "*%i", touchedBy[0].touchCount);
            break;
        }
        default:
            break;
    }
}

static __jo_force_inline void gameplayScore_draw(PPLAYER player) {
    switch (player->teamChoice) 
    {
        case TEAM_1: {
            jo_nbg0_printf(6, 2, "%09d", player->score.points);
            jo_nbg0_printf(15, 2, "*%i", touchedBy[player->playerID].touchCount);
            break;
        }
        case TEAM_2: {
            jo_nbg0_printf(27, 2, "%09d", player->score.points);
            jo_nbg0_printf(36, 2, "*%i", touchedBy[player->playerID].touchCount);
            break;
        }
        case TEAM_3: {
            jo_nbg0_printf(6, 26, "%09d", player->score.points);
            jo_nbg0_printf(15, 26, "*%i", touchedBy[player->playerID].touchCount);
            break;
        }
        case TEAM_4: {
            jo_nbg0_printf(27, 26, "%09d", player->score.points);
            jo_nbg0_printf(36, 26, "*%i", touchedBy[player->playerID].touchCount);
            break;
        }
        default:
            break;
    }
}

static __jo_force_inline void drawGoals(PPLAYER player) {
    if (player->subState == PLAYER_STATE_DEAD) {
        return;
    }    
        // Set goal asset by team selection
    Uint8 i = player->teamChoice-1;
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
    
        // TODO: make a subfunction
    // top
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

    // middle
    goal[i].spr_id = goal[i].anim1.asset[1];
    goal[i].zmode = mid_zmode;
    set_spr_position(&goal[i], x_position, y_position_mid, 120);
    set_spr_scale(&goal[i], 2, goal_scale);
    my_sprite_draw(&goal[i]);
    // shadow
    goal[i].spr_id = goal[i].anim1.asset[3];
    set_spr_position(&goal[i], x_position+2, y_position_mid+2, 125);
    my_sprite_draw(&goal[i]);

    // bottom
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
        // TODO: minutes/seconds (subfunction)
        my_sprite_draw(&timer_num100); // tens
        my_sprite_draw(&timer_num10); // tens
        my_sprite_draw(&timer_num1);  // ones
        
        drawPlayers();
        drawGameItems();
        // don't draw until poppy is reset
        if (!g_Game.isGoalScored && !g_Game.isRoundOver) {
            if (g_Game.isBallActive) {
                update_ball(&pixel_poppy);
            }
            my_sprite_draw(&pixel_poppy);
            return;
        }
        if (g_Game.isGoalScored) {
            initPixelPoppy();
            resetPlayerAttacks();
            start_gameplay_timer = false;
            g_Game.isBallActive = false;
            g_Game.isActive = false;
            g_Game.BeginTimer = 0;
            // g_Game.countofRounds++; // ??
            setItemPositions(); // reset items after each goal
        }
}
static __jo_force_inline void drawClassicMode(void) {
        // SPRITES
        // TODO: minutes/seconds (subfunction)
        my_sprite_draw(&timer_num100); // tens
        my_sprite_draw(&timer_num10); // tens
        my_sprite_draw(&timer_num1);  // ones
        
        drawPlayers();        
        // don't draw until poppy is reset
        if (!g_Game.isGoalScored && !g_Game.isRoundOver) {
            if (g_Game.isBallActive) {
                update_ball(&pixel_poppy);
            }
            my_sprite_draw(&pixel_poppy);
            return;
        }
        if (g_Game.isGoalScored) {
            initPixelPoppy();
            resetPlayerAttacks();
            start_gameplay_timer = false;
            g_Game.isBallActive = false;
            g_Game.isActive = false;
            g_Game.BeginTimer = 0;
        }
}

static __jo_force_inline void drawGameUI(void) {
    for(unsigned int i = 0; i <= g_Game.numPlayers; i++)
    {
        PPLAYER player = &g_Players[i];
        if (player->objectState == OBJECT_STATE_INACTIVE) {
            continue;
        }
        if (player->shield.activate) {
            set_shield_position(player->_sprite, &shield[i], player->shield_pos);
            looped_animation_pow(&shield[i], 4);
            my_sprite_draw(&shield[i]);
        }
        looped_animation_pow(player->_sprite, 4); // TODO: change animations based on player input
        player->_portrait->spr_id = player->_portrait->anim1.asset[player->character.choice];
        gameplayUI_draw(player);
        drawGoals(player);
    }
    // // draw horizontal bars
    // if (g_GameOptions.mesh_display) {
        // set_spr_scale(&menu_bg1, 260, 20);
        // set_spr_position(&menu_bg1, 0, -192, MENU_BG2_DEPTH);
        // my_sprite_draw(&menu_bg1);
        // if (g_Game.currentNumPlayers > 2) {
            // if (g_Game.currentNumPlayers == 3) {
                // set_spr_scale(&menu_bg1, 106, 20);
                // set_spr_position(&menu_bg1, -154, 192, MENU_BG2_DEPTH);
            // }
            // else {
                // set_spr_position(&menu_bg1, 0, 192, MENU_BG2_DEPTH);
            // }
            // my_sprite_draw(&menu_bg1);
        // }
    // }
}

static __jo_force_inline bool startGameplay(void) {
    if (g_Game.isBallActive) {
        if (!g_GameOptions.testCollision) {
            start_ball_movement(&pixel_poppy);
        }
        initTouchCounter(0);
        g_Game.isRoundOver = false;
        g_Game.isGoalScored = false;
        return false;
    }
    return true;
}

