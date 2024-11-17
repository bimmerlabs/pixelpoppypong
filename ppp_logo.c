#include <jo/jo.h>
#include "main.h"
// #include "assets.h"
#include "state.h"
#include "ppp_logo.h"
#include "screen_transition.h"
// #include "objects/explosion.h"

unsigned int g_LogoTimer = 0;
Uint16 ppp_fade_timer = PPP_FADE_TIMER;

// initializations for PPP screen
void pppLogo_init(void)
{
    //g_LogoTimer
    g_LogoTimer = 0;
    jo_set_default_background_color(JO_COLOR_Black);
    jo_set_displayed_screens(JO_NBG0_SCREEN | JO_NBG1_SCREEN);
    slColorCalc(CC_ADD | CC_TOP | JO_NBG1_SCREEN);
    slColOffsetOn(NBG0ON | NBG1ON);
    slColOffsetAUse(NBG0ON);
    slColOffsetBUse(NBG1ON);
    slColOffsetA(nbg0_rate, nbg0_rate, nbg0_rate);
    slColOffsetB(nbg1_rate, nbg1_rate, nbg1_rate);
    
    // initLogoCursor();
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
            transitionState(GAME_STATE_TITLE_SCREEN);
            return;
    }

    // getPlayersInput();
}

// update callback routine for PPP logo
void pppLogo_update(void)
{
    if(g_Game.gameState != GAME_STATE_PPP_LOGO)
    {
        return;
    }

    g_LogoTimer++;
    screenTransition(4, 2, 0, ppp_fade_timer);
    
    // check if the frameAnim has expired
    if(g_LogoTimer > PPP_LOGO_TIMER)
    {
        changeState(GAME_STATE_TITLE_SCREEN);
        return;
    }
    
    // updateExplosions();
}

// draws Pixel Poppy Productions logo
void pppLogo_draw(void)
{
    if(g_Game.gameState != GAME_STATE_PPP_LOGO)
    {
        return;
    }
    jo_nbg0_printf(5, 14, "PIXEL POPPY PRODUCTIONS PRESENTS...");
}

