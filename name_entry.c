#include <jo/jo.h>
#include "main.h"
#include "assets.h"
#include "state.h"
#include "ppp_logo.h"
#include "screen_transition.h"
#include "BG_DEF/nbg1.h"
#include "lighting.h"
#include "BG_DEF/sprite_colors.h"

unsigned int g_NameEntryTimer = 0;
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
void initNameEntry(void)
{
    unloadAllAssets();
    loadNameEntryAssets();
    
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
        
    g_NameEntryTimer = 0;
    
    g_Game.lastState = GAME_STATE_NAME_ENTRY;
    
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
void nameEntry_input(void)
{
    //
    // skip the logo if player 1 hits start
    //
    if(jo_is_pad1_key_down(JO_KEY_START))
    {
        nbg1_rate = NEUTRAL_FADE;
        slColOffsetA(nbg1_rate, nbg1_rate, nbg1_rate);
        slColOffsetB(nbg1_rate, nbg1_rate, nbg1_rate);
        mosaic_x = MOSAIC_MIN;
        mosaic_y = MOSAIC_MIN;
	slScrMosSize(mosaic_x, mosaic_y);
        fade_in_rate = 8;
        // unloadPPPLogoAssets();
        changeState(GAME_STATE_UNINITIALIZED);
        return;
    }
}

// update callback routine for PPP logo
void nameEntry_update(void)
{
    if(g_Game.gameState != GAME_STATE_NAME_ENTRY)
    {
        return;
    }
    g_NameEntryTimer++;

        pppLogo_draw();
        // if (!a_pressed && !b_pressed && !c_pressed) {
            update_light_position();

        if(g_NameEntryTimer > PPP_LOGO_TIMER)
        {
            jo_set_displayed_screens(JO_NBG0_SCREEN | JO_NBG1_SCREEN);
            fade_in_rate = 8;
            // unloadPPPLogoAssets();
            changeState(GAME_STATE_UNINITIALIZED);
            return;
        }

}

#define X_RADIUS toFIXED(160)
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