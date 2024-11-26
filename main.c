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
#include "font.h"
#include "gameplay.h"
#include "pause.h"
#include "assets.h"
#include "ppp_logo.h"
#include "title_screen.h"
#include "team_select.h"
#include "debug.h"
#include "objects/player.h"

GAME g_Game = {0};
ASSETS g_Assets = {0};
bool first_load = true;
bool debug_display = false;
bool widescreen = false;
Uint8 frame = 0;
Uint8 g_GameTimer = TIMEOUT;

void main_loop(void) {
    frame++; // this controls a lot of logic, drawing, & timing..
    if (frame > 240)
        frame = 1;
    jo_nbg0_clear(); // clear text before every frame is drawn
    debux_text();
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
        case GAME_STATE_TITLE_MENU:
            titleScreen_menu();
            break;
        case GAME_STATE_TEAM_SELECT:
            teamSelect_input();
            characterSelect_input();
            break;
        case GAME_STATE_GAMEPLAY:
            gameplay_input();
            // my_input();
            break;
        default:
            break;
    }
}

// returns to title screen if player one presses ABC+Start
void abcStart_callback(void)
{
    if(g_Game.gameState == GAME_STATE_UNINITIALIZED || g_Game.gameState == GAME_STATE_PPP_LOGO)
    {        
        return;
    }
    // if (jo_is_pad1_key_down(JO_KEY_START)
    if (jo_is_pad1_key_down(JO_KEY_X)  // for retrobit controller testing only
        && jo_is_pad1_key_pressed(JO_KEY_A)  
        && jo_is_pad1_key_pressed(JO_KEY_B)  
        && jo_is_pad1_key_pressed(JO_KEY_C)) {
        changeState(GAME_STATE_UNINITIALIZED);
    }
}

void			jo_main(void)
{
    jo_core_init(JO_COLOR_Black);
    #ifndef JO_COMPILE_WITH_3D_SUPPORT
        slZdspLevel(3); // if not using jo_3d (JO_COMPILE_WITH_3D_MODULE = 0)
    #endif
    jo_core_tv_off();
    
    slSetSprTVMode(RESOLUTION_HIGH);
    
    init_font(); // this has to happen first (sprites get 1st palette slot)
    // show loading screen here?
    load_gameplay_assets();
    titleScreen_init();
    
    jo_core_add_callback(transition_draw);
    
    jo_core_add_callback(pause_input);
    jo_core_add_callback(pause_draw);   
 
    jo_core_add_callback(my_input_callback);
    jo_core_add_callback(abcStart_callback);
    
    jo_core_add_callback(pppLogo_update);
    jo_core_add_callback(pppLogo_draw);
    
    jo_core_add_callback(titleScreen_update);
    jo_core_add_callback(titleScreen_draw);
    
    jo_core_add_callback(demo_update);
    
    jo_core_add_callback(teamSelect_update);
    jo_core_add_callback(teamSelect_draw);
    
    jo_core_add_callback(my_color_calc);
    jo_core_add_vblank_callback(my_palette_update);
    jo_core_add_callback(gameplay_draw);
    jo_core_add_callback(gameplay_update);
    
    jo_core_add_vblank_callback(main_loop);
    
    changeState(GAME_STATE_PPP_LOGO);
    // changeState(GAME_STATE_TITLE_SCREEN);
    // changeState(GAME_STATE_TEAM_SELECT);
    // changeState(GAME_STATE_GAMEPLAY);
    
    jo_core_tv_on();
    jo_core_run();
}

/*
** END OF FILE
*/
