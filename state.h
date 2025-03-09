#pragma once

#define TRANSITION_TIMER (3 * 60)
#define STATE_FADE_TIMER (1 * 60)

extern unsigned int g_TransitionTimer;
extern unsigned int g_AttractScreenState;

// each game state has it's own input/update/draw routines
typedef enum
{
    GAME_STATE_UNINITIALIZED = 0,
    GAME_STATE_PPP_LOGO,
    GAME_STATE_TITLE_SCREEN,
    GAME_STATE_TITLE_MENU,
    GAME_STATE_TITLE_OPTIONS,
    GAME_STATE_TEAM_SELECT,
    GAME_STATE_GAMEPLAY,
    // GAME_STATE_NAME_ENTRY,
    GAME_STATE_DEMO_LOOP,
    GAME_STATE_CREDITS,
    GAME_STATE_HIGHSCORES,
    GAME_STATE_CHARACTER_PORTRAITS,
    GAME_STATE_TRANSITION,
} GAME_STATE;

typedef enum
{
    ATTRACT_SCREEN_1 = GAME_STATE_DEMO_LOOP,
    ATTRACT_SCREEN_2 = GAME_STATE_CREDITS,
    ATTRACT_SCREEN_3 = GAME_STATE_HIGHSCORES,
    ATTRACT_SCREEN_MAX = ATTRACT_SCREEN_3+1,
} ATTRACT_SCREEN_STATE;

void changeState(GAME_STATE newState);

void transitionState(GAME_STATE newState);

void game_state_update(void);

void attract_screen_state(void);