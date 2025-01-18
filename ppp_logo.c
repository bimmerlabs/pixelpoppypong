#include <jo/jo.h>
#include "main.h"
#include "assets.h"
#include "state.h"
#include "ppp_logo.h"
#include "screen_transition.h"
// #include "objects/explosion.h"
#include "BG_DEF/BG26.h"

unsigned int g_LogoTimer = 0;
static int transparency_rate = TRANSPARENCY_MAX;
Uint8 current_background = BG_NIGHT;

// initializations for PPP screen
void pppLogo_init(void)
{
    if (g_Game.lastState == GAME_STATE_TEAM_SELECT || g_Game.lastState == GAME_STATE_GAMEPLAY) {
        unloadGameAssets();
    }
    
    // this way only until I implement palette adjustments
    if (game_options.debug_mode || !game_options.use_rtc) {
    // if (game_options.debug_mode) {
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
    init_bg26_img();
    // if (first_load) {
        // // init_bg0_img();
        // init_bg26_img();
        // first_load = false;
    // }
    loadTitleScreenAssets();
    
    g_LogoTimer = 0;
    
    g_Game.lastState = GAME_STATE_PPP_LOGO;
    
    slColRAMMode ( CRM16_1024 ); // CRAM mode 0 - required for ngb0 transparency
    if (!game_options.debug_display) {
        slColorCalc ( CC_RATE | CC_TOP | NBG0ON );
        transparency_rate = TRANSPARENCY_MAX;
        slColRateNbg0 ( transparency_rate );
    }
    else {
        transparency_rate = TRANSPARENCY_MIN;
    }
    
    screenTransition_init(MINIMUM_FADE, MINIMUM_FADE, MINIMUM_FADE);
    
    if (game_options.mosaic_display) {
        mosaicInit();
    }
    fade_in_rate = 2;
    
    if (!game_options.debug_display) {
        slColOffsetOn(NBG0ON | NBG1ON);
        slColOffsetAUse(NBG0ON);
        slColOffsetAUse(NBG0ON);
        slColOffsetBUse(NBG1ON);
        slColOffsetA(nbg0_rate, nbg0_rate, nbg0_rate);
        slColOffsetB(nbg1_rate, nbg1_rate, nbg1_rate);
    }
    else {
        slColOffsetOn(NBG1ON);
        slColOffsetAUse(OFF);
        slColOffsetBUse(NBG1ON);
        slColOffsetB(nbg1_rate, nbg1_rate, nbg1_rate);
    }
    
    mosaic_in_rate = MOSAIC_SLOW_RATE;
    music_in = true;
    transition_in = true;
    
    jo_set_default_background_color(JO_COLOR_Black);
    jo_set_displayed_screens(JO_NBG0_SCREEN | JO_NBG1_SCREEN);
}

// update callback routine for PPP logo
void pppLogo_input(void)
{
    //
    // skip the logo if player 1 hits start/a/c
    //
    if(jo_is_pad1_key_down(JO_KEY_START) ||
       jo_is_pad1_key_down(JO_KEY_A) ||
       jo_is_pad1_key_down(JO_KEY_C))
    {
        // set everything back to defaults
        nbg0_rate = NEUTRAL_FADE;
        slColOffsetA(nbg0_rate, nbg0_rate, nbg0_rate);
        slColOffsetB(nbg0_rate, nbg0_rate, nbg0_rate);
        mosaic_x = MOSAIC_MIN;
        mosaic_y = MOSAIC_MIN;
	slScrMosSize(mosaic_x, mosaic_y);
        fade_in_rate = 8;
        transparency_rate = TRANSPARENCY_MIN;
        slColRateNbg0 ( transparency_rate );
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
        if (frame % 60 == 0 && transparency_rate > TRANSPARENCY_MIN+16 && !fade_out) {
            transparency_rate--;
            slColRateNbg0 ( transparency_rate );
        }
    }    
    // transition mosaic in
    if(g_LogoTimer > PPP_MOSAIC_TIMER)
    {
        mosaic_in = true;
        transition_in = true;
        if (fade_out) {
            fade_out = fadeOut(1, NEUTRAL_FADE);
        }
        if (frame % 15 == 0 && transparency_rate > TRANSPARENCY_MIN) {
            transparency_rate--;
            slColRateNbg0 ( transparency_rate );
        }
        // else if (transparency_rate <= TRANSPARENCY_MIN){
            // slColorCalc(CC_ADD | CC_TOP | JO_NBG1_SCREEN);
        // }
    }
    else if (game_options.mosaic_display) {
        mosaicRandom();
    }
    
    if(g_LogoTimer == PPP_MOSAIC_TIMER-10)
    {
        fade_out = true;
        nbg0_rate = MAXIMUM_FADE;
        if (!game_options.debug_display) {
            slColOffsetA(nbg0_rate, nbg0_rate, nbg0_rate);
        }
        slColOffsetB(nbg0_rate, nbg0_rate, nbg0_rate);
        
        transparency_rate = TRANSPARENCY_MAX;
        slColRateNbg0 ( transparency_rate );
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
    // jo_nbg0_printf(5, 14, "PIXEL POPPY PRODUCTIONS PRESENTS...");
    jo_nbg0_printf(11, 13, "PIXEL POPPY PRODUCTIONS");
    jo_nbg0_printf(19, 15, "PRESENTS...");
}

