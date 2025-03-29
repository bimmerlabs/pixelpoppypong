#include <jo/jo.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "input.h"
#include "assets.h"
#include "util.h"
#include "screen_transition.h"
#include "objects/player.h"

int g_PauseChoice = 0;
int g_PauseCursor = 0;

// #define PAUSE_TRACK 0

extern PLAYER g_Players[MAX_PLAYERS];

typedef enum _PAUSE_OPTIONS
{
    PAUSE_OPTIONS_RESUME = 0,
    PAUSE_OPTIONS_RESTART = 1,
    PAUSE_OPTIONS_QUIT = 2,
    PAUSE_OPTIONS_DEBUG = 3,
    PAUSE_OPTIONS_ANALOG = 4,
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
    if(g_Game.gameState != GAME_STATE_GAMEPLAY)
    {
        return;
    }

    if(g_Game.isPaused == false)
    {
        // only check for pause press if the game is unpaused
        checkForPausePress();
    }
    // don't allow unpause unless mosaic is done
    else if (!mosaic_out)
    {
        // only check for pause menu presses if the game is paused
        checkForPauseMenu();
        if (g_PauseChoice == PAUSE_OPTIONS_ANALOG) {
            analogAdjustmentScreen_input();
        }
    }
    if (mosaic_out) {
        mosaic_out = mosaicOut(NBG1ON);
    }
    if (mosaic_in) {
        mosaic_in = mosaicIn(NBG1ON);
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
        jo_nbg0_printf(19, options_y, "PAWSED");
        options_y += 6;
        drawPauseMenu(options_y);
        drawPauseMenuCursor();
    }
}

// pause the game
void pauseGame(void)
{

    g_PauseChoice = 0;
    slColOffsetOn(NBG1ON);
    slColOffsetAUse(OFF);
    slColOffsetBUse(NBG1ON);
    nbg1_rate = PAUSE_FADE;
    slColOffsetB(nbg1_rate, nbg1_rate, nbg1_rate);
    if (game_options.mosaic_display) {
        mosaic_out = true;
    }
    g_Game.isPaused = true;
    mosaic_in_rate = MOSAIC_FAST_RATE;
    volume = LOWER_VOLUME;
    #ifndef JO_COMPILE_WITH_AUDIO_SUPPORT
    CDDA_SetVolume(volume);
    #else
    jo_audio_set_volume(volume);
    #endif
    pcm_play(g_Assets.bumpPcm16, PCM_VOLATILE, 6);
    // TODO:
    // playCDTrack(track);
    // maybe reduce the audio volume instead
}

//
// Pause private functions
//

