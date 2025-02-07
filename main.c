/*
** Jo Sega Saturn Engine
** Copyright (c) 2012-2020, Johannes Fetz (johannesfetz@gmail.com)
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**     * Redistributions of source code must retain the above copyright
**       notice, this list of conditions and the following disclaimer.
**     * Redistributions in binary form must reproduce the above copyright
**       notice, this list of conditions and the following disclaimer in the
**       documentation and/or other materials provided with the distribution.
**     * Neither the name of the Johannes Fetz nor the
**       names of its contributors may be used to endorse or promote products
**       derived from this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
** ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
** WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
** DISCLAIMED. IN NO EVENT SHALL Johannes Fetz BE LIABLE FOR ANY
** DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
** (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
** LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
** ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
** SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <jo/jo.h>
#include "main.h"
#include "backup.h"
#include "font.h"
#include "gameplay.h"
#include "input.h"
#include "pause.h"
#include "assets.h"
#include "screen_transition.h"
#include "ppp_logo.h"
#include "title_screen.h"
#include "team_select.h"
#include "debug.h"
#include "objects/player.h"
#include "BG_DEF/nbg1.h"
#include "BG_DEF/sprites.h"
#include "credits.h"

#define MAX_SPRITE 50

GAME g_Game = {0};
ASSETS g_Assets = {0};

jo_datetime now;

Uint8 frame = 0;
Uint8 g_GameTimer = TIMEOUT;
int run_once_callback = 0;

GameOptions game_options = {
    .debug_mode = false,
    .debug_display = false,
    .mesh_display = true,
    .mosaic_display = true,
    .use_rtc = true,
};

void loading_screen(void)
{
    if (g_Game.gameState == !GAME_STATE_TRANSITION) {
        return;
    }
    // currently only works in debug mode (need to work into transitions somehow)
    if (g_Game.isLoading) {
        slColOffsetOn(NBG0ON | NBG1ON);
        jo_set_displayed_screens(JO_NBG0_SCREEN);
        // slColRateNbg1 (TRANSPARENCY_MIN);
        slColOffsetOn(NBG1ON);
        jo_nbg0_printf(17, 12, "LOADING!");
        
        if (game_options.debug_mode) {
            jo_nbg0_printf(15, 14, "SPRITES: %i", jo_sprite_count());
        }
        
        // loading bar        
        char dots[MAX_SPRITE]; // Adjust size based on expected max sprites
        int sprite_count = jo_sprite_count()/2;
        
        // Clamp sprite count to prevent overflow
        if (sprite_count >= MAX_SPRITE) {
            sprite_count = MAX_SPRITE - 1; // Reserve 1 byte for null-terminator
        }
        
        // Generate dot string
        for (int i = 0; i < sprite_count; i++) {
            dots[i] = '.';
        }
        dots[sprite_count] = '\0'; // Null-terminate the string
        
        // Display dots on screen
        jo_nbg0_printf(0, 15, "%s", dots);
    }
}

void main_loop(void) {
    frame++; // this controls a lot of logic, drawing, & timing..
    if (frame > 240)
        frame = 1;
    jo_nbg0_clear(); // clear text before every frame is drawn
    debux_text();
    loading_screen();
}

void my_input_callback(void) {
    switch (g_Game.gameState) {
        case GAME_STATE_PPP_LOGO:
            pppLogo_input();
            break;
        case GAME_STATE_TITLE_SCREEN:
            titleScreen_input();
            break;
        case GAME_STATE_DEMO_LOOP:
            demo_input();
            break;
        case GAME_STATE_CREDITS:
            credits_input();
            break;
        case GAME_STATE_TITLE_MENU:
            menuScreen_input();
            break;
        case GAME_STATE_TITLE_OPTIONS:
            optionsScreen_input();
            break;
        case GAME_STATE_TEAM_SELECT:
            teamSelect_input();
            characterSelect_input();
            break;
        case GAME_STATE_GAMEPLAY:
            gameplay_input();
            break;
        default:
            break;
    }
}

// returns to title screen if player one presses ABC+Start
void abcStart_callback(void)
{
    if (game_options.debug_mode) {
        if (jo_is_pad1_key_down(JO_KEY_Z)) {
            g_Game.nextState = g_Game.gameState +1;
            if (g_Game.nextState == GAME_STATE_TRANSITION) {
                g_Game.nextState = GAME_STATE_UNINITIALIZED;
            }
            transitionState(g_Game.nextState);
        }
        if (jo_is_pad1_key_down(JO_KEY_Y)) {
            g_Game.nextState = g_Game.gameState +1;
            if (g_Game.nextState == GAME_STATE_TRANSITION) {
                g_Game.nextState = GAME_STATE_UNINITIALIZED;
            }
            changeState(g_Game.nextState);
        }
    }
    if(g_Game.gameState == GAME_STATE_UNINITIALIZED || g_Game.gameState == GAME_STATE_TRANSITION)
    {        
        return;
    }
    if ((jo_is_pad1_key_down(JO_KEY_START) || jo_is_pad1_key_down(JO_KEY_X)) // X for retrobit controller testing only
        && jo_is_pad1_key_pressed(JO_KEY_A)  
        && jo_is_pad1_key_pressed(JO_KEY_B)  
        && jo_is_pad1_key_pressed(JO_KEY_C)) {
        if(g_Game.gameState == GAME_STATE_PPP_LOGO)
        {
            jo_goto_boot_menu();
        }
        else {
            transitionState(GAME_STATE_UNINITIALIZED);
        }
    }
}

// cycle through HSL colors
void my_color_calc(void)
{
    if (do_update_logo1) {
        update_palette_logo1 = update_sprites_color(&p_rangeLogo);
        do_update_logo1 = false;
    }
    if (do_update_menu1) {
        update_palette_menu1 = update_sprites_color(&p_rangeMenu1);
        do_update_menu1 = false;
    }
    if (do_update_menu2) {
        update_palette_menu2 = update_sprites_color(&p_rangeMenu2);
        do_update_menu2 = false;
    }
    if (do_update_menu3) {
        update_palette_menu3 = update_sprites_color(&p_rangeMenu3);
        do_update_menu3 = false;
    }
    if (do_update_menu4) {
        update_palette_menu4 = update_sprites_color(&p_rangeMenu4);
        do_update_menu4 = false;
    }
}
void my_palette_update(void)
{
    if (update_palette_logo1) {
        update_palette_logo1 = update_sprites_palette(&p_rangeLogo);
    }
    if (update_palette_menu1) {
        update_palette_menu1 = update_sprites_palette(&p_rangeMenu1);
    }
    if (update_palette_menu2) {
        update_palette_menu2 = update_sprites_palette(&p_rangeMenu2);
    }
    if (update_palette_menu3) {
        update_palette_menu3 = update_sprites_palette(&p_rangeMenu3);
    }
    if (update_palette_menu4) {
        update_palette_menu4 = update_sprites_palette(&p_rangeMenu4);
    }
}

void run_once(void) {
    load_game_backup();
    jo_core_remove_callback(run_once_callback);
    changeState(GAME_STATE_UNINITIALIZED);
}

void			jo_main(void)
{
    jo_core_init(JO_COLOR_Black);
    
    
    #ifndef JO_COMPILE_WITH_AUDIO_SUPPORT
        // pone-sound
        load_drv(ADX_MASTER_2304);
        jo_core_add_vblank_callback(sdrv_vblank_rq);
    #endif

    #ifndef JO_COMPILE_WITH_3D_SUPPORT
        slZdspLevel(3); // if not using jo_3d (JO_COMPILE_WITH_3D_MODULE = 0)
    #endif
    jo_core_tv_off();
    
    slSetSprTVMode(RESOLUTION_HIGH);
    // CRAM mode 0 - required for ngb0 transparency in high-res
    slColRAMMode ( CRM16_1024 ); // must be set before loading any palettes
    
    slColorCalc(CC_RATE | CC_TOP | NBG0ON | NBG1ON);
    slColorCalcOn(NBG0ON);
    
    // base assets
    init_font(); // this has to happen first (sprites require 1st palette slot)
    init_nbg1_img();
    loadSoundAssets();
    init_sprites_img();
    
    init_inputs();
    
    run_once_callback = jo_core_add_callback(run_once);
    
    jo_core_add_callback(screenTransition_update);
    jo_core_add_callback(game_state_update);
    
    jo_core_add_callback(pause_input);
    jo_core_add_callback(pause_draw);   
 
    jo_core_add_callback(my_input_callback);
    
    jo_core_add_callback(pppLogo_update);
    
    jo_core_add_callback(my_color_calc);
    jo_core_add_vblank_callback(my_palette_update);
    
    jo_core_add_callback(startScreen_update);
    jo_core_add_callback(titleScreen_update);
    jo_core_add_callback(optionsScreen_update);
    
    jo_core_add_callback(teamSelect_update);
    jo_core_add_callback(teamSelect_draw);
    
    jo_core_add_callback(gameplay_draw);
    jo_core_add_callback(gameplay_update);
    
    jo_core_add_callback(demo_update);
    
    jo_core_add_callback(display_credits);
    
    jo_core_add_callback(abcStart_callback);
    jo_core_add_vblank_callback(main_loop);
            
    jo_core_run();
}

/*
** END OF FILE
*/
