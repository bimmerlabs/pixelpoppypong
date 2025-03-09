#include <jo/jo.h>
#include "main.h"
#include "assets.h"
#include "state.h"
#include "ppp_logo.h"
#include "screen_transition.h"
#include "BG_DEF/nbg1.h"
#include "lighting.h"
#include "BG_DEF/sprite_colors.h"

unsigned int g_LogoTimer = 0;
int transparency_rate = TRANSPARENCY_MAX;
Uint8 current_background = BG_NIGHT;

// // cheat codes?
// static bool draw_start_text = true;
// static bool a_pressed = false;
// static bool b_pressed = false;
// static bool c_pressed = false;
// static bool x_pressed = false;
// static bool y_pressed = false;
// static bool z_pressed = false;

// initializations for PPP screen
void pppLogo_init(void)
{
    if (g_Game.lastState == GAME_STATE_TEAM_SELECT 
     || g_Game.lastState == GAME_STATE_GAMEPLAY 
     || g_Game.lastState == GAME_STATE_DEMO_LOOP 
     || g_Game.lastState == GAME_STATE_CREDITS) {
        unloadGameAssets();
    }
    // loadPPPLogoAssets();
    
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
    
    if (game_options.mesh_display) {
        pppshadow.mesh = MESHon;
    }
    else {
        pppshadow.mesh = MESHoff;
    }
    
    light.x = toFIXED(10);
    light.y = toFIXED(255);
    light.z = toFIXED(255);
    
    set_spr_scale(&pixel_poppy, 1.0, 1.0);
    pixel_poppy.rot.z = 0;
    set_spr_position(&pixel_poppy, 10, 255, 100);
    sprite_frame_reset(&pixel_poppy);
        
    g_LogoTimer = 0;
    
    g_Game.lastState = GAME_STATE_PPP_LOGO;
    
    screenTransition_init(MINIMUM_FADE, MINIMUM_FADE, MINIMUM_FADE);
    
    
    if (game_options.mosaic_display) {
        mosaicInit(NBG1ON);
    }
    fade_in_rate = 1;
    
    if (!game_options.debug_display) {
        jo_set_displayed_screens(JO_NBG0_SCREEN | JO_SPRITE_SCREEN | JO_NBG1_SCREEN);
        slColOffsetOn(NBG0ON | SPRON);
        slColOffsetAUse(NBG0ON);
        slColOffsetBUse(NBG1ON);
        slColOffsetA(nbg1_rate, nbg1_rate, nbg1_rate);
        slColOffsetB(nbg1_rate, nbg1_rate, nbg1_rate);
    }
    else {
        jo_set_displayed_screens(JO_NBG0_SCREEN | JO_SPRITE_SCREEN | JO_NBG1_SCREEN);
        slColOffsetOn(NBG1ON);
        slColOffsetAUse(OFF);
        slColOffsetBUse(SPRON);
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
        // // I kind of like it better this way?
        // g_LogoTimer = PPP_MOSAIC_TIMER-10;
        // set everything back to defaults
        nbg1_rate = NEUTRAL_FADE;
        slColOffsetA(nbg1_rate, nbg1_rate, nbg1_rate);
        slColOffsetB(nbg1_rate, nbg1_rate, nbg1_rate);
        mosaic_x = MOSAIC_MIN;
        mosaic_y = MOSAIC_MIN;
	slScrMosSize(mosaic_x, mosaic_y);
        fade_in_rate = 8;
        // unloadPPPLogoAssets();
        changeState(GAME_STATE_TITLE_SCREEN);
        return;
    }
    
    // // cheat codes? (could be left out)
    // if (jo_is_pad1_key_down(JO_KEY_A)) {
        // a_pressed = true;
    // }
    // if (jo_is_pad1_key_down(JO_KEY_B)) {
        // b_pressed = true;
    // }
    // if (jo_is_pad1_key_down(JO_KEY_C)) {
        // c_pressed = true;
    // }
    // if (jo_is_pad1_key_down(JO_KEY_X)) {
        // x_pressed = true;
    // }
    // if (jo_is_pad1_key_down(JO_KEY_Y)) {
        // y_pressed = true;
    // }
    // if (jo_is_pad1_key_down(JO_KEY_Z)) {
        // z_pressed = true;
    // }
    
    // if (a_pressed && b_pressed && c_pressed) {
        // if (jo_is_pad1_key_pressed(JO_KEY_UP)) {
            // light.y += LIGHT_SPEED;
            // if (light.y > FIXED_255) {
                // light.y = FIXED_255;
            // }
                
                // pixel_poppy.pos.x = 2*(light.x - FIXED_127);
                // pixel_poppy.pos.y = -2.5*(light.y - FIXED_127);
            // do_update_ppplogo = true;
        // }
        // else if (jo_is_pad1_key_pressed(JO_KEY_DOWN)) {
            // light.y -= LIGHT_SPEED;
            // if (light.y < JO_FIXED_0) {
                // light.y = JO_FIXED_0;
            // }
                
                // pixel_poppy.pos.x = 2*(light.x - FIXED_127);
                // pixel_poppy.pos.y = -2.5*(light.y - FIXED_127);
            // do_update_ppplogo = true;
        // }	
        // if (jo_is_pad1_key_pressed(JO_KEY_LEFT)) {
            // light.x -= LIGHT_SPEED;
            // if (light.x < JO_FIXED_0) {
                // light.x = JO_FIXED_0;
            // }
                
                // pixel_poppy.pos.x = 2*(light.x - FIXED_127);
                // pixel_poppy.pos.y = -2.5*(light.y - FIXED_127);
            // do_update_ppplogo = true;
        // }
        // else if (jo_is_pad1_key_pressed(JO_KEY_RIGHT)) {
            // light.x += LIGHT_SPEED;
            // if (light.x > FIXED_255) {
                // light.x = FIXED_255;
            // }
                
                // pixel_poppy.pos.x = 2*(light.x - FIXED_127);
                // pixel_poppy.pos.y = -2.5*(light.y - FIXED_127);
            // do_update_ppplogo = true;
        // }
    // }
}

