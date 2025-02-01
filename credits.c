#include <jo/jo.h>
#include "main.h"
#include "credits.h"
#include "assets.h"
#include "ppp_logo.h"
#include "screen_transition.h"

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
    if (game_options.mosaic_display) {
        mosaic_in = true;
    }
    music_in = true;
    fade_in = true;
    transition_in = true;
    jo_set_default_background_color(JO_COLOR_Black);
    jo_set_displayed_screens(JO_NBG0_SCREEN | JO_SPRITE_SCREEN | JO_NBG1_SCREEN);
    jo_core_set_screens_order(JO_NBG0_SCREEN, JO_SPRITE_SCREEN, JO_NBG1_SCREEN);
    
    // fade bg
    // slColOffsetOn(NBG1ON);
    // slColOffsetAUse(OFF);
    // slColOffsetBUse(NBG1ON);
    // nbg1_rate = PAUSE_FADE;
    // slColOffsetB(nbg1_rate, nbg1_rate, nbg1_rate);
    
    // text transparency
    slColorCalc(CC_RATE | CC_TOP | NBG0ON | NBG1ON | SPRON);
    slColorCalcOn(NBG0ON);
    transparency_rate = CLRate0_32;
    slColRateNbg0 ( transparency_rate );
    // slColRateSpr1 ( transparency_rate );
    
    if (game_options.mesh_display) {
        menu_bg1.mesh = MESHon;
    }
    else {
        menu_bg1.mesh = MESHoff;
    }
    bg_spr_id = 0;
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
    
    playCDTrack(CREDITS_TRACK, true);
}

// this is not final - but at least it's here...
void display_credits(void)
{   
    if(g_Game.gameState != GAME_STATE_CREDITS)
    {
        return;
    }
    g_CreditsTimer++;
    // wait to start credits
    if (g_CreditsTimer > CREDITS_BEGIN && !credits_display) {
        credits_display = true;
        credits_fade_in = true;
    }
    if (credits_display) {    
        
        if (credits_fade_in) {
            if (frame % 4 == 0 && transparency_rate > CLRate31_1) {
                transparency_rate--;
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
                if (menu_bg1.scl.y < toFIXED(48)) {
                    menu_bg1.scl.y += toFIXED(1);
                }
            }
        } 
        
        else if (credits_fade_out) {
            if (frame % 2 == 0 && transparency_rate < CLRate0_32) {
                transparency_rate++;
                slColRateNbg0(transparency_rate); // Fade out effect
                // slColRateSpr1(transparency_rate);
            } 
            else if (transparency_rate == CLRate0_32) { // Fully faded out
                delay_timer = CREDITS_DELAY_TIME; // Set delay before the next text
                credits_fade_out = false;
                credit_id++; // Move to the next text
            }
            if (transparency_rate < CLRate0_32) {
                if (menu_bg1.scl.x > toFIXED(0)) {
                    menu_bg1.scl.x -= toFIXED(8);
                }
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
            if (menu_bg1.scl.x > toFIXED(0)) {
                menu_bg1.scl.x -= toFIXED(8);
            }
            if (menu_bg1.scl.y > toFIXED(0)) {
                menu_bg1.scl.y -= toFIXED(0.5);
            }
            if (delay_timer == 0) {
                bg_spr_id++;
                if (bg_spr_id > 3) {
                    bg_spr_id = 0;
                }
                menu_bg1.spr_id = menu_bg1.anim1.asset[bg_spr_id];
                credits_fade_in = true; // Start fading in
            }
        }
        
        if (game_options.debug_display) {
            jo_nbg0_printf(2, 4, "FADE_IN:%i", credits_fade_in);
            jo_nbg0_printf(2, 5, "DISPLAY:%i", credits_display);
            jo_nbg0_printf(2, 6, "FADE_OUT:%i", credits_fade_out);
            jo_nbg0_printf(2, 7, "DISPLAY_TIMER:%i", display_timer);
            jo_nbg0_printf(2, 8, "DELAY_TIMER:%i", delay_timer);
            jo_nbg0_printf(2, 9, "TRANSPARENCY:%i", transparency_rate);
            jo_nbg0_printf(2, 10, "CREDIT_ID:%i", credit_id);
        }
        
        if (credit_id == 0) {            
            jo_nbg0_printf(CREDITS_X2, 14, "PIXEL POPPY PONG 2025");
        }
        else if (credit_id == 1) {
            jo_nbg0_printf(CREDITS_X1, 14, "PROGRAMMING/GRAPHICS:");
            jo_nbg0_printf(CREDITS_X2, 15, "C.SEIGMAN \"HASSMASCHINE\"");
        }
        
        else if (credit_id == 2) {
            jo_nbg0_printf(CREDITS_X1, 14, "ORIGINAL SOUNDTRACK BY:");
            jo_nbg0_printf(CREDITS_X2, 15, "RANDOM");
        }
        
        else if (credit_id == 3) {
            jo_nbg0_printf(CREDITS_X1, 14, "FEATURING:");    
            jo_nbg0_printf(CREDITS_X2, 15, "PONESOUND");
        }
        
        else if (credit_id == 4) {
            jo_nbg0_printf(CREDITS_X1, 14, "BUILT WITH JO-ENGINE");        
            jo_nbg0_printf(CREDITS_X2, 15, "BY JOHANNES FETZ");
        }
        
        else if (credit_id == 5) {
            jo_nbg0_printf(CREDITS_X1, 13, "ORIGINAL BACKGROUND ART:");
            jo_nbg0_printf(CREDITS_X2, 15, "TATIANA SHERBUL");       
            jo_nbg0_printf(CREDITS_X2, 16, "WWW.BEHANCE.NET/T-SHERBUL");
        }
        
        else if (credit_id == 6) {
            jo_nbg0_printf(CREDITS_X1, 13, "SPECIAL THANKS:");     
            jo_nbg0_printf(CREDITS_X2, 14, "REYME:GENERAL SUPPORT & ADVICE");
            jo_nbg0_printf(CREDITS_X2, 15, "PONUT64:SOUND DRIVER");
            jo_nbg0_printf(CREDITS_X2, 16, "DANNY:MATH LIBRARY");
            jo_nbg0_printf(CREDITS_X2, 17, "SEGAXTREME DISCORD!");
        }
        
        // else if (credit_id == 7) {
            // jo_nbg0_printf(CREDITS_X1, 13, "SPECIAL THANKS:");
            // jo_nbg0_printf(CREDITS_X2, 15, "DANNY:MATH LIBRARY");
            // jo_nbg0_printf(CREDITS_X2, 16, "SEGAXTREME DISCORD!");
        // }
                
        else if (credit_id == 7) {                        
            jo_nbg0_printf(18, 14, "THE END!");    
        }          
        
        if (credit_id <= 7) {
            my_sprite_draw(&menu_bg1); // shadow
        }

    if (credit_id == 8)
    {
        g_CreditsTimer = 0;
        transitionState(GAME_STATE_UNINITIALIZED);
    }

    }
}

void    credits_input(void)	{
    if (jo_is_pad1_key_down(JO_KEY_START)) {
        g_Game.lastState = GAME_STATE_CREDITS;
        transitionState(GAME_STATE_UNINITIALIZED);
    }
}