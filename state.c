#include "main.h"
#include "state.h"
#include "gameplay.h"
#include "ppp_logo.h"
#include "title_screen.h"
#include "screen_transition.h"
#include "team_select.h"
#include "objects/player.h"
// #include "audio.h"
// #include "gameplay.h"

bool transition_complete = false;
Uint16 state_fade_timer = STATE_FADE_TIMER;

// transistions between game states
void changeState(GAME_STATE newState)
{
    if (cd_is_playing) {
        jo_audio_stop_cd();
        volume = MIN_VOLUME;
        jo_audio_set_volume(volume);
        cd_is_playing = false;
    }
    
    transition_complete = false;
    
    switch(newState)
    {
        case GAME_STATE_PPP_LOGO:
        {
            volume = MAX_VOLUME;
            jo_audio_set_volume(volume);
            playCDTrack(LOGO_TRACK, false);
            screenTransition_init(-254, -254, -254);
            pppLogo_init();
            g_Game.gameState = GAME_STATE_PPP_LOGO;
            break;
        }
        case GAME_STATE_TITLE_SCREEN:
        {
            // volume = MAX_VOLUME;
            // jo_audio_set_volume(volume);
            // playCDTrack(TITLE_TRACK);
            screenTransition_init(-254, -254, -254);
            titleScreen_init();
            g_Game.gameState = GAME_STATE_TITLE_SCREEN;
            break;
        }
        case GAME_STATE_TITLE_MENU:
        {
            volume = HALF_VOLUME;
            // jo_audio_set_volume(volume);
            // titleScreen_init();
            g_Game.gameState = GAME_STATE_TITLE_MENU;
            break;
        }
        case GAME_STATE_DEMO_LOOP:
        {
            // volume = MAX_VOLUME;
            // jo_audio_set_volume(volume);
            // playCDTrack(DEMO_TRACK);
            g_Game.nextState = GAME_STATE_DEMO_LOOP;
            initDemoPlayers();
            gameplay_init();
            demo_init();
            g_Game.gameState = g_Game.nextState;
            break;
        }
        case GAME_STATE_GAMEPLAY:
        {
            // volume = MAX_VOLUME;
            // jo_audio_set_volume(volume);
            // jo_audio_stop_cd();
            // playCDTrack(GAME_TRACK);
            g_Game.nextState = GAME_STATE_GAMEPLAY;
            gameplay_init();
            g_Game.gameState = g_Game.nextState;

            break;
        }
        case GAME_STATE_TEAM_SELECT:
        {
            volume = INITIAL_STATE_VOLUME;
            jo_audio_set_volume(volume);
            playCDTrack(SELECT_TRACK, true);
            teamSelect_init();
            g_Game.gameState = GAME_STATE_TEAM_SELECT;
            break;
        }
        case GAME_STATE_UNINITIALIZED:
        {
            jo_core_tv_off();
            frame = 1;
            volume = MAX_VOLUME;
            jo_audio_set_volume(volume);
            slColOffsetOn(OFF);
            jo_set_default_background_color(JO_COLOR_Black);
            jo_set_displayed_screens(JO_BACK_SCREEN);
            changeState(GAME_STATE_PPP_LOGO);
            jo_core_tv_on();
            break;
        }
        default:
        {
            return;
        }
    }
}

void transitionState(GAME_STATE newState)
{
    g_Game.gameState = GAME_STATE_TRANSITION;
    g_Game.nextState = newState;
    g_TransitionTimer = 0;
    
    screenTransition_init(0, 0, 0);
    
    if (!debug_display) {
        slColOffsetOn(NBG0ON | NBG1ON | SPRON);
        slColOffsetAUse(NBG0ON);
        slColOffsetBUse(NBG1ON | SPRON);
        slColOffsetA(nbg0_rate, nbg0_rate, nbg0_rate);
    }
    else {
        slColOffsetOn(NBG1ON | SPRON);
        slColOffsetAUse(OFF);
        slColOffsetBUse(NBG1ON | SPRON);
        slColOffsetB(nbg1_rate, nbg1_rate, nbg1_rate);
    }
}

// screen transition effects (fade etc)
void transition_draw(void)
{
    if(g_Game.gameState != GAME_STATE_TRANSITION)
    {
        return;
    }
    
    // fade out - maybe make a fade in state?
    transition_complete = fadeOut(fade_rate, MINIMUM_FADE);
        
    volume--;
    if (volume < MIN_VOLUME) {
    volume = MIN_VOLUME;
    }
    jo_audio_set_volume(volume);            

    if (volume == MIN_VOLUME) {
    changeState(g_Game.nextState);
    }
}

// // old
// void transition_draw(void)
// {
    // if(g_Game.gameState != GAME_STATE_TRANSITION)
    // {
        // return;
    // }
    
    // screenTransition(8, 4, 2, state_fade_timer);
    // jo_nbg0_printf(17, 14, "PATIENCE..."); // not really..
    // g_TransitionTimer++;
    
    // if (g_TransitionTimer > TRANSITION_TIMER)
    // {
        // slColOffsetOn(OFF);
        // changeState(g_Game.nextState);
    // }
// }

