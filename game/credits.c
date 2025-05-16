#include <jo/jo.h>
#include "../main.h"
#include "credits.h"
#include "ppp_logo.h" // references transparency_rate
#include "../core/assets.h"
#include "../core/screen_transition.h"
#include "../palettefx/nbg1.h"
static CREDITS credits = {0};

void initCreditsStruct(void) {
    credits.timer = 0;
    credits.fade_in = false;
    credits.display = false;
    credits.fade_out = false;
    credits.id = 0;
    credits.display_timer = 0;
    credits.delay_timer = 0;
    credits.bg_spr_id = 4;
}

void init_credits(void)
{
    if (g_GameOptions.mosaic_display) {
        g_Transition.mosaic_in = true;
    }
    g_Transition.music_in = true;
    g_Transition.fade_in = true;
    g_Transition.all_in = true;
    jo_set_default_background_color(JO_COLOR_Black);
    jo_set_displayed_screens(JO_NBG0_SCREEN | JO_SPRITE_SCREEN | JO_NBG1_SCREEN);
    jo_core_set_screens_order(JO_NBG0_SCREEN, JO_SPRITE_SCREEN, JO_NBG1_SCREEN);

    // text transparency
    slColorCalc(CC_RATE | CC_TOP | NBG0ON | SPRON);
    slColorCalcOn(NBG0ON | SPRON);
    transparency_rate = CLRate0_32;
    slColRateNbg0 ( transparency_rate );
    
    g_Transition.mosaic_in_rate = MOSAIC_SLOW_RATE;
    menu_bg1.mesh = MESHoff;
    
    initCreditsStruct();
    
    menu_bg1.spr_id = menu_bg1.anim1.asset[credits.bg_spr_id];
    set_spr_position(&menu_bg1, 0, 0, 95);
    set_spr_scale(&menu_bg1, 0, 0);

    g_Game.lastState = GAME_STATE_CREDITS;
    playCDTrack(CREDITS_TRACK, false);
}

