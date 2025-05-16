#include <jo/jo.h>
#include <stdlib.h>
#include <string.h>
#include "../main.h"
#include "input.h"
#include "assets.h"
#include "backup.h"
#include "util.h"
#include "screen_transition.h"
#include "../objects/player.h"

static int pauseChoice = 0;

typedef enum _PAUSE_OPTIONS
{
    PAUSE_OPTIONS_RESUME = 0,
    PAUSE_OPTIONS_RESTART,
    PAUSE_OPTIONS_QUIT,
    #if ENABLE_DEBUG_MODE == 1
    PAUSE_OPTIONS_DEBUG,
    #endif
    PAUSE_OPTIONS_ANALOG,
    PAUSE_OPTION_MAX,
} PAUSE_OPTIONS;

static void drawPauseMenuCursor(void);
static void drawPauseMenu(int options_y);
static void checkForPausePress(void);
static void checkForPauseMenu(void);

//
// Pause callbacks
//

// input for handling pause
void pause_input(void)
{
    if(g_Game.gameState != GAME_STATE_GAMEPLAY || g_Game.isRoundOver || g_Gameplay.isGameOver)
    {
        return;
    }
    if (g_Transition.mosaic_out) {
        g_Transition.mosaic_out = mosaicOut(NBG1ON);
        return;
    }
    if (g_Transition.mosaic_in) {
        g_Transition.mosaic_in = mosaicIn(NBG1ON);
        return;
    }
    if(!g_Game.isPaused) {
        // only check for pause press if the game is unpaused
        checkForPausePress();
    }
    else {
        // only check for pause menu presses if the game is paused
        checkForPauseMenu();
        if (pauseChoice == PAUSE_OPTIONS_ANALOG) {
            analogAdjustmentScreen_input();
        }
    }
}

void pause_draw(void)
{
    if(g_Game.gameState != GAME_STATE_GAMEPLAY)
    {
        return;
    }

    if(g_Game.isPaused == true)
    {
        int options_y = 6;
        if (g_Game.gameMode == GAME_MODE_STORY) {
            jo_nbg0_printf(19, options_y, "PAWSED");
            options_y += 2;
            jo_nbg0_printf(15, options_y, "SCORE:%09d", g_Players[0].score.points + g_Players[0].score.total);
            options_y += 2;
            jo_nbg0_printf(15, options_y, "CONTINUES:%i", g_Players[0].score.continues);
            options_y += 2;
        }
        else {
            options_y = 10; 
            jo_nbg0_printf(19, options_y, "PAWSED");
            options_y += 2;
        }            
        drawPauseMenu(options_y);
        drawPauseMenuCursor();
    }
}

//
// Pause private functions
//

// pause the game
void pauseGame(void)
{
    pauseChoice = 0;
    slColOffsetOn(NBG1ON);
    slColOffsetAUse(OFF);
    slColOffsetBUse(NBG1ON);
    nbg1_rate = PAUSE_FADE;
    slColOffsetB(nbg1_rate, nbg1_rate, nbg1_rate);
    if (g_GameOptions.mosaic_display) {
        g_Transition.mosaic_out = true;
    }
    g_Game.isPaused = true;
    g_Transition.mosaic_in_rate = MOSAIC_FAST_RATE;
    pcm_play(g_Assets.startPcm8, PCM_PROTECTED, 6);
}

// check if player 1 paused the game
static void checkForPausePress(void)
{
    if (g_Game.selectStoryCharacter)
    {
        return;
    }
    if (jo_is_pad1_key_down(JO_KEY_START))
    {
        pauseGame();
    }
}

