#pragma once

#include <jo/jo.h>
#include "physics.h"
#include "team_select.h"
#include "../core/screen_transition.h"
#include "../core/assets.h"
#include "../core/sprites.h"
#include "../objects/player.h"
#include "../objects/goal.h"
#include "../objects/items.h"
#include "../palettefx/sprite_colors.h"

#define ROUND_BEGIN_TIME_NORMAL (6 * 60)
#define DROP_BALL_TIME_NORMAL (4 * 60)
#define ROUND_BEGIN_TIME_FAST (3.5 * 60)
#define DROP_BALL_TIME_FAST (1.5 * 60)
#define DEMO_TIME (45 * 60)
#define TIMER_MAX 599 // seconds
#define TIMEOUT_BATTLE 599 // seconds
#define TIMEOUT_CLASSIC 240 //
#define TIMEOUT_STORY_EASY 300
#define TIMEOUT_STORY_MEDIUM 180
#define TIMEOUT_STORY_HARD 150

#define BALL_TOUCH_TIMEOUT (5 * 60)
// #define TRANSITION_DELAY_TIMEOUT (8 * 60)
#define GAME_END_DELAY_TIMEOUT (5 * 60)
#define LIFE_COUNT_DELAY_TIMEOUT (2 * 60)
#define NEXT_BATTLE_DELAY_TIMEOUT (1 * 60)
#define WIN_GAME_DELAY_TIMEOUT (1.5 * 60)

#define MAX_ROUNDS 8

#define COLOR_MULTIPLIER 5
#define POWER_METER_HEIGHT 3
    
#define GAMEPLAY_PORTRAIT_X 300
#define GAMEPLAY_PORTRAIT_Y 190
#define GAMEPLAY_PORTRAIT_Y 190

extern PLAYER g_Players[MAX_PLAYERS];

typedef struct _GAMEPLAY
{
    Uint16 GameTimer;
    // Uint16 RoundOverTimer; // not currently used
    Uint16 DemoTimer;
    bool isGameOver;  // not currently used
    
    bool draw_demo_text;
    bool start_gameplay_timer;
    bool round_start;
} GAMEPLAY, *PGAMEPLAY;

// globals
extern GAMEPLAY g_Gameplay;

void gameplay_init(void);
void demo_init(void);
void demo_update(void);

void gameplay_timer(void);
void gameplay_draw(void);
int determineWinner(void);
void gameScore_draw(void);
void gameplayUI_draw(PPLAYER player);
void gameplay_update(void);
void reset_ball_movement(Sprite *ball);
void gameplay_input(void);
void demo_input(void);
void setGameTimer(void);
void drawGameTimer(void);

#define BALL_VELOCITY toFIXED(10)
extern int ball_velocity;
extern bool ball_falling;
extern bool ball_bounce;
bool drop_ball_animation(Sprite *ball);

static __jo_force_inline void initPixelPoppy(void) {
    ball_animation_reset(&pixel_poppy);
    reset_ball_movement(&pixel_poppy);
    pixel_poppy.isColliding = false;
    g_Game.isGoalScored = false;
}static inline void draw_star_element(Sprite *sprite, Uint8 num, int x, int y, int offset) {
    for (int i = 0; i < num; i++) {
        set_spr_position(sprite, x, y, 90);
        my_sprite_draw(sprite);
        x += offset;
    }
}
static __jo_force_inline void draw_heart_element(Sprite *sprite, PPLAYER player, int x, int y, int offset) {
    int lives = player->numLives;
    for (int i = 0; i < player->numLives; i++) {
        set_spr_position(sprite, x, y, 90);
        if (lives > player->totalLives) {
            sprite->spr_id = sprite->anim1.asset[1];
            x += offset;
            my_sprite_draw(sprite);
        }
        else if (player->numLives - player->totalLives < lives) {
	    sprite->spr_id = sprite->anim1.asset[0];
            x += offset;
            my_sprite_draw(sprite);  
	}
        lives--;
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
    if (player->subState == PLAYER_STATE_DEAD) {
        return;
    }
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
static __jo_force_inline void drawVsMode(void) {
        // SPRITES        
        drawPlayers();
        if (g_Game.isActive) {
            drawGameItems();
        }
        // don't draw until poppy is reset
        if (!g_Game.isGoalScored && !g_Game.isRoundOver) {
            if (g_Game.isBallActive) {
                update_ball(&pixel_poppy);
                // g_item._sprite->isColliding = checkItemDistance(&pixel_poppy, g_item._sprite);
                // TODO: handle collision
                // if (g_item._sprite->isColliding) {
                    // handleItemCollision()
                // }
            }
            my_sprite_draw(&pixel_poppy);
            return;
        }
        if (g_Game.isGoalScored) {         
            initPixelPoppy();
            resetPlayerAttacks();
            g_Gameplay.start_gameplay_timer = false;
            g_Game.isBallActive = false;
            g_Game.isActive = false;
            g_Game.BeginTimer = 0;
        }
}
static __jo_force_inline void drawClassicMode(void) {
        // SPRITES        
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
            g_Gameplay.start_gameplay_timer = false;
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
    }
    drawGameTimer();
    drawGoals();
}

static __jo_force_inline bool startGameplay(void) {
    if (g_Game.isBallActive) {
        if (!g_GameOptions.testCollision) {
            start_ball_movement(&pixel_poppy);
        }
        else {
            g_Game.isActive = true;
        }
        setItemPositions();
        initTouchCounter(0);
        g_Game.isRoundOver = false;
        g_Game.isGoalScored = false;
        return false;
    }
    return true;
}


