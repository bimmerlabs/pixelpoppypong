#include <jo/jo.h>
#include "../main.h"
#include "ppp_logo.h"
#include "../core/assets.h"
#include "../core/state.h"
#include "../core/screen_transition.h"
#include "../palettefx/nbg1.h"
#include "../palettefx/lighting.h"
#include "../palettefx/sprite_colors.h"

unsigned int g_LogoTimer = 0; // not worth putting these in a struct..
int transparency_rate = TRANSPARENCY_MAX;

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

    loadTitleScreenAssets();
    
    if (g_GameOptions.mesh_display) {
        pppshadow.mesh = MESHon;
    }
    else {
        pppshadow.mesh = MESHoff;
    }
    
    light.x = toFIXED(10);
    light.y = FIXED_255;
    light.z = FIXED_255;
    
    set_spr_scale(&pixel_poppy, 1.0, 1.0);
    pixel_poppy.rot.z = 0;
    set_spr_position(&pixel_poppy, 10, 255, 100);
    sprite_frame_reset(&pixel_poppy);
        
    g_LogoTimer = 0;
    
    g_Game.lastState = GAME_STATE_PPP_LOGO;
    
    screenTransition_init(MINIMUM_FADE, MINIMUM_FADE, MINIMUM_FADE);
    
    
    if (g_GameOptions.mosaic_display) {
        mosaicInit(NBG1ON);
    }
    g_Transition.fade_in_rate = 1;
    if (!g_GameOptions.debug_display) {
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
    
    g_Transition.mosaic_in_rate = MOSAIC_SLOW_RATE;
    g_Transition.music_in = true;
    g_Transition.all_in = true;
    
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
        g_Transition.mosaic_x = MOSAIC_MIN;
        g_Transition.mosaic_y = MOSAIC_MIN;
	slScrMosSize(g_Transition.mosaic_x, g_Transition.mosaic_y);
        g_Transition.fade_in_rate = 8;
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
        g_Transition.slow_fade_in = true;
        g_Transition.all_in = true;
    }    
    // transition mosaic in
    if(g_LogoTimer > PPP_MOSAIC_TIMER)
    {
        g_Transition.mosaic_in = true;
        g_Transition.all_in = true;
        if (g_Transition.fade_out) {
            g_Transition.fade_out = fadeOut(1, NEUTRAL_FADE);
        }
        pppLogo_draw();
        update_light_position();
    }
    else if (g_GameOptions.mosaic_display) {
        mosaicRandom(NBG1ON);
    }
    
    if(g_LogoTimer == PPP_MOSAIC_TIMER)
    {
        g_Transition.fade_out = true;
        nbg1_rate = MAXIMUM_FADE;
        if (!g_GameOptions.debug_display) {
            slColOffsetA(nbg1_rate, nbg1_rate, nbg1_rate);
        }
        slColOffsetB(nbg1_rate, nbg1_rate, nbg1_rate);
        
        if (!g_GameOptions.mosaic_display) {
            jo_set_displayed_screens(JO_NBG0_SCREEN | JO_NBG1_SCREEN);
        }
    }
    if(g_LogoTimer > PPP_LOGO_TIMER)
    {
        jo_set_displayed_screens(JO_NBG0_SCREEN | JO_NBG1_SCREEN);
        g_Transition.fade_in_rate = 8;
        changeState(GAME_STATE_TITLE_SCREEN);
        return;
    }
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
    #if ENABLE_DEBUG_MODE == 1
    if (g_GameOptions.debug_display) {
        jo_nbg0_printf(2, 25, "LIGHT:X=%3d,Y=%3d,Z=%3d", JO_FIXED_TO_INT(light.x), JO_FIXED_TO_INT(light.y), JO_FIXED_TO_INT(light.z));
        jo_nbg0_printf(2, 26, "LIGHTANGLE:%3d", lightAngle);
    }
    #endif
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