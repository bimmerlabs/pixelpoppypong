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
#include "core/backup.h"
#include "core/input.h"
#include "core/pause.h"
#include "core/assets.h"
#include "core/screen_transition.h"
#include "game/ppp_logo.h"
#include "game/title_screen.h"
#include "game/gameplay.h"
#include "game/team_select.h"
#include "game/credits.h"
#include "game/highscores.h"
#include "game/name_entry.h"
#include "objects/player.h"
#include "palettefx/font.h"
#include "palettefx/nbg1.h"
#include "palettefx/sprite_colors.h"

GAME g_Game = {0};

// explcitly create callback names so they can be added / removed as neccesary?
int run_once_callback = 0;

GameOptions g_GameOptions = {
    .debug_mode = false,
    .debug_display = false,
    .testCollision = false,
    .mesh_display = true,
    .mosaic_display = true,
    .use_rtc = false,
    .unlockBigHeadMode = false,
    .bigHeadMode = false,
    .enableItems = true,
    .enableMeows = true,
    .reservedBool = false,
    .bombTouchCounter = 0,
    .fishTouchCounter = 0,
    .redShroomTouchCounter = 0,
    .blueShroomTouchCounter = 0,
    .craigTouchCounter = 0,
    .garfTouchCounter = 0,
    .reservedInt = 0,
};

void initGame(void) {
    g_Game.frame = 1, // frame counter
    g_Game.cursor_angle = 0, // for title & pawsed menus
    
    // current game state
    g_Game.gameState = GAME_STATE_UNINITIALIZED;
    g_Game.nextState = GAME_STATE_UNINITIALIZED;
    g_Game.lastState = GAME_STATE_UNINITIALIZED;

    // number of players
    g_Game.minPlayers = 0;
    g_Game.maxPlayers = 0;
    g_Game.numPlayers = 0;
    g_Game.currentNumPlayers = 0,
    
    // classic, story, battle
    g_Game.gameMode = 0;

    // easy, medium, hard
    g_Game.gameDifficulty = GAME_DIFFICULTY_MEDIUM;
        
    // TIMERS
    g_Game.endDelayTimer = 0;
    g_Game.BeginTimer = 0;
    g_Game.roundBeginTimer = 0;
    g_Game.dropBallTimer = 0;
    // g_Game.transitionOutTimer = 0;
    g_Game.time_over = false;

    g_Game.selectStoryCharacter = false;

    // is the game loading?
    g_Game.isLoading = false;
    g_Game.isSoundLoading = false;

    // is the game is paused?
    g_Game.isPaused = false;

    // did somebody score a goal?
    g_Game.isGoalScored = false;
    g_Game.goalID = -1;

    // is the game finished?
    g_Game.isRoundOver = false;
    g_Game.countofRounds = 0;
   
    g_Game.winner = -2;
    
    // is the game playing?
    g_Game.isActive = false;
    g_Game.isBallActive = false;
    
    g_Game.explodeBall = false;
}

void loading_screen(void)
{
    if (g_Game.gameState == !GAME_STATE_TRANSITION) {
        return;
    }
    if (g_Game.isLoading) {
        slColOffsetOn(NBG0ON | NBG1ON);
        jo_set_displayed_screens(JO_NBG0_SCREEN);
        slColOffsetOn(NBG1ON);
        
        jo_nbg0_printf(17, 12, "LOADING!");
        
        if (g_Game.isSoundLoading) {
            #if ENABLE_DEBUG_MODE == 1
            if (g_GameOptions.debug_mode) {
                jo_nbg0_printf(15, 14, "SOUNDFX: %i", numberPCMs);
            }
            else {
            #endif
                jo_nbg0_printf(17, 14, "SOUNDFX..");
            #if ENABLE_DEBUG_MODE == 1
            }
            #endif
            // Generate dot string
            char dots[50];
            for (int i = 0; i < numberPCMs; i++) {
                dots[i] = '.';
            }
            dots[numberPCMs] = '\0'; // Null-terminate the string
            // Display dots on screen
            jo_nbg0_printf(0, 15, "%s", dots);            
        }
        else {
            #if ENABLE_DEBUG_MODE == 1
            if (g_GameOptions.debug_mode) {
                jo_nbg0_printf(15, 14, "SPRITES: %i", jo_sprite_count());
            }
            else {
            #endif
                jo_nbg0_printf(17, 14, "SPRITES..");
            #if ENABLE_DEBUG_MODE == 1
            }
            #endif
            // loading bar      
            int sprite_count = jo_sprite_count()/3;
            // Clamp sprite count to prevent overflow
            if (sprite_count >= 50) {
                sprite_count = 49; // Reserve 1 byte for null-terminator
            }
            char dots[50];            
            // Generate dot string
            for (int i = 0; i < sprite_count; i++) {
                dots[i] = '.';
            }
            dots[sprite_count] = '\0'; // Null-terminate the string
            
            // Display dots on screen
            jo_nbg0_printf(0, 15, "%s", dots);            
        }
    }
}

