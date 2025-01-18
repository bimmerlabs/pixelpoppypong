#include "main.h"
#include "state.h"
#include "assets.h"
#include "gameplay.h"
#include "ppp_logo.h"
#include "title_screen.h"
#include "screen_transition.h"
#include "team_select.h"
#include "objects/player.h"
#include "audio.h"
// #include "gameplay.h"

Uint16 state_fade_timer = STATE_FADE_TIMER;

// transistions between game states
void changeState(GAME_STATE newState)
{
    
    switch(newState)
    {
        case GAME_STATE_PPP_LOGO:
        {        
            pppLogo_init();
            volume = MAX_VOLUME;
            jo_audio_set_volume(volume);
            playCDTrack(LOGO_TRACK, false);
            g_Game.gameState = GAME_STATE_PPP_LOGO;
            jo_core_tv_on();
            break;
        }
        case GAME_STATE_TITLE_SCREEN:
        {
            g_Game.nextState = GAME_STATE_TITLE_SCREEN;
            titleScreen_init();
            reset_audio(HALF_VOLUME);
            playCDTrack(TITLE_TRACK, true);
            g_Game.gameState = g_Game.nextState;
            break;
        }
        case GAME_STATE_TITLE_MENU:
        {
            titleMenu_init();
            g_Game.nextState = GAME_STATE_TITLE_MENU;
            if (g_Game.gameState == GAME_STATE_GAMEPLAY || g_Game.gameState == GAME_STATE_TEAM_SELECT) {
                reset_audio(HALF_VOLUME);
                playCDTrack(TITLE_TRACK, true);
            }
            g_Game.gameState = g_Game.nextState;
            break;
        }
        case GAME_STATE_TITLE_OPTIONS:
        {
            optionsScreen_init();
            g_Game.gameState = GAME_STATE_TITLE_OPTIONS;
            break;
        }
        case GAME_STATE_DEMO_LOOP:
        {
            g_Game.nextState = GAME_STATE_DEMO_LOOP;
            demo_init();
            gameplay_init();
            g_Game.gameState = g_Game.nextState;
            break;
        }
        case GAME_STATE_CHARACTER_PORTRAITS:
        {
            changeState(GAME_STATE_UNINITIALIZED);
            break;
        }
        case GAME_STATE_TEAM_SELECT:
        {
            reset_audio(HALF_VOLUME);
            teamSelect_init();
            playCDTrack(SELECT_TRACK, true);
            g_Game.gameState = GAME_STATE_TEAM_SELECT;
            break;
        }
        case GAME_STATE_GAMEPLAY:
        {
            g_Game.nextState = GAME_STATE_GAMEPLAY;
            gameplay_init();
            g_Game.gameState = g_Game.nextState;
            break;
        }
        case GAME_STATE_CREDITS:
        {
            changeState(GAME_STATE_UNINITIALIZED);
            break;
        }
        case GAME_STATE_UNINITIALIZED:
        {
            jo_core_tv_off();
            // jo_set_default_background_color(JO_COLOR_Black);
            // jo_set_displayed_screens(JO_BACK_SCREEN);
            jo_sprite_free_all();
            g_Game.isLoading = true;
            loadCommonAssets();
            frame = 1;
            reset_audio(MAX_VOLUME);
            // slColOffsetOn(OFF);
            mosaic_in_rate = MOSAIC_SLOW_RATE;
            mosaic_in = false;
            fade_in = false;
            transition_in = false;
            mosaic_out = false;
            fade_out = false;
            transition_out = false;
            music_out = false;
            music_in = false;
            reset_inputs();
            g_Game.lastState = GAME_STATE_UNINITIALIZED;
            g_Game.nextState = GAME_STATE_PPP_LOGO;
            changeState(g_Game.nextState);
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

    // transition in has to be false
    mosaic_in = false;
    music_in = false;
    fade_in = false;
    transition_in = false;
    
    if (game_options.mosaic_display) {
        mosaic_out = true;
    }
    music_out = true;
    fade_out = true;
    transition_out = true;
    
    screenTransition_init(0, 0, 0);
    
    
    if (!game_options.debug_display) {
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

void game_state_update(void)
{
    if(g_Game.gameState != GAME_STATE_TRANSITION)
    {
        return;
    }
    
    if (!transition_out) {
        changeState(g_Game.nextState);
    }
}

