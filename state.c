#include "main.h"
#include "state.h"
#include "input.h"
#include "gameplay.h"
#include "ppp_logo.h"
#include "title_screen.h"
#include "screen_transition.h"
// #include "team_select.h"
// #include "gameplay.h"
// #include "audio.h"

//#include "assets/audio.h"

Uint16 state_fade_timer = STATE_FADE_TIMER;

// transistions between game states
void transitionState(GAME_STATE newState)
{
    switch(newState)
    {
        case GAME_STATE_PPP_LOGO:
        {
            // playCDTrack(TITLE_TRACK);
            screenTransition_init(-254, -254, -254);
            pppLogo_init();
            g_Game.gameState = GAME_STATE_PPP_LOGO;
            break;
        }
        case GAME_STATE_TITLE_SCREEN:
        {
            // if coming from PPP screen, don't restart the audio
            if(g_Game.gameState != GAME_STATE_PPP_LOGO)
            {
                // playCDTrack(TITLE_TRACK);
            }
            titleScreen_init();
            g_Game.gameState = GAME_STATE_TITLE_SCREEN;
            break;
        }
        case GAME_STATE_TITLE_MENU:
        {
            // titleScreen_init();
            g_Game.gameState = GAME_STATE_TITLE_MENU;
            break;
        }
        case GAME_STATE_DEMO_LOOP:
        {
            // playCDTrack(DEMO_TRACK);
            gameplay_init();
            demo_init();
            g_Game.gameState = GAME_STATE_DEMO_LOOP;
            break;
        }
        case GAME_STATE_GAMEPLAY:
        {
            // jo_audio_stop_cd();
            // playCDTrack(GAME_TRACK);
            gameplay_init();
            g_Game.gameState = GAME_STATE_GAMEPLAY;

            break;
        }
        case GAME_STATE_TEAM_SELECT:
        {
            teamSelect_init();
            g_Game.gameState = GAME_STATE_TEAM_SELECT;
            break;
        }
        case GAME_STATE_UNINITIALIZED:
        {
            jo_core_tv_off();
            frame = 1;
            slColOffsetOn(OFF);
            jo_set_default_background_color(JO_COLOR_Black);
            jo_set_displayed_screens(JO_BACK_SCREEN);
            transitionState(GAME_STATE_PPP_LOGO);
            jo_core_tv_on();
            break;
        }
        default:
        {
            return;
        }
    }
}

// do screen transition effects here (fade etc)
void changeState(GAME_STATE newState)
{
    g_Game.gameState = GAME_STATE_TRANSITION;
    g_Game.nextState = newState;
    g_TransitionTimer = 0;
    
    screenTransition_init(-254, -254, -254);
    
    jo_set_default_background_color(JO_COLOR_Black);
    jo_set_displayed_screens(JO_NBG0_SCREEN);
    slColOffsetOn(NBG0ON | NBG1ON);
    slColOffsetAUse(NBG0ON);
    slColOffsetBUse(NBG1ON);
    slColOffsetA(nbg0_rate, nbg0_rate, nbg0_rate);
    slColOffsetB(nbg1_rate, nbg1_rate, nbg1_rate);
}

// do screen transition effects here (fade etc)
void transition_draw(void)
{
    if(g_Game.gameState != GAME_STATE_TRANSITION)
    {
        return;
    }
    
    screenTransition(8, 4, 2, state_fade_timer);
    jo_nbg0_printf(17, 14, "LOADING..."); // not really..
    g_TransitionTimer++;
    
    if (g_TransitionTimer > TRANSITION_TIMER)
    {
        slColOffsetOn(OFF);
        transitionState(g_Game.nextState);
    }
}