void main_loop(void) {
    g_Game.frame++; // this controls a lot of logic, drawing, & timing..
    if (g_Game.frame > 240)
        g_Game.frame = 1;
    jo_nbg0_clear(); // clear text before every frame is drawn
    gameScore_draw();
    #if ENABLE_DEBUG_MODE == 1
    debux_text();
    #endif
    loading_screen();
}

// returns to title screen if player one presses ABC+Start
void abcStart_callback(void)
{
    #if ENABLE_DEBUG_MODE == 1
    if (g_GameOptions.debug_mode) {
        // manually switch states
        if (jo_is_pad1_key_down(JO_KEY_Z) && g_Game.lastState != GAME_STATE_PPP_LOGO) {
            g_Game.nextState = g_Game.gameState +1;
            if (g_Game.nextState == GAME_STATE_TRANSITION) {
                g_Game.nextState = GAME_STATE_UNINITIALIZED;
            }
            transitionState(g_Game.nextState);
        }
    }
    #endif
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

void my_input_callback(void) {
    abcStart_callback();
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
        case GAME_STATE_HIGHSCORES:
            score_input();
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
            pause_input();
            break;
        case GAME_STATE_NAME_ENTRY:
            nameEntryInput();
            break;
        default:
            break;
    }
}

// cycle through HSL colors
void my_color_calc(void)
{
    // if (g_Game.gameState != GAME_STATE_UNINITIALIZED && g_Game.gameState != GAME_STATE_TRANSITION && g_Game.gameState != GAME_STATE_PPP_LOGO) {
        // updateStarsColors();
    // }
    switch (g_Game.gameState) {
        case GAME_STATE_PPP_LOGO: {
            if (do_update_ppplogo) {
                update_ppplogo_color();
                do_update_ppplogo = false;
                update_palette_ppplogo = true;
            }
            break;
        }
        case GAME_STATE_TITLE_SCREEN: {
            if (do_update_logo1) {
                update_palette_logo1 = update_sprites_color(&p_rangeLogo, HSL_LOGO);
                do_update_logo1 = false;
            }
            break;
        }
        case GAME_STATE_TITLE_MENU: {
            updateTitleMenuColors();
            break;
        }
        case GAME_STATE_TEAM_SELECT: {
            updateTeamSelectColors();
            break;
        }
        case GAME_STATE_DEMO_LOOP: {
            updateGameColors();
            break;
        }
        case GAME_STATE_GAMEPLAY: {
            updateGameColors();
            break;
        }
        case GAME_STATE_NAME_ENTRY: {
            updateNameEntryColors();
            break;
        }
        default:
        {
            return;
        }
    }
}
void my_palette_update(void)
{
    // if (g_Game.gameState != GAME_STATE_UNINITIALIZED && g_Game.gameState != GAME_STATE_TRANSITION && g_Game.gameState != GAME_STATE_PPP_LOGO) {
        // updateStarsPalette();
    // }
    switch (g_Game.gameState) {
        case GAME_STATE_PPP_LOGO: {
            if (update_palette_ppplogo) {
                update_ppplogo_palette();
                update_palette_ppplogo = false;
            }
            break;
        }
        case GAME_STATE_TITLE_SCREEN: {
            if (update_palette_logo1) {
                update_palette_logo1 = update_sprites_palette(&p_rangeLogo);
            }
            break;
        }
        case GAME_STATE_TITLE_MENU: {
            updateTitleMenuPalette();
            break;
        }
        case GAME_STATE_TEAM_SELECT: {
            updateTeamSelectPalette();
            break;
        }
        case GAME_STATE_DEMO_LOOP: {
            updateGamePalette();
            break;
        }
        case GAME_STATE_GAMEPLAY: {
            updateGamePalette();
            break;
        }
        case GAME_STATE_NAME_ENTRY: {
            updateNameEntryPalette();
            break;
        }
        default:
        {
            return;
        }
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
    
    // pone-sound
    load_drv(ADX_MASTER_2304);
    jo_core_add_vblank_callback(sdrv_vblank_rq);

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
    loadCoreSoundAssets();
    init_sprites_img();
    
    initUnlockedCharacters();
    init_inputs();
    highScore_init();
    run_once_callback = jo_core_add_callback(run_once);
    
    jo_core_add_callback(screenTransition_update);
    jo_core_add_callback(game_state_update);
    
    jo_core_add_callback(pause_draw);   
 
    jo_core_add_callback(my_input_callback);
    
    jo_core_add_callback(pppLogo_update);
    
    jo_core_add_callback(startScreen_update);
    jo_core_add_callback(titleScreen_update);
    jo_core_add_callback(optionsScreen_update);
    
    jo_core_add_callback(teamSelect_update);
    jo_core_add_callback(teamSelect_draw);
    
    jo_core_add_callback(gameplay_draw);
    jo_core_add_callback(gameplay_update);  
    jo_core_add_callback(demo_update);
    
    jo_core_add_callback(nameEntryUpdate);
    
    jo_core_add_callback(display_credits);
    jo_core_add_callback(display_scores);
    
    jo_core_add_vblank_callback(main_loop);
    
    // do this last?
    jo_core_add_callback(my_color_calc);
    // jo_core_add_vblank_callback(animateStars);
    jo_core_add_vblank_callback(my_palette_update);
            
    jo_core_run();
}

/*
** END OF FILE
*/
