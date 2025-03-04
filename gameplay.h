#pragma once

#include <jo/jo.h>
#include "team_select.h"
#include "objects/player.h"

// #define ROUND_OVER_TIME (4 * 60)

#define GAME_BEGIN_TIME (3 * 60)
#define DEMO_TIME (30 * 60)
#define TIMEOUT 99 // seconds - MAYBE MAKE AN OPTION?

#define GOAL_TOP toFIXED(-100.0)
#define GOAL_BOTTOM toFIXED(100.0)
extern PLAYER g_Players[MAX_PLAYERS];

extern ANGLE ball_rotation;
extern unsigned int g_DemoTimer;
extern bool start_timer;
extern bool round_start;
extern bool g_RoundOver;
extern Uint16 g_GameBeginTimer;
extern Uint16 g_RoundOverTimer;
void gameplay_init(void);
void demo_init(void);
void demo_update(void);
void game_timer(void);
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
static inline void draw_ui_element(Sprite *sprite, Uint8 num, int x, int y, int offset) {
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

static inline void gameplayScores_draw(PPLAYER player) {
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