#pragma once

#include <jo/jo.h>
#include "screen_transition.h"
#include "assets.h"
#include "sprites.h"
#include "physics.h"
#include "team_select.h"
#include "objects/player.h"
#include "BG_DEF/sprite_colors.h"

#define ROUND_BEGIN_TIME_NORMAL (6 * 60)
#define DROP_BALL_TIME_NORMAL (4 * 60) // MAYBE THIS VARIES BASED ON SCORE OR EXPLODING?
#define ROUND_BEGIN_TIME_FAST (3.5 * 60)
#define DROP_BALL_TIME_FAST (1.5 * 60) // MAYBE THIS VARIES BASED ON SCORE OR EXPLODING?
#define DEMO_TIME (30 * 60)
#define BOMB_TIMER (15 * 60)
#define TIMEOUT_BATTLE 999 // seconds
#define TIMEOUT_CLASSIC 199
#define TIMEOUT_STORY_EASY 599
#define TIMEOUT_STORY_MEDIUM 399
#define TIMEOUT_STORY_HARD 349

#define SHROOM_TIMER (15 * 60)

#define BALL_TOUCH_TIMEOUT (5 * 60)
#define GAME_END_DELAY_TIMEOUT (5 * 60)

#define MAX_ROUNDS 8

#define COLOR_MULTIPLIER 5
#define POWER_METER_HEIGHT 3
    
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

// static bool draw_bomb;
// static bool explode_bomb;

typedef enum _GAME_ITEMS
{
    GAME_ITEM_BOMB = 0,
    GAME_ITEM_FISH,
    GAME_ITEM_SHROOM,
    GAME_ITEM_GARF,
    GAME_ITEM_CRAIG,
    GAME_ITEM_MAX,
} GAME_ITEMS;

typedef struct {
    unsigned int id;
    unsigned int timer[MAX_PLAYERS];
    Sprite *_sprite;
} Item;
extern Item g_item;

// static unsigned int currentItem;

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
static inline void draw_heart_element(Sprite *sprite, PPLAYER player, int x, int y, int offset) {
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
    }    // Set goal asset by team selection
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
                goal_scale = g_Game.goalScale;
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
                goal_scale = GOAL_SCALE_VS_MODE;            
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
    
    drawGoalSprites(&goal[i], 0, 2, top_zmode, top_flip, x_position, y_position_top, 2);
    drawGoalSprites(&goal[i], 1, 3, mid_zmode, top_flip, x_position, y_position_mid, goal_scale);
    drawGoalSprites(&goal[i], 0, 2, bot_zmode, bot_flip, x_position, y_position_bot, 2);
}

static float item_scale = 0.1;
static float item_velocity = 0.0;

static __jo_force_inline void setItemPositions(void) {
    if (g_GameOptions.debug_mode || !g_GameOptions.enableItems) {
        // g_item.id = GAME_ITEM_BOMB;
        // g_item.id = GAME_ITEM_FISH;
        // g_item.id = GAME_ITEM_SHROOM;
        // g_item.id = GAME_ITEM_GARF;
        // g_item.id = GAME_ITEM_CRAIG;
        g_item.id = GAME_ITEM_MAX;
    }
    else {
        // TODO: implement weighted average random selection
        g_item.id = my_random_range(GAME_ITEM_BOMB, GAME_ITEM_MAX);
        if (g_item.id >= GAME_ITEM_MAX) {
            switch(g_Game.gameDifficulty)
            {
                case GAME_DIFFICULTY_EASY:
                    g_item.id = GAME_ITEM_FISH;
                    break;
                case GAME_DIFFICULTY_MEDIUM:
                    g_item.id = GAME_ITEM_SHROOM;
                    break;
                case GAME_DIFFICULTY_HARD:
                    g_item.id = GAME_ITEM_BOMB;
                    break;
                default:
                    g_item.id = GAME_ITEM_BOMB;
                    break;
            }
        }
    }
    item_scale = 0.1;
    item_velocity = 0.0;
    switch (g_item.id) {
        case GAME_ITEM_BOMB:
            g_Game.explodeBomb = false;
            g_Game.BombTimer = BOMB_TIMER;
            g_item._sprite = &bomb_item;
            bomb_item.visible = true;
            sprite_frame_reset(&bomb_item);
            set_item_position(&bomb_item);
            set_spr_scale(&bomb_item, item_scale, item_scale);
            break;
        case GAME_ITEM_FISH:
            g_item._sprite = &fishtank_item;
            fishtank_item.visible = true;
            set_item_position(&fishtank_item);
            set_spr_scale(&fishtank_item, item_scale, item_scale);
            break;
        case GAME_ITEM_SHROOM:
            g_item._sprite = &shroom_item;
            shroom_item.visible = true;
            set_item_position(&shroom_item);
            set_spr_scale(&shroom_item, item_scale, item_scale);
            break;
        case GAME_ITEM_GARF:
            g_item._sprite = &garfield_item;
            garfield_item.visible = true;
            set_item_position(&garfield_item);
            set_spr_scale(&garfield_item, item_scale, item_scale);
            break;
        case GAME_ITEM_CRAIG:
            g_item._sprite = &craig_item;
            craig_item.visible = true;
            set_item_position(&craig_item);
            set_spr_scale(&craig_item, item_scale, item_scale);
            break;
        default:
            g_item._sprite = &fishtank_item;
            g_item._sprite->visible = false;
            break;
    }
    g_item._sprite->isColliding = false;
}

