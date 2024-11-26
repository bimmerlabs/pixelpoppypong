#pragma once

#define TRANSITION_TIMER (3 * 60)
#define STATE_FADE_TIMER (1 * 60)

extern bool transition_complete;
extern unsigned int g_TransitionTimer;
// various game states
// each game state has it's own input/update/draw routines
typedef enum
{
    GAME_STATE_UNINITIALIZED = 0,
    GAME_STATE_PPP_LOGO,
    GAME_STATE_TITLE_SCREEN,
    GAME_STATE_TITLE_MENU,
    GAME_STATE_DEMO_LOOP,
    GAME_STATE_TEAM_SELECT,
    GAME_STATE_GAMEPLAY,
    GAME_STATE_CREDITS,
    GAME_STATE_TRANSITION,
} GAME_STATE;

void changeState(GAME_STATE newState);

void transitionState(GAME_STATE newState);

void transition_draw(void);