// this is not final - but at least it's here...
void display_credits(void)
{
    if(g_Game.gameState != GAME_STATE_CREDITS)
    {
        return;
    }
    static int bg_height = 48;
    credits.timer++;
    attrNbg1.x_pos += attrNbg1.x_scroll;
    if (attrNbg1.x_pos > toFIXED(512.0)) {
        attrNbg1.x_pos = FIXED_0;
    }
    slScrPosNbg1(attrNbg1.x_pos, attrNbg1.y_pos);    
    
    // wait to start credits
    if (credits.timer == CREDITS_BEGIN) {
        if (!g_GameOptions.debug_display && g_GameOptions.mosaic_display) {
            mosaicInit(NBG0ON);
        }
        credits.display = true;
        credits.fade_in = true;
    }
    if (credits.display) {
        if (credits.fade_in) {
            if (!g_GameOptions.debug_display && g_GameOptions.mosaic_display) {
                g_Transition.mosaic_in = mosaicIn(NBG0ON);
            }
            if (JO_MOD_POW2(g_Game.frame, 4) == 0 && transparency_rate > CLRate31_1) { // modulus
                transparency_rate -= 1;
                slColRateNbg0(transparency_rate); // Fade in effect
            }
            else if (transparency_rate == CLRate31_1) { // Fully faded in
                credits.display_timer = CREDITS_DISPLAY_TIME;
                credits.fade_in = false;
            }
            if (transparency_rate > CLRate31_1) {
                if (menu_bg1.scl.x < toFIXED(704)) {
                    menu_bg1.scl.x += FIXED_8;
                }
                if (menu_bg1.scl.y < toFIXED(bg_height)) {
                    menu_bg1.scl.y += FIXED_1;
                }
            }
        }

        else if (credits.fade_out) {
            if (!g_GameOptions.debug_display && g_GameOptions.mosaic_display) {
                g_Transition.mosaic_out = mosaicOut(NBG0ON);
            }
            if (JO_MOD_POW2(g_Game.frame, 2) == 0 && transparency_rate < CLRate0_32) { // modulus
                transparency_rate += 1;
                slColRateNbg0(transparency_rate); // Fade out effect
            }
            else if (transparency_rate == CLRate0_32) { // Fully faded out
                credits.delay_timer = CREDITS_DELAY_TIME; // Set delay before the next text
                credits.fade_out = false;
                credits.id++; // Move to the next text
            }
            if (transparency_rate < CLRate0_32) {
                if (menu_bg1.scl.y > FIXED_0) {
                    menu_bg1.scl.y -= FIXED_HALF;
                }
            }
        }

        else if (credits.display_timer > 0) { // Display text
            credits.display_timer--;
            if (credits.display_timer == 0) {
                credits.fade_out = true; // Start fading out
            }
        }
        else if (credits.delay_timer > 0) { // Delay before fading in
            credits.delay_timer--;
            if (menu_bg1.scl.y > FIXED_0) {
                menu_bg1.scl.y -= toFIXED(1.0);
            }
            if (credits.delay_timer == 0) {
                menu_bg1.scl.x = FIXED_0;
                menu_bg1.scl.y = toFIXED(bg_height); // test
                menu_bg1.spr_id = menu_bg1.anim1.asset[credits.bg_spr_id];
                if (!g_GameOptions.debug_display && g_GameOptions.mosaic_display) {
                    g_Transition.mosaic_in = true;
                }
                credits.fade_in = true; // Start fading in
            }
        }

        if (credits.id == 0) {
            bg_height = 48;
            jo_nbg0_printf(CREDITS_X2, 14, "PIXEL POPPY PONG 2025");
            jo_nbg0_printf(19, 15, "CREDITS");
        }
        else if (credits.id == 1) {
            bg_height = 48;
            jo_nbg0_printf(CREDITS_X1, 14, "PROGRAMMING/GRAPHICS:");
            jo_nbg0_printf(CREDITS_X2, 15, "C.SEIGMAN \"HASSMASCHINE\"");
        }

        else if (credits.id == 2) {
            bg_height = 48;
            jo_nbg0_printf(CREDITS_X1, 14, "ORIGINAL SOUNDTRACK BY:");
            jo_nbg0_printf(CREDITS_X2, 15, "RANDOM");
        }

        else if (credits.id == 3) {
            bg_height = 48;
            jo_nbg0_printf(CREDITS_X1, 14, "FEATURING:");
            jo_nbg0_printf(CREDITS_X2, 15, "PONESOUND");
        }

        else if (credits.id == 4) {
            bg_height = 48;
            jo_nbg0_printf(CREDITS_X1, 14, "BUILT WITH JO-ENGINE");
            jo_nbg0_printf(CREDITS_X2, 15, "BY JOHANNES FETZ");
        }

        else if (credits.id == 5) {
            bg_height = 48;
            slScrPosNbg0(FIXED_0, toFIXED(-4));
            jo_nbg0_printf(CREDITS_X1, 13, "ORIGINAL BACKGROUND ART:");
            jo_nbg0_printf(CREDITS_X2, 14, "TATIANA SHERBUL");
            jo_nbg0_printf(CREDITS_X2, 15, "WWW.BEHANCE.NET/T-SHERBUL");
        }

        else if (credits.id == 6) {
            bg_height = 64;
            slScrPosNbg0(FIXED_0, FIXED_0);
            jo_nbg0_printf(CREDITS_X1, 12, "SPECIAL THANKS:");
            jo_nbg0_printf(CREDITS_X2, 13, "REYME:SUPPORT & ADVICE");
            jo_nbg0_printf(CREDITS_X2, 14, "PONUT64:SOUND DRIVER");
            jo_nbg0_printf(CREDITS_X2, 15, "DANNY:MATH LIBRARY");
            jo_nbg0_printf(CREDITS_X2, 16, "SLINGA:SATURN CODE & IDEAS");
            jo_nbg0_printf(CREDITS_X2, 17, "SEGAXTREME DISCORD!");
        }

        else if (credits.id == 7) {
            bg_height = 32;
            slScrPosNbg0(FIXED_0, toFIXED(-4));
            jo_nbg0_printf(18, 14, "THE END!");
        }

        else if (credits.id == 8)
        {
            slColorCalcOn(OFF);
            slScrMosaicOn(OFF);
            slScrMosSize(MOSAIC_MIN, MOSAIC_MIN);
            slScrPosNbg0(FIXED_0, FIXED_0);
            g_Game.lastState = GAME_STATE_CREDITS;
            transitionState(GAME_STATE_UNINITIALIZED);
        }

        my_sprite_draw(&menu_bg1); // shadow

    }
}

void    credits_input(void)	{
    if (jo_is_pad1_key_down(JO_KEY_START)) {
        slColorCalcOn(OFF);
        slScrMosaicOn(OFF);
        slScrMosSize(MOSAIC_MIN, MOSAIC_MIN);
        slScrPosNbg0(FIXED_0, FIXED_0);
        g_Game.lastState = GAME_STATE_CREDITS;
        transitionState(GAME_STATE_UNINITIALIZED);
    }
}