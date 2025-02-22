#include <jo/jo.h>
#include "main.h"
#include "assets.h"
#include "state.h"
#include "ppp_logo.h"
#include "screen_transition.h"
#include "BG_DEF/nbg1.h"

unsigned int g_LogoTimer = 0;
int transparency_rate = TRANSPARENCY_MAX;
Uint8 current_background = BG_NIGHT;

// initializations for PPP screen
void pppLogo_init(void)
{
    if (g_Game.lastState == GAME_STATE_TEAM_SELECT 
     || g_Game.lastState == GAME_STATE_GAMEPLAY 
     || g_Game.lastState == GAME_STATE_DEMO_LOOP 
     || g_Game.lastState == GAME_STATE_CREDITS) {
        unloadGameAssets();
    }
    
    // this way only until I implement palette adjustments
    if (!game_options.use_rtc) {
        current_background++;
        if (current_background > 4) {
            current_background = 1;
        }
    }
    else {
        // update based on time of day
        jo_getdate(&now);
        // dawn
        if (now.hour >= T_DAWN && now.hour < T_DAY) {
            current_background = BG_DAWN;
        }
        // day
        if (now.hour >= T_DAY && now.hour < T_DUSK) {
            current_background = BG_DAY;
        }
        // dusk
        if (now.hour >= T_DUSK && now.hour < T_NIGHT) {
            current_background = BG_DUSK;
        }
        // night
        if (now.hour >= T_NIGHT && now.hour < T_DAWN) {
            current_background = BG_NIGHT;
        }
    }
    update_nbg1_palette();
    loadTitleScreenAssets();
    
    g_LogoTimer = 0;
    
    g_Game.lastState = GAME_STATE_PPP_LOGO;
    
    screenTransition_init(MINIMUM_FADE, MINIMUM_FADE, MINIMUM_FADE);
    
    
    if (game_options.mosaic_display) {
        mosaicInit(NBG1ON);
    }
    fade_in_rate = 1;
    
    if (!game_options.debug_display) {
        jo_set_displayed_screens(JO_NBG1_SCREEN);
        slColOffsetOn(NBG0ON | NBG1ON);
        slColOffsetAUse(NBG0ON);
        slColOffsetBUse(NBG1ON);
        slColOffsetA(nbg1_rate, nbg1_rate, nbg1_rate);
        slColOffsetB(nbg1_rate, nbg1_rate, nbg1_rate);
    }
    else {
        jo_set_displayed_screens(JO_NBG0_SCREEN | JO_NBG1_SCREEN);
        slColOffsetOn(NBG1ON);
        slColOffsetAUse(OFF);
        slColOffsetBUse(NBG1ON);
        slColOffsetB(nbg1_rate, nbg1_rate, nbg1_rate);
    }
    
    mosaic_in_rate = MOSAIC_SLOW_RATE;
    music_in = true;
    transition_in = true;
    
    jo_set_default_background_color(JO_COLOR_Black);
}

// update callback routine for PPP logo
void pppLogo_input(void)
{
    //
    // skip the logo if player 1 hits start
    //
    if(jo_is_pad1_key_down(JO_KEY_START))
    {
        // set everything back to defaults
        nbg1_rate = NEUTRAL_FADE;
        slColOffsetA(nbg1_rate, nbg1_rate, nbg1_rate);
        slColOffsetB(nbg1_rate, nbg1_rate, nbg1_rate);
        mosaic_x = MOSAIC_MIN;
        mosaic_y = MOSAIC_MIN;
	slScrMosSize(mosaic_x, mosaic_y);
        fade_in_rate = 8;
        changeState(GAME_STATE_TITLE_SCREEN);
        return;
    }
}

// update callback routine for PPP logo
void pppLogo_update(void)
{
    if(g_Game.gameState != GAME_STATE_PPP_LOGO)
    {
        return;
    }
    g_LogoTimer++;
    
    pppLogo_draw();
    
    // transition mosaic in
    if(g_LogoTimer > 4)
    {
        slow_fade_in = true;
        transition_in = true;
    }    
    // transition mosaic in
    if(g_LogoTimer > PPP_MOSAIC_TIMER)
    {
        mosaic_in = true;
        transition_in = true;
        jo_set_displayed_screens(JO_NBG0_SCREEN | JO_NBG1_SCREEN);
        if (fade_out) {
            fade_out = fadeOut(1, NEUTRAL_FADE);
        }
    }
    else if (game_options.mosaic_display) {
        mosaicRandom(NBG1ON);
    }
    
    if(g_LogoTimer == PPP_MOSAIC_TIMER-10)
    {
        fade_out = true;
        nbg1_rate = MAXIMUM_FADE;
        if (!game_options.debug_display) {
            slColOffsetA(nbg1_rate, nbg1_rate, nbg1_rate);
        }
        slColOffsetB(nbg1_rate, nbg1_rate, nbg1_rate);
        
        if (!game_options.mosaic_display) {
            jo_set_displayed_screens(JO_NBG0_SCREEN | JO_NBG1_SCREEN);
        }
    }

    if(g_LogoTimer > PPP_LOGO_TIMER)
    {
        fade_in_rate = 8;
        changeState(GAME_STATE_TITLE_SCREEN);
        return;
    }
}

void pppLogo_draw(void)
{
    jo_nbg0_printf(11, 9, "PIXEL POPPY PRODUCTIONS");
    jo_nbg0_printf(18, 11, "PRESENTS...");
}