static __jo_force_inline void drawGameItems(void) {
    switch (g_item.id) {
        case GAME_ITEM_BOMB:
            if (item_scale < 2.0) {
                if (item_velocity < 0.5) {
                    item_velocity += 0.01;
                }
                item_scale += item_velocity;
                set_spr_scale(&bomb_item, item_scale, item_scale);
            }
            if (g_Game.explodeBomb) {
                if (explosion_flash) {
                    explosion_flash = explosionEffect();
                }
                g_Game.explodeBomb = explode_animation(&bomb_item);
                if (g_Game.explodeBomb == false) {
                    bomb_item.visible = false;
                }
            }
            else {
                looped_animation_pow(&bomb_item, 4);
            }
            break;
        case GAME_ITEM_FISH:
            if (item_scale < 2.0) {
                if (item_velocity < 0.5) {
                    item_velocity += 0.01;
                }
                item_scale += item_velocity;
                set_spr_scale(&fishtank_item, item_scale, item_scale);
            }
            looped_animation_pow(&fishtank_item, 8);
            break;
        case GAME_ITEM_SHROOM:
            if (item_scale < 2.0) {
                if (item_velocity < 0.5) {
                    item_velocity += 0.01;
                }
                item_scale += item_velocity;
                set_spr_scale(&shroom_item, item_scale, item_scale);
            }
            looped_animation_pow(&shroom_item, 4);
            hsl_incSprites.h += 2; // TODO: replace with one just for the mushroom?
            do_update_shroom = true;
            break;
        case GAME_ITEM_GARF:
            if (item_scale < 1.0) {
                if (item_velocity < 0.5) {
                    item_velocity += 0.01;
                }
                item_scale += item_velocity;
                set_spr_scale(&garfield_item, item_scale, item_scale);
            }
            break;
        case GAME_ITEM_CRAIG:
            if (item_scale < 1.6) {
                if (item_velocity < 0.5) {
                    item_velocity += 0.01;
                }
                item_scale += item_velocity;
                set_spr_scale(&craig_item, item_scale, item_scale);
            }
            break;
        default:
            g_item._sprite->visible = false;
            break;    }
    if (g_item._sprite->visible) {
        my_sprite_draw(g_item._sprite);
    }
}

static __jo_force_inline void handlePlayerItemCollision(PPLAYER player) {
    switch (g_item.id) {
        case GAME_ITEM_BOMB:
            g_Game.explodeBomb = true;
            explosion_flash = true;
            if (!player->isExploded) {
                player->isExploded = explodePLayer(player);
            }
            break;
        case GAME_ITEM_FISH:
            pcm_play(g_Assets.bloopPcm8, PCM_PROTECTED, 7);
            if (player->numLives < player->totalLives) {
                player->numLives = player->totalLives;
            }
            else if (player->numLives < (player->totalLives * 2)) {
                player->numLives = player->numLives + 1;
            }
            if (player->numLives > (player->totalLives * 2)) {
                player->numLives = (player->totalLives * 2);
            }
            fishtank_item.visible = false;
            break;
        case GAME_ITEM_SHROOM:
            shroom_item.visible = false;
            Uint16 shroom_angle = hslSprites.color[p_rangeShroom.lower].h;
            g_item.timer[player->playerID] = SHROOM_TIMER;
            // affect the player depending on the color of the shroom
            if ((shroom_angle > 0 && shroom_angle <= 90) || (shroom_angle > 270 && shroom_angle <= 360)) {
                pcm_play(g_Assets.growPcm8, PCM_PROTECTED, 7);
                player->_sprite->pos.r = PLAYER_RADIUS_LARGE;
                player->isBig = true;
                player->isSmall = false;
            }
            if (shroom_angle > 90 && shroom_angle <= 270) {
                pcm_play(g_Assets.shrinkPcm8, PCM_PROTECTED, 7);
                player->_sprite->pos.r = PLAYER_RADIUS_SMALL;
                player->isBig = false;
                player->isSmall = true;
            }
            break;
        case GAME_ITEM_GARF:
            garfield_item.visible = false;
            // "i hate mondays" etc
            pcm_play(g_Assets.chain5Pcm8, PCM_PROTECTED, 7);
            if (touchedBy[player->playerID].touchCount == 0) {
                touchedBy[player->playerID].touchCount = 20;
            }
            else {
                touchedBy[player->playerID].touchCount *= 2;
                if (touchedBy[player->playerID].touchCount > 99) {
                    touchedBy[player->playerID].touchCount = 99;
                }
            }
            break;
        case GAME_ITEM_CRAIG:
            craig_item.visible = false;
            // "nice shot"
            pcm_play(g_Assets.stadlerPcm8, PCM_PROTECTED, 7);
            player->score.points += 100000;
            break;
        default:
            g_item._sprite->visible = false;
            break;
    }
}
static __jo_force_inline void drawVsMode(void) {
        // SPRITES
        // TODO: minutes/seconds (subfunction)
        my_sprite_draw(&timer_num100); // tens
        my_sprite_draw(&timer_num10); // tens
        my_sprite_draw(&timer_num1);  // ones
        
        drawPlayers();
        if (g_Game.isActive) {
            drawGameItems();
        }
        // don't draw until poppy is reset
        if (!g_Game.isGoalScored && !g_Game.isRoundOver) {
            if (g_Game.isBallActive) {
                update_ball(&pixel_poppy);
                g_item._sprite->isColliding = checkItemDistance(&pixel_poppy, g_item._sprite);
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
            start_gameplay_timer = false;
            g_Game.isBallActive = false;
            g_Game.isActive = false;
            g_Game.BeginTimer = 0;
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


