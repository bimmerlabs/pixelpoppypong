#include <jo/jo.h>
#include "main.h"
#include "credits.h"
#include "assets.h"
#include "ppp_logo.h"
#include "screen_transition.h"
#include "BG_DEF/nbg1.h"

unsigned int g_CreditsTimer = 0;
static bool credits_fade_in = false;
static bool credits_display = false;
static bool credits_fade_out = false;
static Uint8 credit_id = 0;
static int display_timer = 0;
static int delay_timer = 0;
static Uint8 bg_spr_id = 0;
void init_credits(void)
{
    if (g_GameOptions.mosaic_display) {
        mosaic_in = true;
    }
    music_in = true;
    fade_in = true;
    transition_in = true;
    jo_set_default_background_color(JO_COLOR_Black);
    jo_set_displayed_screens(JO_NBG0_SCREEN | JO_SPRITE_SCREEN | JO_NBG1_SCREEN);
    jo_core_set_screens_order(JO_NBG0_SCREEN, JO_SPRITE_SCREEN, JO_NBG1_SCREEN);

    // text transparency
    slColorCalc(CC_RATE | CC_TOP | NBG0ON | SPRON);
    slColorCalcOn(NBG0ON | SPRON);
    transparency_rate = CLRate0_32;
    slColRateNbg0 ( transparency_rate );
    // slColRateSpr1 ( transparency_rate );
    
    mosaic_in_rate = MOSAIC_SLOW_RATE;
    menu_bg1.mesh = MESHoff;
    bg_spr_id = 4;
    menu_bg1.spr_id = menu_bg1.anim1.asset[bg_spr_id];
    set_spr_position(&menu_bg1, 0, 0, 95);
    set_spr_scale(&menu_bg1, 0, 0);

    g_Game.lastState = GAME_STATE_CREDITS;

    g_CreditsTimer = 0;
    credits_fade_in = false;
    credits_display = false;
    credits_fade_out = false;
    display_timer = 0;
    delay_timer = 0;
    credit_id = 0;

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
    g_CreditsTimer++;
    attrNbg1.x_pos += attrNbg1.x_scroll;
    if (attrNbg1.x_pos > toFIXED(512.0)) {
        attrNbg1.x_pos = toFIXED(0);
    }
    slScrPosNbg1(attrNbg1.x_pos, attrNbg1.y_pos);    
    
    // wait to start credits
    if (g_CreditsTimer == CREDITS_BEGIN) {
        if (!g_GameOptions.debug_display && g_GameOptions.mosaic_display) {
            mosaicInit(NBG0ON);
        }
        credits_display = true;
        credits_fade_in = true;
    }
    if (credits_display) {
        if (credits_fade_in) {
            if (!g_GameOptions.debug_display && g_GameOptions.mosaic_display) {
                mosaic_in = mosaicIn(NBG0ON);
            }
            if (JO_MOD_POW2(frame, 4) == 0 && transparency_rate > CLRate31_1) { // modulus
                transparency_rate -= 1;
                slColRateNbg0(transparency_rate); // Fade in effect
                // slColRateSpr1(transparency_rate);
            }
            else if (transparency_rate == CLRate31_1) { // Fully faded in
                display_timer = CREDITS_DISPLAY_TIME;
                credits_fade_in = false;
            }
            if (transparency_rate > CLRate31_1) {
                if (menu_bg1.scl.x < toFIXED(704)) {
                    menu_bg1.scl.x += toFIXED(8);
                }
                if (menu_bg1.scl.y < toFIXED(bg_height)) {
                    menu_bg1.scl.y += toFIXED(1);
                }
            }
        }

        else if (credits_fade_out) {
            if (!g_GameOptions.debug_display && g_GameOptions.mosaic_display) {
                mosaic_out = mosaicOut(NBG0ON);
            }
            if (JO_MOD_POW2(frame, 2) == 0 && transparency_rate < CLRate0_32) { // modulus
                transparency_rate += 1;
                slColRateNbg0(transparency_rate); // Fade out effect
                // slColRateSpr1(transparency_rate);
            }
            else if (transparency_rate == CLRate0_32) { // Fully faded out
                delay_timer = CREDITS_DELAY_TIME; // Set delay before the next text
                credits_fade_out = false;
                credit_id++; // Move to the next text
            }
            if (transparency_rate < CLRate0_32) {
                if (menu_bg1.scl.y > toFIXED(0)) {
                    menu_bg1.scl.y -= toFIXED(0.5);
                }
            }
        }

        else if (display_timer > 0) { // Display text
            display_timer--;
            if (display_timer == 0) {
                credits_fade_out = true; // Start fading out
            }
        }
        else if (delay_timer > 0) { // Delay before fading in
            delay_timer--;
            if (menu_bg1.scl.y > toFIXED(0)) {
                menu_bg1.scl.y -= toFIXED(1.0);
            }
            if (delay_timer == 0) {
                menu_bg1.scl.x = toFIXED(0);
                menu_bg1.scl.y = toFIXED(bg_height); // test
                menu_bg1.spr_id = menu_bg1.anim1.asset[bg_spr_id];
                if (!g_GameOptions.debug_display && g_GameOptions.mosaic_display) {
                    mosaic_in = true;
                }
                credits_fade_in = true; // Start fading in
            }
        }

        // if (g_GameOptions.debug_display) {
            // jo_nbg0_printf(2, 4, "FADE_IN:%i", credits_fade_in);
            // jo_nbg0_printf(2, 5, "DISPLAY:%i", credits_display);
            // jo_nbg0_printf(2, 6, "FADE_OUT:%i", credits_fade_out);
            // jo_nbg0_printf(2, 7, "DISPLAY_TIMER:%i", display_timer);
            // jo_nbg0_printf(2, 8, "DELAY_TIMER:%i", delay_timer);
            // jo_nbg0_printf(2, 9, "TRANSPARENCY:%i", transparency_rate);
            // jo_nbg0_printf(2, 10, "CREDIT_ID:%i", credit_id);
        // }

        if (credit_id == 0) {
            bg_height = 48;
            jo_nbg0_printf(CREDITS_X2, 14, "PIXEL POPPY PONG 2025");
            jo_nbg0_printf(19, 15, "CREDITS");
        }
        else if (credit_id == 1) {
            bg_height = 48;
            jo_nbg0_printf(CREDITS_X1, 14, "PROGRAMMING/GRAPHICS:");
            jo_nbg0_printf(CREDITS_X2, 15, "C.SEIGMAN \"HASSMASCHINE\"");
        }

        else if (credit_id == 2) {
            bg_height = 48;
            jo_nbg0_printf(CREDITS_X1, 14, "ORIGINAL SOUNDTRACK BY:");
            jo_nbg0_printf(CREDITS_X2, 15, "RANDOM");
        }

        else if (credit_id == 3) {
            bg_height = 48;
            jo_nbg0_printf(CREDITS_X1, 14, "FEATURING:");
            jo_nbg0_printf(CREDITS_X2, 15, "PONESOUND");
        }

        else if (credit_id == 4) {
            bg_height = 48;
            jo_nbg0_printf(CREDITS_X1, 14, "BUILT WITH JO-ENGINE");
            jo_nbg0_printf(CREDITS_X2, 15, "BY JOHANNES FETZ");
        }

        else if (credit_id == 5) {
            bg_height = 48;
            slScrPosNbg0(toFIXED(0), toFIXED(-4));
            jo_nbg0_printf(CREDITS_X1, 13, "ORIGINAL BACKGROUND ART:");
            jo_nbg0_printf(CREDITS_X2, 14, "TATIANA SHERBUL");
            jo_nbg0_printf(CREDITS_X2, 15, "WWW.BEHANCE.NET/T-SHERBUL");
        }

        else if (credit_id == 6) {
            bg_height = 64;
            slScrPosNbg0(toFIXED(0), toFIXED(0));
            jo_nbg0_printf(CREDITS_X1, 12, "SPECIAL THANKS:");
            jo_nbg0_printf(CREDITS_X2, 13, "REYME:SUPPORT & ADVICE");
            jo_nbg0_printf(CREDITS_X2, 14, "PONUT64:SOUND DRIVER");
            jo_nbg0_printf(CREDITS_X2, 15, "DANNY:MATH LIBRARY");
            jo_nbg0_printf(CREDITS_X2, 16, "SLINGA:SATURN CODE & IDEAS");
            jo_nbg0_printf(CREDITS_X2, 17, "SEGAXTREME DISCORD!");
        }

        else if (credit_id == 7) {
            bg_height = 32;
            slScrPosNbg0(toFIXED(0), toFIXED(-4));
            jo_nbg0_printf(18, 14, "THE END!");
        }

        else if (credit_id == 8)
        {
            mosaic_in_rate = MOSAIC_FAST_RATE;
            slColorCalcOn(OFF);
            slScrMosaicOn(OFF);
            slScrMosSize(MOSAIC_MIN, MOSAIC_MIN);
            slScrPosNbg0(toFIXED(0), toFIXED(0));
            g_Game.lastState = GAME_STATE_CREDITS;
            transitionState(GAME_STATE_UNINITIALIZED);
        }

        my_sprite_draw(&menu_bg1); // shadow

    }
}

void    credits_input(void)	{
    if (jo_is_pad1_key_down(JO_KEY_START)) {
        mosaic_in_rate = MOSAIC_FAST_RATE;
        slColorCalcOn(OFF);
        slScrMosaicOn(OFF);
        slScrMosSize(MOSAIC_MIN, MOSAIC_MIN);
        slScrPosNbg0(toFIXED(0), toFIXED(0));
        g_Game.lastState = GAME_STATE_CREDITS;
        transitionState(GAME_STATE_UNINITIALIZED);
    }
}