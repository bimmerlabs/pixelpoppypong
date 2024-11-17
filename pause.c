#include <jo/jo.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "assets.h"
#include "util.h"
// #include "objects/player.h"
// #include "audio.h"

// extern PLAYER g_Players[MAX_PLAYERS];

int g_PauseChoice = 0;
int g_PauseCursor = 0;

#define PAUSE_TRACK 0

typedef enum _PAUSE_OPTIONS
{
    PAUSE_OPTIONS_RESUME = 0,
    PAUSE_OPTIONS_RESTART = 1,
    PAUSE_OPTIONS_QUIT = 2,
    PAUSE_OPTIONS_DEBUG = 3,
    PAUSE_OPTION_MAX,
} PAUSE_OPTIONS;

// static int sortByScore(const void * macchi, const void * jelly);
// //static int sortByTime(const void * macchi, const void * jelly);
// static void validateScores(void);

static void drawPauseMenuCursor(void);
static void drawPauseMenu(void);

static void checkForPausePress(void);
static void checkForPauseMenu(void);


//
// Pause callbacks
//

void convertNumberToDigits(int number, unsigned char* hunds, unsigned char* tens, unsigned char* ones)
{
    if(number == 0)
    {
        *hunds = 0;
        *tens = 0;
        *ones = 0;
        return;
    }

    *hunds = (number / 100);
    *tens = ((number - ((number/100) * 100))/10);
    *ones = (number % 10);
}

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
    else
    {
        // only check for pause menu presses if the game is paused
        checkForPauseMenu();
    }
}

// draw the current score while the game is paused
void pause_draw(void)
{
    if(g_Game.gameState != GAME_STATE_GAMEPLAY)
    {
        return;
    }

    if(g_Game.isPaused == true)
    {
        jo_nbg0_printf(19, 12, "PAWSED");
        //
        // score
        //

        // drawPauseScore();
        drawPauseMenu();
        drawPauseMenuCursor();
    }
}

// pause the game
void pauseGame(void)
{

    g_PauseChoice = 0;
    // slColOffsetA(-150,-150,-150);
    slColOffsetB(-75,-75,-75);
    g_Game.isPaused = true;

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
            g_PauseChoice--;
    }

    if (jo_is_pad1_key_down(JO_KEY_DOWN))
    {
            g_PauseChoice++;
    }
    
    if (jo_is_pad1_key_down(JO_KEY_LEFT))
    {
        switch(g_PauseChoice)
        {
            case PAUSE_OPTIONS_DEBUG:
                debug_display = !debug_display;
                break;
            default:
                break;
        }
    }

    if (jo_is_pad1_key_down(JO_KEY_RIGHT))
    {
        switch(g_PauseChoice)
        {
            case PAUSE_OPTIONS_DEBUG:
                debug_display = !debug_display;
                break;
            default:
                break;
        }
    }

    // keep pause screen choice in range
    sanitizeValue(&g_PauseChoice, 0, PAUSE_OPTION_MAX);

    if (jo_is_pad1_key_down(JO_KEY_START))
    {
        switch(g_PauseChoice)
        {
            case PAUSE_OPTIONS_RESUME:
                // simply unpause
                slColOffsetB(0,0,0);
                g_Game.isPaused = false;
                break;

            case PAUSE_OPTIONS_RESTART:
                // start a new game without going to title or team select
                transitionState(GAME_STATE_GAMEPLAY);
                break;

            case PAUSE_OPTIONS_QUIT:
                // quits to the title screen
                // directly to menu instead of restarting the entire game (ABC+START)
                // need to make sure the game is completely initialized here
                transitionState(GAME_STATE_TITLE_SCREEN);
                break;

            case PAUSE_OPTIONS_DEBUG:
                // simply unpause
                slColOffsetB(0,0,0);
                g_Game.isPaused = false;
                break;

            default:
                break;
        }
    }
}

// Options menu + values
static void drawPauseMenu(void)
{
    jo_nbg0_printf(19, 16, "RESUME");

    jo_nbg0_printf(19, 18, "RESTART");

    jo_nbg0_printf(19, 20, "QUIT");
    
    if (debug_display) {
        jo_nbg0_printf(19, 22, "DEBUG ON");
    }
    else {
        jo_nbg0_printf(19, 22, "DEBUG OFF");
    }
    
}

// The cursor is a random flag
static void drawPauseMenuCursor(void)
{
    FIXED offset = jo_fixed_mult(jo_fixed_sin(jo_fixed_deg2rad(toFIXED(cursor_angle))), toFIXED(8));
    cursor.pos.x = toFIXED(-80) + offset;
    cursor.pos.y = toFIXED(32 + (g_PauseChoice * 33)); // vertical position varies based on selection
    my_sprite_draw(&cursor);
    cursor_angle += 8;
    if (cursor_angle > 360) {
        cursor_angle = 0;
    }
}