// pause menu options
static void checkForPauseMenu(void)
{
    if (jo_is_pad1_key_down(JO_KEY_UP))
    {
        pcm_play(g_Assets.cursorPcm8, PCM_PROTECTED, 6);
        pauseChoice--;
    }

    if (jo_is_pad1_key_down(JO_KEY_DOWN))
    {
        pcm_play(g_Assets.cursorPcm8, PCM_PROTECTED, 6);
        pauseChoice++;
    } 
     
    if (jo_is_pad1_key_down(JO_KEY_LEFT))
    {
        switch(pauseChoice)
        {
            #if ENABLE_DEBUG_MODE == 1
            case PAUSE_OPTIONS_DEBUG:
                pcm_play(g_Assets.cursorPcm8, PCM_PROTECTED, 6);
                g_GameOptions.debug_display = !g_GameOptions.debug_display;
                break;
            #endif
            default:
                break;
        }
    }
    else if (jo_is_pad1_key_down(JO_KEY_RIGHT))
    {
        switch(pauseChoice)
        {
            #if ENABLE_DEBUG_MODE == 1
            case PAUSE_OPTIONS_DEBUG:
                pcm_play(g_Assets.cursorPcm8, PCM_PROTECTED, 6);
                g_GameOptions.debug_display = !g_GameOptions.debug_display;
                break;
            #endif
            default:
                break;
        }
    }
    
    // keep pause screen choice in range
    sanitizeValue(&pauseChoice, 0, PAUSE_OPTION_MAX);

    if (jo_is_pad1_key_down(JO_KEY_START) || jo_is_pad1_key_down(JO_KEY_A) || jo_is_pad1_key_down(JO_KEY_C))
    {
        save_game_backup();
        switch(pauseChoice)
        {
            case PAUSE_OPTIONS_RESUME:
                pcm_play(g_Assets.cancelPcm8, PCM_PROTECTED, 6);
                // simply unpause
                g_Transition.mosaic_in_rate = MOSAIC_FAST_RATE;
                if (g_GameOptions.mosaic_display) {
                    g_Transition.mosaic_in = true;
                }
                slColOffsetB(NEUTRAL_FADE, NEUTRAL_FADE, NEUTRAL_FADE);
                g_Game.isPaused = false;
                break;

            case PAUSE_OPTIONS_RESTART:
                pcm_play(g_Assets.nextPcm8, PCM_PROTECTED, 6);
                // start a new game without going to title or team select
                g_Transition.mosaic_in_rate = MOSAIC_FAST_RATE;
                if (g_GameOptions.mosaic_display) {
                    g_Transition.mosaic_in = true;
                }
                slColOffsetB(NEUTRAL_FADE, NEUTRAL_FADE, NEUTRAL_FADE);
                changeState(GAME_STATE_GAMEPLAY);
                break;

            case PAUSE_OPTIONS_QUIT:
                pcm_play(g_Assets.cancelPcm8, PCM_PROTECTED, 6);
                transitionState(GAME_STATE_UNINITIALIZED);
                break;
            #if ENABLE_DEBUG_MODE == 1
            case PAUSE_OPTIONS_DEBUG:
                pcm_play(g_Assets.cancelPcm8, PCM_PROTECTED, 6);
                // simply unpause
                g_Transition.mosaic_in_rate = MOSAIC_FAST_RATE;
                if (g_GameOptions.mosaic_display) {
                    g_Transition.mosaic_in = true;
                }
                slColOffsetB(NEUTRAL_FADE, NEUTRAL_FADE, NEUTRAL_FADE);
                g_Game.isPaused = false;
                break;
            #endif
            case PAUSE_OPTIONS_ANALOG:
                pcm_play(g_Assets.cancelPcm8, PCM_PROTECTED, 6);
                // simply unpause
                g_Transition.mosaic_in_rate = MOSAIC_FAST_RATE;
                if (g_GameOptions.mosaic_display) {
                    g_Transition.mosaic_in = true;
                }
                slColOffsetB(NEUTRAL_FADE, NEUTRAL_FADE, NEUTRAL_FADE);
                g_Game.isPaused = false;
                break;

            default:
                break;
        }
    }
    else if (jo_is_pad1_key_down(JO_KEY_B))
    {
        save_game_backup();
        pcm_play(g_Assets.cancelPcm8, PCM_PROTECTED, 6);
        // simply unpause
        g_Transition.mosaic_in_rate = MOSAIC_FAST_RATE;
        if (g_GameOptions.mosaic_display) {
        g_Transition.mosaic_in = true;
        }
        slColOffsetB(NEUTRAL_FADE, NEUTRAL_FADE, NEUTRAL_FADE);
        g_Game.isPaused = false;
    }
}

// Options menu + values
static void drawPauseMenu(int options_y)
{                
    int options_x = 15;
    jo_nbg0_printf(options_x, options_y, "RESUME");
    options_y += 2;
    jo_nbg0_printf(options_x, options_y, "RESTART");
    options_y += 2;
    jo_nbg0_printf(options_x, options_y, "QUIT");
    #if ENABLE_DEBUG_MODE == 1
    options_y += 2;
    jo_nbg0_printf(options_x, options_y, g_GameOptions.debug_display ? "DEBUG:ON" : "DEBUG:OFF");
    #endif
    options_y += 2;
    jo_nbg0_printf(options_x, options_y, "ANALOG ADJUSTMENT:");
    options_y += 1;
    analogAdjustmentScreen_draw(options_x, options_y);    
}

static void drawPauseMenuCursor(void)
{
    FIXED offset = jo_fixed_mult(jo_fixed_sin(jo_fixed_deg2rad(toFIXED(g_Game.cursor_angle))), toFIXED(8));
    cursor.pos.x = toFIXED(-124) + offset;
    cursor.pos.y = toFIXED(-32 + (pauseChoice * 32));
    my_sprite_draw(&cursor);
    g_Game.cursor_angle += 8;
    if (g_Game.cursor_angle > 360) {
        g_Game.cursor_angle = 0;
    }
}