// update callback routine for PPP logo
void pppLogo_update(void)
{
    if(g_Game.gameState != GAME_STATE_PPP_LOGO)
    {
        return;
    }
    g_LogoTimer++;

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
        if (fade_out) {
            fade_out = fadeOut(1, NEUTRAL_FADE);
        }
        pppLogo_draw();
        // if (!a_pressed && !b_pressed && !c_pressed) {
            update_light_position();
        // }
    }
    else if (game_options.mosaic_display) {
        mosaicRandom(NBG1ON);
    }
    
    if(g_LogoTimer == PPP_MOSAIC_TIMER)
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

    // if (!a_pressed && !b_pressed && !c_pressed) {
        if(g_LogoTimer > PPP_LOGO_TIMER)
        {
            jo_set_displayed_screens(JO_NBG0_SCREEN | JO_NBG1_SCREEN);
            fade_in_rate = 8;
            // unloadPPPLogoAssets();
            changeState(GAME_STATE_TITLE_SCREEN);
            return;
        }
    // }
    // if (a_pressed && b_pressed && c_pressed) {
        // if (frame % 10 == 0) { // modulus
            // draw_start_text = !draw_start_text;
        // }
        // if (draw_start_text) {
            // jo_nbg0_printf(16, 27, "PRESS START");
        // }
    // }
}

#define X_RADIUS toFIXED(155)
#define Y_RADIUS toFIXED(80)
static int lightAngle = 360;
    
void pppLogo_draw(void)
{    
    my_sprite_draw(&ppplogo);
    my_sprite_draw(&pppshadow);
    
    my_sprite_draw(&pixel_poppy);
    jo_nbg0_printf(17, 19, "PRESENTS...");
        
    if (game_options.debug_display) {
        jo_nbg0_printf(2, 25, "LIGHT:X=%3d,Y=%3d,Z=%3d", JO_FIXED_TO_INT(light.x), JO_FIXED_TO_INT(light.y), JO_FIXED_TO_INT(light.z));
        jo_nbg0_printf(2, 26, "LIGHTANGLE:%3d", lightAngle);
    }
}

// draw an ellipse
void update_light_position(void) {
    lightAngle -= 2;
    if (lightAngle == 0)
        lightAngle = 360;
    light.x = FIXED_127 + jo_fixed_mult(jo_cos(lightAngle), X_RADIUS);
    light.y = FIXED_127 + jo_fixed_mult(my_fixed_sine(lightAngle), Y_RADIUS);

    pppshadow.pos.x = -jo_fixed_mult(jo_cos(lightAngle), toFIXED(9));
    pppshadow.pos.y = jo_fixed_mult(my_fixed_sine(lightAngle), toFIXED(6));
    
    pixel_poppy.pos.x = 2*(light.x - FIXED_127);
    pixel_poppy.pos.y = -2*(light.y - FIXED_127);
    pixel_poppy.rot.z = 90-2*lightAngle;

    do_update_ppplogo = true;
}