// check if player 1 paused the game
static void checkForPausePress(void)
{
    if (!g_Game.isActive)
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
        pcm_play(g_Assets.bumpPcm16, PCM_VOLATILE, 6);
        g_PauseChoice--;
    }

    if (jo_is_pad1_key_down(JO_KEY_DOWN))
    {
        pcm_play(g_Assets.bumpPcm16, PCM_VOLATILE, 6);
        g_PauseChoice++;
    } 
     
    if (jo_is_pad1_key_down(JO_KEY_LEFT))
    {
        switch(g_PauseChoice)
        {
            case PAUSE_OPTIONS_DEBUG:
                pcm_play(g_Assets.bumpPcm16, PCM_VOLATILE, 6);
                game_options.debug_display = !game_options.debug_display;
                break;
            default:
                break;
        }
    }
    else if (jo_is_pad1_key_down(JO_KEY_RIGHT))
    {
        switch(g_PauseChoice)
        {
            case PAUSE_OPTIONS_DEBUG:
                pcm_play(g_Assets.bumpPcm16, PCM_VOLATILE, 6);
                game_options.debug_display = !game_options.debug_display;
                break;
            default:
                break;
        }
    }
    
    // keep pause screen choice in range
    sanitizeValue(&g_PauseChoice, 0, PAUSE_OPTION_MAX);

    if (jo_is_pad1_key_down(JO_KEY_START) || jo_is_pad1_key_down(JO_KEY_A) || jo_is_pad1_key_down(JO_KEY_C))
    {
        switch(g_PauseChoice)
        {
            case PAUSE_OPTIONS_RESUME:
                pcm_play(g_Assets.bumpPcm16, PCM_VOLATILE, 6);
                // simply unpause
                mosaic_in_rate = MOSAIC_FAST_RATE;
                if (game_options.mosaic_display) {
                    mosaic_in = true;
                }
                slColOffsetB(NEUTRAL_FADE, NEUTRAL_FADE, NEUTRAL_FADE);
                g_Game.isPaused = false;
                volume = MAX_VOLUME;
                #ifndef JO_COMPILE_WITH_AUDIO_SUPPORT
                CDDA_SetVolume(volume);
                #else
                jo_audio_set_volume(volume);
                #endif
                break;

            case PAUSE_OPTIONS_RESTART:
                pcm_play(g_Assets.bumpPcm16, PCM_VOLATILE, 6);
                // start a new game without going to title or team select
                mosaic_in_rate = MOSAIC_FAST_RATE;
                if (game_options.mosaic_display) {
                    mosaic_in = true;
                }
                slColOffsetB(NEUTRAL_FADE, NEUTRAL_FADE, NEUTRAL_FADE);
                changeState(GAME_STATE_GAMEPLAY);
                break;

            case PAUSE_OPTIONS_QUIT:
                pcm_play(g_Assets.bumpPcm16, PCM_VOLATILE, 6);
                transitionState(GAME_STATE_UNINITIALIZED);
                break;

            case PAUSE_OPTIONS_DEBUG:
                pcm_play(g_Assets.bumpPcm16, PCM_VOLATILE, 6);
                // simply unpause
                mosaic_in_rate = MOSAIC_FAST_RATE;
                if (game_options.mosaic_display) {
                    mosaic_in = true;
                }
                slColOffsetB(NEUTRAL_FADE, NEUTRAL_FADE, NEUTRAL_FADE);
                g_Game.isPaused = false;
                volume = MAX_VOLUME;
                #ifndef JO_COMPILE_WITH_AUDIO_SUPPORT
                CDDA_SetVolume(volume);
                #else
                jo_audio_set_volume(volume);
                #endif
                break;
            case PAUSE_OPTIONS_ANALOG:
                pcm_play(g_Assets.bumpPcm16, PCM_VOLATILE, 6);
                // simply unpause
                mosaic_in_rate = MOSAIC_FAST_RATE;
                if (game_options.mosaic_display) {
                    mosaic_in = true;
                }
                slColOffsetB(NEUTRAL_FADE, NEUTRAL_FADE, NEUTRAL_FADE);
                g_Game.isPaused = false;
                volume = MAX_VOLUME;
                // jo_audio_set_volume(volume);
                CDDA_SetVolume(volume);
                break;

            default:
                break;
        }
    }
    else if (jo_is_pad1_key_down(JO_KEY_B))
    {
        pcm_play(g_Assets.bumpPcm16, PCM_VOLATILE, 6);
        // simply unpause
        mosaic_in_rate = MOSAIC_FAST_RATE;
        if (game_options.mosaic_display) {
        mosaic_in = true;
        }
        slColOffsetB(NEUTRAL_FADE, NEUTRAL_FADE, NEUTRAL_FADE);
        g_Game.isPaused = false;
        volume = MAX_VOLUME;
        #ifndef JO_COMPILE_WITH_AUDIO_SUPPORT
        CDDA_SetVolume(volume);
        #else
        jo_audio_set_volume(volume);
        #endif
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
    options_y += 2;
    if (game_options.debug_display) {
        jo_nbg0_printf(options_x, options_y, "DEBUG:ON");
    }
    else {
        jo_nbg0_printf(options_x, options_y, "DEBUG:OFF");
    }
    options_y += 2;
    jo_nbg0_printf(options_x, options_y, "ANALOG ADJUSTMENT:");
    options_y += 1;
    analogAdjustmentScreen_draw(options_x, options_y);    
}

static void drawPauseMenuCursor(void)
{
    FIXED offset = jo_fixed_mult(jo_fixed_sin(jo_fixed_deg2rad(toFIXED(cursor_angle))), toFIXED(8));
    cursor.pos.x = toFIXED(-124) + offset;
    cursor.pos.y = toFIXED(-32 + (g_PauseChoice * 32)); // vertical position varies based on selection
    my_sprite_draw(&cursor);
    cursor_angle += 8;
    if (cursor_angle > 360) {
        cursor_angle = 0;
    }
}
